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
  ONYX_IPCC_BANK **Owner;
  UINT8 ClientIndex;
  UINT8 First;
  UINT8 Last;
  ONYX_IPCC_CALLBACK Callback;
  VOID *Context;
} ONYX_IPCC_REGISTRATION;
// A handle is the address of an occupied bank-pointer slot. Multiple opens
// of the same bank own their registrations independently.
STATIC ONYX_IPCC_BANK *mIpccHandles[32];
STATIC ONYX_IPCC_REGISTRATION mIpccRegistrations[64];
STATIC EFI_HARDWARE_INTERRUPT2_PROTOCOL *mIpccGic;
STATIC BOOLEAN mIpccStopping;

STATIC ONYX_IPCC_BANK *IpccBankForHandle (VOID *Handle)
{
  UINTN Offset = (UINTN)Handle - (UINTN)mIpccHandles;
  if (Offset >= sizeof (mIpccHandles) || Offset % sizeof (*mIpccHandles) != 0) {
    return NULL;
  }
  return mIpccHandles[Offset / sizeof (*mIpccHandles)];
}

STATIC CONST UINT8 *IpccClient (ONYX_IPCC_BANK *Bank, UINT32 Client, UINT32 First, UINT32 Last)
{
  if (Bank == NULL || Client > MAX_UINT8 || First > Last || Last >= Bank->Signals) {
    return NULL;
  }
  return ScanMem8 (Bank->Clients, Bank->ClientCount, (UINT8)Client);
}

STATIC VOID IpccWriteSignals (UINTN Register, UINT32 Client, UINT32 First, UINT32 Last)
{
  for (UINT32 Signal = First; Signal <= Last; Signal++) {
    MmioWrite32 (Register, (Client << 16) | Signal);
  }
}

