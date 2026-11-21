/** @file
  onyx SC8581 supply path driven by Android MCA notifications.

  This driver leaves UCSI, USB role selection, Host, UsbFn and Mass Storage
  protocols to the native firmware drivers.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#include <Uefi.h>
#include <Guid/EventGroup.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/TimerLib.h>
#include <Library/UefiBootServicesTableLib.h>

// The early image owns the GLink callbacks. A second dispatch of this source,
// after PmicGlinkDxe, starts it at TPL_APPLICATION: PIL performs file I/O and
// must never run from a timer notification.
STATIC EFI_GUID mSupplyStartGuid =
  { 0x18587161, 0x0E7A, 0x4ED4, { 0xAA, 0xFC, 0x71, 0x00, 0x79, 0xF2, 0xD0, 0x93 } };
typedef struct {
  UINT64 Revision;
  EFI_STATUS (EFIAPI *Start)(VOID);
} ONYX_SUPPLY_START_PROTOCOL;
STATIC_ASSERT (OFFSET_OF (ONYX_SUPPLY_START_PROTOCOL, Start) == 8, "supply Start ABI");

#ifdef ONYX_SUPPLY_BOOTSTRAP_BUILD

EFI_STATUS EFIAPI
OnyxUsbSupplyBootstrapEntry (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
  ONYX_SUPPLY_START_PROTOCOL *Supply;
  EFI_STATUS Status;
  Status = gBS->LocateProtocol (&mSupplyStartGuid, NULL, (VOID **)&Supply);
  if (!EFI_ERROR (Status)) {
    if ((Supply == NULL) || (Supply->Revision != 1) || (Supply->Start == NULL)) {
      Status = EFI_UNSUPPORTED;
    } else {
      Status = Supply->Start ();
    }
  }
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "OnyxPower: startup failed: %r\n", Status));
  }
  return Status;
}

#else

#define ONYX_POWER_POLL_PERIOD  100000ULL
#define ONYX_BOOT_TIMEOUT_US    45000000ULL
#define ONYX_MCA_OWNER          0x800AU
#define ONYX_MCA_READ           1U
#define ONYX_MCA_WRITE          2U
#define ONYX_MCA_NOTIFY         2U
#define ONYX_MCA_DATA_SIZE      256U
#define ONYX_MCA_QUEUE_SIZE     32U
#define ONYX_MCA_TIMEOUT_US     500000ULL
#define ONYX_RETRY_US           250000ULL

typedef struct {
  UINT32 FrequencyKhz;
  UINT32 SlaveAddress;
  UINT32 Mode;
  UINT32 StretchUs;
  UINT32 Core1;
  UINT32 Core2;
} ONYX_I2C_CONFIG;

typedef UINT32 (EFIAPI *ONYX_I2C_REGISTER_ACCESS)(VOID *Handle, ONYX_I2C_CONFIG *Config,
                                                UINT16 Register, UINT16 RegisterAddressSize,
                                                UINT8 *Data, UINT16 DataSize, UINT32 *Count,
                                                UINT32 TimeoutUs);

typedef struct {
  UINT64 Revision;
  UINT32 (EFIAPI *Open)(UINT32 Instance, VOID **Handle);
  ONYX_I2C_REGISTER_ACCESS Read;
  ONYX_I2C_REGISTER_ACCESS Write;
  VOID *Transfer;
  UINT32 (EFIAPI *Close)(VOID *Handle);
} ONYX_I2C;

typedef struct {
  UINT64 Revision;
  EFI_STATUS (EFIAPI *ConfigGpio)(UINT32 Config, UINT32 Enable);
  VOID *ConfigGpioGroup;
  VOID *GpioIn;
  EFI_STATUS (EFIAPI *GpioOut)(UINT32 Config, UINT32 Value);
} ONYX_TLMM;

typedef VOID (EFIAPI *ONYX_GLINK_RX)(VOID *, VOID *, VOID *, CONST VOID *, UINTN, UINTN);
typedef VOID (EFIAPI *ONYX_GLINK_STATE)(VOID *, VOID *, UINT32);
typedef VOID (EFIAPI *ONYX_GLINK_TX_DONE)(VOID *, VOID *, VOID *, CONST VOID *, UINTN);

typedef struct {
  CONST CHAR8 *Transport;
  CONST CHAR8 *Remote;
  CONST CHAR8 *Name;
  UINT32 Options;
  UINT32 Reserved;
  VOID *Private;
  ONYX_GLINK_RX Receive;
  VOID *ReceiveVector;
  VOID *ReceiveIntentRequest;
  ONYX_GLINK_TX_DONE TransmitDone;
  ONYX_GLINK_STATE State;
  VOID *Other50To70[5];
  UINT32 IntentTimeout;
  UINT32 Reserved7c;
  VOID *Other80;
  VOID *Other88;
} ONYX_GLINK_OPEN_CONFIG;

typedef EFI_STATUS (EFIAPI *ONYX_GLINK_OPEN)(ONYX_GLINK_OPEN_CONFIG *, VOID **, INT32 *);
typedef EFI_STATUS (EFIAPI *ONYX_GLINK_TX)(VOID *, VOID *, CONST VOID *, UINTN, UINT32, INT32 *);
typedef struct {
  UINT64 Revision;
  VOID *Register;
  VOID *Deregister;
  ONYX_GLINK_OPEN Open;
  VOID *Close;
  ONYX_GLINK_TX Transmit;
  VOID *TransmitVector;
  VOID *QueueRxIntent;
  EFI_STATUS (EFIAPI *RxDone)(VOID *, CONST VOID *, BOOLEAN, INT32 *);
  VOID *Other48To78[7];
  EFI_STATUS (EFIAPI *Poll)(VOID *, INT32 *);
} ONYX_GLINK;

typedef struct {
  UINT64 Revision;
  EFI_STATUS (EFIAPI *Load)(CONST CHAR16 *Name);
  VOID *Reserved10To28[4];
  EFI_STATUS (EFIAPI *GetConfig)(CONST CHAR16 *Name, VOID *Config);
} ONYX_PIL;

typedef struct {
  UINT64 Revision;
  VOID *Reserved08To38[7];
  EFI_STATUS (EFIAPI *Connect)(VOID);
  EFI_STATUS (EFIAPI *Connected)(BOOLEAN *Connected);
} ONYX_PMIC_ROUTER;

typedef struct {
  UINT64 Revision;
  VOID *Reserved08To1C0[56];
  EFI_STATUS (EFIAPI *SetOs)(UINT32 Os);
} ONYX_PMIC_BOOT;

// The transport may deliver these little-endian wire records at any alignment.
#pragma pack(push, 1)
typedef struct {
  UINT32 Owner;
  UINT32 Type;
  UINT32 Opcode;
  UINT32 Property;
} ONYX_MCA_HEADER;

typedef struct {
  ONYX_MCA_HEADER Header;
  UINT32 Sequence;
  UINT8 Data[ONYX_MCA_DATA_SIZE];
} ONYX_MCA_REQUEST;

typedef struct {
  ONYX_MCA_HEADER Header;
  UINT32 Result;
  UINT32 Sequence;
  UINT8 Data[ONYX_MCA_DATA_SIZE];
} ONYX_MCA_REPLY;

typedef struct {
  ONYX_MCA_HEADER Header;
  UINT32 Value;
  UINT8 Reserved[ONYX_MCA_DATA_SIZE - sizeof (UINT32)];
} ONYX_MCA_NOTIFY_PACKET;
#pragma pack(pop)

STATIC_ASSERT (sizeof (ONYX_MCA_REQUEST) == 276, "MCA request length");
STATIC_ASSERT (sizeof (ONYX_MCA_REPLY) == 280, "MCA reply length");
STATIC_ASSERT (sizeof (ONYX_MCA_NOTIFY_PACKET) == 272, "MCA notification length");

typedef struct {
  UINT32 Type;
  UINT32 Value;
} ONYX_MCA_NOTIFICATION;

typedef struct {
  ONYX_I2C *Protocol;
  VOID *Handle;
  ONYX_I2C_CONFIG Config;
  BOOLEAN Initialized;
} ONYX_SC8581_BUS;

STATIC_ASSERT (sizeof (ONYX_I2C_CONFIG) == 24, "onyx I2C config ABI");
STATIC_ASSERT (OFFSET_OF (ONYX_GLINK_OPEN_CONFIG, State) == 0x48, "onyx GLink open ABI");
STATIC_ASSERT (OFFSET_OF (ONYX_GLINK, Poll) == 0x80, "onyx GLink ABI");
STATIC_ASSERT (OFFSET_OF (ONYX_PIL, GetConfig) == 0x30, "onyx PIL ABI");
STATIC_ASSERT (OFFSET_OF (ONYX_PMIC_ROUTER, Connect) == 0x40, "onyx PMIC router ABI");
STATIC_ASSERT (OFFSET_OF (ONYX_PMIC_BOOT, SetOs) == 0x1C8, "onyx PMIC boot ABI");

STATIC EFI_GUID mGlinkGuid =
  { 0x3BFCF64D, 0x00CD, 0x41CB, { 0xBE, 0xA5, 0xA7, 0x6C, 0x3A, 0x24, 0x3D, 0xED } };
STATIC EFI_GUID mPmicGlinkGuid =
  { 0x7429EB07, 0xF2E3, 0x4A6B, { 0xAA, 0x96, 0xC9, 0x09, 0xCD, 0x95, 0x6E, 0xF7 } };
STATIC EFI_GUID mPilGuid =
  { 0x1F4F1DA3, 0x4D8D, 0x4894, { 0xBA, 0x2A, 0x13, 0xD3, 0x20, 0x7F, 0x97, 0x39 } };
STATIC EFI_GUID mPmicBootGuid =
  { 0xAE6AE96E, 0x483F, 0x42AE, { 0x9C, 0xC1, 0x9F, 0xAC, 0x1B, 0x58, 0x47, 0x28 } };
STATIC EFI_GUID mI2cGuid =
  { 0xB27AE8B1, 0x3E10, 0x4D07, { 0xAB, 0x5C, 0xEB, 0x9A, 0x6D, 0xC6, 0xFA, 0x8F } };
STATIC EFI_GUID mTlmmGuid =
  { 0xAD9AEC18, 0x7BF0, 0x4809, { 0x9E, 0x96, 0x30, 0x12, 0x30, 0x9F, 0x3D, 0xF7 } };

STATIC ONYX_GLINK *mGlink;
STATIC ONYX_GLINK_OPEN mGlinkOpen;
STATIC ONYX_GLINK_TX mGlinkTransmit;
STATIC ONYX_GLINK_RX mRouterReceive;
STATIC ONYX_GLINK_STATE mRouterState;
STATIC ONYX_GLINK_TX_DONE mRouterTransmitDone;
STATIC VOID *mRouterHandle;
STATIC BOOLEAN mRouterUp;
STATIC BOOLEAN mRouterOpening;
STATIC BOOLEAN mAdapterInstalled;
STATIC BOOLEAN mStopping;
STATIC EFI_EVENT mWorkerEvent;
STATIC EFI_EVENT mExitEvent;
STATIC EFI_HANDLE mSupplyHandle;
STATIC BOOLEAN mStartupBusy;
STATIC BOOLEAN mWorkerBusy;
STATIC BOOLEAN mSupplyStarted;
STATIC BOOLEAN mOtgDisablePending;
STATIC UINTN mBootPhase;
STATIC EFI_STATUS mBootStatus = EFI_NOT_READY;
STATIC ONYX_PIL *mPil;
STATIC ONYX_PMIC_ROUTER *mPmicRouter;
STATIC ONYX_PMIC_BOOT *mPmicBoot;
STATIC ONYX_MCA_REQUEST mRequest;
STATIC UINT32 mReplyResult;
STATIC UINT8 mReplyValue;
STATIC UINT32 mSequence = 0x4F4E5900U;
STATIC UINT64 mRequestStarted;
STATIC BOOLEAN mRequestBusy;
STATIC BOOLEAN mRequestDone;
STATIC BOOLEAN mRequestTxDone;
STATIC BOOLEAN mRequestTimedOut;
STATIC BOOLEAN mNativeRequest;
STATIC UINT32 mNativeProperty;
STATIC UINT32 mNativeSequence;
STATIC UINT32 mNativeOpcode;
STATIC UINT64 mNativeStarted;
STATIC BOOLEAN mMcaSynchronized;
STATIC UINTN mSyncIndex;
STATIC BOOLEAN mSyncAwaitingReply;
STATIC UINT64 mRetryAt;
STATIC UINTN mSyncRetries;
STATIC BOOLEAN mSupplyFault;
STATIC ONYX_MCA_NOTIFICATION mNotifications[ONYX_MCA_QUEUE_SIZE];
STATIC UINTN mNotifyHead;
STATIC UINTN mNotifyTail;
STATIC BOOLEAN mNotifyOverflow;
STATIC UINT32 mCpRevert;
STATIC UINT32 mLastCpPosition = 1;
STATIC BOOLEAN mCpWorkActive;
STATIC UINTN mCpWorkPhase;
STATIC UINT32 mCpWorkPosition;
STATIC UINT8 mCpWorkPresent;
STATIC UINT64 mCpWorkDue;
STATIC UINTN mCpWorkRetries;
STATIC ONYX_SC8581_BUS mChargePump;

STATIC UINT64
NowUs (VOID)
{
  return GetTimeInNanoSecond (GetPerformanceCounter ()) / 1000;
}

STATIC EFI_STATUS
Sc8581Access (BOOLEAN Write, UINT8 Reg, UINT8 *Value)
{
  ONYX_I2C_REGISTER_ACCESS Transfer;
  UINT32 Count = 0;
  UINT32 Result;
  if (mChargePump.Handle == NULL) { return EFI_NOT_READY; }
  Transfer = Write ? mChargePump.Protocol->Write : mChargePump.Protocol->Read;
  Result = Transfer (mChargePump.Handle, &mChargePump.Config, Reg, 1, Value, 1, &Count, 2500);
  // Some revisions include the one-byte register address in the transfer count.
  if ((Result != 0) || ((Count != 1) && (Count != 2))) {
    DEBUG ((DEBUG_ERROR, "OnyxPower: SC8581 %a %02x result=%u count=%u\n",
            Write ? "write" : "read", Reg, Result, Count));
    return EFI_DEVICE_ERROR;
  }
  return EFI_SUCCESS;
}

STATIC EFI_STATUS
Sc8581Read (UINT8 Reg, UINT8 *Value)
{
  return Sc8581Access (FALSE, Reg, Value);
}

STATIC EFI_STATUS
Sc8581Write (UINT8 Reg, UINT8 Value)
{
  return Sc8581Access (TRUE, Reg, &Value);
}

STATIC EFI_STATUS
Sc8581CheckId (VOID)
{
  UINT8 Id;
  EFI_STATUS Status = Sc8581Read (0x6E, &Id);
  if (EFI_ERROR (Status)) { return Status; }
  return Id == 0x81 ? EFI_SUCCESS : EFI_UNSUPPORTED;
}

STATIC EFI_STATUS
Sc8581Update (UINT8 Reg, UINT8 Mask, UINT8 Value)
{
  UINT8 Old;
  EFI_STATUS Status;
  Status = Sc8581Read (Reg, &Old);
  if (EFI_ERROR (Status)) { return Status; }
  return Sc8581Write (Reg, (UINT8)((Old & (UINT8)~Mask) | (Value & Mask)));
}

typedef struct {
  UINT8 Reg;
  UINT8 Mask;
  UINT8 Value;
} SC8581_SETTING;

STATIC EFI_STATUS
Sc8581Apply (CONST SC8581_SETTING *Table, UINTN Count)
{
  UINTN Index;
  EFI_STATUS Status;
  for (Index = 0; Index < Count; Index++) {
    Status = Sc8581Update (Table[Index].Reg, Table[Index].Mask, Table[Index].Value);
    if (EFI_ERROR (Status)) { return Status; }
  }
  return EFI_SUCCESS;
}

STATIC EFI_STATUS
Sc8581SetMode (UINT8 Mode)
{
  STATIC CONST SC8581_SETTING Protection[] = {
    { 0x01, 0x80, 0x00 }, { 0x02, 0x80, 0x80 },
    { 0x06, 0x80, 0x00 }, { 0x07, 0x80, 0x00 },
    { 0x08, 0x80, 0x00 }, { 0x09, 0x80, 0x00 },
    { 0x6C, 0x80, 0x00 }, { 0x6D, 0x80, 0x00 },
    { 0x01, 0x1F, 12 }, { 0x02, 0x0F, 4 },
    { 0x6C, 0x1F, 8 }, { 0x05, 0xFC, 0xA0 },
    { 0x03, 0x0F, 3 }, { 0x06, 0x1F, 27 },
    { 0x04, 0x0F, 10 }, { 0x05, 0x03, 1 },
    { 0x09, 0x07, 1 }, { 0x08, 0x07, 4 }
  };
  EFI_STATUS Status = Sc8581Apply (Protection, ARRAY_SIZE (Protection));
  if (EFI_ERROR (Status)) { return Status; }
  return Sc8581Update (0x0E, 0x07, Mode);
}

STATIC EFI_STATUS
Sc8581Initialize (VOID)
{
  STATIC CONST SC8581_SETTING Init[] = {
    {0x0E,0x08,0x08}, {0x0E,0x80,0}, {0x0B,0x38,0x38},
    {0x0D,0x38,0x28}, {0x07,0x30,0x10}, {0x7C,0x08,0x08},
    {0x0E,0x10,0}, {0x70,0x08,0}, {0x0C,0x03,0x03},
    {0x03,0x80,0}, {0x15,0x40,0}, {0x15,0x01,0},
    {0x16,0x80,0}, {0x16,0x40,0}, {0x16,0x20,0},
    {0x16,0x10,0}, {0x16,0x08,0}, {0x16,0x04,0},
    {0x16,0x02,0}, {0x16,0x01,0}, {0x15,0x80,0},
    {0x6C,0,0}, {0x6D,0,0}
  };
  EFI_STATUS Status = Sc8581Apply (Init, ARRAY_SIZE (Init));
  if (EFI_ERROR (Status)) { return Status; }
  Status = Sc8581SetMode (1);
  if (EFI_ERROR (Status)) { return Status; }
  Status = Sc8581Update (0x0F, 0x20, 0x20);
  if (EFI_ERROR (Status)) { return Status; }
  Status = Sc8581Update (0x7C, 0x01, 0x01);
  if (EFI_ERROR (Status)) { return Status; }
  Status = Sc8581Update (0x0C, 0xF8, 0x60);
  if (EFI_ERROR (Status)) { return Status; }
  return Sc8581Write (0x42, 0x82);
}

STATIC EFI_STATUS
Sc8581OtgConfig (BOOLEAN Enable)
{
  EFI_STATUS Status;
  UINT8 Mode;
  UINT8 Control;
  UINTN Retry;
  Status = Sc8581CheckId ();
  if (EFI_ERROR (Status)) { return Status; }
  for (Retry = 0; Retry < 10; Retry++) {
    if (Enable) {
      Status = Sc8581SetMode (5);
      if (!EFI_ERROR (Status)) { Status = Sc8581Update (0x0B, 0x08, 0x08); }
      if (!EFI_ERROR (Status)) { Status = Sc8581Read (0x0B, &Control); }
      if (!EFI_ERROR (Status)) { Status = Sc8581Read (0x0E, &Mode); }
      if (EFI_ERROR (Status)) { return Status; }
      if (((Control & 0x08) != 0) && ((Mode & 0x07) == 5)) {
        return EFI_SUCCESS;
      }
    } else {
      Status = Sc8581Update (0x0B, 0x80, 0);
      if (!EFI_ERROR (Status)) { Status = Sc8581Update (0x0B, 0x40, 0); }
      if (!EFI_ERROR (Status)) { Status = Sc8581SetMode (1); }
      if (!EFI_ERROR (Status)) { Status = Sc8581Read (0x0E, &Mode); }
      if (!EFI_ERROR (Status)) { Status = Sc8581Read (0x0B, &Control); }
      if (EFI_ERROR (Status)) { return Status; }
      if (((Mode & 0x07) == 1) && ((Control & 0x80) == 0)) {
        return EFI_SUCCESS;
      }
    }
    MicroSecondDelay (20000);
  }
  return EFI_TIMEOUT;
}

STATIC EFI_STATUS
Sc8581Open (VOID)
{
  ONYX_TLMM *Tlmm;
  EFI_STATUS Status;
  UINT32 Result;
  UINT32 Nlpm = 0x20000000U | (126U << 4) | BIT14;
  if (mChargePump.Initialized) { return EFI_SUCCESS; }
  Status = gBS->LocateProtocol (&mI2cGuid, NULL, (VOID **)&mChargePump.Protocol);
  if (EFI_ERROR (Status)) { return Status; }
  if ((mChargePump.Protocol->Revision != 0x10000) ||
      (mChargePump.Protocol->Open == NULL) ||
      (mChargePump.Protocol->Read == NULL) ||
      (mChargePump.Protocol->Write == NULL) ||
      (mChargePump.Protocol->Close == NULL)) { return EFI_UNSUPPORTED; }
  Status = gBS->LocateProtocol (&mTlmmGuid, NULL, (VOID **)&Tlmm);
  if (EFI_ERROR (Status)) { return Status; }
  if ((Tlmm->Revision != 1) || (Tlmm->ConfigGpio == NULL) ||
      (Tlmm->GpioOut == NULL)) { return EFI_UNSUPPORTED; }
  Status = Tlmm->ConfigGpio (Nlpm, 1);
  if (EFI_ERROR (Status)) { return Status; }
  Status = Tlmm->GpioOut (Nlpm, 1);
  if (EFI_ERROR (Status)) { return Status; }
  MicroSecondDelay (400000);
  mChargePump.Config.FrequencyKhz = 400;
  mChargePump.Config.SlaveAddress = 0x6F;
  mChargePump.Config.StretchUs = 500;
  Result = mChargePump.Protocol->Open (12, &mChargePump.Handle);
  if ((Result != 0) || (mChargePump.Handle == NULL)) {
    mChargePump.Handle = NULL;
    return EFI_DEVICE_ERROR;
  }
  Status = Sc8581CheckId ();
  if (!EFI_ERROR (Status)) {
    mChargePump.Initialized = TRUE;
    Status = Sc8581Initialize ();
  }
  if (EFI_ERROR (Status)) {
    mChargePump.Protocol->Close (mChargePump.Handle);
    mChargePump.Handle = NULL;
    mChargePump.Initialized = FALSE;
    return Status;
  }
  return EFI_SUCCESS;
}

STATIC VOID
QueueNotification (UINT32 Type, UINT32 Value)
{
  UINTN Next = (mNotifyHead + 1) % ONYX_MCA_QUEUE_SIZE;
  if (Next == mNotifyTail) {
    mNotifyOverflow = TRUE;
    return;
  }
  mNotifications[mNotifyHead].Type = Type;
  mNotifications[mNotifyHead].Value = Value;
  mNotifyHead = Next;
}

STATIC VOID EFIAPI
RouterTransmitDone (VOID *Handle, VOID *Private, VOID *PacketPrivate,
                    CONST VOID *Packet, UINTN Length)
{
  if ((PacketPrivate == &mRequest) && (Packet == &mRequest)) {
    mRequestTxDone = TRUE;
    if (mRequestTimedOut) { mRequestBusy = FALSE; }
    return;
  }
  if (mRouterTransmitDone != NULL) {
    mRouterTransmitDone (Handle, Private, PacketPrivate, Packet, Length);
  }
}

STATIC VOID EFIAPI
RouterStateChanged (VOID *Handle, VOID *Private, UINT32 State)
{
  mRouterUp = (State == 0);
  mRouterHandle = mRouterUp ? Handle : NULL;
  mMcaSynchronized = FALSE;
  mSyncIndex = 0;
  mSyncAwaitingReply = FALSE;
  mSyncRetries = 0;
  mRetryAt = 0;
  mRequestDone = FALSE;
  if (!mRequestBusy || mRequestTxDone) { mRequestBusy = FALSE; }
  if (mRequestBusy) { mRequestTimedOut = TRUE; }
  mNativeRequest = FALSE;
  mCpWorkActive = FALSE;
  mCpWorkPhase = 0;
  if (!mRouterUp) {
    mNotifyHead = mNotifyTail = 0;
    mNotifyOverflow = FALSE;
    mCpRevert = 0;
    if (mSupplyStarted && mChargePump.Initialized) {
      mOtgDisablePending = TRUE;
    }
  }
  if (mRouterState != NULL) { mRouterState (Handle, Private, State); }
}

STATIC VOID EFIAPI
RouterReceive (VOID *Handle, VOID *Private, VOID *PacketPrivate,
               CONST VOID *Packet, UINTN Length, UINTN IntentSize)
{
  CONST ONYX_MCA_HEADER *Header = Packet;
  CONST ONYX_MCA_REPLY *Reply = Packet;
  CONST ONYX_MCA_NOTIFY_PACKET *Notify = Packet;
  INT32 Raw = 0;
  EFI_STATUS Status;

  if ((Packet == NULL) || (Length < sizeof (*Header)) || (Header->Owner != ONYX_MCA_OWNER)) {
    goto Forward;
  }
  if ((Header->Type == ONYX_MCA_READ) && (Length == sizeof (*Reply))) {
    if (mRequestBusy && !mRequestTimedOut && (Header->Opcode == mRequest.Header.Opcode) &&
        (Header->Property == mRequest.Header.Property) && (Reply->Sequence == mRequest.Sequence)) {
      mReplyResult = Reply->Result;
      mReplyValue = Reply->Data[0];
      mRequestDone = TRUE;
    } else if (mNativeRequest && (Header->Opcode == mNativeOpcode) &&
               (Header->Property == mNativeProperty) && (Reply->Sequence == mNativeSequence)) {
      mRouterReceive (Handle, Private, PacketPrivate, Packet, Length, IntentSize);
      mNativeRequest = FALSE;
      return;
    }
    // Also drain late/unknown MCA replies. The native callback would signal
    // its unrelated UCSI waiter even on an unknown packet.
  } else if ((Header->Type == ONYX_MCA_NOTIFY) && (Length == sizeof (*Notify))) {
    if ((Header->Property == 3) || (Header->Property == 9) || (Header->Property == 10)) {
      QueueNotification (Header->Property, Notify->Value);
    }
  } else {
    goto Forward;
  }
  Status = mGlink->RxDone (Handle, Packet, TRUE, &Raw);
  if (EFI_ERROR (Status) || (Raw != 0)) {
    mSupplyFault = TRUE;
    DEBUG ((DEBUG_ERROR, "OnyxPower: GLINK RxDone failed %r raw=%d\n", Status, Raw));
  }
  return;

Forward:
  mRouterReceive (Handle, Private, PacketPrivate, Packet, Length, IntentSize);
}

STATIC EFI_STATUS EFIAPI
RouterTransmit (VOID *Handle, VOID *Private, CONST VOID *Packet,
                UINTN Length, UINT32 Options, INT32 *Raw)
{
  CONST ONYX_MCA_REQUEST *Request = Packet;
  BOOLEAN IsMcaRequest = (Handle == mRouterHandle) && (Request != NULL) &&
    (Length == sizeof (*Request)) && (Request->Header.Owner == ONYX_MCA_OWNER) &&
    (Request->Header.Type == ONYX_MCA_READ) && ((Request->Header.Opcode == ONYX_MCA_READ) ||
                                                (Request->Header.Opcode == ONYX_MCA_WRITE));
  EFI_STATUS Status;
  if (IsMcaRequest) {
    if (mRequestBusy || mNativeRequest) {
      if (Raw != NULL) { *Raw = -6; }
      return EFI_NOT_READY;
    }
    mNativeProperty = Request->Header.Property;
    mNativeSequence = Request->Sequence;
    mNativeOpcode = Request->Header.Opcode;
    mNativeStarted = NowUs ();
    mNativeRequest = TRUE;
  }
  Status = mGlinkTransmit (Handle, Private, Packet, Length, Options, Raw);
  if (IsMcaRequest && (EFI_ERROR (Status) || (Raw != NULL && *Raw != 0))) {
    mNativeRequest = FALSE;
  }
  return Status;
}

STATIC EFI_STATUS EFIAPI
RouterOpen (ONYX_GLINK_OPEN_CONFIG *Config, VOID **Handle, INT32 *Raw)
{
  ONYX_GLINK_OPEN_CONFIG Wrapped;
  EFI_STATUS Status;
  if ((Config == NULL) || (Config->Transport == NULL) || (Config->Remote == NULL) ||
      (Config->Name == NULL) || (AsciiStrCmp (Config->Transport, "SMEM") != 0) ||
      (AsciiStrCmp (Config->Remote, "lpass") != 0) ||
      (AsciiStrCmp (Config->Name, "PMIC_RTR_ADSP_APPS") != 0)) {
    return mGlinkOpen (Config, Handle, Raw);
  }
  if (mRouterOpening || mRouterUp || (Config->Receive == NULL) ||
      (Config->State == NULL) || (Config->TransmitDone == NULL)) {
    if (Raw != NULL) { *Raw = -1; }
    return EFI_ACCESS_DENIED;
  }
  Wrapped = *Config;
  mRouterReceive = Config->Receive;
  mRouterState = Config->State;
  mRouterTransmitDone = Config->TransmitDone;
  Wrapped.Receive = RouterReceive;
  Wrapped.State = RouterStateChanged;
  Wrapped.TransmitDone = RouterTransmitDone;
  mRouterOpening = TRUE;
  Status = mGlinkOpen (&Wrapped, Handle, Raw);
  mRouterOpening = FALSE;
  if (EFI_ERROR (Status) || ((Raw != NULL) && (*Raw != 0))) {
    mRouterHandle = NULL;
    mRouterUp = FALSE;
  } else if (Handle != NULL) {
    mRouterHandle = *Handle;
  }
  return Status;
}

STATIC EFI_STATUS
InstallRouterAdapter (VOID)
{
  VOID *Existing;
  EFI_STATUS Status;
  if (mAdapterInstalled) { return EFI_ALREADY_STARTED; }
  Status = gBS->LocateProtocol (&mPmicGlinkGuid, NULL, &Existing);
  if (!EFI_ERROR (Status)) { return EFI_ALREADY_STARTED; }
  Status = gBS->LocateProtocol (&mGlinkGuid, NULL, (VOID **)&mGlink);
  if (EFI_ERROR (Status)) { mGlink = NULL; return Status; }
  if ((mGlink->Revision != 0x10001) || (mGlink->Open == NULL) ||
      (mGlink->Transmit == NULL) || (mGlink->RxDone == NULL) ||
      (mGlink->Poll == NULL)) {
    mGlink = NULL;
    return EFI_UNSUPPORTED;
  }
  mGlinkOpen = mGlink->Open;
  mGlinkTransmit = mGlink->Transmit;
  mGlink->Open = RouterOpen;
  mGlink->Transmit = RouterTransmit;
  mAdapterInstalled = TRUE;
  return EFI_SUCCESS;
}

STATIC EFI_STATUS
RouterBegin (UINT32 Opcode, UINT32 Property, CONST VOID *Payload, UINTN PayloadLength)
{
  EFI_STATUS Status;
  INT32 Raw = 0;
  if (!mRouterUp || (mRouterHandle == NULL) || mSupplyFault) { return EFI_NOT_READY; }
  if (mRequestBusy || mNativeRequest) { return EFI_NOT_READY; }
  if (PayloadLength > ONYX_MCA_DATA_SIZE) { return EFI_BAD_BUFFER_SIZE; }
  mRequest.Header.Owner = ONYX_MCA_OWNER;
  mRequest.Header.Type = ONYX_MCA_READ;
  mRequest.Header.Opcode = Opcode;
  mRequest.Header.Property = Property;
  mRequest.Sequence = ++mSequence;
  ZeroMem (mRequest.Data, sizeof (mRequest.Data));
  if ((Payload != NULL) && (PayloadLength != 0)) {
    CopyMem (mRequest.Data, Payload, PayloadLength);
  }
  mRequestBusy = TRUE;
  mRequestDone = FALSE;
  mRequestTxDone = FALSE;
  mRequestTimedOut = FALSE;
  mRequestStarted = NowUs ();
  Status = mGlinkTransmit (mRouterHandle, &mRequest, &mRequest, sizeof (mRequest), 0, &Raw);
  if (EFI_ERROR (Status) || (Raw != 0)) {
    mRequestBusy = FALSE;
    return EFI_DEVICE_ERROR;
  }
  return EFI_SUCCESS;
}

STATIC EFI_STATUS
RouterFinish (UINT8 *Value)
{
  if (!mRequestBusy) { return EFI_NOT_READY; }
  if (mRequestDone && mRequestTxDone) {
    mRequestBusy = FALSE;
    if (mReplyResult != 0) { return EFI_DEVICE_ERROR; }
    if (Value != NULL) { *Value = mReplyValue; }
    return EFI_SUCCESS;
  }
  if (NowUs () - mRequestStarted < ONYX_MCA_TIMEOUT_US) { return EFI_NOT_READY; }
  // TxDone releases the transport's reference to mRequest. Until then the
  // buffer must stay pinned; afterwards a new sequence safely rejects late RX.
  mRequestTimedOut = TRUE;
  if (mRequestTxDone) { mRequestBusy = FALSE; }
  DEBUG ((DEBUG_ERROR, "[OnyxPower] MCA timeout property=%x tx=%u rx=%u\n",
          mRequest.Header.Property, mRequestTxDone, mRequestDone));
  return EFI_TIMEOUT;
}

STATIC EFI_STATUS
SynchronizeMca (VOID)
{
  // Android MCA startup properties, in their required write order.
  STATIC CONST struct {
    UINT32 Property;
    UINT32 Length;
    UINT32 Data[4];
  } Settings[] = {
    { 0x201C, 1,  { 1 } },                           // Charge pump present
    { 0x201A, 16, { 0, 1, MAX_UINT32, MAX_UINT32 } }, // OTG configuration
    { 0x201B, 4,  { 0x0704 } },                      // CID configuration
    { 0x2028, 1,  { 0 } }                            // EU model
  };
  EFI_STATUS Status;
  if (mMcaSynchronized) { return EFI_SUCCESS; }
  if (NowUs () < mRetryAt) { return EFI_NOT_READY; }
  // StartSupply initializes the charge pump before arming this worker.
  if (!mSyncAwaitingReply) {
    Status = RouterBegin (ONYX_MCA_WRITE, Settings[mSyncIndex].Property,
                          Settings[mSyncIndex].Data, Settings[mSyncIndex].Length);
    if (Status == EFI_NOT_READY) { return Status; }
    if (EFI_ERROR (Status)) { goto Retry; }
    mSyncAwaitingReply = TRUE;
    return EFI_NOT_READY;
  }
  Status = RouterFinish (NULL);
  if (Status == EFI_NOT_READY) { return Status; }
  if (EFI_ERROR (Status)) { goto Retry; }
  mSyncAwaitingReply = FALSE;
  if (++mSyncIndex == ARRAY_SIZE (Settings)) {
    mMcaSynchronized = TRUE;
    return EFI_SUCCESS;
  }
  return EFI_NOT_READY;
Retry:
  mSyncAwaitingReply = FALSE;
  mSyncIndex = 0;
  mRetryAt = NowUs () + ONYX_RETRY_US;
  mSyncRetries++;
  if ((mSyncRetries == 1) || ((mSyncRetries % 10) == 0)) {
    DEBUG ((DEBUG_WARN, "[OnyxPower] MCA sync retry=%u status=%r\n", mSyncRetries, Status));
  }
  return Status;
}

STATIC VOID
PopNotification (VOID)
{
  mNotifyTail = (mNotifyTail + 1) % ONYX_MCA_QUEUE_SIZE;
  mCpWorkActive = FALSE;
  mCpWorkPhase = 0;
  mCpWorkRetries = 0;
}

STATIC EFI_STATUS
BeginCpRevert (UINT32 AuthPosition)
{
  EFI_STATUS Status;
  if (!mCpWorkActive) {
    mCpWorkActive = TRUE;
    mCpWorkPhase = 0;
    mCpWorkPosition = AuthPosition & 0xFF;
    mCpWorkPresent = 0;
  }
  if (mCpWorkPhase == 0) {
    if (((AuthPosition >> 24) & 0xFF) != 0) {
      mCpWorkPresent = 0;
      mCpWorkPhase = 2;
      return EFI_NOT_READY;
    }
    Status = RouterBegin (ONYX_MCA_READ, 0x21020, NULL, 0);
    if (EFI_ERROR (Status)) { return Status; }
    mCpWorkPhase = 1;
    return EFI_NOT_READY;
  }
  if (mCpWorkPhase == 1) {
    Status = RouterFinish (&mCpWorkPresent);
    if (Status != EFI_SUCCESS) { return Status; }
    mCpWorkPhase = 2;
  }
  if (mCpWorkPhase == 2) {
    UINT8 Byte;
    if (mCpWorkPresent && (mCpWorkPosition == mLastCpPosition)) {
      mCpWorkPhase = 6;
    } else if (mCpWorkPresent && (mCpWorkPosition == 2)) {
      Status = Sc8581Update (0x0B, 0x20, 0x20);
      if (!EFI_ERROR (Status)) { Status = Sc8581Update (0x0D, 0xFF, 0); }
      if (!EFI_ERROR (Status)) { Status = Sc8581SetMode (5); }
      if (!EFI_ERROR (Status)) { Status = Sc8581Update (0x0B, 0x10, 0); }
      if (!EFI_ERROR (Status)) { Status = Sc8581Update (0x0B, 0x08, 0x08); }
      if (!EFI_ERROR (Status)) { Status = Sc8581Read (0x0B, &Byte); }
      if (!EFI_ERROR (Status) && ((Byte & 0x08) == 0)) { Status = EFI_DEVICE_ERROR; }
      if (!EFI_ERROR (Status)) { Status = Sc8581Update (0x0B, 0x40, 0x40); }
      if (!EFI_ERROR (Status)) { Status = Sc8581Update (0x0B, 0x80, 0x80); }
      if (EFI_ERROR (Status)) { return Status; }
      mCpWorkDue = NowUs () + 400000;
      mCpWorkPhase = 3;
      return EFI_NOT_READY;
    } else if (!mCpWorkPresent) {
      Status = Sc8581Update (0x0D, 0xFF, 40);
      if (EFI_ERROR (Status)) { return Status; }
      mCpWorkDue = NowUs () + 400000;
      mCpWorkPhase = 3;
      return EFI_NOT_READY;
    } else {
      mCpWorkPhase = 6;
    }
  }
  if (mCpWorkPhase == 3) {
    UINT32 BoostDisable = 0x200;
    if (NowUs () < mCpWorkDue) { return EFI_NOT_READY; }
    Status = RouterBegin (ONYX_MCA_WRITE, 0x200F, &BoostDisable, sizeof (BoostDisable));
    if (EFI_ERROR (Status)) { return Status; }
    mCpWorkPhase = 4;
    return EFI_NOT_READY;
  }
  if (mCpWorkPhase == 4) {
    Status = RouterFinish (NULL);
    if (Status != EFI_SUCCESS) { return Status; }
    mCpWorkPhase = 5;
  }
  if (mCpWorkPhase == 5) {
    if (!mCpWorkPresent) {
      Status = Sc8581Write (0x0D, 40);
      if (EFI_ERROR (Status)) { return Status; }
      mCpWorkPosition = 1;
    }
    mCpWorkPhase = 6;
  }
  if (mCpWorkPhase == 6) {
    mLastCpPosition = mCpWorkPresent ? mCpWorkPosition : 1;
    return EFI_SUCCESS;
  }
  return EFI_NOT_READY;
}

STATIC EFI_STATUS
ServiceNotifications (VOID)
{
  ONYX_MCA_NOTIFICATION *Notification;
  EFI_STATUS Status;
  UINT32 Value;
  if (mNotifyOverflow) {
    mSupplyFault = TRUE;
    if (mChargePump.Initialized) { Sc8581OtgConfig (FALSE); }
    DEBUG ((DEBUG_ERROR, "[OnyxPower] notification queue overflow; supply disabled\n"));
    return EFI_OUT_OF_RESOURCES;
  }
  if (NowUs () < mRetryAt) { return EFI_NOT_READY; }
  if (mNotifyTail == mNotifyHead) { return EFI_SUCCESS; }
  Notification = &mNotifications[mNotifyTail];
  if ((Notification->Type == 3) || (Notification->Type == 9)) {
    if (Notification->Type == 9) {
      mCpRevert = Notification->Value;
    } else if (Notification->Value == 0) {
      mCpRevert = 0;
    }
    Value = mCpRevert;
    Status = BeginCpRevert (Value);
    if (Status == EFI_SUCCESS) {
      PopNotification ();
      return EFI_SUCCESS;
    }
    if (Status == EFI_NOT_READY) { return Status; }
  } else if (Notification->Type == 10) {
    Status = Sc8581OtgConfig (Notification->Value != 0);
    if (!EFI_ERROR (Status)) {
      PopNotification ();
      return EFI_SUCCESS;
    }
  } else {
    PopNotification ();
    return EFI_SUCCESS;
  }
  mCpWorkRetries++;
  mCpWorkActive = FALSE;
  mCpWorkPhase = 0;
  mRetryAt = NowUs () + ONYX_RETRY_US;
  if (mCpWorkRetries >= 10) {
    mSupplyFault = TRUE;
    if (mChargePump.Initialized) { Sc8581OtgConfig (FALSE); }
    DEBUG ((DEBUG_ERROR, "[OnyxPower] notification type=%u failed: %r\n",
            Notification->Type, Status));
    return Status;
  }
  return EFI_NOT_READY;
}

STATIC BOOLEAN
PilNameMatches (CONST CHAR16 *Name)
{
  STATIC CONST CHAR16 Expected[] = L"FULL_ADSP";
  UINTN Index;
  for (Index = 0; Index < ARRAY_SIZE (Expected); Index++) {
    if (CharToUpper (Name[Index]) != Expected[Index]) { return FALSE; }
  }
  return TRUE;
}

STATIC EFI_STATUS
ServiceBootstrap (VOID)
{
  EFI_STATUS Status;
  BOOLEAN Connected;
  CONST CHAR16 *Name;
  if (mBootPhase == 5) { return mBootStatus; }
  if (mBootPhase == 6) { return EFI_SUCCESS; }
  if (mBootPhase == 0) {
    UINT64 PilConfig[0x238 / sizeof (UINT64)];
    Status = gBS->LocateProtocol (&mPilGuid, NULL, (VOID **)&mPil);
    if (EFI_ERROR (Status)) { return EFI_NOT_READY; }
    Status = gBS->LocateProtocol (&mPmicGlinkGuid, NULL, (VOID **)&mPmicRouter);
    if (EFI_ERROR (Status)) { return EFI_NOT_READY; }
    Status = gBS->LocateProtocol (&mPmicBootGuid, NULL, (VOID **)&mPmicBoot);
    if (EFI_ERROR (Status)) { return EFI_NOT_READY; }
    if ((mPil->Revision != 0x10005) || (mPmicRouter->Revision != 0x1000B) ||
        (mPmicBoot->Revision != 0x10026) || (mPil->Load == NULL) ||
        (mPil->GetConfig == NULL) || (mPmicRouter->Connect == NULL) ||
        (mPmicRouter->Connected == NULL) || (mPmicBoot->SetOs == NULL)) {
      Status = EFI_UNSUPPORTED;
      goto Failed;
    }
    Connected = FALSE;
    Status = mPmicRouter->Connected (&Connected);
    if (EFI_ERROR (Status)) { goto Failed; }
    if (Connected && mRouterUp) {
      mBootPhase = 6;
      mBootStatus = EFI_SUCCESS;
      return EFI_SUCCESS;
    }
    ZeroMem (PilConfig, sizeof (PilConfig));
    Status = mPil->GetConfig (L"FULL_ADSP", PilConfig);
    if (Status != EFI_SUCCESS) { goto Failed; }
    Name = (CONST CHAR16 *)((CONST UINT8 *)PilConfig + 0x0C);
    if (!PilNameMatches (Name) || (PilConfig[0] == 0)) {
      Status = EFI_COMPROMISED_DATA;
      goto Failed;
    }
    mBootPhase = 1;
    return EFI_NOT_READY;
  }
  if (mBootPhase == 1) {
    Status = mPmicBoot->SetOs (0);
    if (Status != EFI_SUCCESS) { goto Failed; }
    mBootPhase = 2;
    return EFI_NOT_READY;
  }
  if (mBootPhase == 2) {
    Status = mPil->Load (L"FULL_ADSP");
    if (Status != EFI_SUCCESS) { goto Failed; }
    mBootPhase = 3;
    return EFI_NOT_READY;
  }
  if (mBootPhase == 3) {
    Connected = FALSE;
    Status = mPmicRouter->Connected (&Connected);
    if (EFI_ERROR (Status)) { goto Failed; }
    // Connect clears the native connection flag; never reconnect a live link.
    if (!Connected) {
      Status = mPmicRouter->Connect ();
      if (Status != EFI_SUCCESS) { goto Failed; }
    }
    mBootPhase = 4;
    return EFI_NOT_READY;
  }
  if (mBootPhase == 4) {
    Connected = FALSE;
    Status = mPmicRouter->Connected (&Connected);
    if (EFI_ERROR (Status)) { goto Failed; }
    if (!Connected || !mRouterUp) { return EFI_NOT_READY; }
    mBootPhase = 6;
    mBootStatus = EFI_SUCCESS;
    return EFI_SUCCESS;
  }
  Status = EFI_UNSUPPORTED;
Failed:
  DEBUG ((DEBUG_ERROR, "[OnyxPower] ADSP bootstrap failed phase=%u status=%r\n",
          mBootPhase, Status));
  mBootPhase = 5;
  mBootStatus = Status;
  return Status;
}

STATIC VOID EFIAPI
SupplyWorker (EFI_EVENT Event, VOID *Context)
{
  EFI_STATUS Status;
  INT32 Raw = 0;
  if (mStopping || mStartupBusy || mWorkerBusy || !mSupplyStarted ||
      (mBootPhase != 6)) { return; }
  mWorkerBusy = TRUE;
  if (mOtgDisablePending && mChargePump.Initialized) {
    Status = Sc8581OtgConfig (FALSE);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "OnyxPower: SC8581 disconnect shutdown failed %r\n", Status));
      goto Done;
    }
    mOtgDisablePending = FALSE;
  }
  if (!mRouterUp || (mRouterHandle == NULL)) { goto Done; }
  Status = mGlink->Poll (mRouterHandle, &Raw);
  if (EFI_ERROR (Status) || (Raw != 0)) {
    DEBUG ((DEBUG_ERROR, "OnyxPower: GLink Poll failed %r raw=%d\n", Status, Raw));
    goto Done;
  }
  if (mNativeRequest && (NowUs () - mNativeStarted >= 25000000)) {
    DEBUG ((DEBUG_ERROR, "OnyxPower: native MCA request timed out property=%x\n",
            mNativeProperty));
    mNativeRequest = FALSE;
  }
  Status = SynchronizeMca ();
  if (EFI_ERROR (Status) && (Status != EFI_NOT_READY)) { goto Done; }
  if (mMcaSynchronized && !mSupplyFault) {
    Status = ServiceNotifications ();
    if (EFI_ERROR (Status) && (Status != EFI_NOT_READY)) {
      DEBUG ((DEBUG_ERROR, "OnyxPower: notification worker failed %r\n", Status));
    }
  }
Done:
  mWorkerBusy = FALSE;
}

STATIC EFI_STATUS EFIAPI
StartSupply (VOID)
{
  EFI_STATUS Status;
  EFI_TPL OldTpl;
  UINT64 Started;
  UINTN Phase;
  INT32 Raw;
  OldTpl = gBS->RaiseTPL (TPL_HIGH_LEVEL);
  gBS->RestoreTPL (OldTpl);
  if (OldTpl != TPL_APPLICATION) { return EFI_UNSUPPORTED; }
  if (mStopping || mStartupBusy) { return EFI_NOT_READY; }
  if (mSupplyStarted) { return EFI_ALREADY_STARTED; }
  if (!mAdapterInstalled || (mWorkerEvent == NULL)) { return EFI_NOT_READY; }
  mStartupBusy = TRUE;
  Started = NowUs ();
  Phase = mBootPhase;
  do {
    // Pump the handshake while waiting for both native and wrapped state.
    if (mRouterHandle != NULL) {
      Raw = 0;
      Status = mGlink->Poll (mRouterHandle, &Raw);
      if (EFI_ERROR (Status) || (Raw != 0)) {
        Status = EFI_DEVICE_ERROR;
        goto Done;
      }
    }
    Status = ServiceBootstrap ();
    if ((Status != EFI_NOT_READY) || (mBootPhase == 5)) { break; }
    if (Phase != mBootPhase) {
      Phase = mBootPhase;
      Started = NowUs ();
    }
    if (NowUs () - Started >= ONYX_BOOT_TIMEOUT_US) {
      Status = EFI_TIMEOUT;
      DEBUG ((DEBUG_ERROR, "[OnyxPower] bootstrap timeout phase=%u router=%u\n",
              mBootPhase, mRouterUp));
      goto Done;
    }
    MicroSecondDelay (10000);
  } while (!mStopping);
  if (EFI_ERROR (Status)) { goto Done; }
  // GPIO nLPM settling and initial I2C setup also run at application TPL.
  Status = Sc8581Open ();
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[OnyxPower] SC8581 initialization failed: %r\n", Status));
    goto Done;
  }
  Status = gBS->SetTimer (mWorkerEvent, TimerPeriodic, ONYX_POWER_POLL_PERIOD);
  if (!EFI_ERROR (Status)) {
    mSupplyStarted = TRUE;
  }
Done:
  mStartupBusy = FALSE;
  return Status;
}

STATIC ONYX_SUPPLY_START_PROTOCOL mSupplyProtocol = { 1, StartSupply };

STATIC VOID EFIAPI
ExitBootServices (EFI_EVENT Event, VOID *Context)
{
  mStopping = TRUE;
  if (mWorkerEvent != NULL) { gBS->SetTimer (mWorkerEvent, TimerCancel, 0); }
}

EFI_STATUS EFIAPI
OnyxUsbSupplyDxeEntry (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status;
  EFI_STATUS CleanupStatus;
  if (mAdapterInstalled) { return EFI_ALREADY_STARTED; }
  Status = gBS->CreateEvent (EVT_TIMER | EVT_NOTIFY_SIGNAL, TPL_CALLBACK,
                             SupplyWorker, NULL, &mWorkerEvent);
  if (EFI_ERROR (Status)) { return Status; }
  Status = gBS->CreateEventEx (EVT_NOTIFY_SIGNAL, TPL_NOTIFY,
                               ExitBootServices, NULL,
                               &gEfiEventExitBootServicesGuid, &mExitEvent);
  if (EFI_ERROR (Status)) { goto Failed; }
  Status = gBS->InstallProtocolInterface (&mSupplyHandle, &mSupplyStartGuid,
                                          EFI_NATIVE_INTERFACE, &mSupplyProtocol);
  if (EFI_ERROR (Status)) { goto Failed; }
  // Last mandatory fallible operation: the image must not return an error after
  // publishing its callbacks in another driver's protocol table.
  Status = InstallRouterAdapter ();
  if (EFI_ERROR (Status)) {
    CleanupStatus = gBS->UninstallProtocolInterface (mSupplyHandle, &mSupplyStartGuid,
                                                    &mSupplyProtocol);
    if (EFI_ERROR (CleanupStatus)) {
      // Keep the image resident if another driver retained the start protocol.
      // StartSupply rejects calls while the adapter is absent.
      DEBUG ((DEBUG_ERROR, "[OnyxPower] setup=%r cleanup=%r; image retained\n",
              Status, CleanupStatus));
      return EFI_SUCCESS;
    }
    mSupplyHandle = NULL;
    goto Failed;
  }
  return EFI_SUCCESS;
Failed:
  if (mExitEvent != NULL) { gBS->CloseEvent (mExitEvent); mExitEvent = NULL; }
  if (mWorkerEvent != NULL) { gBS->CloseEvent (mWorkerEvent); mWorkerEvent = NULL; }
  DEBUG ((DEBUG_ERROR, "[OnyxPower] adapter setup failed: %r\n", Status));
  return Status;
}

#endif // ONYX_SUPPLY_BOOTSTRAP_BUILD
