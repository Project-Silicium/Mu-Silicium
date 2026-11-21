/** @file
  onyx IPCC mailbox for the native GLink drivers.

  The receive path follows the explicit acknowledgement in Linux qcom-ipcc.
  Bank addresses, clients and the callback ABI match the onyx firmware.
  SPDX-License-Identifier: GPL-2.0-only
**/
#include <Uefi.h>
#include <Library/IoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/HardwareInterrupt2.h>
#include <Guid/EventGroup.h>

// qcom-ipcc.c: explicit RECV_ID/CLEAR, with CLEAR_ON_RECV_RD disabled.
// Bank/client layout and native callback ABI are from onyx IPCCDxe, not
// guessed from a different Qualcomm SoC. Bank zero APSS is DT 0x406000.
typedef VOID (EFIAPI *ONYX_IPCC_CALLBACK) (VOID *, UINT32, UINT32);
typedef struct {
  UINTN Base;
  UINT32 Irq;
  UINT32 Signals;
  UINT8 Clients[23];
  UINT32 ClientCount;
  BOOLEAN Started;
  volatile BOOLEAN Pending;
} ONYX_IPCC_BANK;
STATIC ONYX_IPCC_BANK mIpccBanks[] = {
    {0x406000, 261, 8, {0, 1, 2, 3, 5, 6, 8, 33, 34, 35, 26, 45, 46, 24, 23}, 15, FALSE, FALSE},
    {0x443000, 104, 32, {2, 3, 6, 8, 9, 33, 34, 35, 10, 11, 47, 12, 25, 45, 46}, 15, FALSE, FALSE},
    {0x483000, 119, 32, {2, 3, 6, 8, 9, 33, 34, 35, 10, 11, 47, 12, 25, 45, 46}, 15, FALSE, FALSE},
    {0x4c3000, 561, 32, {2, 3, 6, 8, 13, 24, 33, 34, 35, 45}, 10, FALSE, FALSE},
    {0x503000,
     121,
     32,
     {2,   3,   6,   8,   9,   33,  34,  35, 10, 11, 47, 128,
      129, 130, 131, 132, 133, 134, 135, 12, 25, 45, 46},
     23,
     FALSE,
     FALSE}};
typedef struct {
  BOOLEAN Used;
  UINT32 Bank;
} ONYX_IPCC_HANDLE;
typedef struct {
  ONYX_IPCC_HANDLE *Owner;
  UINT32 Client;
  UINT32 First;
  UINT32 Last;
  ONYX_IPCC_CALLBACK Callback;
  VOID *Context;
} ONYX_IPCC_REGISTRATION;
STATIC ONYX_IPCC_HANDLE mIpccHandles[32];
STATIC ONYX_IPCC_REGISTRATION mIpccRegistrations[64];
STATIC EFI_HARDWARE_INTERRUPT2_PROTOCOL *mIpccGic;
STATIC EFI_EVENT mIpccTimer;
STATIC EFI_EVENT mIpccExitEvent;
STATIC BOOLEAN mIpccStopping;

STATIC ONYX_IPCC_HANDLE *IpccHandle (VOID *Handle)
{
  UINTN Index;
  for (Index = 0; Index < ARRAY_SIZE (mIpccHandles); Index++) {
    if (Handle == &mIpccHandles[Index] && mIpccHandles[Index].Used) {
      return Handle;
    }
  }
  return NULL;
}

STATIC INT32 IpccClientIndex (ONYX_IPCC_BANK *Bank, UINT32 Client)
{
  UINTN Index;
  for (Index = 0; Index < Bank->ClientCount; Index++) {
    if (Bank->Clients[Index] == Client) {
      return (INT32)Index;
    }
  }
  return -1;
}

