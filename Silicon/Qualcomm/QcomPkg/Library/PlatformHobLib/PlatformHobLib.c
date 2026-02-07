/**
  Copyright (c) 2011-2014, ARM Limited. All rights reserved.
  Copyright (c) 2014, Linaro Limited. All rights reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <PiPei.h>

#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/PlatformHobLib.h>
#include <Library/ConfigurationMapLib.h>
#include <Library/ConfigurationMapHelperLib.h>
#include <Library/MemoryMapHelperLib.h>
#include <Library/SerialPortLib.h>

#include <Protocol/EFIKernelInterface.h>

#include "PlatformHobs.h"

STATIC
EFI_STATUS
CfgGetMemInfoByName (
  IN  CHAR8                           *RegionName,
  OUT EFI_MEMORY_REGION_DESCRIPTOR_EX *MemRegions)
{
  // Get Memory Region by Name
  return LocateMemoryMapAreaByName (RegionName, MemRegions);
}

STATIC
EFI_STATUS
CfgGetMemInfoByAddress (
  IN  UINT64                           RegionBaseAddress,
  OUT EFI_MEMORY_REGION_DESCRIPTOR_EX *MemRegions)
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
  EFI_MEMORY_REGION_DESCRIPTOR_EX InfoBlkRegion;

  // Get XBL HOB Addresses
  UINT64 SchedAddress        = FixedPcdGet64 (PcdScheduleInterfaceAddr);
  UINT64 DtbExtAddress       = FixedPcdGet64 (PcdDtbExtensionAddr);
  UINT64 FvDecompressAddress = FixedPcdGet64 (PcdFvDecompressAddr);

  // Locate "Info Blk" Memory Region
  Status  = LocateMemoryMapAreaByName ("Info Blk", &InfoBlkRegion);
  Status |= LocateMemoryMapAreaByName ("Info_Blk", &InfoBlkRegion);
  if (EFI_ERROR (Status) && !InfoBlkRegion.Address) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Locate 'Info Blk' Memory Region! Status = %r\n", __FUNCTION__, Status));
  } else if (FixedPcdGetBool (PcdEnableInfoBlkHob)) {
    // Set Info BLK Address
    UINTN InfoBlkAddress = InfoBlkRegion.Address;

    // Build Info Blk HOB
    BuildGuidDataHob (&gEfiInfoBlkHobGuid, &InfoBlkAddress, sizeof (InfoBlkAddress));
  }

  // Check PCD Value
  if (FixedPcdGetBool (PcdEnableShimHob)) {
    // Set Sh Lib Address
    UINTN ShLibAddress = (UINTN)&ShLib;

    // Build Shim HOB
    BuildGuidDataHob (&gEfiShimLibraryHobGuid, &ShLibAddress, sizeof (ShLibAddress));
  }

  // Check PCD Value
  if (FixedPcdGetBool (PcdEnableProdmodeHob)) {
    // Set Prodmode Value
    BOOLEAN Prodmode = FALSE;

    // Build Prodmode HOB
    BuildGuidDataHob (&gEfiProdmodeHobGuid, &Prodmode, sizeof (Prodmode));
  }

  // Check FV Decompress Address
  if (FvDecompressAddress) {
    // Build FV Decompress HOB
    BuildGuidDataHob (&gFvDecompressHobGuid, &FvDecompressAddress, sizeof (FvDecompressAddress));
  }

  // Check Schedule Interface Address
  if (SchedAddress) {
    // Populate Schedule Interface Protocol
    EFI_KERNEL_PROTOCOL *SchedIntfProtocol = (VOID *)SchedAddress;

    // Build Schedule Interface HOB
    BuildGuidDataHob (&gEfiScheduleInterfaceHobGuid, &SchedIntfProtocol, sizeof (SchedIntfProtocol));
  }

  // Check DTB Extension Address
  if (DtbExtAddress) {
    // Populate DTB Extension Protocol
    EFI_DTB_EXTN_PROTOCOL *DTBExtnProtocol = (VOID *)DtbExtAddress;

    // Build DTB Extension HOB
    BuildGuidDataHob (&gEfiDTBExtnHobGuid, &DTBExtnProtocol, sizeof (DTBExtnProtocol));
  }
}
