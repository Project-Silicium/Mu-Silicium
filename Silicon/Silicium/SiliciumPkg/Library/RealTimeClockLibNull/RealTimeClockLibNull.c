/**
  Copyright (c), 2018, Andrei Warkentin <andrey.warkentin@gmail.com>
  Copyright (c) Microsoft Corporation. All rights reserved.

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD
  License which accompanies this distribution.  The full text of the license may
  be found at http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR
  IMPLIED.
**/

#include <Library/DebugLib.h>
#include <Library/ArmGenericTimerCounterLib.h>
#include <Library/RealTimeClockLib.h>
#include <Library/TimerLib.h>

EFI_STATUS
EFIAPI
LibGetTime (
  OUT EFI_TIME              *Time,
  OUT EFI_TIME_CAPABILITIES *Capabilities)
{
  UINT64 SecondsPerDay;
  UINT64 SecondsPerHour;
  UINT64 SecondsPerMinute;
  UINT64 ElapsedSeconds;
  UINT32 Freq;

  // Check for Invalid Parameters
  if (Time == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  // Get Timer Freq
  Freq = ArmGenericTimerGetTimerFreq ();
  ASSERT (Freq);

  // Set Capabilities
  if (Capabilities != NULL) {
    Capabilities->Accuracy   = 0;
    Capabilities->Resolution = Freq;
    Capabilities->SetsToZero = FALSE;
  }

  // Get System Run Time
  ElapsedSeconds = GetPerformanceCounter () / Freq;

  // Set Dummy Year & Month
  Time->Year  = 2019;
  Time->Month = 1;

  // Set Dummy Day
  SecondsPerDay = 24 * 60 * 60;
  Time->Day = (ElapsedSeconds / SecondsPerDay);
  ElapsedSeconds %= SecondsPerDay;

  // Set Dummy Hour
  SecondsPerHour = 60 * 60;
  Time->Hour = (ElapsedSeconds / SecondsPerHour);
  ElapsedSeconds %= SecondsPerHour;

  // Set Dummy Minute
  SecondsPerMinute = 60;
  Time->Minute = (ElapsedSeconds / SecondsPerMinute);
  ElapsedSeconds %= SecondsPerMinute;

  // Set Dummy Second
  Time->Second = ElapsedSeconds;

  // Set more Dummy Values
  Time->Nanosecond = 0;
  Time->TimeZone   = 0;
  Time->Daylight   = 0;

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
LibSetTime (IN EFI_TIME *Time)
{
  return EFI_UNSUPPORTED;
}

EFI_STATUS
EFIAPI
LibGetWakeupTime (
  OUT BOOLEAN  *Enabled,
  OUT BOOLEAN  *Pending,
  OUT EFI_TIME *Time)
{
  return EFI_UNSUPPORTED;
}

EFI_STATUS
EFIAPI
LibSetWakeupTime (
  IN  BOOLEAN   Enabled,
  OUT EFI_TIME *Time)
{
  return EFI_UNSUPPORTED;
}

VOID
EFIAPI
LibRtcVirtualNotifyEvent (
  IN EFI_EVENT  Event,
  IN VOID      *Context)
{
  return;
}

EFI_STATUS
EFIAPI
LibRtcInitialize (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  return EFI_SUCCESS;
}
