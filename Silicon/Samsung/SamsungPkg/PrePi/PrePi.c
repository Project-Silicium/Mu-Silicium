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
#include <Library/IoLib.h>
#include <Library/SerialPortLib.h>
#include <Library/PlatformPrePiLib.h>

#include <libfdt.h>

#include <Ppi/SecPerformance.h>

#include "PrePi.h"

STATIC ARM_MEMORY_REGION_DESCRIPTOR_EX UefiFd;

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
  ARM_MEMORY_REGION_DESCRIPTOR_EX DxeHeap;
  ARM_MEMORY_REGION_DESCRIPTOR_EX UefiStack;
  FIRMWARE_SEC_PERFORMANCE        Performance;

  // Get DXE Heap Memory Region Infos
  Status = LocateMemoryMapAreaByName ("DXE Heap", &DxeHeap);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Get DXE Heap Memory Region!\n"));
    ASSERT_EFI_ERROR (Status);
  }

  // Get UEFI Stack Memory Region Infos
  Status = LocateMemoryMapAreaByName ("UEFI Stack", &UefiStack);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Get UEFI Stack Memory Region!\n"));
    ASSERT_EFI_ERROR (Status);
  }

  // Declare UEFI Regions
  UINTN MemoryBase     = FixedPcdGet32 (PcdSystemMemoryBase);
  UINTN UefiMemoryBase = DxeHeap.Address;
  UINTN UefiMemorySize = DxeHeap.Length;
  UINTN StacksSize     = UefiStack.Length;
  UINTN StacksBase     = UefiMemoryBase + UefiMemorySize - StacksSize;

  // Check if we are in System Memory and beyond
  ASSERT (IS_XIP () || ((UefiFd.Address >= MemoryBase) && ((UINT64)(UefiFd.Address + UefiFd.Length) <= (UINT64)mSystemMemoryEnd)));

  // Init Architecture Specific Bits
  ArchInitialize ();

  // Init Debug Agent
  InitializeDebugAgent          (DEBUG_AGENT_INIT_POSTMEM_SEC, NULL, NULL);
  SaveAndSetDebugTimerInterrupt (TRUE);

  // Declare PI/UEFI Memory Region
  HobList = HobConstructor ((VOID *)UefiMemoryBase, UefiMemorySize, (VOID *)UefiMemoryBase, (VOID *)StacksBase);
  PrePeiSetHobList (HobList);

  DEBUG ((EFI_D_WARN, "MMU In\r"));

  // Init MMU and Memory HOBs
  Status = MemoryPeim (UefiMemoryBase, UefiMemorySize);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Enable MMU and Memory HOBs!\n"));
    ASSERT_EFI_ERROR (Status);
  }

  // Build Stack HOB
  BuildStackHob (StacksBase, StacksSize);

  // Build CPU HOB
  BuildCpuHob ((UINT8)ArmGetPhysicalAddressBits (), PcdGet8 (PcdPrePiCpuIoSize));

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

    // Build MP Core Info Table
    Status = ArmMpCoreInfoPpi->GetMpCoreInfo (&ArmCoreCount, &ArmCoreInfoTable);
    if (!EFI_ERROR (Status) && ArmCoreCount > 0) {
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
    DEBUG ((EFI_D_ERROR, "Failed to Load DXE Core!\n"));
    ASSERT_EFI_ERROR (Status);
  }
}

