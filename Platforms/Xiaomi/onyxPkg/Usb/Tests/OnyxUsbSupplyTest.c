#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#undef NULL
#include "../OnyxUsbSupplyDxe.c"

EFI_BOOT_SERVICES *gBS;
EFI_GUID gEfiEventExitBootServicesGuid;

#define CHECK(Expression) do { \
  if (!(Expression)) { \
    fprintf (stderr, "line %d: %s\n", __LINE__, #Expression); \
    exit (1); \
  } \
} while (0)

STATIC UINT8 mRegisters[256];
STATIC UINT8 mWriteRegisters[512];
STATIC UINTN mWriteCount;
STATIC UINTN mReadCount;
STATIC BOOLEAN mIgnoreOvpGate;
STATIC UINT64 mTestTicks;
STATIC UINT32 mLastProperty;
STATIC ONYX_I2C mTestI2c;

typedef enum {
  FailNothing,
  FailWorkerEvent,
  FailExitEvent,
  FailProtocolInstall,
  FailAdapter
} FAILURE_POINT;

typedef struct {
  BOOLEAN Live;
  EFI_TPL Tpl;
  EFI_EVENT_NOTIFY Notify;
} TEST_EVENT;

STATIC EFI_BOOT_SERVICES mBootServices;
STATIC ONYX_GLINK mTestGlink;
STATIC TEST_EVENT mTestEvents[2];
STATIC FAILURE_POINT mFailure;
STATIC EFI_TPL mTestTpl;
STATIC UINTN mCreateCalls;
STATIC UINTN mCreateExCalls;
STATIC UINTN mInstallCalls;
STATIC UINTN mUninstallCalls;
STATIC UINTN mCloseCalls;
STATIC UINTN mTimerCalls;
STATIC UINTN mLocateCalls;
STATIC UINTN mPollCalls;
STATIC UINTN mPollDepth;
STATIC UINTN mRxDoneCalls;
STATIC UINTN mNativeRxCalls;
STATIC UINTN mTransportTxCalls;
STATIC BOOLEAN mInstalled;
STATIC BOOLEAN mTimerArmed;
STATIC EFI_STATUS mPollStatus;
STATIC EFI_STATUS mTimerStatus;
STATIC EFI_STATUS mUninstallStatus;
STATIC EFI_STATUS mPilLoadStatus;
STATIC BOOLEAN mBootstrapDependencies;
STATIC BOOLEAN mTestConnected;
STATIC ONYX_PIL mTestPil;
STATIC ONYX_PMIC_ROUTER mTestPmicRouter;
STATIC ONYX_PMIC_BOOT mTestPmicBoot;
STATIC ONYX_TLMM mTestTlmm;
STATIC UINTN mGetConfigCalls;
STATIC UINTN mSetOsCalls;
STATIC UINTN mPilLoadCalls;
STATIC UINTN mConnectCalls;
STATIC UINTN mI2cOpenCalls;
STATIC EFI_STATUS EFIAPI
OriginalOpen (ONYX_GLINK_OPEN_CONFIG *Config, VOID **Handle, INT32 *Raw)
{
  return EFI_UNSUPPORTED;
}

STATIC EFI_STATUS EFIAPI
OriginalTransmit (VOID *Handle, VOID *Private, CONST VOID *Packet,
                  UINTN Length, UINT32 Options, INT32 *Raw)
{
  return EFI_UNSUPPORTED;
}

STATIC EFI_STATUS EFIAPI
FakeRxDone (VOID *Handle, CONST VOID *Packet, BOOLEAN Reuse, INT32 *Raw)
{
  mRxDoneCalls++;
  *Raw = 0;
  return EFI_SUCCESS;
}

STATIC VOID
CheckUnhooked (VOID)
{
  CHECK (mTestGlink.Open == OriginalOpen);
  CHECK (mTestGlink.Transmit == OriginalTransmit);
}

STATIC EFI_STATUS EFIAPI
FakeLocate (EFI_GUID *Protocol, VOID *Registration, VOID **Interface)
{
  mLocateCalls++;
  if (memcmp (Protocol, &mGlinkGuid, sizeof (*Protocol)) == 0) {
    *Interface = &mTestGlink;
    return EFI_SUCCESS;
  }
  if (mBootstrapDependencies) {
    if (memcmp (Protocol, &mPilGuid, sizeof (*Protocol)) == 0) {
      *Interface = &mTestPil;
    } else if (memcmp (Protocol, &mPmicGlinkGuid, sizeof (*Protocol)) == 0) {
      *Interface = &mTestPmicRouter;
    } else if (memcmp (Protocol, &mPmicBootGuid, sizeof (*Protocol)) == 0) {
      *Interface = &mTestPmicBoot;
    } else if (memcmp (Protocol, &mI2cGuid, sizeof (*Protocol)) == 0) {
      *Interface = &mTestI2c;
    } else if (memcmp (Protocol, &mTlmmGuid, sizeof (*Protocol)) == 0) {
      *Interface = &mTestTlmm;
    } else {
      return EFI_NOT_FOUND;
    }
    return EFI_SUCCESS;
  }
  return EFI_NOT_FOUND;
}

STATIC EFI_TPL EFIAPI
FakeRaiseTpl (EFI_TPL NewTpl)
{
  EFI_TPL OldTpl = mTestTpl;
  CHECK (NewTpl >= OldTpl);
  mTestTpl = NewTpl;
  return OldTpl;
}

STATIC VOID EFIAPI
FakeRestoreTpl (EFI_TPL OldTpl)
{
  CHECK (OldTpl <= mTestTpl);
  mTestTpl = OldTpl;
}

