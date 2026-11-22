/**
  Stub implementation of the XBL EFI_KERNEL_PROTOCOL (scheduler interface).

  The embedded ZTE XBLCore image provides scheduler/lock data structures that
  are not initialized in the Mu-Silicium boot flow.  Calling into the real
  XBLCore protocol (e.g. Lock->InitLock, or any scheduler method from DXE)
  causes synchronous exceptions when it touches uninitialized globals.

  This stub publishes a safe, no-op/success protocol so HOB consumers can
  proceed without executing XBLCore code.  It is intentionally minimal and may
  need extension if a consumer relies on real scheduler semantics.
**/

#include <Uefi.h>
#include <Library/DebugLib.h>

#include <Protocol/EFIKernelInterface.h>

//
// Opaque handle storage
//
struct LockType {
  UINT64  State;
};

struct event {
  UINT32  State;
};

struct mutex {
  UINT32  State;
};

struct semaphore {
  UINT32  Count;
};

struct spinlock {
  UINT32  State;
};

struct thread {
  UINT32  State;
};

struct timer {
  UINT32  State;
};

STATIC struct LockType  mStubLock      = { 0 };
STATIC struct event     mStubEvent     = { SCHED_EVENT_STATE_UNSIGNALED };
STATIC struct mutex     mStubMutex     = { 0 };
STATIC struct semaphore mStubSemaphore = { 0 };
STATIC struct spinlock  mStubSpinlock  = { 0 };
STATIC struct thread    mStubThread    = { 0 };
STATIC struct timer     mStubTimer     = { 0 };

STATIC ThreadStats    mStubThreadStats = { 0 };
STATIC CpuSchedStats  mStubCpuStats    = { 0 };

//
// Interrupt interface
//
STATIC
VOID
StubRegisterIntrHandler (
  UINT32      Vector,
  IntrHandler Handler,
  VOID        *Arg
  )
{
}

STATIC
KStatus
StubMaskInterrupt (
  UINT32 Vector
  )
{
  return NO_ERROR;
}

STATIC
KStatus
StubUnmaskInterrupt (
  UINT32 Vector
  )
{
  return NO_ERROR;
}

STATIC
KStatus
StubConfigureInterrupt (
  UINT32          Vector,
  enum IntrConfig Config,
  VOID            *Arg
  )
{
  return NO_ERROR;
}

STATIC InterruptIntf mStubInterruptIntf = {
  StubRegisterIntrHandler,
  StubMaskInterrupt,
  StubUnmaskInterrupt,
  StubConfigureInterrupt
};

//
// Event interface
//
STATIC
Event *
StubEventInit (
  UINT64 Id OPTIONAL,
  _Bool  Initial,
  UINT32 Flags
  )
{
  mStubEvent.State = Initial ? SCHED_EVENT_STATE_SIGNALED : SCHED_EVENT_STATE_UNSIGNALED;
  return &mStubEvent;
}

STATIC
VOID
StubEventDestroy (
  Event *Evt
  )
{
}

STATIC
_Bool
StubEventInitialized (
  Event *Evt
  )
{
  return (Evt != NULL);
}

STATIC
KStatus
StubEventWait (
  Event *Evt
  )
{
  return NO_ERROR;
}

STATIC
KStatus
StubEventWaitTimeout (
  Event        *Evt,
  TimeDuration  Timeout
  )
{
  return NO_ERROR;
}

STATIC
KStatus
StubEventSignal (
  Event *Evt,
  _Bool Reschedule
  )
{
  if (Evt != NULL) {
    Evt->State = SCHED_EVENT_STATE_SIGNALED;
  }
  return NO_ERROR;
}

STATIC
KStatus
StubEventUnsignal (
  Event *Evt
  )
{
  if (Evt != NULL) {
    Evt->State = SCHED_EVENT_STATE_UNSIGNALED;
  }
  return NO_ERROR;
}

STATIC
_Bool
StubEventGetSignalState (
  Event *Evt
  )
{
  if (Evt == NULL) {
    return 0;
  }
  return (Evt->State == SCHED_EVENT_STATE_SIGNALED);
}

STATIC EventIntf mStubEventIntf = {
  StubEventInit,
  StubEventDestroy,
  StubEventInitialized,
  StubEventWait,
  StubEventWaitTimeout,
  StubEventSignal,
  StubEventUnsignal,
  StubEventGetSignalState
};

