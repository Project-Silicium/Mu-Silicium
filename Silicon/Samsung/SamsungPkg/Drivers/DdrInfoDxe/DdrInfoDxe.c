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
#include <Library/PcdLib.h>
#include <Library/IoLib.h>

#include <Protocol/EfiDdrInfo.h>

//
// Global Variables
//
STATIC ARM_MEMORY_REGION_DESCRIPTOR_EX DdrInfoRegion;
STATIC UINT32                          InfoOffset;
STATIC UINT32                          SizeInfoOffset;

DDR_DETAILS_TABLE
GetDetails ()
{
  DDR_DETAILS_TABLE Details;
  UINT32            Value;

  // Read DDR Info
  Value = MmioRead32 (DdrInfoRegion.Address + InfoOffset);

  // Populate Details Structures
  Details.ManufacturerId = (Value >> 8) & 0xFF;
  Details.MemoryType     = Value & 0xF;
  Details.RankNumber     = (Value >> 4) & 0xFF;

  return Details;
}

UINT64
GetRamSize ()
{
  UINT64 RamSize  = 0;
  UINT32 Value[2] = {0, 0};

  // Get DDR Size Info
  Value[0] = MmioRead32 (DdrInfoRegion.Address + SizeInfoOffset);
  Value[1] = MmioRead32 (DdrInfoRegion.Address + SizeInfoOffset + 0x4);

  // Get RAM Size
  RamSize |= (UINT64)(Value[0]);
  RamSize |= (UINT64)(Value[1]) << 32;

  return RamSize;
}

EFI_DDR_INFO_PROTOCOL mDdrInfo = {
  GetDetails,
  GetRamSize
};

EFI_STATUS
EFIAPI
InitDdrInfoDriver (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status;

  // Locate DDR Info Memory Region
  Status = LocateMemoryMapAreaByName ("DDR Info", &DdrInfoRegion);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate 'DDR Info' Memory Region! Status = %r\n", Status));
    return Status;
  }

  // Set Offsets
  InfoOffset     = FixedPcdGet32 (PcdDdrInfoOffset);
  SizeInfoOffset = FixedPcdGet32 (PcdDdrSizeInfoOffset);

  // Register DDR Info Protocol
  Status = gBS->InstallMultipleProtocolInterfaces (&ImageHandle, &gEfiDdrInfoProtocolGuid, &mDdrInfo, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Register DDR Info Protocol!\n"));
    ASSERT_EFI_ERROR (Status);
  }

  return EFI_SUCCESS;
}