STATIC VOID EFIAPI IpccInterrupt (HARDWARE_INTERRUPT_SOURCE Source, EFI_SYSTEM_CONTEXT Context)
{
  UINTN Index;
  // Mask before EOI: a level interrupt stays asserted until the mailbox is
  // drained. Never execute GLINK callbacks or boot services in IRQ context.
  mIpccGic->DisableInterruptSource (mIpccGic, Source);
  for (Index = 0; Index < ARRAY_SIZE (mIpccBanks); Index++) {
    if (mIpccBanks[Index].Irq == Source) {
      mIpccBanks[Index].Pending = TRUE;
      break;
    }
  }
  mIpccGic->EndOfInterrupt (mIpccGic, Source);
}

STATIC VOID EFIAPI IpccPump (EFI_EVENT Event, VOID *Context)
{
  UINTN BankIndex, Received, Index;
  UINT32 Id, Client, Signal;
  INT32 ClientIndex;
  ONYX_IPCC_BANK *Bank;
  ONYX_IPCC_REGISTRATION Registration;
  if (mIpccStopping) {
    return;
  }
  for (BankIndex = 0; BankIndex < ARRAY_SIZE (mIpccBanks); BankIndex++) {
    Bank = &mIpccBanks[BankIndex];
    if (!Bank->Started || !Bank->Pending) {
      continue;
    }
    // Bounded drain: keep IRQ masked and resume next tick if traffic is heavy.
    for (Received = 0; Received < 64; Received++) {
      Id = MmioRead32 (Bank->Base + 0x10);
      if (Id == MAX_UINT32) {
        Bank->Pending = FALSE;
        mIpccGic->EnableInterruptSource (mIpccGic, Bank->Irq);
        break;
      }
      MmioWrite32 (Bank->Base + 0x1c, Id);
      Client = Id >> 16;
      Signal = Id & 0xffff;
      ClientIndex = IpccClientIndex (Bank, Client);
      for (Index = 0; Index < ARRAY_SIZE (mIpccRegistrations); Index++) {
        Registration = mIpccRegistrations[Index];
        if (Registration.Owner != NULL && Registration.Owner->Bank == BankIndex &&
            Registration.Client == Client && Signal >= Registration.First &&
            Signal <= Registration.Last && ClientIndex >= 0) {
          // Native RECV_ID_INDEX (+0x48) supplies client INDEX to callbacks.
          // Linux RECV_ID supplies wire ID; translate instead of mixing ABIs.
          Registration.Callback (Registration.Context, (UINT32)ClientIndex, Signal);
          break;
        }
      }
    }
  }
}

STATIC INT32 EFIAPI IpccOpen (VOID **Handle, UINT32 BankId)
{
  EFI_STATUS Status;
  EFI_TPL OldTpl;
  UINTN Index;
  ONYX_IPCC_BANK *Bank;
  if (Handle == NULL || BankId >= ARRAY_SIZE (mIpccBanks) || mIpccStopping) {
    return -3;
  }
  *Handle = NULL;
  OldTpl = gBS->RaiseTPL (TPL_CALLBACK);
  for (Index = 0; Index < ARRAY_SIZE (mIpccHandles); Index++) {
    if (!mIpccHandles[Index].Used) {
      break;
    }
  }
  if (Index == ARRAY_SIZE (mIpccHandles)) {
    gBS->RestoreTPL (OldTpl);
    return -2;
  }
  Bank = &mIpccBanks[BankId];
  if (!Bank->Started) {
    // RegisterInterruptSource enables the GIC source on success. The handler
    // is already installed when it can first run; mask again during setup.
    Status =
        mIpccGic->SetTriggerType (mIpccGic, Bank->Irq, EFI_HARDWARE_INTERRUPT2_TRIGGER_LEVEL_HIGH);
    if (!EFI_ERROR (Status)) {
      Status = mIpccGic->RegisterInterruptSource (mIpccGic, Bank->Irq, IpccInterrupt);
    }
    if (EFI_ERROR (Status)) {
      gBS->RestoreTPL (OldTpl);
      return -1;
    }
    mIpccGic->DisableInterruptSource (mIpccGic, Bank->Irq);
    MmioAnd32 (Bank->Base + 0x08, (UINT32)~BIT0);
    Bank->Started = TRUE;
    Bank->Pending = TRUE;
  }
  mIpccHandles[Index].Bank = BankId;
  mIpccHandles[Index].Used = TRUE;
  *Handle = &mIpccHandles[Index];
  gBS->RestoreTPL (OldTpl);
  return 0;
}

