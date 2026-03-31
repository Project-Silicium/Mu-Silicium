/**
  Copyright (c) 2011-2017, ARM Limited. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/PrePiLib.h>
#include <Library/DebugAgentLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/SerialPortLib.h>
#include <Library/PerformanceLib.h>
#include <Library/PlatformSecLib.h>
#include <Library/PrePiHobListPointerLib.h>
#include <Library/CacheMaintenanceLib.h>
#include <Library/MemoryMapHelperLib.h>
#include <Library/TimerLib.h>
#include <Library/HobLib.h>

#include <Ppi/SecPerformance.h>

#include "Sec.h"

STATIC
VOID
ClearFrameBuffer ()
{
  EFI_MEMORY_REGION_DESCRIPTOR FrameBufferRegion;

  // Locate "Display Reserved" Memory Region
  LocateMemoryRegionByName ("Display Reserved", &FrameBufferRegion);
  LocateMemoryRegionByName ("Display_Reserved", &FrameBufferRegion);

  // Verify Memory Region
  if (!FrameBufferRegion.Address) {
    return;
  }

  // Clear Memory Region
  ZeroMem ((VOID *)FrameBufferRegion.Address, FrameBufferRegion.Length);
}

STATIC
VOID
PrintFirmwareVersion ()
{
  // Clear Frame Buffer
  ClearFrameBuffer ();

  // Initialize Serial Port
  SerialPortInitialize ();

  // Print UEFI Version Message
  DEBUG ((EFI_D_WARN, "\n"));
  DEBUG ((EFI_D_WARN, "Project Silicium for %a %a\n",            FixedPcdGetPtr (PcdSmbiosSystemManufacturer), FixedPcdGetPtr (PcdSmbiosSystemModel)));
  DEBUG ((EFI_D_WARN, "Firmware Version %s Build at %a on %a\n", PcdGetPtr (PcdFirmwareVersionString), __TIME__, __DATE__));
  DEBUG ((EFI_D_WARN, "\n"));
}

STATIC
VOID
InitializeMemory (
  IN UINTN StackBase,
  IN UINTN StackSize)
{
  EFI_STATUS                    Status;
  EFI_MEMORY_REGION_DESCRIPTOR  UefiMemoryRegion;
  EFI_HOB_HANDOFF_INFO_TABLE   *HobList;

  // Locate "DXE Heap" Memory Region
  LocateMemoryRegionByName ("DXE Heap", &UefiMemoryRegion);
  LocateMemoryRegionByName ("DXE_Heap", &UefiMemoryRegion);

  // Save UEFI Memory Details
  UINTN UefiMemoryBase = UefiMemoryRegion.Address;
  UINTN UefiMemorySize = UefiMemoryRegion.Length;

  // Verify UEFI Memory Base
  if (!UefiMemoryBase) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate \"DXE Heap\" Memory Region!\n"));
    CpuDeadLoop ();
  }

  // Initialize HOB List
  HobList = HobConstructor ((VOID *)UefiMemoryBase, UefiMemorySize, (VOID *)UefiMemoryBase, (VOID *)(UefiMemoryBase + UefiMemorySize));
  PrePeiSetHobList (HobList);

  // Initialize MMU & Memory HOBs
  Status = MemoryPeim (UefiMemoryBase, UefiMemorySize);
  if (EFI_ERROR (Status)) {
    CpuDeadLoop ();
  }

  // Build Stack HOB
  BuildStackHob (StackBase, StackSize);
}

EFI_STATUS
SecCoreGetMpCoreInfo (
  OUT UINTN          *CoreCount,
  OUT ARM_CORE_INFO **ArmCoreTable)
{
  // Get Core Table Data
  GetPlatformCoreTable (ArmCoreTable, CoreCount);

  return EFI_SUCCESS;
}

ARM_MP_CORE_INFO_PPI mMpCoreInfoPpi = {
  SecCoreGetMpCoreInfo
};

EFI_PEI_PPI_DESCRIPTOR gPlatformPpiTable[] = {
  {
    EFI_PEI_PPI_DESCRIPTOR_PPI,
    &gArmMpCoreInfoPpiGuid,
    &mMpCoreInfoPpi
  }
};

STATIC
EFI_STATUS
GetPlatformPpi (
  IN  EFI_GUID  *PpiGuid,
  OUT VOID     **Ppi)
{
  // Go thru each PPI List Entry
  for (UINTN i = 0; i < ARRAY_SIZE (gPlatformPpiTable); i++) {
    // Compare PPI GUID
    if (CompareGuid (gPlatformPpiTable[i].Guid, PpiGuid)) {
      // Pass PPI
      *Ppi = gPlatformPpiTable[i].Ppi;

      return EFI_SUCCESS;
    }
  }

  return EFI_NOT_FOUND;
}

STATIC
VOID
InitializePpi ()
{
  EFI_STATUS            Status;
  ARM_MP_CORE_INFO_PPI *ArmMpCoreInfoPpi;
  ARM_CORE_INFO        *ArmCoreInfoTable;
  UINTN                 ArmCoreCount;

  // Get Platform PPI
  Status = GetPlatformPpi (&gArmMpCoreInfoPpiGuid, (VOID **)&ArmMpCoreInfoPpi);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to get Platform PPI!\n"));
    CpuDeadLoop ();
  }

  // Get MPCore Infos
  Status = ArmMpCoreInfoPpi->GetMpCoreInfo (&ArmCoreCount, &ArmCoreInfoTable);
  if (!EFI_ERROR (Status) && ArmCoreCount > 0) {
    // Build MPCore Info HOB
    BuildGuidDataHob (&gArmMpCoreInfoGuid, ArmCoreInfoTable, sizeof (ARM_CORE_INFO) * ArmCoreCount);
  }
}

STATIC
VOID
DecompressFvs ()
{
  EFI_PEI_FILE_HANDLE FileHandle;
  EFI_PEI_FV_HANDLE   VolumeHandle;
  UINTN               Instance;

  // Go thru each FFS Volume
  for (Instance = 0; !EFI_ERROR (FfsFindNextVolume (Instance, &VolumeHandle)); Instance++) {
    // Reset File Handle
    FileHandle = NULL;

    // Go thru all FFS Volume Files
    while (!EFI_ERROR (FfsFindNextFile (EFI_FV_FILETYPE_FIRMWARE_VOLUME_IMAGE, VolumeHandle, &FileHandle))) {
      // Process FV File
      FfsProcessFvFile (FileHandle, VolumeHandle);
    }
  }
}

STATIC
VOID
SecMain (
  IN UINT64 StartTimeStamp,
  IN UINTN  StackBase,
  IN UINTN  StackSize)
{
  EFI_STATUS               Status;
  FIRMWARE_SEC_PERFORMANCE Performance;

  // Print Firmware Version
  PrintFirmwareVersion ();

  // Initialize Debug Agent
  InitializeDebugAgent (DEBUG_AGENT_INIT_POSTMEM_SEC, NULL, NULL);
  SaveAndSetDebugTimerInterrupt (TRUE);

  // Initialize Memory
  InitializeMemory (StackBase, StackSize);

  // Build CPU HOB
  BuildCpuHob (ArmGetPhysicalAddressBits (), PcdGet8 (PcdPrePiCpuIoSize));

  // Initialize PPI
  if (ArmIsMpCore ()) {
    InitializePpi ();
  }

  // Store Timer Value
  Performance.ResetEnd = GetTimeInNanoSecond (StartTimeStamp);

  // Build SEC Performance Data Hob
  BuildGuidDataHob (&gEfiFirmwarePerformanceGuid, &Performance, sizeof (Performance));

  // Set the Boot Mode
  SetBootMode (BOOT_WITH_DEFAULT_SETTINGS);

  // Initialize Platform HOBs (CpuHob and FvHob)
  Status = PlatformPeim ();
  if (EFI_ERROR (Status)) {
    CpuDeadLoop ();
  }

  // Register Performance Information
  PERF_START (NULL, "PEI", NULL, StartTimeStamp);

  // Run Library Cconstructors
  ProcessLibraryConstructorList ();

  // Decompress all FVs
  DecompressFvs ();

  // Load DXE Core
  Status = LoadDxeCoreFromFv (NULL, 0);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Load DXE Core!\n"));
  }
}

VOID
CEntryPoint (
  IN UINTN StackBase,
  IN UINTN StackSize)
{
  UINT64 StartTimeStamp;

  // Verify Exception Vector Table
  ASSERT (((UINTN)SecVectorTable & ARM_VECTOR_TABLE_ALIGNMENT) == 0);

  // Enable new Exception Vector Table
  ArmWriteVBar ((UINTN)SecVectorTable);

  // Do Platform Specific Initialization
  PlatformInitialize ();

  // Get Start Time Stamp
  if (PerformanceMeasurementEnabled ()) {
    StartTimeStamp = GetPerformanceCounter ();
  } else {
    StartTimeStamp = 0;
  }

  // Invalidate Stack D-Cache
  InvalidateDataCacheRange ((VOID *)StackBase, StackSize);

  // Enter SEC Main Function
  SecMain (StartTimeStamp, StackBase, StackSize);

  // Should not get Here
  CpuDeadLoop ();
}
