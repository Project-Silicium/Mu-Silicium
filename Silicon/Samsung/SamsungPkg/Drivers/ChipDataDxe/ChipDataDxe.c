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

#include <Protocol/EfiChipData.h>

#include "ChipData.h"

//
// Global Variables
//
STATIC EFI_MEMORY_REGION_DESCRIPTOR_EX ChipInfoRegion;

VOID
GetId (OUT UINT64 *Id)
{
  UINT32 ChipId[4];

  // Read Chip ID
  ChipId[0] = MmioRead32 (ChipInfoRegion.Address + CHIPID0_OFFSET);
  ChipId[1] = MmioRead32 (ChipInfoRegion.Address + CHIPID1_OFFSET) & 0xFFFF;

  // Combine Chip ID
  *Id = ((UINT64)ChipId[1] << 32) | ChipId[0];
}

VOID
GetRevision (OUT UINT32 Revision[2])
{
  UINT32 Value;

  // Read Chip Revision
  Value = MmioRead32 (ChipInfoRegion.Address + CHIPID_REV_OFFSET);

  //Pass Revision
  Revision[0] = (Value >> 20) & 0xF;
  Revision[1] = (Value >> 16) & 0xF;
}

EFI_CHIP_DATA_PROTOCOL mChipData = {
  GetId,
  GetRevision
};

EFI_STATUS
EFIAPI
InitChipDataDriver (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status;

  // Locate Chip Info Memory Region
  Status = LocateMemoryMapAreaByName ("Chip Info", &ChipInfoRegion);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate 'Chip Info' Memory Region! Status = %r\n", Status));
    return Status;
  }

  // Register Chip Data Protocol
  Status = gBS->InstallMultipleProtocolInterfaces (&ImageHandle, &gEfiChipDataProtocolGuid, &mChipData, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Register Chip Data Protocol!\n"));
    ASSERT_EFI_ERROR (Status);
  }

  return EFI_SUCCESS;
}