VOID
CEntryPoint ()
{
  EFI_STATUS                      Status;
  ARM_MEMORY_REGION_DESCRIPTOR_EX Display;
  ARM_MEMORY_REGION_DESCRIPTOR_EX FdtPointer;
  UINT64                          StartTimeStamp;
  INT32                           NodeOffset;

  // Init Serial Port
  SerialPortInitialize ();

#ifdef EFI_DEBUG
  ARM_MEMORY_REGION_DESCRIPTOR_EX PStore;

  // Clear PStore Region
  Status = LocateMemoryMapAreaByName ("PStore", &PStore);
  if (!EFI_ERROR (Status)) {
    UINT8 *Base = (UINT8 *)PStore.Address;

    for (UINTN i = 0; i < PStore.Length; i++) {
      Base[i] = 0;
    }
  }
#endif

  if (FixedPcdGetBool (PcdForceEnableFBWrites)) {
    // Locate FDT Pointer
    Status = LocateMemoryMapAreaByName ("FDT Pointer", &FdtPointer);
    if (EFI_ERROR (Status)) {
      // Enable Frame Buffer Writes
      MmioWrite32 (PcdGet32 (PcdDeconAddress) + HW_SW_TRIG_CONTROL, 0x1281);

      goto PlatformInit;
    }

    // Get FDT Base Address
    CONST VOID *FDT = (CONST VOID*)(UINTN)MmioRead32 (FdtPointer.Address);

    // Check for Valid Header
    if (fdt_check_header (FDT)) {
      DEBUG ((EFI_D_WARN, "Invalid FDT Header!\n"));
      goto PlatformInit;
    }

    NodeOffset = 0;

    // Parse all FDT Nodes
    while ((NodeOffset = fdt_next_node (FDT, NodeOffset, NULL)) >= 0) {

      // Get Current Node Name
      CONST CHAR8 *NodeName = fdt_get_name (FDT, NodeOffset, NULL);

      // Check for "decon_f" Node
      if (NodeName && AsciiStrStr (NodeName, "decon_f@")) {
        INT32 RegLength;

        // Get "decon_f" Register Length
        CONST UINT32 *Reg = fdt_getprop (FDT, NodeOffset, "reg", &RegLength);

        // Check for Valid Base & Length Address
        if (Reg && RegLength > 0) {
          // Get Number of "decon_f" Nodes
          INT32 NumberOfRegs = RegLength / sizeof (INT32);

          for (INT32 i = 0; i < NumberOfRegs; i++) {
            // Get "deconf_f" Base Address
            UINT32 RegValue = fdt32_to_cpu (Reg[i]);

            // Check for Valid Base Address
            if (RegValue != 0x0 && RegValue != 0x10000) {
              // Enable Frame Buffer Writes
              MmioWrite32 (RegValue + HW_SW_TRIG_CONTROL, 0x1281);
            }
          }
        }
      }
    }
  }

PlatformInit:
  // Run Specific SoC Code
  PlatformInitialize ();

  // Clear Screen
  Status = LocateMemoryMapAreaByName ("Display Reserved", &Display);
  if (!EFI_ERROR (Status)) {
    ZeroMem ((VOID *)Display.Address, Display.Length);
  }

  // Clear Primary Screen
  Status = LocateMemoryMapAreaByName ("Display Reserved-1", &Display);
  if (!EFI_ERROR (Status)) {
    ZeroMem ((VOID *)Display.Address, Display.Length);
  }

  // Clear Secondary Screen
  Status = LocateMemoryMapAreaByName ("Display Reserved-2", &Display);
  if (!EFI_ERROR (Status)) {
    ZeroMem ((VOID *)Display.Address, Display.Length);
  }

  // Print UEFI Message
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
    StartTimeStamp = 0;
  }

  // Disable Data Cache
  ArmDisableDataCache ();

  // Invalidate Instruction Cache
  ArmInvalidateInstructionCache ();

  // Enable Instruction Caches
  ArmEnableInstructionCache ();

  // Define Global Variable Region
  if (!IS_XIP ()) {
    if (ArmIsMpCore ()) {
      // Signal Variable Region is Defined
      ArmCallSEV ();
    }
  }

  // Get UEFI FD Memory Region Infos
  Status = LocateMemoryMapAreaByName ("UEFI FD", &UefiFd);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Get UEFI FD Memory Region!\n"));
    ASSERT_EFI_ERROR (Status);
  }

  // Invalidate Data Cache
  InvalidateDataCacheRange ((VOID *)UefiFd.Address, UefiFd.Length);

  // Go to Main Function
  PrePiMain (StartTimeStamp);
}