STATIC EFI_STATUS EFIAPI
FakeCreateEvent (UINT32 Type, EFI_TPL NotifyTpl, EFI_EVENT_NOTIFY Notify,
                 VOID *Context, EFI_EVENT *Event)
{
  mCreateCalls++;
  CheckUnhooked ();
  // Match this tree's DXE Core: signal callbacks at application TPL are invalid.
  if (((Type & (EVT_NOTIFY_SIGNAL | EVT_NOTIFY_WAIT)) != 0) &&
      ((NotifyTpl <= TPL_APPLICATION) || (NotifyTpl >= TPL_HIGH_LEVEL) ||
       (Notify == NULL))) {
    return EFI_INVALID_PARAMETER;
  }
  CHECK (Type == (EVT_TIMER | EVT_NOTIFY_SIGNAL));
  CHECK (Notify == SupplyWorker && Context == NULL);
  if (mFailure == FailWorkerEvent) { return EFI_OUT_OF_RESOURCES; }
  CHECK (!mTestEvents[0].Live);
  mTestEvents[0].Live = TRUE;
  mTestEvents[0].Tpl = NotifyTpl;
  mTestEvents[0].Notify = Notify;
  *Event = &mTestEvents[0];
  return EFI_SUCCESS;
}

STATIC EFI_STATUS EFIAPI
FakeCreateEventEx (UINT32 Type, EFI_TPL NotifyTpl, EFI_EVENT_NOTIFY Notify,
                   CONST VOID *Context, CONST EFI_GUID *Group, EFI_EVENT *Event)
{
  mCreateExCalls++;
  CheckUnhooked ();
  CHECK (Type == EVT_NOTIFY_SIGNAL && NotifyTpl == TPL_NOTIFY);
  CHECK (Notify == ExitBootServices && Context == NULL);
  CHECK (Group == &gEfiEventExitBootServicesGuid);
  if (mFailure == FailExitEvent) { return EFI_OUT_OF_RESOURCES; }
  CHECK (!mTestEvents[1].Live);
  mTestEvents[1].Live = TRUE;
  mTestEvents[1].Tpl = NotifyTpl;
  mTestEvents[1].Notify = Notify;
  *Event = &mTestEvents[1];
  return EFI_SUCCESS;
}

STATIC EFI_STATUS EFIAPI
FakeCloseEvent (EFI_EVENT Event)
{
  TEST_EVENT *Test = Event;
  CHECK (Test >= mTestEvents && Test < mTestEvents + ARRAY_SIZE (mTestEvents));
  CHECK (Test->Live);
  CheckUnhooked ();
  Test->Live = FALSE;
  mCloseCalls++;
  return EFI_SUCCESS;
}

STATIC EFI_STATUS EFIAPI
FakeInstallProtocol (EFI_HANDLE *Handle, EFI_GUID *Protocol,
                     EFI_INTERFACE_TYPE Type, VOID *Interface)
{
  mInstallCalls++;
  CheckUnhooked ();
  CHECK (Type == EFI_NATIVE_INTERFACE && Interface == &mSupplyProtocol);
  CHECK (mTestEvents[0].Live && mTestEvents[1].Live && !mInstalled);
  if (mFailure == FailProtocolInstall) { return EFI_OUT_OF_RESOURCES; }
  *Handle = (EFI_HANDLE)&mInstalled;
  mInstalled = TRUE;
  return EFI_SUCCESS;
}

STATIC EFI_STATUS EFIAPI
FakeUninstallProtocol (EFI_HANDLE Handle, EFI_GUID *Protocol, VOID *Interface)
{
  CHECK (Handle == (EFI_HANDLE)&mInstalled && Interface == &mSupplyProtocol);
  CHECK (mInstalled);
  CheckUnhooked ();
  mUninstallCalls++;
  if (!EFI_ERROR (mUninstallStatus)) { mInstalled = FALSE; }
  return mUninstallStatus;
}

STATIC EFI_STATUS EFIAPI
FakeSetTimer (EFI_EVENT Event, EFI_TIMER_DELAY Type, UINT64 TriggerTime)
{
  CHECK (Event == &mTestEvents[0] && mTestEvents[0].Live);
  mTimerCalls++;
  if (Type == TimerCancel) {
    CHECK (TriggerTime == 0);
    mTimerArmed = FALSE;
  } else {
    CHECK (Type == TimerPeriodic && TriggerTime == ONYX_POWER_POLL_PERIOD);
    CHECK (mBootPhase == 6 && mChargePump.Initialized);
    if (EFI_ERROR (mTimerStatus)) { return mTimerStatus; }
    mTimerArmed = TRUE;
  }
  return EFI_SUCCESS;
}

STATIC EFI_STATUS EFIAPI
FakePoll (VOID *Handle, INT32 *Raw)
{
  CHECK (Handle == mRouterHandle && (mWorkerBusy || mStartupBusy));
  CHECK (mPollDepth == 0);
  mPollCalls++;
  mPollDepth++;
  // Simulate a native callback recursively dispatching the same worker.
  SupplyWorker (mWorkerEvent, NULL);
  mPollDepth--;
  *Raw = 0;
  return mPollStatus;
}

