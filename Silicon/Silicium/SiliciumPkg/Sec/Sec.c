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
#include <Library/PlatformSecLib.h>
#include <Library/PrePiHobListPointerLib.h>
#include <Library/CacheMaintenanceLib.h>
#include <Library/MemoryMapHelperLib.h>
#include <Library/TimerLib.h>
#include <Library/HobLib.h>

#include "Sec.h"

STATIC EFI_MEMORY_REGION_DESCRIPTOR UefiFdRegion;

#ifndef MDEPKG_NDEBUG
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
#endif

STATIC
VOID
PrintFirmwareVersion ()
{
#ifndef MDEPKG_NDEBUG
  // Clear Frame Buffer
  ClearFrameBuffer ();
#endif

  // Print UEFI Version Message
  DEBUG ((EFI_D_WARN, "\n"));
  DEBUG ((EFI_D_WARN, "Project Silicium for %a %a\n",            FixedPcdGetPtr (PcdSmbiosSystemManufacturer), FixedPcdGetPtr (PcdSmbiosSystemModel)));
  DEBUG ((EFI_D_WARN, "Firmware Version %s Build at %a on %a\n", PcdGetPtr (PcdFirmwareVersionString), __TIME__, __DATE__));
  DEBUG ((EFI_D_WARN, "\n"));
}

STATIC
EFI_STATUS
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
    return EFI_NOT_FOUND;
  }

  // Initialize HOB List
  HobList = HobConstructor ((VOID *)UefiMemoryBase, UefiMemorySize, (VOID *)UefiMemoryBase, (VOID *)(UefiMemoryBase + UefiMemorySize));
  PrePeiSetHobList (HobList);

  // Initialize MMU & Memory HOBs
  Status = MemoryPeim (UefiMemoryBase, UefiMemorySize);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Build Stack HOB
  BuildStackHob (StackBase, StackSize);

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
InitializeMpCoreInfo ()
{
  ARM_CORE_INFO *ArmCoreInfoTable;
  UINTN          ArmCoreCount;

  // Get Core Table Data
  GetPlatformCoreTable (&ArmCoreInfoTable, &ArmCoreCount);

  // Verify ARM Core Info Table
  if (ArmCoreInfoTable == NULL) {
    DEBUG ((EFI_D_ERROR, "ARM Core Info Table is NULL!\n"));
    return EFI_UNSUPPORTED;
  }

  // Build MPCore Info HOB
  BuildGuidDataHob (&gArmMpCoreInfoGuid, ArmCoreInfoTable, sizeof (ARM_CORE_INFO) * ArmCoreCount);

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
DecompressFvs ()
{
  EFI_PEI_FV_HANDLE VolumeHandle = NULL;
  BOOLEAN           FvsExist     = FALSE;

  // Go thru each FFS Volume
  for (UINTN Instance = 0; !EFI_ERROR (FfsFindNextVolume (Instance, &VolumeHandle)); Instance++) {
    EFI_PEI_FILE_HANDLE FileHandle = NULL;

    // Go thru all FFS Volume Files
    while (!EFI_ERROR (FfsFindNextFile (EFI_FV_FILETYPE_FIRMWARE_VOLUME_IMAGE, VolumeHandle, &FileHandle))) {
      EFI_STATUS Status;

      // Process FV File
      Status = FfsProcessFvFile (FileHandle, VolumeHandle);
      if (EFI_ERROR (Status)) {
        DEBUG ((EFI_D_ERROR, "Failed to Process FV Instance %u! Status = %r\n", Instance, Status));
        continue;
      }

      // Set FV Bool
      FvsExist = TRUE;
    }
  }

  // Verify Amount of Instances
  if (!FvsExist) {
    DEBUG ((EFI_D_ERROR, "No FVs were Found!\n"));
    return EFI_NOT_FOUND;
  }

  return EFI_SUCCESS;
}

STATIC
VOID
SecMain (
  IN UINTN StackBase,
  IN UINTN StackSize)
{
  EFI_STATUS Status;

  // Initialize Serial Port
  Status = SerialPortInitialize ();
  if (EFI_ERROR (Status)) {
    return;
  }

  // Print Firmware Version
  PrintFirmwareVersion ();

  // Initialize Debug Agent
  InitializeDebugAgent (DEBUG_AGENT_INIT_POSTMEM_SEC, NULL, NULL);
  SaveAndSetDebugTimerInterrupt (TRUE);

  // Initialize Memory
  Status = InitializeMemory (StackBase, StackSize);
  if (EFI_ERROR (Status)) {
    return;
  }

  // Build CPU HOB
  BuildCpuHob (ArmGetPhysicalAddressBits (), PcdGet8 (PcdPrePiCpuIoSize));

  // Initialize PPI
  if (ArmIsMpCore ()) {
    Status = InitializeMpCoreInfo ();
    if (EFI_ERROR (Status)) {
      return;
    }
  }

  // Set the Boot Mode
  SetBootMode (BOOT_WITH_DEFAULT_SETTINGS);

  // Initialize Platform HOBs (CpuHob and FvHob)
  Status = PlatformPeim ();
  if (EFI_ERROR (Status)) {
    return;
  }

  // Run Library Cconstructors
  ProcessLibraryConstructorList ();

  // Decompress all FVs
  Status = DecompressFvs ();
  if (EFI_ERROR (Status)) {
    return;
  }

  // Load DXE Core
  Status = LoadDxeCoreFromFv (NULL, 0);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Load DXE Core! Status = %r\n", Status));
  }
}

VOID
CEntryPoint (
  IN UINTN StackBase,
  IN UINTN StackSize)
{
    EFI_STATUS Status;
  // Verify Exception Vector Table
  ASSERT (((UINTN)SecVectorTable & ARM_VECTOR_TABLE_ALIGNMENT) == 0);

  // Enable new Exception Vector Table
  ArmWriteVBar ((UINTN)SecVectorTable);

  // Do Platform Specific Initialization
  PlatformInitialize ();

  // Locate "UEFI FD" Memory Region
  Status = LocateMemoryRegionByName ("UEFI FD", &UefiFdRegion);
  if (EFI_ERROR (Status)) {
    // Locate "UEFI_FD" Memory Region
    Status = LocateMemoryRegionByName ("UEFI_FD", &UefiFdRegion);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "Failed to Locate 'UEFI FD' Memory Region!\n"));
      ASSERT_EFI_ERROR (Status);
    }
  }

  // Invalidate Stack D-Cache
  InvalidateDataCacheRange ((VOID *)StackBase, StackSize);

  // Enter SEC Main Function
  SecMain (StackBase, StackSize);

  // Something went Wrong
  CpuDeadLoop ();
}
