#ifndef _MSDC_IMPL_LIB_H_
#define _MSDC_IMPL_LIB_H_

typedef struct {
  UINT8 NumberOfHosts;
  BOOLEAN UseTop;
  UINT32 MsdcPadTuneReg;
  UINT32 TuningStep[2];
  BOOLEAN AsyncFifo;
  BOOLEAN BusyCheck;
  BOOLEAN StopClkFix;
  BOOLEAN EnhanceRx;
} MSDC_PLATFORM_INFO;

VOID
GetSourceClockRate (
  UINT32 Index,
  UINTN *Hz
  );

VOID
SourceClockControl (
  UINT32 Index,
  BOOLEAN Enable
  );

VOID
ClockControl (
  UINT32 Index,
  BOOLEAN Enable
  );

VOID
PowerControl (
  UINT32 Index,
  BOOLEAN Enable
  );

VOID
InitGpio (
  UINT32 Index
  );

extern MSDC_PLATFORM_INFO PlatformInfo;

#endif /* _MSDC_IMPL_LIB_H_ */