STATIC VOID
ResetFirmware (FAILURE_POINT Failure)
{
  memset (&mBootServices, 0, sizeof (mBootServices));
  mBootServices.LocateProtocol = FakeLocate;
  mBootServices.RaiseTPL = FakeRaiseTpl;
  mBootServices.RestoreTPL = FakeRestoreTpl;
  mBootServices.CreateEvent = FakeCreateEvent;
  mBootServices.CreateEventEx = FakeCreateEventEx;
  mBootServices.CloseEvent = FakeCloseEvent;
  mBootServices.InstallProtocolInterface = FakeInstallProtocol;
  mBootServices.UninstallProtocolInterface = FakeUninstallProtocol;
  mBootServices.SetTimer = FakeSetTimer;
  gBS = &mBootServices;
  memset (&mTestGlink, 0, sizeof (mTestGlink));
  mTestGlink.Revision = (Failure == FailAdapter) ? 0 : 0x10001;
  mTestGlink.Open = OriginalOpen;
  mTestGlink.Transmit = OriginalTransmit;
  mTestGlink.RxDone = FakeRxDone;
  mTestGlink.Poll = FakePoll;
  memset (mTestEvents, 0, sizeof (mTestEvents));
  mFailure = Failure;
  mTestTpl = TPL_APPLICATION;
  mCreateCalls = mCreateExCalls = mInstallCalls = mUninstallCalls = 0;
  mCloseCalls = mTimerCalls = mLocateCalls = mPollCalls = mPollDepth = 0;
  mInstalled = mTimerArmed = FALSE;
  mPollStatus = mTimerStatus = mUninstallStatus = mPilLoadStatus = EFI_SUCCESS;
  mBootstrapDependencies = mTestConnected = FALSE;
  mGetConfigCalls = mSetOsCalls = mPilLoadCalls = mConnectCalls = mI2cOpenCalls = 0;
  mTestTicks = 0;
  memset (&mTestPil, 0, sizeof (mTestPil));
  memset (&mTestPmicRouter, 0, sizeof (mTestPmicRouter));
  memset (&mTestPmicBoot, 0, sizeof (mTestPmicBoot));
  memset (&mTestTlmm, 0, sizeof (mTestTlmm));
  mGlink = NULL;
  mGlinkOpen = NULL;
  mGlinkTransmit = NULL;
  mAdapterInstalled = mStopping = mStartupBusy = mWorkerBusy = FALSE;
  mSupplyStarted = mRouterUp = mRouterOpening = FALSE;
  mSupplyHandle = NULL;
  mRouterHandle = NULL;
  mWorkerEvent = mExitEvent = NULL;
  mBootPhase = 0;
  mBootStatus = EFI_NOT_READY;
  mNativeRequest = mRequestBusy = mRequestTimedOut = FALSE;
  mOtgDisablePending = FALSE;
  mMcaSynchronized = TRUE;
  mSupplyFault = FALSE;
  mNotifyHead = mNotifyTail = 0;
  mNotifyOverflow = FALSE;
  mRetryAt = 0;
  mChargePump.Initialized = FALSE;
}

STATIC VOID
TestEntryLifetime (VOID)
{
  FAILURE_POINT Failure;
  EFI_STATUS Status;
  EFI_EVENT InvalidEvent = NULL;
  ResetFirmware (FailNothing);
  CHECK (FakeCreateEvent (EVT_TIMER | EVT_NOTIFY_SIGNAL, TPL_APPLICATION,
                          SupplyWorker, NULL, &InvalidEvent) == EFI_INVALID_PARAMETER);
  CHECK (InvalidEvent == NULL);
  for (Failure = FailWorkerEvent; Failure <= FailAdapter; Failure++) {
    ResetFirmware (Failure);
    Status = OnyxUsbSupplyDxeEntry (NULL, NULL);
    CHECK (EFI_ERROR (Status));
    CHECK (!mTestEvents[0].Live && !mTestEvents[1].Live && !mInstalled);
    CHECK (mWorkerEvent == NULL && mExitEvent == NULL && mSupplyHandle == NULL);
    CHECK (!mAdapterInstalled && !mTimerArmed && mTimerCalls == 0);
    CheckUnhooked ();
    CHECK (mCreateCalls == 1);
    CHECK (mCreateExCalls == (UINTN)(Failure != FailWorkerEvent));
    CHECK (mInstallCalls == (UINTN)(Failure >= FailProtocolInstall));
    CHECK (mUninstallCalls == (UINTN)(Failure == FailAdapter));
    CHECK (mCloseCalls == ((Failure == FailWorkerEvent) ? 0U :
                           (Failure == FailExitEvent) ? 1U : 2U));
  }
  ResetFirmware (FailNothing);
  CHECK (OnyxUsbSupplyDxeEntry (NULL, NULL) == EFI_SUCCESS);
  CHECK (mInstalled && mAdapterInstalled);
  CHECK (mTestEvents[0].Live && mTestEvents[0].Tpl == TPL_CALLBACK);
  CHECK (mTestEvents[1].Live && mTestEvents[1].Tpl == TPL_NOTIFY);
  CHECK (mTestGlink.Open == RouterOpen && mTestGlink.Transmit == RouterTransmit);
  CHECK (mSupplyProtocol.Revision == 1 && mSupplyProtocol.Start == StartSupply);
  CHECK (!mSupplyStarted && !mTimerArmed && mTimerCalls == 0 && mBootPhase == 0);

  // A protocol retained by another driver cannot point into an unloaded image.
  ResetFirmware (FailAdapter);
  mUninstallStatus = EFI_ACCESS_DENIED;
  CHECK (OnyxUsbSupplyDxeEntry (NULL, NULL) == EFI_SUCCESS);
  CHECK (mInstalled && mSupplyHandle != NULL && !mAdapterInstalled);
  CHECK (mTestEvents[0].Live && mTestEvents[1].Live && mCloseCalls == 0);
  CHECK (mUninstallCalls == 1 && !mTimerArmed);
  CheckUnhooked ();
  CHECK (mSupplyProtocol.Start () == EFI_NOT_READY);
  CHECK (!mSupplyStarted && !mStartupBusy && mTimerCalls == 0);
  puts ("PASS: real DXE event TPL rules, entry failure rollback and hook lifetime");
}

