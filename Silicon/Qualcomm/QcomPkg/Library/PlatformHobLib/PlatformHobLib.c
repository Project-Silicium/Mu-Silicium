/**
  Copyright (c) 2011-2014, ARM Limited. All rights reserved.
  Copyright (c) 2014, Linaro Limited. All rights reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <PiPei.h>

#include <Library/PcdLib.h>
#include <Library/PlatformHobLib.h>
#include <Library/DeviceConfigurationMapLib.h>
#include <Library/ConfigurationMapHelperLib.h>
#include <Library/MemoryMapHelperLib.h>
#include <Library/SerialPortLib.h>

#include <Protocol/EFIKernelInterface.h>

#include "PlatformHobs.h"

STATIC
EFI_STATUS
CfgGetMemInfoByName (
  IN  CHAR8                           *RegionName,
  OUT ARM_MEMORY_REGION_DESCRIPTOR_EX *MemRegions)
{
  // Get Memory Region by Name
  return LocateMemoryMapAreaByName (RegionName, MemRegions);
}

STATIC
EFI_STATUS
CfgGetMemInfoByAddress (
  IN  UINT64                           RegionBaseAddress,
  OUT ARM_MEMORY_REGION_DESCRIPTOR_EX *MemRegions)
{
  // Get Memory Region by Base Address
  return LocateMemoryMapAreaByAddress (RegionBaseAddress, MemRegions);
}

STATIC
EFI_STATUS
CfgGetCfgInfoString (
  IN  CHAR8 *Key,
  OUT CHAR8 *Value,
  IN  UINTN *ValBuffSize)
{
  // Compare Names
  if (AsciiStriCmp (Key, "OsTypeString") == 0) {
    // Convert Name to String
    AsciiStrCpyS (Value, *ValBuffSize, FixedPcdGetPtr (PcdPlatformType));
    return EFI_SUCCESS;
  }

  return EFI_NOT_FOUND;
}

STATIC
EFI_STATUS
CfgGetCfgInfoVal (
  IN  CHAR8  *Key,
  OUT UINT32 *Value)
{
  // Get Configuration Value by Name (32)
  return LocateConfigurationMapUINT32ByName (Key, Value);
}

STATIC
EFI_STATUS
CfgGetCfgInfoVal64 (
  IN  CHAR8  *Key,
  OUT UINT64 *Value)
{
  // Get Configuration Value by Name (64)
  return LocateConfigurationMapUINT64ByName (Key, Value);
}

STATIC
UINTN
SFlush ()
{
  return EFI_SUCCESS;
}

STATIC
UINTN
SControl (
  IN UINTN Arg,
  IN UINTN Param)
{
  return EFI_SUCCESS;
}

STATIC
BOOLEAN
SPoll ()
{
  return TRUE;
}

STATIC
UINTN
SDrain ()
{
  return EFI_SUCCESS;
}

//
// Config Lib Structure
//
UefiCfgLibType ConfigLib = {
  0x10002,
  CfgGetMemInfoByName,
  CfgGetCfgInfoString,
  CfgGetCfgInfoVal,
  CfgGetCfgInfoVal64, 
  CfgGetMemInfoByAddress
};

//
// SerialPort Lib Structure
//
SioPortLibType SioLib = {
  0x10001,
  SerialPortRead,
  SerialPortWrite,
  SPoll,
  SDrain,
  SFlush,
  SControl,
  SerialPortSetAttributes
};

STATIC
EFI_STATUS
ShInstallLib (
  IN CHAR8  *LibName,
  IN UINT32 LibVersion,
  IN VOID   *LibIntf)
{
  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
ShLoadLib (
  IN  CHAR8   *LibName,
  IN  UINT32   LibVersion,
  OUT VOID   **LibIntf)
{
  // Check Parameter
  if (LibIntf == NULL) {
    return EFI_NOT_FOUND;
  }

  // Compare Library Name
  if (AsciiStriCmp (LibName, "UEFI Config Lib") == 0) {
    *LibIntf = &ConfigLib;
    return EFI_SUCCESS;
  }

  // Compare Library Name
  if (AsciiStriCmp (LibName, "SerialPort Lib") == 0) {
    *LibIntf = &SioLib;
    return EFI_SUCCESS;
  }

  return EFI_NOT_FOUND;
}

//
// Sh Library Type Structure
//
ShLibLoaderType ShLib = {
  0x00010001,
  ShInstallLib,
  ShLoadLib
};

VOID
BuildPlatformHobs ()
{
  EFI_STATUS                      Status;
  ARM_MEMORY_REGION_DESCRIPTOR_EX InfoBlkRegion;
  ARM_MEMORY_REGION_DESCRIPTOR_EX UefiFdRegion;
  UINT64                          SchedAddress;
  UINT64                          DtbExtAddress;

  // Get XBL HOB Addresses
  SchedAddress  = FixedPcdGet64 (PcdScheduleInterfaceAddr);
  DtbExtAddress = FixedPcdGet64 (PcdDTBExtensionAddr);

  // Build Info Blk HOB
  Status = LocateMemoryMapAreaByName ("Info Blk", &InfoBlkRegion);
  if (!EFI_ERROR (Status) && FixedPcdGetBool (PcdEnableInfoBlkHob)) {
    UINTN InfoBlkAddress = InfoBlkRegion.Address;

    BuildGuidDataHob (&gEfiInfoBlkHobGuid, &InfoBlkAddress, sizeof (InfoBlkAddress));
  }

  // Build Shim HOB
  if (FixedPcdGetBool (PcdEnableShimHob)) {
    UINTN ShLibAddress = (UINTN)&ShLib;

    BuildGuidDataHob (&gEfiShimLibraryHobGuid, &ShLibAddress, sizeof (ShLibAddress));
  }

  // Build FV Decompress HOB
  Status = LocateMemoryMapAreaByName ("UEFI FD", &UefiFdRegion);
  if (!EFI_ERROR (Status) && FixedPcdGetBool (PcdEnableFvDecompressHob)) {
    UINTN FvDecompressAddress = UefiFdRegion.Address + 0x403D0;

    BuildGuidDataHob (&gFvDecompressHobGuid, &FvDecompressAddress, sizeof (FvDecompressAddress));
  }

  // Build Prodmode HOB
  if (FixedPcdGetBool (PcdEnableProdmodeHob)) {
    BOOLEAN Prodmode = FALSE;

    BuildGuidDataHob (&gEfiProdmodeHobGuid, &Prodmode, sizeof (Prodmode));
  }

  // Build Schedule Interface HOB
  if (SchedAddress) {
    EFI_KERNEL_PROTOCOL *SchedIntfProtocol = (VOID *)SchedAddress;

    BuildGuidDataHob (&gEfiScheduleInterfaceHobGuid, &SchedIntfProtocol, sizeof (SchedIntfProtocol));
  }

  // Build DTB Extension HOB
  if (DtbExtAddress) {
    EFI_DTB_EXTN_PROTOCOL *DTBExtnProtocol = (VOID *)DtbExtAddress;

    BuildGuidDataHob (&gEfiDTBExtnHobGuid, &DTBExtnProtocol, sizeof (DTBExtnProtocol));
  }
}