STATIC INT32 EFIAPI IpccRegister (VOID *Handle, UINT32 Client, UINT32 First, UINT32 Last,
                                  ONYX_IPCC_CALLBACK Callback, VOID *Context)
{
  ONYX_IPCC_HANDLE *Owner = IpccHandle (Handle);
  ONYX_IPCC_BANK *Bank;
  EFI_TPL OldTpl;
  UINTN Index, FreeIndex = ARRAY_SIZE (mIpccRegistrations);
  UINT32 Signal;
  if (Owner == NULL || Callback == NULL || First > Last || mIpccStopping) {
    return -3;
  }
  Bank = &mIpccBanks[Owner->Bank];
  if (Last >= Bank->Signals || IpccClientIndex (Bank, Client) < 0) {
    return -3;
  }
  OldTpl = gBS->RaiseTPL (TPL_CALLBACK);
  for (Index = 0; Index < ARRAY_SIZE (mIpccRegistrations); Index++) {
    ONYX_IPCC_REGISTRATION *Registration = &mIpccRegistrations[Index];
    if (Registration->Owner == NULL) {
      FreeIndex = Index;
      continue;
    }
    if (Registration->Owner->Bank == Owner->Bank && Registration->Client == Client &&
        First <= Registration->Last && Last >= Registration->First) {
      gBS->RestoreTPL (OldTpl);
      return -4;
    }
  }
  if (FreeIndex == ARRAY_SIZE (mIpccRegistrations)) {
    gBS->RestoreTPL (OldTpl);
    return -2;
  }
  mIpccRegistrations[FreeIndex] =
      (ONYX_IPCC_REGISTRATION){Owner, Client, First, Last, Callback, Context};
  for (Signal = First; Signal <= Last; Signal++) {
    MmioWrite32 (Bank->Base + 0x14, (Client << 16) | Signal);
  }
  gBS->RestoreTPL (OldTpl);
  return 0;
}

STATIC INT32 EFIAPI IpccUnregister (VOID *Handle, UINT32 Client, UINT32 First, UINT32 Last)
{
  ONYX_IPCC_HANDLE *Owner = IpccHandle (Handle);
  EFI_TPL OldTpl;
  UINTN Index;
  UINT32 Signal;
  if (Owner == NULL || First > Last) {
    return -3;
  }
  OldTpl = gBS->RaiseTPL (TPL_CALLBACK);
  for (Index = 0; Index < ARRAY_SIZE (mIpccRegistrations); Index++) {
    ONYX_IPCC_REGISTRATION *Registration = &mIpccRegistrations[Index];
    if (Registration->Owner == Owner && Registration->Client == Client &&
        Registration->First == First && Registration->Last == Last) {
      for (Signal = First; Signal <= Last; Signal++) {
        MmioWrite32 (mIpccBanks[Owner->Bank].Base + 0x18, (Client << 16) | Signal);
      }
      ZeroMem (Registration, sizeof (*Registration));
      gBS->RestoreTPL (OldTpl);
      return 0;
    }
  }
  gBS->RestoreTPL (OldTpl);
  return -3;
}