STATIC VOID
TestStartupTplAndWorker (VOID)
{
  UINTN LocateBefore;
  ResetFirmware (FailNothing);
  CHECK (OnyxUsbSupplyDxeEntry (NULL, NULL) == EFI_SUCCESS);
  LocateBefore = mLocateCalls;
  mTestTpl = TPL_CALLBACK;
  CHECK (EFI_ERROR (StartSupply ()));
  CHECK (mTestTpl == TPL_CALLBACK);
  mTestTpl = TPL_NOTIFY;
  CHECK (EFI_ERROR (StartSupply ()));
  CHECK (mTestTpl == TPL_NOTIFY);
  CHECK (mLocateCalls == LocateBefore && mTimerCalls == 0 && mBootPhase == 0);
  CHECK (!mStartupBusy && !mSupplyStarted);

  // With bootstrap/probe complete, application TPL can arm the resident worker.
  mTestTpl = TPL_APPLICATION;
  mBootPhase = 6;
  mBootStatus = EFI_SUCCESS;
  mChargePump.Initialized = TRUE;
  mRouterUp = TRUE;
  mRouterHandle = (VOID *)2;
  CHECK (StartSupply () == EFI_SUCCESS);
  CHECK (mSupplyStarted && mTimerArmed && !mStartupBusy);
  CHECK (mTimerCalls == 1 && mTestTpl == TPL_APPLICATION);
  CHECK (mPollCalls == 1);
  mPollCalls = 0;

  mTestTpl = TPL_CALLBACK;
  mSupplyStarted = FALSE;
  SupplyWorker (mWorkerEvent, NULL);
  mSupplyStarted = TRUE;
  mStartupBusy = TRUE;
  SupplyWorker (mWorkerEvent, NULL);
  mStartupBusy = FALSE;
  mWorkerBusy = TRUE;
  SupplyWorker (mWorkerEvent, NULL);
  mWorkerBusy = FALSE;
  mBootPhase = 0;
  SupplyWorker (mWorkerEvent, NULL);
  CHECK (mPollCalls == 0 && mLocateCalls == LocateBefore);

  mBootPhase = 6;
  mPollStatus = EFI_DEVICE_ERROR;
  SupplyWorker (mWorkerEvent, NULL);
  CHECK (mPollCalls == 1 && !mWorkerBusy);
  mPollStatus = EFI_SUCCESS;
  SupplyWorker (mWorkerEvent, NULL);
  CHECK (mPollCalls == 2 && !mWorkerBusy);
  ExitBootServices (mExitEvent, NULL);
  CHECK (mStopping && !mTimerArmed);
  SupplyWorker (mWorkerEvent, NULL);
  CHECK (mPollCalls == 2);
  puts ("PASS: bootstrap rejects high TPL, worker blocks reentry and releases error guard");
}

STATIC EFI_STATUS EFIAPI
RecordingTransmit (VOID *Handle, VOID *Private, CONST VOID *Packet,
                   UINTN Length, UINT32 Options, INT32 *Raw)
{
  CHECK (Handle == mRouterHandle);
  mTransportTxCalls++;
  *Raw = 0;
  return EFI_SUCCESS;
}

STATIC VOID EFIAPI
NativeReceive (VOID *Handle, VOID *Private, VOID *PacketPrivate,
               CONST VOID *Packet, UINTN Length, UINTN IntentSize)
{
  INT32 Raw;
  mNativeRxCalls++;
  // Native callbacks own the receive buffer only for forwarded legacy packets.
  CHECK (mGlink->RxDone (Handle, Packet, TRUE, &Raw) == EFI_SUCCESS);
}

STATIC VOID
ResetRouter (VOID)
{
  ResetFirmware (FailNothing);
  mGlink = &mTestGlink;
  mGlinkTransmit = RecordingTransmit;
  mRouterReceive = NativeReceive;
  mRouterHandle = (VOID *)2;
  mRouterUp = TRUE;
  mRequestDone = mRequestTxDone = FALSE;
  mRxDoneCalls = mNativeRxCalls = mTransportTxCalls = 0;
  mTestTicks = 0;
}

STATIC VOID
PacketHeader (UINT8 *Packet, UINTN Length, UINT32 Type, UINT32 Opcode)
{
  memset (Packet, 0, Length);
  WriteLe32 (Packet, ONYX_MCA_OWNER);
  WriteLe32 (Packet + 4, Type);
  WriteLe32 (Packet + 8, Opcode);
}

STATIC VOID
TestRouterFraming (VOID)
{
  UINT8 Packet[280];
  INT32 Raw = 0;
  UINTN Count;
  ResetRouter ();
  // Native PMIC charger opcode 0x30 is a 24-byte legacy request, not MCA.
  PacketHeader (Packet, sizeof (Packet), 1, 0x30);
  WriteLe32 (Packet + 16, 6);
  CHECK (RouterTransmit (mRouterHandle, Packet, Packet, 24, 0, &Raw) == EFI_SUCCESS);
  CHECK (!mNativeRequest && mTransportTxCalls == 1);
  CHECK (RouterTransmit (mRouterHandle, Packet, Packet, 276, 0, &Raw) == EFI_SUCCESS);
  CHECK (!mNativeRequest && mTransportTxCalls == 2);
  WriteLe32 (Packet + 8, ONYX_MCA_READ);
  CHECK (RouterTransmit (mRouterHandle, Packet, Packet, 275, 0, &Raw) == EFI_SUCCESS);
  CHECK (!mNativeRequest && mTransportTxCalls == 3);

  WriteLe32 (Packet + 12, 0x201C);
  WriteLe32 (Packet + 16, 42);
  CHECK (RouterTransmit (mRouterHandle, Packet, Packet, 276, 0, &Raw) == EFI_SUCCESS);
  CHECK (mNativeRequest && mTransportTxCalls == 4);
  PacketHeader (Packet, sizeof (Packet), 1, ONYX_MCA_WRITE);
  WriteLe32 (Packet + 12, 0x201C);
  WriteLe32 (Packet + 20, 42);
  RouterReceive (mRouterHandle, NULL, NULL, Packet, 280, 280);
  CHECK (mNativeRequest && mNativeRxCalls == 0 && mRxDoneCalls == 1);
  WriteLe32 (Packet + 8, ONYX_MCA_READ);
  RouterReceive (mRouterHandle, NULL, NULL, Packet, 280, 280);
  CHECK (!mNativeRequest && mNativeRxCalls == 1 && mRxDoneCalls == 2);

  // An unknown Android notification must not signal native UCSI completion.
  PacketHeader (Packet, sizeof (Packet), ONYX_MCA_NOTIFY, 0x80);
  WriteLe32 (Packet + 12, 0x99);
  RouterReceive (mRouterHandle, NULL, NULL, Packet, 272, 272);
  CHECK (mNativeRxCalls == 1 && mRxDoneCalls == 3 && mNotifyHead == mNotifyTail);
  WriteLe32 (Packet + 12, 10);
  WriteLe32 (Packet + 16, 1);
  RouterReceive (mRouterHandle, NULL, NULL, Packet, 272, 272);
  CHECK (mNativeRxCalls == 1 && mRxDoneCalls == 4 && mNotifyHead == 1);

  // Unmatched/late custom replies are drained once instead of reaching native.
  PacketHeader (Packet, sizeof (Packet), 1, ONYX_MCA_READ);
  WriteLe32 (Packet + 20, 0x1234);
  RouterReceive (mRouterHandle, NULL, NULL, Packet, 280, 280);
  CHECK (mNativeRxCalls == 1 && mRxDoneCalls == 5);
  // Preserve native receive ownership for its existing legacy reply layouts.
  for (Count = 16; Count <= 40; Count += 8) {
    PacketHeader (Packet, sizeof (Packet), 1, 0x30);
    RouterReceive (mRouterHandle, NULL, NULL, Packet, Count, Count);
  }
  CHECK (mNativeRxCalls == 5 && mRxDoneCalls == 9);
  puts ("PASS: MCA length/opcode framing, late/unknown isolation and single receive ownership");
}

