/** @file

  Copyright (c) 2011-2017, ARM Limited. All rights reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiPei.h>

#include <Library/CacheMaintenanceLib.h>
#include <Library/DebugAgentLib.h>
#include <Library/PrePiLib.h>
#include <Library/PrintLib.h>
#include <Library/PrePiHobListPointerLib.h>
#include <Library/TimerLib.h>
#include <Library/PerformanceLib.h>

#include <Ppi/GuidedSectionExtraction.h>
#include <Ppi/ArmMpCoreInfo.h>
#include <Ppi/SecPerformance.h>

#include "PrePi.h"

#include <Library/MemoryMapHelperLib.h>

#include <Guid/DxeMemoryProtectionSettings.h>
#include <Guid/MmMemoryProtectionSettings.h>

#define IS_XIP()  (((UINT64)FixedPcdGet64 (PcdFdBaseAddress) > mSystemMemoryEnd) ||\
                  ((FixedPcdGet64 (PcdFdBaseAddress) + FixedPcdGet32 (PcdFdSize)) <= FixedPcdGet64 (PcdSystemMemoryBase)))

UINT64  mSystemMemoryEnd = FixedPcdGet64 (PcdSystemMemoryBase) +
                           FixedPcdGet64 (PcdSystemMemorySize) - 1;

EFI_STATUS
GetPlatformPpi (
  IN  EFI_GUID  *PpiGuid,
  OUT VOID      **Ppi
  )
{
  UINTN                   PpiListSize;
  UINTN                   PpiListCount;
  EFI_PEI_PPI_DESCRIPTOR  *PpiList;
  UINTN                   Index;

  PpiListSize = 0;
  ArmPlatformGetPlatformPpiList (&PpiListSize, &PpiList);
  PpiListCount = PpiListSize / sizeof (EFI_PEI_PPI_DESCRIPTOR);
  for (Index = 0; Index < PpiListCount; Index++, PpiList++) {
    if (CompareGuid (PpiList->Guid, PpiGuid) == TRUE) {
      *Ppi = PpiList->Ppi;
      return EFI_SUCCESS;
    }
  }

  return EFI_NOT_FOUND;
}

VOID
PrePiMain (
  IN  UINT64  StartTimeStamp
  )
{
  EFI_HOB_HANDOFF_INFO_TABLE  *HobList;
  ARM_MP_CORE_INFO_PPI        *ArmMpCoreInfoPpi;
  UINTN                       ArmCoreCount;
  ARM_CORE_INFO               *ArmCoreInfoTable;
  EFI_STATUS                  Status;
  UINTN                       StacksSize;
  FIRMWARE_SEC_PERFORMANCE    Performance;

  DXE_MEMORY_PROTECTION_SETTINGS DxeSettings;
  MM_MEMORY_PROTECTION_SETTINGS  MmSettings;

  UINTN MemoryBase     = 0;
  UINTN MemorySize     = 0;
  UINTN UefiMemoryBase = 0;
  UINTN UefiMemorySize = 0;
  UINTN StacksBase     = 0;

  ARM_MEMORY_REGION_DESCRIPTOR_EX DxeHeap;
  ARM_MEMORY_REGION_DESCRIPTOR_EX UefiStack;
  ARM_MEMORY_REGION_DESCRIPTOR_EX UefiFd;

  Status = LocateMemoryMapAreaByName("DXE Heap", &DxeHeap);
  ASSERT_EFI_ERROR (Status);

  Status = LocateMemoryMapAreaByName("UEFI Stack", &UefiStack);
  ASSERT_EFI_ERROR (Status);

  Status = LocateMemoryMapAreaByName("UEFI FD", &UefiFd);
  ASSERT_EFI_ERROR (Status);

  // Declare UEFI region
  MemoryBase     = FixedPcdGet32(PcdSystemMemoryBase);
  MemorySize     = FixedPcdGet32(PcdSystemMemorySize);
  UefiMemoryBase = DxeHeap.Address;
  UefiMemorySize = DxeHeap.Length;
  StacksBase     = UefiStack.Address;
  StacksSize     = UefiStack.Length;
  StacksBase     = UefiMemoryBase + UefiMemorySize - StacksSize;

  DEBUG ((EFI_D_WARN, "1\n"));

  // If ensure the FD is either part of the System Memory or totally outside of the System Memory (XIP)
  ASSERT (
    IS_XIP () ||
    ((UefiFd.Address >= MemoryBase) &&
     ((UINT64)(UefiFd.Address + UefiFd.Length) <= (UINT64)mSystemMemoryEnd))
    );

  DEBUG ((EFI_D_WARN, "2\n"));

  // Initialize the architecture specific bits
  ArchInitialize ();

  DEBUG ((EFI_D_WARN, "3\n"));

  // Initialize the Debug Agent for Source Level Debugging
  InitializeDebugAgent (DEBUG_AGENT_INIT_POSTMEM_SEC, NULL, NULL);
  SaveAndSetDebugTimerInterrupt (TRUE);

  DEBUG ((EFI_D_WARN, "4\n"));

  // Declare the PI/UEFI memory region
  HobList = HobConstructor (
              (VOID *)UefiMemoryBase,
              UefiMemorySize,
              (VOID *)UefiMemoryBase,
              (VOID *)StacksBase // The top of the UEFI Memory is reserved for the stacks
              );
  PrePeiSetHobList (HobList);

  DEBUG ((EFI_D_WARN, "5\n"));

  // Initialize MMU and Memory HOBs (Resource Descriptor HOBs)
  Status = MemoryPeim (UefiMemoryBase, UefiMemorySize);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((EFI_D_WARN, "6\n"));

  BuildStackHob (StacksBase, StacksSize);

  DEBUG ((EFI_D_WARN, "7\n"));

  // TODO: Call CpuPei as a library
  BuildCpuHob (ArmGetPhysicalAddressBits (), PcdGet8 (PcdPrePiCpuIoSize));

  DEBUG ((EFI_D_WARN, "8\n"));

  if (ArmIsMpCore ()) {
    // Only MP Core platform need to produce gArmMpCoreInfoPpiGuid
    Status = GetPlatformPpi (&gArmMpCoreInfoPpiGuid, (VOID **)&ArmMpCoreInfoPpi);

    DEBUG ((EFI_D_WARN, "9\n"));

    // On MP Core Platform we must implement the ARM MP Core Info PPI (gArmMpCoreInfoPpiGuid)
    ASSERT_EFI_ERROR (Status);

    // Build the MP Core Info Table
    ArmCoreCount = 0;
    Status       = ArmMpCoreInfoPpi->GetMpCoreInfo (&ArmCoreCount, &ArmCoreInfoTable);
    if (!EFI_ERROR (Status) && (ArmCoreCount > 0)) {
      // Build MPCore Info HOB
      BuildGuidDataHob (&gArmMpCoreInfoGuid, ArmCoreInfoTable, sizeof (ARM_CORE_INFO) * ArmCoreCount);
    }
  }

  DEBUG ((EFI_D_WARN, "10\n"));

  // Store timer value logged at the beginning of firmware image execution
  Performance.ResetEnd = GetTimeInNanoSecond (StartTimeStamp);

  DEBUG ((EFI_D_WARN, "11\n"));

  // Build SEC Performance Data Hob
  BuildGuidDataHob (&gEfiFirmwarePerformanceGuid, &Performance, sizeof (Performance));

  DEBUG ((EFI_D_WARN, "12\n"));

  // Set the Boot Mode
  SetBootMode (ArmPlatformGetBootMode ());

  DEBUG ((EFI_D_WARN, "13\n"));

  // Initialize Platform HOBs (CpuHob and FvHob)
  Status = PlatformPeim ();
  ASSERT_EFI_ERROR (Status);

  DEBUG ((EFI_D_WARN, "14\n"));

  DxeSettings = (DXE_MEMORY_PROTECTION_SETTINGS)DXE_MEMORY_PROTECTION_SETTINGS_OFF;
  MmSettings  = (MM_MEMORY_PROTECTION_SETTINGS)MM_MEMORY_PROTECTION_SETTINGS_OFF;

  BuildGuidDataHob(&gDxeMemoryProtectionSettingsGuid, &DxeSettings, sizeof(DxeSettings));
  BuildGuidDataHob(&gMmMemoryProtectionSettingsGuid, &MmSettings, sizeof(MmSettings));

  DEBUG ((EFI_D_WARN, "15\n"));

  // Now, the HOB List has been initialized, we can register performance information
  PERF_START (NULL, "PEI", NULL, StartTimeStamp);

  DEBUG ((EFI_D_WARN, "16\n"));

  // SEC phase needs to run library constructors by hand.
  ProcessLibraryConstructorList ();

  DEBUG ((EFI_D_WARN, "17\n"));

  // Assume the FV that contains the SEC (our code) also contains a compressed FV.
  Status = DecompressFirstFv ();
  ASSERT_EFI_ERROR (Status);

  DEBUG ((EFI_D_WARN, "18\n"));

  // Load the DXE Core and transfer control to it
  Status = LoadDxeCoreFromFv (NULL, 0);
  ASSERT_EFI_ERROR (Status);
}

VOID
CEntryPoint ()
{
  UINT64  StartTimeStamp;
  EFI_STATUS Status;
  ARM_MEMORY_REGION_DESCRIPTOR_EX UefiFd;

  // Initialize the platform specific controllers
  ArmPlatformInitialize (0);

  if (PerformanceMeasurementEnabled ()) {
    // Initialize the Timer Library to setup the Timer HW controller
    TimerConstructor ();
    // We cannot call yet the PerformanceLib because the HOB List has not been initialized
    StartTimeStamp = GetPerformanceCounter ();
  } else {
    StartTimeStamp = 0;
  }

  // Data Cache enabled on Primary core when MMU is enabled.
  ArmDisableDataCache ();
  // Invalidate instruction cache
  ArmInvalidateInstructionCache ();
  // Enable Instruction Caches on all cores.
  ArmEnableInstructionCache ();

  // Define the Global Variable region when we are not running in XIP
  if (!IS_XIP ()) {
    if (ArmIsMpCore ()) {
      // Signal the Global Variable Region is defined (event: ARM_CPU_EVENT_DEFAULT)
      ArmCallSEV ();
    }
  }

  Status = LocateMemoryMapAreaByName("UEFI FD", &UefiFd);
  ASSERT_EFI_ERROR (Status);

  InvalidateDataCacheRange (
    (VOID *)UefiFd.Address,
    UefiFd.Length
    );

  // Goto primary Main.
  PrePiMain (StartTimeStamp);

  // DXE Core should always load and never return
  ASSERT (FALSE);
}

