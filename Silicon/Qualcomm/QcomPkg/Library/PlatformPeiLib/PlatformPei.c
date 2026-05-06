/**
  Copyright (c) 2011-2014, ARM Limited. All rights reserved.
  Copyright (c) 2014, Linaro Limited. All rights reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/ConfigurationMapLib.h>
#include <Library/ConfigurationMapHelperLib.h>
#include <Library/MemoryMapHelperLib.h>
#include <Library/SerialPortLib.h>
#include <Library/HobLib.h>

#include "EFIUefiConfig.h"
#include "EFISerialPort.h"
#include "EFIShim.h"

STATIC
EFI_STATUS
LocateConfigurationEntryString (
  IN  CHAR8 *EntryName,
  OUT CHAR8 *EntryValue,
  IN  UINTN *EntryValueLength)
{
  // Verify Parameters
  if (EntryName == NULL || EntryValue == NULL || EntryValueLength == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  // Get Platform Type
  CHAR8 *PlatformType = FixedPcdGetPtr (PcdPlatformType);

  // Verify Platform Type
  ASSERT (PlatformType != "NULL");

  // Compare Entry Names
  if (!AsciiStriCmp (EntryName, "OsTypeString")) {
    // Convert Entry Value to String
    AsciiStrCpyS (EntryValue, *EntryValueLength, PlatformType);

    return EFI_SUCCESS;
  }

  return EFI_NOT_FOUND;
}

STATIC
UINTN
SerialPortFlush ()
{
  return EFI_SUCCESS;
}

STATIC
UINTN
SerialPortControl (
  IN UINTN Arg,
  IN UINTN Param)
{
  return EFI_SUCCESS;
}

STATIC
UINTN
SerialPortDrain ()
{
  return EFI_SUCCESS;
}

//
// UEFI Config Library
//
EFI_UEFI_CONFIG_LIBRARY
ConfigLib = {
  0x10002,
  LocateMemoryRegionByName,
  LocateConfigurationEntryString,
  LocateConfigurationEntry32,
  LocateConfigurationEntry64, 
  LocateMemoryRegionByAddress
};

//
// Serial Port Library
//
EFI_SERIAL_PORT_LIBRARY
SioLib = {
  0x10001,
  SerialPortRead,
  SerialPortWrite,
  SerialPortPoll,
  SerialPortDrain,
  SerialPortFlush,
  SerialPortControl,
  SerialPortSetAttributes
};

STATIC
EFI_STATUS
ShimInstallLib (
  IN CHAR8  *LibName,
  IN UINT32  LibVersion,
  IN VOID   *LibIntf)
{
  return EFI_SUCCESS;
}

EFI_SHIM_LIBRARY_INSTANCE_DATA
gShimLibraryInstanceData[] = {
  // Library Instance Name, Library Instance Pointer
  {"UEFI Config Lib", &ConfigLib},
  {"SerialPort Lib",  &SioLib}
};

STATIC
EFI_STATUS
ShimLoadLib (
  IN  CHAR8   *LibName,
  IN  UINT32   LibVersion,
  OUT VOID   **LibIntf)
{
  // Very Parameters
  if (LibName == NULL || LibIntf == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  // Go thru each Shim Library Instance
  for (UINT8 i = 0; i < ARRAY_SIZE (gShimLibraryInstanceData); i++) {
    // Compare Library Instance Names
    if (AsciiStriCmp (LibName, gShimLibraryInstanceData[i].LibName)) {
      continue;
    }

    // Pass Library Instance
    *LibIntf = gShimLibraryInstanceData[i].LibIntf;

    return EFI_SUCCESS;
  }

  return EFI_NOT_FOUND;
}

//
// Shim Library Loader
//
EFI_SHIM_LIBRARY_LOADER
ShLibraryLoader = {
  0x00010001,
  ShimInstallLib,
  ShimLoadLib
};

VOID
BuildXblHobs (
  IN EFI_PHYSICAL_ADDRESS FvDecompressAddr,
  IN EFI_PHYSICAL_ADDRESS SchedulerInterfaceAddr,
  IN EFI_PHYSICAL_ADDRESS DtbExtensionAddr)
{
  EFI_MEMORY_REGION_DESCRIPTOR InfoBlkRegion        = {0};
  UINTN                        ShimLibLoaderAddress = (UINTN)&ShLibraryLoader;
  BOOLEAN                      Prodmode             = FALSE;

  // Locate "Info Blk" Memory Region
  LocateMemoryRegionByName ("Info Blk", &InfoBlkRegion);
  LocateMemoryRegionByName ("Info_Blk", &InfoBlkRegion);

  // Verify Memory Region
  if (!InfoBlkRegion.Address) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate \"Info Blk\" Memory Region!\n"));
  } else {
    // Build Info Blk HOB
    BuildGuidDataHob (&gEfiInfoBlkHobGuid, &InfoBlkRegion.Address, sizeof (InfoBlkRegion.Address));
  }

  // Build Shim HOB
  BuildGuidDataHob (&gEfiShimLibraryHobGuid, &ShimLibLoaderAddress, sizeof (ShimLibLoaderAddress));

  // Build Prodmode HOB
  BuildGuidDataHob (&gEfiProdmodeHobGuid, &Prodmode, sizeof (Prodmode));

  // Build FV Decompress HOB
  if (FvDecompressAddr) {
    BuildGuidDataHob (&gFvDecompressHobGuid, &FvDecompressAddr, sizeof (FvDecompressAddr));
  }

  // Build Scheduler Interface HOB
  if (SchedulerInterfaceAddr) {
    BuildGuidDataHob (&gEfiSchedulerInterfaceHobGuid, &SchedulerInterfaceAddr, sizeof (SchedulerInterfaceAddr));
  }

  // Build DTB Extension HOB
  if (DtbExtensionAddr) {
    BuildGuidDataHob (&gEfiDtbExtnHobGuid, &DtbExtensionAddr, sizeof (DtbExtensionAddr));
  }
}

EFI_STATUS
EFIAPI
PlatformPeim ()
{
  // Build FV HOB
  BuildFvHob (PcdGet64 (PcdFvBaseAddress), PcdGet32 (PcdFvSize));

  // Get XBL HOB Addresses
  EFI_PHYSICAL_ADDRESS FvDecompressAddr       = FixedPcdGet64 (PcdFvDecompressAddr);
  EFI_PHYSICAL_ADDRESS SchedulerInterfaceAddr = FixedPcdGet64 (PcdSchedulerInterfaceAddr);
  EFI_PHYSICAL_ADDRESS DtbExtensionAddr       = FixedPcdGet64 (PcdDtbExtensionAddr);

  // Build XBL HOBs
  BuildXblHobs (FvDecompressAddr, SchedulerInterfaceAddr, DtbExtensionAddr);

  return EFI_SUCCESS;
}
