/**
  Copyright (C) Microsoft Corporation. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/BootGraphics.h>
#include <Library/BootGraphicsProviderLib.h>
#include <Library/DxeServicesLib.h>

#include "BootGraphicsProvider.h"

EFI_STATUS
EFIAPI
GetBootGraphic (
  IN  BOOT_GRAPHIC   Graphic,
  OUT UINTN         *ImageSize,
  OUT UINT8        **ImageData)
{
  // Go thru each Graphic
  for (UINT8 i = 0; i < ARRAY_SIZE (BootGraphics); i++) {
    // Compare Graphic ID
    if (Graphic != BootGraphics[i].Id) {
      continue;
    }

    // Return Graphic File Data
    return GetSectionFromAnyFv (BootGraphics[i].FileGuid, EFI_SECTION_RAW, 0, (VOID **)ImageData, ImageSize);
  }

  return EFI_NOT_FOUND;
}

UINT32
EFIAPI
GetBackgroundColor ()
{
  return 0;
}
