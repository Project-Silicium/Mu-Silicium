/**
  Copyright (c) 2011-2017, ARM Limited. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/CacheMaintenanceLib.h>
#include <Library/DebugAgentLib.h>
#include <Library/PrePiLib.h>
#include <Library/PrePiHobListPointerLib.h>
#include <Library/TimerLib.h>
#include <Library/PerformanceLib.h>
#include <Library/MemoryMapHelperLib.h>
#include <Library/SoCPlatformLib.h>
#include <Library/PcdLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/SerialPortLib.h>
#include <Library/PlatformPrePiLib.h>
#include <Library/MemoryProtectionLib.h>

#include <Ppi/SecPerformance.h>

#include "PrePi.h"

//
// Global Variables
//
STATIC ARM_MEMORY_REGION_DESCRIPTOR_EX UefiFdRegion;

EFI_STATUS
PrePeiCoreGetMpCoreInfo (
  OUT UINTN          *CoreCount,
  OUT ARM_CORE_INFO **ArmCoreTable)
{
  // Get Core Table & Count
  *CoreCount    = PcdGet32 (PcdCoreCount);
  *ArmCoreTable = GetCoreTable ();

  return EFI_SUCCESS;
}

ARM_MP_CORE_INFO_PPI mMpCoreInfoPpi = { PrePeiCoreGetMpCoreInfo };

EFI_PEI_PPI_DESCRIPTOR gPlatformPpiTable[] = {
  {
    EFI_PEI_PPI_DESCRIPTOR_PPI,
    &gArmMpCoreInfoPpiGuid,
    &mMpCoreInfoPpi
  }
};

EFI_STATUS
GetPlatformPpi (
  IN  EFI_GUID *PpiGuid,
  OUT VOID    **Ppi)
{
  UINTN                   PpiListSize  = sizeof (gPlatformPpiTable);
  UINTN                   PpiListCount = PpiListSize / sizeof (EFI_PEI_PPI_DESCRIPTOR);
  EFI_PEI_PPI_DESCRIPTOR *PpiList      = gPlatformPpiTable;

  // Get Correct Ppi
  for (UINTN Index = 0; Index < PpiListCount; Index++, PpiList++) {
    // Compare GUIDs
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
  ARM_MEMORY_REGION_DESCRIPTOR_EX DxeHeapRegion;
  ARM_MEMORY_REGION_DESCRIPTOR_EX UefiStackRegion;
  FIRMWARE_SEC_PERFORMANCE        Performance;

  // Locate "DXE Heap" Memory Region
  Status  = LocateMemoryMapAreaByName ("DXE Heap", &DxeHeapRegion);
  Status |= LocateMemoryMapAreaByName ("DXE_Heap", &DxeHeapRegion);
  if (EFI_ERROR (Status) && !DxeHeapRegion.Address) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate 'DXE Heap' Memory Region!\n"));
    ASSERT_EFI_ERROR (Status);
  }

  // Locate "UEFI Stack" Memory Region
  Status  = LocateMemoryMapAreaByName ("UEFI Stack", &UefiStackRegion);
  Status |= LocateMemoryMapAreaByName ("UEFI_Stack", &UefiStackRegion);
  if (EFI_ERROR (Status) && !UefiStackRegion.Address) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate 'UEFI Stack' Memory Region!\n"));
    ASSERT_EFI_ERROR (Status);
  }

  // Declare UEFI Regions
  UINTN MemoryBase     = FixedPcdGet32 (PcdSystemMemoryBase);
  UINTN UefiMemoryBase = DxeHeapRegion.Address;
  UINTN UefiMemorySize = DxeHeapRegion.Length;
  UINTN StacksSize     = UefiStackRegion.Length;
  UINTN StacksBase     = UefiMemoryBase + UefiMemorySize - StacksSize;

  // Check UEFI Memory Location
  ASSERT (IS_XIP () || ((UefiFdRegion.Address >= MemoryBase) && ((UINT64)(UefiFdRegion.Address + UefiFdRegion.Length) <= (UINT64)mSystemMemoryEnd)));

  // Init Architecture Specific Bits
  ArchInitialize ();

  // Init Debug Agent
  InitializeDebugAgent (DEBUG_AGENT_INIT_POSTMEM_SEC, NULL, NULL);

  // Set Debug Timer Interrupt
  SaveAndSetDebugTimerInterrupt (TRUE);

  // Declare PI/UEFI Memory Region
  HobList = HobConstructor ((VOID *)UefiMemoryBase, UefiMemorySize, (VOID *)UefiMemoryBase, (VOID *)StacksBase);
  PrePeiSetHobList (HobList);

  DEBUG ((EFI_D_WARN, "MMU In\r"));

  // Init MMU and Build Memory HOBs
  Status = MemoryPeim (UefiMemoryBase, UefiMemorySize);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Enable MMU and Build Memory HOBs!\n"));
    ASSERT_EFI_ERROR (Status);
  }

  // Build Stack HOB
  BuildStackHob (StacksBase, StacksSize);

  // Build CPU HOB
  BuildCpuHob ((UINT8)ArmGetPhysicalAddressBits (), PcdGet8 (PcdPrePiCpuIoSize));

  // Check 
  if (ArmIsMpCore ()) {
    ARM_MP_CORE_INFO_PPI *ArmMpCoreInfoPpi = NULL;
    ARM_CORE_INFO        *ArmCoreInfoTable = NULL;
    UINTN                 ArmCoreCount     = 0;

    // Get Platform Ppi
    Status = GetPlatformPpi (&gArmMpCoreInfoPpiGuid, (VOID *)&ArmMpCoreInfoPpi);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "Failed to Get Platform PPI!\n"));
      ASSERT_EFI_ERROR (Status);
    }

    // Get MP Core Infos
    Status = ArmMpCoreInfoPpi->GetMpCoreInfo (&ArmCoreCount, &ArmCoreInfoTable);
    if (!EFI_ERROR (Status) && ArmCoreCount > 0) {
      // Build MP Core Data HOB
      BuildGuidDataHob (&gArmMpCoreInfoGuid, ArmCoreInfoTable, sizeof (ARM_CORE_INFO) * ArmCoreCount);
    }
  }

  // Store Timer Value
  Performance.ResetEnd = GetTimeInNanoSecond (StartTimeStamp);

  // Build SEC Performance Data HOB
  BuildGuidDataHob (&gEfiFirmwarePerformanceGuid, &Performance, sizeof (Performance));

  // Set Boot Mode
  SetBootMode (BOOT_WITH_DEFAULT_SETTINGS);

  // Init Platform HOBs
  PlatformPeim ();

  // Enable Memory Protection
  EnableMemoryProtection ();

  // Register Performance Info
  PERF_START (NULL, "PEI", NULL, StartTimeStamp);

  // Run Library Constructors
  ProcessLibraryConstructorList ();

  // Decompress FV
  Status = DecompressFirstFv ();
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Decompress FV!\n"));
    ASSERT_EFI_ERROR (Status);
  }

  // Load DXE Core
  Status = LoadDxeCoreFromFv (NULL, 0);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Load DXE Core from FV!\n"));
    ASSERT_EFI_ERROR (Status);
  }
}

VOID
CEntryPoint ()
{
  EFI_STATUS                      Status;
  ARM_MEMORY_REGION_DESCRIPTOR_EX DisplayRegion;
  UINT64                          StartTimeStamp;

  // Run SoC Specific Code
  PlatformInitialize ();

  //
  // !!! Make this Debug only !!!
  //

  // Locate 'Display Reserved' Memory Region
  Status = LocateMemoryMapAreaByName ("Display Reserved", &DisplayRegion);
  if (!EFI_ERROR (Status)) {
    // Clear Display
    ZeroMem ((VOID *)DisplayRegion.Address, DisplayRegion.Length);
  }

  // Locate 'Display_Reserved' Memory Region
  Status = LocateMemoryMapAreaByName ("Display_Reserved", &DisplayRegion);
  if (!EFI_ERROR (Status)) {
    // Clear Display
    ZeroMem ((VOID *)DisplayRegion.Address, DisplayRegion.Length);
  }

  //
  // !!! Make this Debug only !!!
  //

  // Init Serial Port
  SerialPortInitialize ();

  // Print UEFI Version Message
  DEBUG ((EFI_D_WARN, "\n"));
  DEBUG ((EFI_D_WARN, "Project Silicium for %a\n",               FixedPcdGetPtr (PcdSmbiosSystemModel)));
  DEBUG ((EFI_D_WARN, "Firmware Version %s Build at %a on %a\n", PcdGetPtr (PcdFirmwareVersionString), __TIME__, __DATE__));
  DEBUG ((EFI_D_WARN, "\n"));

  if (PerformanceMeasurementEnabled ()) {
    // Init Timer HW Controller
    TimerConstructor ();

    // Get Start Time Stamp
    StartTimeStamp = GetPerformanceCounter ();
  } else {
    // Set Dummy Start Time Stamp Value
    StartTimeStamp = 0;
  }

  // Check UEFI Memory Location
  if (!IS_XIP ()) {
    if (ArmIsMpCore ()) {
      // Signal Variable Region is Defined
      ArmCallSEV ();
    }
  }

  // Locate 'UEFI FD' Memory Region
  Status  = LocateMemoryMapAreaByName ("UEFI FD", &UefiFdRegion);
  Status |= LocateMemoryMapAreaByName ("UEFI_FD", &UefiFdRegion);
  if (EFI_ERROR (Status) && !UefiFdRegion.Address) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate 'UEFI FD' Memory Region!\n"));
    ASSERT_EFI_ERROR (Status);
  }

  // Invalidate Data Cache
  InvalidateDataCacheRange ((VOID *)UefiFdRegion.Address, UefiFdRegion.Length);

  // Go to Main Function
  PrePiMain (StartTimeStamp);
}