STATIC INT32 EFIAPI IpccClose (VOID **Handle)
{
  ONYX_IPCC_HANDLE *Owner;
  EFI_TPL OldTpl;
  UINTN Index;
  if (Handle == NULL || (Owner = IpccHandle (*Handle)) == NULL) {
    return -3;
  }
  OldTpl = gBS->RaiseTPL (TPL_CALLBACK);
  for (Index = 0; Index < ARRAY_SIZE (mIpccRegistrations); Index++) {
    ONYX_IPCC_REGISTRATION *Registration = &mIpccRegistrations[Index];
    if (Registration->Owner == Owner) {
      IpccUnregister (Owner, Registration->Client, Registration->First, Registration->Last);
    }
  }
  Owner->Used = FALSE;
  *Handle = NULL;
  gBS->RestoreTPL (OldTpl);
  return 0;
}

STATIC INT32 EFIAPI IpccSend (VOID *Handle, UINT32 Client, UINT32 First, UINT32 Last)
{
  ONYX_IPCC_HANDLE *Owner = IpccHandle (Handle);
  ONYX_IPCC_BANK *Bank;
  UINT32 Signal;
  if (Owner == NULL || First > Last || mIpccStopping) {
    return -3;
  }
  Bank = &mIpccBanks[Owner->Bank];
  if (Last >= Bank->Signals || IpccClientIndex (Bank, Client) < 0) {
    return -3;
  }
  for (Signal = First; Signal <= Last; Signal++) {
    MmioWrite32 (Bank->Base + 0x0c, (Client << 16) | Signal);
  }
  return 0;
}

STATIC struct {
  UINT64 Revision;
  INT32 (EFIAPI *Open) (VOID **, UINT32);
  INT32 (EFIAPI *Close) (VOID **);
  INT32 (EFIAPI *Register) (VOID *, UINT32, UINT32, UINT32, ONYX_IPCC_CALLBACK, VOID *);
  INT32 (EFIAPI *Unregister) (VOID *, UINT32, UINT32, UINT32);
  INT32 (EFIAPI *Send) (VOID *, UINT32, UINT32, UINT32);
} mIpccProtocol = {0x10002, IpccOpen, IpccClose, IpccRegister, IpccUnregister, IpccSend};
STATIC EFI_GUID mIpccGuid = {
    0x52858ad7, 0x8b16, 0x4137, {0x9c, 0x64, 0xdf, 0xe0, 0x4d, 0x94, 0x2e, 0x80}};

STATIC VOID EFIAPI IpccExit (EFI_EVENT Event, VOID *Context)
{
  UINTN Index;
  mIpccStopping = TRUE;
  for (Index = 0; Index < ARRAY_SIZE (mIpccBanks); Index++) {
    if (mIpccBanks[Index].Started) {
      mIpccGic->DisableInterruptSource (mIpccGic, mIpccBanks[Index].Irq);
    }
  }
}

EFI_STATUS EFIAPI OnyxIpccEntry (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status;
  EFI_HANDLE Handle = NULL;
  Status = gBS->LocateProtocol (&gHardwareInterrupt2ProtocolGuid, NULL, (VOID **)&mIpccGic);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  Status =
      gBS->CreateEvent (EVT_TIMER | EVT_NOTIFY_SIGNAL, TPL_CALLBACK, IpccPump, NULL, &mIpccTimer);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  Status = gBS->SetTimer (mIpccTimer, TimerPeriodic, 10000); // 1 ms, deferred callbacks
  if (!EFI_ERROR (Status)) {
    Status = gBS->CreateEventEx (EVT_NOTIFY_SIGNAL, TPL_NOTIFY, IpccExit, NULL,
                                 &gEfiEventExitBootServicesGuid, &mIpccExitEvent);
  }
  if (!EFI_ERROR (Status)) {
    Status = gBS->InstallMultipleProtocolInterfaces (&Handle, &mIpccGuid, &mIpccProtocol, NULL);
  }
  if (EFI_ERROR (Status)) {
    gBS->CloseEvent (mIpccTimer);
    if (mIpccExitEvent != NULL) {
      gBS->CloseEvent (mIpccExitEvent);
    }
  }
  return Status;
}