//
// Mutex interface
//
STATIC
Mutex *
StubMutexInit (
  UINT64 Id OPTIONAL
  )
{
  return &mStubMutex;
}

STATIC
VOID
StubMutexDestroy (
  Mutex *Mtx
  )
{
}

STATIC
KStatus
StubMutexAcquire (
  Mutex *Mtx
  )
{
  return NO_ERROR;
}

STATIC
KStatus
StubMutexAcquireTimeout (
  Mutex        *Mtx,
  TimeDuration  Timeout
  )
{
  return NO_ERROR;
}

STATIC
KStatus
StubMutexRelease (
  Mutex *Mtx
  )
{
  return NO_ERROR;
}

STATIC
_Bool
StubIsMutexHeld (
  Mutex *Mtx
  )
{
  return 0;
}

STATIC MutexIntf mStubMutexIntf = {
  StubMutexInit,
  StubMutexDestroy,
  StubMutexAcquire,
  StubMutexAcquireTimeout,
  StubMutexRelease,
  StubIsMutexHeld
};

//
// Semaphore interface
//
STATIC
Semaphore *
StubSemInit (
  UINT64 Id OPTIONAL,
  UINT32 Cnt
  )
{
  mStubSemaphore.Count = Cnt;
  return &mStubSemaphore;
}

STATIC
VOID
StubSemDestroy (
  Semaphore *Sem
  )
{
}

STATIC
INT32
StubSemPost (
  Semaphore *Sem,
  _Bool      Resched
  )
{
  if (Sem != NULL) {
    Sem->Count++;
  }
  return NO_ERROR;
}

STATIC
KStatus
StubSemWait (
  Semaphore *Sem
  )
{
  return NO_ERROR;
}

STATIC
KStatus
StubSemTryWait (
  Semaphore *Sem
  )
{
  return NO_ERROR;
}

STATIC
KStatus
StubSemTimedWait (
  Semaphore    *Sem,
  TimeDuration  Timeout
  )
{
  return NO_ERROR;
}

STATIC SemIntf mStubSemIntf = {
  StubSemInit,
  StubSemDestroy,
  StubSemPost,
  StubSemWait,
  StubSemTryWait,
  StubSemTimedWait
};

//
// Spinlock interface
//
STATIC
Spinlock *
StubSpinlockInit (
  UINT64 Id OPTIONAL
  )
{
  return &mStubSpinlock;
}

STATIC
_Bool
StubSpinlockHeld (
  Spinlock *Lock
  )
{
  return 0;
}

STATIC
INTN
StubSpinTryLock (
  Spinlock *Lock
  )
{
  return 1;
}

STATIC
VOID
StubSpinLock (
  Spinlock *Lock
  )
{
}

STATIC
VOID
StubSpinUnlock (
  Spinlock *Lock
  )
{
}

STATIC SpinlockIntf mStubSpinlockIntf = {
  StubSpinlockInit,
  StubSpinlockHeld,
  StubSpinTryLock,
  StubSpinLock,
  StubSpinUnlock
};

//
// Thread interface
//
STATIC
Thread *
StubThreadCreate (
  const char         *Name,
  ThreadStartRoutine  EntryPoint,
  VOID               *Arg,
  INT32               Priority,
  UINTN               Stack_size
  )
{
  return &mStubThread;
}

STATIC
KStatus
StubThreadResume (
  Thread *Thr
  )
{
  return NO_ERROR;
}

STATIC
VOID
StubThreadExit (
  INT32 RetCode
  )
{
}

STATIC
Thread *
StubGetCurrentThread (
  VOID
  )
{
  return &mStubThread;
}

STATIC
INT32
StubThreadGetCurrCpu (
  Thread *Thr
  )
{
  return 0;
}

STATIC
INT32
StubThreadGetPinnedCpu (
  Thread *Thr
  )
{
  return 0;
}

STATIC
VOID
StubThreadSetPinnedCpu (
  Thread *Thr,
  INT32   Cpu
  )
{
}

STATIC
VOID
StubThreadSetName (
  const char *Name
  )
{
}

STATIC
VOID
StubThreadSetPriority (
  INT32 Priority
  )
{
}

STATIC
VOID
StubThreadSleep (
  TimeDuration Delay
  )
{
}

