/**
  Copyright@ Samsung Electronics Co. LTD

  This software is proprietary of Samsung Electronics.
  No part of this software, either material or conceptual may be copied or distributed, transmitted,
  transcribed, stored in a retrieval system or translated into any human or computer language in any form by any means,
  electronic, mechanical, manual or otherwise, or disclosed
  to third parties without the express written permission of Samsung Electronics.
**/

#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryMapHelperLib.h>
#include <Library/IoLib.h>

#include <Protocol/EFIChipInfo.h>

#include "ChipInfo.h"

//
// Global Variables
//
STATIC EFI_CHIP_INFO *ChipInfo = NULL;

EFI_STATUS
GetChipId (OUT UINT64 *Id)
{
  // Verify Parameter
  if (Id == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  // Get Chip IDs
  UINT32 Id1 = MmioRead32 ((UINTN)&ChipInfo->Id[0]);
  UINT32 Id2 = MmioRead32 ((UINTN)&ChipInfo->Id[1]);

  // Pass Merged ID
  *Id = ((UINT64)Id2 << 32) | Id1;

  return EFI_SUCCESS;
}

EFI_STATUS
GetChipRevision (
  OUT UINT8 *MajorRev,
  OUT UINT8 *MinorRev)
{
  // Verify Parameters
  if (MajorRev == NULL || MinorRev == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  // Get Chip Revision
  UINT32 Revision = MmioRead32 ((UINTN)&ChipInfo->Revision);

  // Pass Main & Sub Revision
  *MajorRev = (Revision >> MAJOR_REVISION_SHIFT) & REVISION_MASK;
  *MinorRev = (Revision >> MINOR_REVISION_SHIFT) & REVISION_MASK;

  return EFI_SUCCESS;
}

STATIC EFI_CHIP_INFO_PROTOCOL mChipinfo = {
  GetChipId,
  GetChipRevision
};

EFI_STATUS
EFIAPI
RegisterChipInfo (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS                   Status;
  EFI_MEMORY_REGION_DESCRIPTOR ChipInfoRegion;

  // Locate "Chip Info" Memory Region
  Status = LocateMemoryRegionByName ("Chip Info", &ChipInfoRegion);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate \"Chip Info\" Memory Region!\n"));
    return Status;
  }

  // Populate Chip Info Structure
  ChipInfo = (EFI_CHIP_INFO *)ChipInfoRegion.Address;

  // Register Chip Info Protocol
  Status = gBS->InstallProtocolInterface (&ImageHandle, &gEfiChipInfoProtocolGuid, EFI_NATIVE_INTERFACE, &mChipinfo);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Register Chip Info Protocol!\n"));
    return Status;
  }

  return EFI_SUCCESS;
}