STATIC VOID
TestRequestTimeout (VOID)
{
  UINT8 Packet[280];
  UINT8 Snapshot[276];
  UINT8 Result = 0;
  UINT32 OldSequence;
  ResetRouter ();
  CHECK (RouterBegin (ONYX_MCA_READ, 0x201C, NULL, 0) == EFI_SUCCESS);
  OldSequence = mRequestSequence;
  PacketHeader (Packet, sizeof (Packet), 1, ONYX_MCA_WRITE);
  WriteLe32 (Packet + 12, mRequestProperty);
  WriteLe32 (Packet + 20, mRequestSequence);
  RouterReceive (mRouterHandle, NULL, NULL, Packet, sizeof (Packet), sizeof (Packet));
  CHECK (!mRequestDone && mNativeRxCalls == 0 && mRxDoneCalls == 1);
  WriteLe32 (Packet + 8, ONYX_MCA_READ);
  Packet[24] = 0x81;
  RouterReceive (mRouterHandle, NULL, NULL, Packet, sizeof (Packet), sizeof (Packet));
  CHECK (mRequestDone && mNativeRxCalls == 0 && mRxDoneCalls == 2);
  CHECK (RouterFinish (&Result, 1) == EFI_NOT_READY);
  RouterTransmitDone (mRouterHandle, NULL, mRequest, mRequest, sizeof (mRequest));
  CHECK (RouterFinish (&Result, 1) == EFI_SUCCESS && Result == 0x81);

  CHECK (RouterBegin (ONYX_MCA_READ, 0x201C, NULL, 0) == EFI_SUCCESS);
  CHECK (mRequestSequence != OldSequence);
  RouterTransmitDone (mRouterHandle, NULL, mRequest, mRequest, sizeof (mRequest));
  mTestTicks += (ONYX_MCA_TIMEOUT_US + 1) * 1000;
  CHECK (RouterFinish (NULL, 0) == EFI_TIMEOUT);
  CHECK (!mRequestBusy && !mSupplyFault);
  CHECK (RouterBegin (ONYX_MCA_READ, 0x201C, NULL, 0) == EFI_SUCCESS);

  // Transport still owns a packet until TxDone, even if its reply deadline passed.
  memcpy (Snapshot, mRequest, sizeof (Snapshot));
  mTestTicks += (ONYX_MCA_TIMEOUT_US + 1) * 1000;
  CHECK (RouterFinish (NULL, 0) == EFI_TIMEOUT);
  CHECK (mRequestBusy);
  CHECK (RouterBegin (ONYX_MCA_WRITE, 0x201A, NULL, 0) == EFI_NOT_READY);
  CHECK (memcmp (Snapshot, mRequest, sizeof (Snapshot)) == 0);
  RouterTransmitDone (mRouterHandle, NULL, mRequest, mRequest, sizeof (mRequest));
  // TxDone retires the expired request; retry does not require a reboot.
  CHECK (!mRequestBusy && !mSupplyFault);
  CHECK (RouterBegin (ONYX_MCA_WRITE, 0x201A, NULL, 0) == EFI_SUCCESS);
  puts ("PASS: response opcode matching and timeout retry without premature TX buffer reuse");
}

VOID *EFIAPI
CopyMem (VOID *Destination, CONST VOID *Source, UINTN Length)
{
  return memcpy (Destination, Source, Length);
}

VOID *EFIAPI
ZeroMem (VOID *Buffer, UINTN Length)
{
  return memset (Buffer, 0, Length);
}

UINT64 EFIAPI
GetPerformanceCounter (VOID)
{
  return mTestTicks;
}

UINT64 EFIAPI
GetTimeInNanoSecond (UINT64 Ticks)
{
  return Ticks;
}

UINTN EFIAPI
MicroSecondDelay (UINTN Microseconds)
{
  mTestTicks += Microseconds * 1000;
  return Microseconds;
}

STATIC UINT32 EFIAPI
FakeI2cRead (VOID *Handle, ONYX_I2C_CONFIG *Config, UINT16 Reg,
             UINT16 AddressSize, UINT8 *Data, UINT16 DataSize,
             UINT32 *Count, UINT32 TimeoutUs)
{
  if (mBootstrapDependencies) { CHECK (mTestTpl == TPL_APPLICATION); }
  CHECK (Handle == (VOID *)1);
  CHECK (Config->SlaveAddress == 0x6F && Config->FrequencyKhz == 400);
  CHECK (AddressSize == 1 && DataSize == 1 && TimeoutUs == 2500);
  mReadCount++;
  *Data = mRegisters[Reg];
  *Count = 1;
  return 0;
}