STATIC
KStatus
StubThreadDetach (
  Thread *Thr
  )
{
  return NO_ERROR;
}

STATIC
KStatus
StubThreadJoin (
  Thread       *Thr,
  INT32        *RetCode,
  TimeDuration  Timeout
  )
{
  if (RetCode != NULL) {
    *RetCode = 0;
  }
  return NO_ERROR;
}

STATIC
KStatus
StubThreadDetachAndResume (
  Thread *Thr
  )
{
  return NO_ERROR;
}

STATIC
UINT64
StubThreadGetTimestamp (
  VOID
  )
{
  return 0;
}

STATIC
ThreadStats *
StubThreadGetStats (
  Thread      *Thr,
  ThreadStats *Tsp
  )
{
  if (Tsp != NULL) {
    return Tsp;
  }
  return &mStubThreadStats;
}

STATIC
VOID **
StubThreadGetUnsafeSpCurrent (
  Thread *Thr
  )
{
  return NULL;
}

STATIC
VOID *
StubThreadGetUnsafeSpBase (
  Thread *Thr
  )
{
  return NULL;
}

STATIC
KStatus
StubThreadSetUnsafeSp (
  Thread             *Thr,
  ThrUnsafeStackIntf *Intf
  )
{
  return NO_ERROR;
}

STATIC
UINT64
StubThreadGetUnsafeSpPaddingSize (
  VOID
  )
{
  return 0;
}

STATIC ThreadIntf mStubThreadIntf = {
  StubThreadCreate,
  StubThreadResume,
  StubThreadExit,
  StubGetCurrentThread,
  StubThreadGetCurrCpu,
  StubThreadGetPinnedCpu,
  NULL, // Reserved3
  StubThreadSetPinnedCpu,
  StubThreadSetName,
  StubThreadSetPriority,
  StubThreadSleep,
  StubThreadDetach,
  StubThreadJoin,
  StubThreadDetachAndResume,
  NULL, // Reserved0
  NULL, // Reserved1
  NULL, // Reserved2
  NULL, // Reserved4
  StubThreadGetTimestamp,
  StubThreadGetStats,
  StubThreadGetUnsafeSpCurrent,
  StubThreadGetUnsafeSpBase,
  StubThreadSetUnsafeSp,
  StubThreadGetUnsafeSpPaddingSize
};

//
// Timer interface
//
STATIC
Timer *
StubTimerInit (
  VOID
  )
{
  return &mStubTimer;
}

STATIC
VOID
StubTimerSetOneshot (
  Timer         *Tmr,
  TimeDuration   Delay,
  TimerCallback  Cb,
  VOID          *Arg
  )
{
}

STATIC
VOID
StubTimerSetPeriodic (
  Timer         *Tmr,
  TimeDuration   Period,
  TimerCallback  Cb,
  VOID          *Arg
  )
{
}

STATIC
VOID
StubTimerCancel (
  Timer *Tmr
  )
{
}

STATIC TimerIntf mStubTimerIntf = {
  StubTimerInit,
  StubTimerSetOneshot,
  StubTimerSetPeriodic,
  StubTimerCancel
};

//
// MP CPU interface
//
STATIC
UINT32
StubMpcoreGetMaxCpuCount (
  VOID
  )
{
  return 1;
}

STATIC
UINT32
StubMpcoreGetAvailCpuCount (
  VOID
  )
{
  return 1;
}

STATIC
UINT32
StubMpcoreInitDeferredCores (
  UINT32 Cpu_Mask
  )
{
  return 0;
}

STATIC
UINT32
StubMpcoreGetCurrCpu (
  VOID
  )
{
  return 0;
}

STATIC
UINT32
StubMpcoreIsCpuActive (
  UINT32 Cpu
  )
{
  return (Cpu == 0) ? 1 : 0;
}

STATIC
UINT32
StubMpcoreGetActiveMask (
  VOID
  )
{
  return 1;
}

STATIC
UINT32
StubMpcoreGetOnlineMask (
  VOID
  )
{
  return 1;
}

STATIC
VOID
StubMpcoreShutdownScheduler (
  VOID
  )
{
}

STATIC
VOID
StubMpcorePowerOffCpu (
  UINT32 CpuMask
  )
{
}

STATIC
VOID
StubMpcorePowerOnCpu (
  UINT32 CpuMask
  )
{
}