STATIC VOID IpccRemoveRegistration (ONYX_IPCC_REGISTRATION *Registration)
{
  ONYX_IPCC_BANK *Bank = *Registration->Owner;
  IpccWriteSignals (Bank->Base + 0x18, Bank->Clients[Registration->ClientIndex],
                    Registration->First, Registration->Last);
  ZeroMem (Registration, sizeof (*Registration));
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
  ONYX_IPCC_BANK *Bank;
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
      for (Index = 0; Index < ARRAY_SIZE (mIpccRegistrations); Index++) {
        ONYX_IPCC_REGISTRATION *Registration = &mIpccRegistrations[Index];
        if (Registration->Owner != NULL && *Registration->Owner == Bank &&
            Bank->Clients[Registration->ClientIndex] == Client &&
            Signal >= Registration->First && Signal <= Registration->Last) {
          // Native callbacks take the client index, while RECV_ID carries its wire ID.
          Registration->Callback (Registration->Context, Registration->ClientIndex, Signal);
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
  ONYX_IPCC_BANK *Bank;
  ONYX_IPCC_BANK **Slot;
  if (Handle == NULL || BankId >= ARRAY_SIZE (mIpccBanks) || mIpccStopping) {
    return -3;
  }
  *Handle = NULL;
  OldTpl = gBS->RaiseTPL (TPL_CALLBACK);
  Slot = ScanMemN (mIpccHandles, sizeof (mIpccHandles), 0);
  if (Slot == NULL) {
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
  *Slot = Bank;
  *Handle = Slot;
  gBS->RestoreTPL (OldTpl);
  return 0;
}

STATIC INT32 EFIAPI IpccRegister (VOID *Handle, UINT32 Client, UINT32 First, UINT32 Last,
                                  ONYX_IPCC_CALLBACK Callback, VOID *Context)
{
  ONYX_IPCC_BANK *Bank;
  CONST UINT8 *WireClient;
  EFI_TPL OldTpl;
  UINTN Index;
  ONYX_IPCC_REGISTRATION *Free = NULL;
  OldTpl = gBS->RaiseTPL (TPL_CALLBACK);
  Bank = IpccBankForHandle (Handle);
  WireClient = IpccClient (Bank, Client, First, Last);
  if (WireClient == NULL || Callback == NULL || mIpccStopping) {
    gBS->RestoreTPL (OldTpl);
    return -3;
  }
  for (Index = 0; Index < ARRAY_SIZE (mIpccRegistrations); Index++) {
    ONYX_IPCC_REGISTRATION *Registration = &mIpccRegistrations[Index];
    if (Registration->Owner == NULL) {
      Free = Registration;
      continue;
    }
    if (*Registration->Owner == Bank && Bank->Clients[Registration->ClientIndex] == Client &&
        First <= Registration->Last && Last >= Registration->First) {
      gBS->RestoreTPL (OldTpl);
      return -4;
    }
  }
  if (Free == NULL) {
    gBS->RestoreTPL (OldTpl);
    return -2;
  }
  *Free = (ONYX_IPCC_REGISTRATION){
      Handle, (UINT8)(WireClient - Bank->Clients), (UINT8)First, (UINT8)Last, Callback, Context};
  IpccWriteSignals (Bank->Base + 0x14, Client, First, Last);
  gBS->RestoreTPL (OldTpl);
  return 0;
}

STATIC INT32 EFIAPI IpccUnregister (VOID *Handle, UINT32 Client, UINT32 First, UINT32 Last)
{
  ONYX_IPCC_BANK *Bank;
  EFI_TPL OldTpl;
  UINTN Index;
  OldTpl = gBS->RaiseTPL (TPL_CALLBACK);
  Bank = IpccBankForHandle (Handle);
  if (Bank == NULL || First > Last) {
    gBS->RestoreTPL (OldTpl);
    return -3;
  }
  for (Index = 0; Index < ARRAY_SIZE (mIpccRegistrations); Index++) {
    ONYX_IPCC_REGISTRATION *Registration = &mIpccRegistrations[Index];
    if (Registration->Owner == Handle && Bank->Clients[Registration->ClientIndex] == Client &&
        Registration->First == First && Registration->Last == Last) {
      IpccRemoveRegistration (Registration);
      gBS->RestoreTPL (OldTpl);
      return 0;
    }
  }
  gBS->RestoreTPL (OldTpl);
  return -3;
}

STATIC INT32 EFIAPI IpccClose (VOID **Handle)
{
  ONYX_IPCC_BANK **Slot;
  EFI_TPL OldTpl;
  UINTN Index;
  OldTpl = gBS->RaiseTPL (TPL_CALLBACK);
  if (Handle == NULL || IpccBankForHandle (*Handle) == NULL) {
    gBS->RestoreTPL (OldTpl);
    return -3;
  }
  Slot = *Handle;
  for (Index = 0; Index < ARRAY_SIZE (mIpccRegistrations); Index++) {
    ONYX_IPCC_REGISTRATION *Registration = &mIpccRegistrations[Index];
    if (Registration->Owner == Slot) {
      IpccRemoveRegistration (Registration);
    }
  }
  *Slot = NULL;
  *Handle = NULL;
  gBS->RestoreTPL (OldTpl);
  return 0;
}

STATIC INT32 EFIAPI IpccSend (VOID *Handle, UINT32 Client, UINT32 First, UINT32 Last)
{
  ONYX_IPCC_BANK *Bank = IpccBankForHandle (Handle);
  if (mIpccStopping || IpccClient (Bank, Client, First, Last) == NULL) {
    return -3;
  }
  IpccWriteSignals (Bank->Base + 0x0c, Client, First, Last);
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
  EFI_EVENT Timer;
  EFI_EVENT ExitEvent = NULL;
  Status = gBS->LocateProtocol (&gHardwareInterrupt2ProtocolGuid, NULL, (VOID **)&mIpccGic);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  Status =
      gBS->CreateEvent (EVT_TIMER | EVT_NOTIFY_SIGNAL, TPL_CALLBACK, IpccPump, NULL, &Timer);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  Status = gBS->SetTimer (Timer, TimerPeriodic, 10000); // 1 ms, deferred callbacks
  if (!EFI_ERROR (Status)) {
    Status = gBS->CreateEventEx (EVT_NOTIFY_SIGNAL, TPL_NOTIFY, IpccExit, NULL,
                                 &gEfiEventExitBootServicesGuid, &ExitEvent);
  }
  if (!EFI_ERROR (Status)) {
    Status = gBS->InstallMultipleProtocolInterfaces (&Handle, &mIpccGuid, &mIpccProtocol, NULL);
  }
  if (EFI_ERROR (Status)) {
    gBS->CloseEvent (Timer);
    if (ExitEvent != NULL) {
      gBS->CloseEvent (ExitEvent);
    }
  }
  return Status;
}