STATIC UINT32 EFIAPI
FakeI2cWrite (VOID *Handle, ONYX_I2C_CONFIG *Config, UINT16 Reg,
              UINT16 AddressSize, UINT8 *Data, UINT16 DataSize,
              UINT32 *Count, UINT32 TimeoutUs)
{
  if (mBootstrapDependencies) { CHECK (mTestTpl == TPL_APPLICATION); }
  CHECK (Handle == (VOID *)1);
  CHECK (Config->SlaveAddress == 0x6F && Config->FrequencyKhz == 400);
  CHECK (AddressSize == 1 && DataSize == 1 && TimeoutUs == 2500);
  CHECK (mWriteCount < ARRAY_SIZE (mWriteRegisters));
  mWriteRegisters[mWriteCount++] = (UINT8)Reg;
  mRegisters[Reg] = (mIgnoreOvpGate && Reg == 0x0B) ?
                    (UINT8)(*Data & (UINT8)~0x08) : *Data;
  *Count = 1;
  return 0;
}

STATIC EFI_STATUS EFIAPI
FakeTransmit (VOID *Handle, VOID *Private, CONST VOID *Packet,
              UINTN Length, UINT32 Options, INT32 *Raw)
{
  CONST UINT8 *Data = Packet;
  CHECK (Handle == mRouterHandle && Private == Packet && Length == 276);
  mLastProperty = ReadLe32 (Data + 12);
  *Raw = 0;
  return EFI_SUCCESS;
}

STATIC VOID
ResetPump (VOID)
{
  ZeroMem (mRegisters, sizeof (mRegisters));
  ZeroMem (mWriteRegisters, sizeof (mWriteRegisters));
  mWriteCount = 0;
  mReadCount = 0;
  mIgnoreOvpGate = FALSE;
  mRegisters[0x6E] = 0x81;
  memset (&mTestI2c, 0, sizeof (mTestI2c));
  mChargePump.Protocol = &mTestI2c;
  mChargePump.Protocol->Read = FakeI2cRead;
  mChargePump.Protocol->Write = FakeI2cWrite;
  mChargePump.Handle = (VOID *)1;
  mChargePump.Initialized = TRUE;
  mChargePump.Config.FrequencyKhz = 400;
  mChargePump.Config.SlaveAddress = 0x6F;
  mChargePump.Config.StretchUs = 500;
}

STATIC VOID
CheckStartupContext (VOID)
{
  CHECK (mTestTpl == TPL_APPLICATION && mStartupBusy);
  CHECK (!mSupplyStarted && !mTimerArmed);
}

STATIC EFI_STATUS EFIAPI
FakeGetConfig (CONST CHAR16 *Name, VOID *Config)
{
  CheckStartupContext ();
  CHECK (memcmp (Name, L"FULL_ADSP", sizeof (L"FULL_ADSP")) == 0);
  CHECK (mSetOsCalls == 0 && mPilLoadCalls == 0 && mConnectCalls == 0);
  mGetConfigCalls++;
  memset (Config, 0, sizeof (mPilConfig));
  ((UINT64 *)Config)[0] = 1;
  memcpy ((UINT8 *)Config + 0x0C, L"FULL_ADSP", sizeof (L"FULL_ADSP"));
  return EFI_SUCCESS;
}

STATIC EFI_STATUS EFIAPI
FakeSetOs (UINT32 Os)
{
  CheckStartupContext ();
  CHECK (Os == 0 && mGetConfigCalls == 1 && mPilLoadCalls == 0);
  mSetOsCalls++;
  return EFI_SUCCESS;
}

STATIC EFI_STATUS EFIAPI
FakePilLoad (CONST CHAR16 *Name)
{
  CheckStartupContext ();
  CHECK (memcmp (Name, L"FULL_ADSP", sizeof (L"FULL_ADSP")) == 0);
  CHECK (mSetOsCalls == 1 && mConnectCalls == 0);
  mPilLoadCalls++;
  return mPilLoadStatus;
}

STATIC EFI_STATUS EFIAPI
FakeConnected (BOOLEAN *Connected)
{
  CheckStartupContext ();
  *Connected = mTestConnected;
  return EFI_SUCCESS;
}

STATIC EFI_STATUS EFIAPI
FakeConnect (VOID)
{
  CheckStartupContext ();
  CHECK (!mTestConnected && mPilLoadCalls == 1);
  mConnectCalls++;
  mTestConnected = TRUE;
  mRouterHandle = (VOID *)2;
  mRouterUp = TRUE;
  return EFI_SUCCESS;
}

STATIC EFI_STATUS EFIAPI
FakeConfigGpio (UINT32 Config, UINT32 Enable)
{
  CheckStartupContext ();
  CHECK (mBootPhase == 6 && Enable == 1);
  CHECK (Config == (0x20000000U | (126U << 4) | BIT14));
  return EFI_SUCCESS;
}

STATIC EFI_STATUS EFIAPI
FakeGpioOut (UINT32 Config, UINT32 Value)
{
  CheckStartupContext ();
  CHECK (mBootPhase == 6 && Value == 1);
  CHECK (Config == (0x20000000U | (126U << 4) | BIT14));
  return EFI_SUCCESS;
}

STATIC UINT32 EFIAPI
FakeI2cOpen (UINT32 Instance, VOID **Handle)
{
  CheckStartupContext ();
  CHECK (Instance == 12 && mBootPhase == 6);
  CHECK (NowUs () >= 400000);
  mI2cOpenCalls++;
  *Handle = (VOID *)1;
  return 0;
}

STATIC UINT32 EFIAPI
FakeI2cClose (VOID *Handle)
{
  CHECK (Handle == (VOID *)1);
  return 0;
}