STATIC
INT32
StubMpcoreSleepCpu (
  UINT64 DurationMs
  )
{
  return 0;
}

STATIC
EFI_STATUS
StubRegisterPwrTransitionNotify (
  PwrTxnNotifyFn CbFn,
  VOID          *Arg
  )
{
  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
StubUnRegisterPwrTransitionNotify (
  PwrTxnNotifyFn CbFn
  )
{
  return EFI_SUCCESS;
}

STATIC
CpuSchedStats *
StubMpcoreGetCpuSchedStats (
  UINT32         Cpu,
  CpuSchedStats *Csp
  )
{
  if (Csp != NULL) {
    return Csp;
  }
  return &mStubCpuStats;
}

STATIC MpCpuIntf mStubMpCpuIntf = {
  StubMpcoreGetMaxCpuCount,
  StubMpcoreGetAvailCpuCount,
  StubMpcoreInitDeferredCores,
  StubMpcoreGetCurrCpu,
  StubMpcoreIsCpuActive,
  StubMpcoreGetActiveMask,
  StubMpcoreGetOnlineMask,
  StubMpcoreShutdownScheduler,
  StubMpcorePowerOffCpu,
  StubMpcorePowerOnCpu,
  StubMpcoreSleepCpu,
  StubRegisterPwrTransitionNotify,
  StubUnRegisterPwrTransitionNotify,
  StubMpcoreGetCpuSchedStats
};

//
// Watchdog interface
//
STATIC
EFI_STATUS
StubWdogEnable (
  VOID
  )
{
  return EFI_SUCCESS;
}

STATIC
VOID
StubWdogDisable (
  VOID
  )
{
}

STATIC
EFI_STATUS
StubWdogSetBiteTimeout (
  UINT32 Timeoutms
  )
{
  return EFI_SUCCESS;
}

STATIC
VOID
StubWdogForcePet (
  VOID
  )
{
}

STATIC
VOID
StubWdogForceBite (
  VOID
  )
{
}

STATIC
EFI_STATUS
StubWdogSetPetTimerPeriod (
  UINT32 Timeoutms
  )
{
  return EFI_SUCCESS;
}

STATIC WDogIntf mStubWDogIntf = {
  StubWdogEnable,
  StubWdogDisable,
  StubWdogSetBiteTimeout,
  StubWdogForcePet,
  StubWdogForceBite,
  StubWdogSetPetTimerPeriod
};

//
// Lock interface
//
STATIC
EFI_STATUS
StubInitLock (
  IN  CONST CHAR8  *LockStrID OPTIONAL,
  OUT LockHandle  **LockHandlePtr
  )
{
  if (LockHandlePtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  *LockHandlePtr = &mStubLock;
  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
StubInitRecursiveLock (
  IN  CONST CHAR8  *LockStrID OPTIONAL,
  OUT LockHandle  **LockHandlePtr
  )
{
  if (LockHandlePtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  *LockHandlePtr = &mStubLock;
  return EFI_SUCCESS;
}

STATIC
VOID
StubDestroyLock (
  LockHandle *Lock
  )
{
}

STATIC
VOID
StubAcquireLock (
  LockHandle *Lock
  )
{
}

STATIC
VOID
StubReleaseLock (
  LockHandle *Lock
  )
{
}

STATIC LockIntf mStubLockIntf = {
  StubInitLock,
  StubInitRecursiveLock,
  StubDestroyLock,
  StubAcquireLock,
  StubReleaseLock
};

//
// Protocol instance
//
STATIC
UINT32
StubGetLibVersion (
  VOID
  )
{
  return EFI_KERNEL_PROTOCOL_VERSION;
}

STATIC EFI_KERNEL_PROTOCOL mStubSchedulerProtocol = {
  EFI_KERNEL_PROTOCOL_VERSION,
  &mStubInterruptIntf,
  &mStubTimerIntf,
  &mStubThreadIntf,
  &mStubEventIntf,
  &mStubMutexIntf,
  &mStubSemIntf,
  &mStubSpinlockIntf,
  &mStubMpCpuIntf,
  &mStubWDogIntf,
  StubGetLibVersion,
  &mStubLockIntf
};

EFI_PHYSICAL_ADDRESS
GetXblSchedulerStubProtocol (
  VOID
  )
{
  return (EFI_PHYSICAL_ADDRESS)(UINTN)&mStubSchedulerProtocol;
}
