/**
  Copyright (c) 2011 - 2021, Arm Limited. All rights reserved.<BR>
  Copyright (C) 2022 Svyatoslav Ryhel <clamor95@gmail.com>

  SPDX-License-Identifier: GPL-2.0+
**/

#include <Library/TimerLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryMapHelperLib.h>
#include <Library/IoLib.h>

#ifdef MDE_CPU_ARM
#define MULT_U64_X_N MultU64x32
#else
#define MULT_U64_X_N MultU64x64
#endif

STATIC EFI_MEMORY_REGION_DESCRIPTOR_EX TegraTimerMemoryRegion;

RETURN_STATUS
EFIAPI
TimerConstructor ()
{
  EFI_STATUS Status;

  // Get Tegra Timer Memory Region
  Status = LocateMemoryMapAreaByName ("Tegra Timer", &TegraTimerMemoryRegion);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Get Tegra Timer Memory Region! Status = %r\n", Status));
    return RETURN_UNSUPPORTED;
  }

  return RETURN_SUCCESS;
}

STATIC
UINTN
EFIAPI
GetPlatformTimerFreq () { return 1000000; /* 1 MHz */ }

UINT32
EFIAPI
GetTegraTimerSystemCount () { return MmioRead32 (TegraTimerMemoryRegion.Address + 0x10); }

UINTN
EFIAPI
MicroSecondDelay (IN UINTN MicroSeconds)
{
  UINT32 TimerTicks32;
  UINT32 SystemCounterVal;

  // Calculate Timer Ticks for Loop
  TimerTicks32 = DivU64x32 (MULT_U64_X_N (MicroSeconds, GetPlatformTimerFreq ()), 1000000U);

  // Get System Counter
  SystemCounterVal = GetTegraTimerSystemCount ();

  // Add System Counter to Timer Ticks
  TimerTicks32 += SystemCounterVal;

  // Loop Until All Microseconds are passed
  while (SystemCounterVal < TimerTicks32) {
    SystemCounterVal = GetTegraTimerSystemCount ();
  }

  return MicroSeconds;
}

UINTN
EFIAPI
NanoSecondDelay (IN UINTN NanoSeconds)
{
  UINTN MicroSeconds;

  // Calculate Microseconds
  MicroSeconds  = NanoSeconds / 1000;
  MicroSeconds += ((NanoSeconds % 1000) == 0) ? 0 : 1;

  MicroSecondDelay (MicroSeconds);

  return NanoSeconds;
}

UINT64
EFIAPI
GetPerformanceCounter () { return (UINT32)GetTegraTimerSystemCount (); }

UINT64
EFIAPI
GetPerformanceCounterProperties (
  OUT UINT64  *StartValue OPTIONAL,
  OUT UINT64  *EndValue   OPTIONAL)
{
  if (StartValue != NULL) {
    *StartValue = (UINT32)0ULL;
  }

  if (EndValue != NULL) {
    *EndValue = (UINT32)0xFFFFFFFFFFFFFFFFUL;
  }

  return (UINT32)GetPlatformTimerFreq ();
}

UINT64
EFIAPI
GetTimeInNanoSecond (IN UINT64 Ticks)
{
  UINT64 NanoSeconds;
  UINT32 Remainder;
  UINT32 TimerFreq;

  // Get Timer Freq
  TimerFreq = (UINT32)GetPlatformTimerFreq ();

  // Calculate Nano Seconds
  NanoSeconds  = MULT_U64_X_N (DivU64x32Remainder (Ticks, TimerFreq, &Remainder), 1000000000U);
  NanoSeconds += DivU64x32 (MULT_U64_X_N (Remainder, 1000000000U), TimerFreq);

  return NanoSeconds;
}