STATIC VOID
PrepareBootstrap (VOID)
{
  ResetFirmware (FailNothing);
  CHECK (OnyxUsbSupplyDxeEntry (NULL, NULL) == EFI_SUCCESS);
  // PmicGlink is dispatched only after the resident adapter was installed.
  mBootstrapDependencies = TRUE;
  mTestPil.Revision = 0x10005;
  mTestPil.GetConfig = FakeGetConfig;
  mTestPil.Load = FakePilLoad;
  mTestPmicRouter.Revision = 0x1000B;
  mTestPmicRouter.Connect = FakeConnect;
  mTestPmicRouter.Connected = FakeConnected;
  mTestPmicBoot.Revision = 0x10026;
  mTestPmicBoot.SetOs = FakeSetOs;
  mTestTlmm.Revision = 1;
  mTestTlmm.ConfigGpio = FakeConfigGpio;
  mTestTlmm.GpioOut = FakeGpioOut;
  ResetPump ();
  mTestI2c.Revision = 0x10000;
  mTestI2c.Open = FakeI2cOpen;
  mTestI2c.Close = FakeI2cClose;
  mChargePump.Initialized = FALSE;
  mChargePump.Handle = NULL;
}

STATIC VOID
CheckResidentAfterStartupFailure (VOID)
{
  CHECK (mInstalled && mAdapterInstalled && !mStartupBusy && !mSupplyStarted);
  CHECK (mTestEvents[0].Live && mTestEvents[1].Live && !mTimerArmed);
  CHECK (mTestGlink.Open == RouterOpen && mTestGlink.Transmit == RouterTransmit);
  CHECK (mCloseCalls == 0 && mUninstallCalls == 0);
}

STATIC VOID
TestFullBootstrap (VOID)
{
  PrepareBootstrap ();
  CHECK (mBootPhase == 0 && !mChargePump.Initialized);
  CHECK (StartSupply () == EFI_SUCCESS);
  CHECK (mBootPhase == 6 && mChargePump.Initialized && mSupplyStarted && mTimerArmed);
  CHECK (mGetConfigCalls == 1 && mSetOsCalls == 1 && mPilLoadCalls == 1);
  CHECK (mConnectCalls == 1 && mI2cOpenCalls == 1 && mTimerCalls == 1);
  CHECK (mPollCalls > 0 && mTestTpl == TPL_APPLICATION && !mStartupBusy);

  PrepareBootstrap ();
  mTestConnected = TRUE;
  mRouterUp = TRUE;
  mRouterHandle = (VOID *)2;
  CHECK (StartSupply () == EFI_SUCCESS);
  CHECK (mBootPhase == 6 && mSupplyStarted && mChargePump.Initialized);
  CHECK (mGetConfigCalls == 0 && mSetOsCalls == 0 && mPilLoadCalls == 0);
  CHECK (mConnectCalls == 0 && mI2cOpenCalls == 1 && mTimerArmed);

  PrepareBootstrap ();
  mPilLoadStatus = EFI_LOAD_ERROR;
  CHECK (StartSupply () == EFI_LOAD_ERROR);
  CHECK (mBootPhase == 5 && mSetOsCalls == 1 && mPilLoadCalls == 1);
  CHECK (mConnectCalls == 0 && mI2cOpenCalls == 0 && mTimerCalls == 0);
  CheckResidentAfterStartupFailure ();

  PrepareBootstrap ();
  mTimerStatus = EFI_DEVICE_ERROR;
  CHECK (StartSupply () == EFI_DEVICE_ERROR);
  CHECK (mBootPhase == 6 && mChargePump.Initialized && mTimerCalls == 1);
  CheckResidentAfterStartupFailure ();
  puts ("PASS: complete application-TPL bootstrap, connected-link reuse and failed startup residency");
}

STATIC VOID
TestSc8581 (VOID)
{
  ResetPump ();
  mRegisters[0x0B] = 0x30;
  mRegisters[0x0E] = 0xA1;
  CHECK (Sc8581OtgConfig (TRUE) == EFI_SUCCESS);
  CHECK ((mRegisters[0x0B] & 0x08) != 0);
  CHECK ((mRegisters[0x0E] & 0x07) == 5);
  CHECK (mRegisters[0x01] == 12 && mRegisters[0x02] == 0x84);
  CHECK (mRegisters[0x03] == 3 && mRegisters[0x04] == 10);
  CHECK (mRegisters[0x06] == 27 && mRegisters[0x08] == 4);
  CHECK (Sc8581OtgConfig (FALSE) == EFI_SUCCESS);
  CHECK ((mRegisters[0x0B] & 0xC0) == 0);
  CHECK ((mRegisters[0x0E] & 0x07) == 1);
  mRegisters[0x6E] = 0x85;
  mWriteCount = 0;
  CHECK (Sc8581OtgConfig (TRUE) == EFI_UNSUPPORTED && mWriteCount == 0);
  mRegisters[0x6E] = 0x81;
  mIgnoreOvpGate = TRUE;
  CHECK (Sc8581OtgConfig (TRUE) == EFI_TIMEOUT);
  CHECK (mWriteCount >= 200);
  puts ("PASS: SC8581 OTG enable/disable, chip guard and retry limit");
}

STATIC VOID
TestDisconnectOtgShutdown (VOID)
{
  UINTN Reads;
  UINTN Writes;

  ResetFirmware (FailNothing);
  ResetPump ();
  mGlink = &mTestGlink;
  mRouterUp = TRUE;
  mRouterHandle = (VOID *)2;
  mSupplyStarted = TRUE;
  mBootPhase = 6;
  mChargePump.Initialized = TRUE;
  mMcaSynchronized = TRUE;
  mTestTpl = TPL_CALLBACK;
  Reads = mReadCount;
  Writes = mWriteCount;
  RouterStateChanged (mRouterHandle, NULL, 1);
  CHECK (!mRouterUp && mRouterHandle == NULL && mOtgDisablePending);
  CHECK (mReadCount == Reads && mWriteCount == Writes);
  CHECK (mPollCalls == 0);

  // An unsupported ID keeps the shutdown request pending for the next tick.
  mRegisters[0x6E] = 0x85;
  SupplyWorker (mWorkerEvent, NULL);
  CHECK (mOtgDisablePending && mReadCount == Reads + 1);
  CHECK (mWriteCount == Writes && mPollCalls == 0 && !mWorkerBusy);

  // Once the SC8581 responds, shutdown completes despite the down router.
  mRegisters[0x6E] = 0x81;
  SupplyWorker (mWorkerEvent, NULL);
  CHECK (!mOtgDisablePending && mReadCount > Reads + 1);
  CHECK (mWriteCount > Writes && mPollCalls == 0 && !mWorkerBusy);
  puts ("PASS: disconnect callback defers SC8581 shutdown; worker retries without polling down link");
}

