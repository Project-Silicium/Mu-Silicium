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
#include <Library/MemoryMapHelperLib.h>

#include <Ppi/GuidedSectionExtraction.h>
#include <Ppi/ArmMpCoreInfo.h>
#include <Ppi/SecPerformance.h>

#include "PrePi.h"

#define IS_XIP()                (((UINT64)FixedPcdGet64 (PcdFdBaseAddress) > mSystemMemoryEnd) || ((FixedPcdGet64 (PcdFdBaseAddress) + FixedPcdGet32 (PcdFdSize)) <= FixedPcdGet64 (PcdSystemMemoryBase)))

ARM_MEMORY_REGION_DESCRIPTOR_EX UefiFd;
UINT64                          mSystemMemoryEnd;

EFI_STATUS
GetPlatformPpi (
  IN  EFI_GUID *PpiGuid,
  OUT VOID    **Ppi)
{
  UINTN                   PpiListSize  = 0;
  UINTN                   PpiListCount = 0;
  EFI_PEI_PPI_DESCRIPTOR *PpiList      = NULL;

  ArmPlatformGetPlatformPpiList (&PpiListSize, &PpiList);

  PpiListCount = PpiListSize / sizeof (EFI_PEI_PPI_DESCRIPTOR);

  for (UINTN Index = 0; Index < PpiListCount; Index++, PpiList++) {
    if (CompareGuid (PpiList->Guid, PpiGuid) == TRUE) {
      *Ppi = PpiList->Ppi;
      return EFI_SUCCESS;
    }
  }

  return EFI_NOT_FOUND;
}

VOID
PrePiMain (IN UINT64 StartTimeStamp)
{
  EFI_STATUS                      Status;
  EFI_HOB_HANDOFF_INFO_TABLE     *HobList;
  ARM_MP_CORE_INFO_PPI           *ArmMpCoreInfoPpi;
  ARM_CORE_INFO                  *ArmCoreInfoTable;
  ARM_MEMORY_REGION_DESCRIPTOR_EX DxeHeap;
  ARM_MEMORY_REGION_DESCRIPTOR_EX UefiStack;
  ARM_MEMORY_REGION_DESCRIPTOR_EX UefiFd;
  FIRMWARE_SEC_PERFORMANCE        Performance;
  UINTN                           ArmCoreCount;

  Status = LocateMemoryMapAreaByName("DXE Heap", &DxeHeap);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Get DXE Heap Memory Region!\n"));
    ASSERT_EFI_ERROR (Status);
  }

  Status = LocateMemoryMapAreaByName("UEFI Stack", &UefiStack);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Get UEFI Stack Memory Region!\n"));
    ASSERT_EFI_ERROR (Status);
  }

  // Declare UEFI region
  UINTN MemoryBase     = FixedPcdGet32(PcdSystemMemoryBase);
  UINTN UefiMemoryBase = DxeHeap.Address;
  UINTN UefiMemorySize = DxeHeap.Length;
  UINTN StacksSize     = UefiStack.Length;
  UINTN StacksBase     = UefiMemoryBase + UefiMemorySize - StacksSize;

  // Get System Memory End
  mSystemMemoryEnd = FixedPcdGet64 (PcdSystemMemoryBase) + FixedPcdGet64 (PcdSystemMemorySize) - 1;

  // If ensure the FD is either part of the System Memory or totally outside of the System Memory (XIP)
  ASSERT (IS_XIP () || ((UefiFd.Address >= MemoryBase) && ((UINT64)(UefiFd.Address + UefiFd.Length) <= (UINT64)mSystemMemoryEnd)));

  // Initialize the architecture specific bits
  ArchInitialize ();

  // Initialize the Debug Agent for Source Level Debugging
  InitializeDebugAgent (DEBUG_AGENT_INIT_POSTMEM_SEC, NULL, NULL);
  SaveAndSetDebugTimerInterrupt (TRUE);

  // Declare the PI/UEFI memory region
  HobList = HobConstructor ((VOID *)UefiMemoryBase, UefiMemorySize, (VOID *)UefiMemoryBase, (VOID *)StacksBase);
  PrePeiSetHobList (HobList);

  DEBUG ((EFI_D_WARN, "MMU In\r"));

  // Initialize MMU and Memory HOBs (Resource Descriptor HOBs)
  Status = MemoryPeim (UefiMemoryBase, UefiMemorySize);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Enable MMU and Memory HOBs!\n"));
    ASSERT_EFI_ERROR (Status);
  }

  BuildStackHob (StacksBase, StacksSize);

  // TODO: Call CpuPei as a library
  BuildCpuHob (ArmGetPhysicalAddressBits (), PcdGet8 (PcdPrePiCpuIoSize));

  if (ArmIsMpCore ()) {
    // Only MP Core platform need to produce gArmMpCoreInfoPpiGuid
    Status = GetPlatformPpi (&gArmMpCoreInfoPpiGuid, (VOID **)&ArmMpCoreInfoPpi);

    // On MP Core Platform we must implement the ARM MP Core Info PPI (gArmMpCoreInfoPpiGuid)
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "Failed to Get Platform PPI!\n"));
      ASSERT_EFI_ERROR (Status);
    }

    // Build the MP Core Info Table
    ArmCoreCount = 0;
    Status       = ArmMpCoreInfoPpi->GetMpCoreInfo (&ArmCoreCount, &ArmCoreInfoTable);
    if (!EFI_ERROR (Status) && (ArmCoreCount > 0)) {
      // Build MPCore Info HOB
      BuildGuidDataHob (&gArmMpCoreInfoGuid, ArmCoreInfoTable, sizeof (ARM_CORE_INFO) * ArmCoreCount);
    }
  }

  // Store timer value logged at the beginning of firmware image execution
  Performance.ResetEnd = GetTimeInNanoSecond (StartTimeStamp);

  // Build SEC Performance Data Hob
  BuildGuidDataHob (&gEfiFirmwarePerformanceGuid, &Performance, sizeof (Performance));

  // Set the Boot Mode
  SetBootMode (ArmPlatformGetBootMode ());

  // Initialize Platform HOBs (CpuHob and FvHob)
  PlatformPeim ();

  // Now, the HOB List has been initialized, we can register performance information
  PERF_START (NULL, "PEI", NULL, StartTimeStamp);

  // SEC phase needs to run library constructors by hand.
  ProcessLibraryConstructorList ();

  // Assume the FV that contains the SEC (our code) also contains a compressed FV.
  Status = DecompressFirstFv ();
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Decompress First FV!\n"));
    ASSERT_EFI_ERROR (Status);
  }

  // Load the DXE Core and transfer control to it
  Status = LoadDxeCoreFromFv (NULL, 0);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Load DXE Core!\n"));
    ASSERT_EFI_ERROR (Status);
  }
}

VOID
CEntryPoint ()
{
  EFI_STATUS Status;
  UINT64     StartTimeStamp;

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
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Get UEFI FD Memory Region!\n"));
    ASSERT_EFI_ERROR (Status);
  }

  InvalidateDataCacheRange ((VOID *)UefiFd.Address, UefiFd.Length);

  // Goto primary Main.
  PrePiMain (StartTimeStamp);
}