STATIC VOID
TestNotificationOrder (VOID)
{
  EFI_STATUS Status;
  ResetPump ();
  mRouterUp = TRUE;
  mRouterHandle = (VOID *)2;
  mGlinkTransmit = FakeTransmit;
  mSupplyFault = FALSE;
  mMcaSynchronized = TRUE;
  mNotifyHead = mNotifyTail = 0;
  mNotifyOverflow = FALSE;
  mCpWorkActive = FALSE;
  mCpWorkPhase = 0;
  mCpWorkRetries = 0;
  mRequestBusy = FALSE;
  mRequestDone = FALSE;
  mRequestTxDone = FALSE;
  mLastProperty = 0;
  QueueNotification (9, 2);
  QueueNotification (10, 1);

  Status = ServiceNotifications ();
  CHECK (Status == EFI_NOT_READY);
  CHECK (mLastProperty == 0x21020 && mNotifyTail == 0);
  CHECK (mWriteCount == 0);

  mRequestTxDone = TRUE;
  WriteLe32 (mResponse + 16, 0);
  mResponse[24] = 0;
  mRequestDone = TRUE;
  Status = ServiceNotifications ();
  CHECK (Status == EFI_NOT_READY);
  CHECK (mNotifyTail == 0 && mCpWorkPhase == 3);
  CHECK (mRegisters[0x0D] == 40);

  mCpWorkDue = 0;
  Status = ServiceNotifications ();
  CHECK (Status == EFI_NOT_READY);
  CHECK (mLastProperty == 0x200F && mNotifyTail == 0);
  CHECK ((mRegisters[0x0E] & 7) != 5);

  mRequestTxDone = TRUE;
  mRequestDone = TRUE;
  WriteLe32 (mResponse + 16, 0);
  Status = ServiceNotifications ();
  CHECK (Status == EFI_SUCCESS && mNotifyTail == 1);
  CHECK ((mRegisters[0x0E] & 7) != 5);
  Status = ServiceNotifications ();
  CHECK (Status == EFI_SUCCESS && mNotifyTail == 2);
  CHECK ((mRegisters[0x0E] & 7) == 5);
  CHECK ((mRegisters[0x0B] & 0x08) != 0);
  puts ("PASS: CP_REVERT remains ahead of later OTG config notifications");
}

STATIC VOID
TestCpPluginReply (VOID)
{
  UINT8 Packet[280];
  UINT8 Present = 0;
  ResetRouter ();
  mRouterState = NULL;
  mReadCount = mWriteCount = 0;
  mCpWorkActive = FALSE;
  mLastCpPosition = 1;
  CHECK (BeginCpRevert (0) == EFI_NOT_READY);
  CHECK (mRequestProperty == 0x21020);
  PacketHeader (Packet, sizeof (Packet), 1, ONYX_MCA_READ);
  WriteLe32 (Packet + 12, 0x21020);
  WriteLe32 (Packet + 20, mRequestSequence + 1);
  Packet[24] = 0xF8;
  RouterReceive (mRouterHandle, NULL, NULL, Packet, sizeof (Packet), sizeof (Packet));
  CHECK (!mRequestDone);
  WriteLe32 (Packet + 20, mRequestSequence);
  WriteLe32 (Packet + 12, 0x21021);
  RouterReceive (mRouterHandle, NULL, NULL, Packet, sizeof (Packet), sizeof (Packet));
  CHECK (!mRequestDone);
  WriteLe32 (Packet + 12, 0x21020);
  WriteLe32 (Packet + 8, ONYX_MCA_WRITE);
  RouterReceive (mRouterHandle, NULL, NULL, Packet, sizeof (Packet), sizeof (Packet));
  CHECK (!mRequestDone);
  WriteLe32 (Packet + 8, ONYX_MCA_READ);
  RouterReceive (mRouterHandle, NULL, NULL, Packet, sizeof (Packet), sizeof (Packet));
  CHECK (mRequestDone);
  RouterTransmitDone (mRouterHandle, NULL, mRequest, mRequest, sizeof (mRequest));
  CHECK (BeginCpRevert (0) == EFI_SUCCESS);
  CHECK (mCpWorkPresent == 0xF8 && mLastCpPosition == 0);
  CHECK (mReadCount == 0 && mWriteCount == 0);

  // Once the transaction is retired, even the old matching sequence is late.
  Packet[24] = 0;
  RouterReceive (mRouterHandle, NULL, NULL, Packet, sizeof (Packet), sizeof (Packet));
  CHECK (RouterBegin (ONYX_MCA_READ, 0x21020, NULL, 0) == EFI_SUCCESS);
  RouterReceive (mRouterHandle, NULL, NULL, Packet, sizeof (Packet), sizeof (Packet));
  CHECK (!mRequestDone);
  RouterTransmitDone (mRouterHandle, NULL, mRequest, mRequest, sizeof (mRequest));
  mTestTicks += (ONYX_MCA_TIMEOUT_US + 1) * 1000;
  CHECK (RouterFinish (&Present, sizeof (Present)) == EFI_TIMEOUT);
  WriteLe32 (Packet + 20, mRequestSequence);
  RouterReceive (mRouterHandle, NULL, NULL, Packet, sizeof (Packet), sizeof (Packet));
  CHECK (Present == 0 && !mRequestBusy);
  puts ("PASS: OTG_PLUGIN accepts the matched reply and ignores stale transactions");
}

int
main (VOID)
{
  TestEntryLifetime ();
  TestStartupTplAndWorker ();
  TestFullBootstrap ();
  TestRouterFraming ();
  TestRequestTimeout ();
  TestSc8581 ();
  TestDisconnectOtgShutdown ();
  TestNotificationOrder ();
  TestCpPluginReply ();
  return 0;
}
