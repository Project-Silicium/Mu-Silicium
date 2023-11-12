/** @file
Library used to display things on the Frame buffer by memory copying

Copyright (C) Microsoft Corporation. All rights reserved.
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiPei.h>

#include <Ppi/Graphics.h>

#include <Protocol/GraphicsOutput.h>

#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>

STATIC EFI_GRAPHICS_OUTPUT_MODE_INFORMATION  mInfo        = { 0 };
STATIC EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE     mMode        = { 0, 0, &mInfo, sizeof (mInfo), 0 };
STATIC BOOLEAN                               mInitialized = FALSE;

/***
 Get Graphics Information
*/
EFI_STATUS
GetGraphicsInfo (
  EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE  **Mode
  )
{
  EFI_PEI_GRAPHICS_PPI  *GfxInitPpi;
  EFI_STATUS            Status;

  Status = EFI_SUCCESS;
  if (!mInitialized) {
    Status = PeiServicesLocatePpi (&gEfiPeiGraphicsPpiGuid, 0, NULL, (VOID *)&GfxInitPpi);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a: Failed to locate Pei Graphics Ppi. Code=%r\n", __FUNCTION__, Status));
    } else {
      Status = GfxInitPpi->GraphicsPpiGetMode (&mMode);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "%a: GraphicsPpiGetMode failed %r.\n", __FUNCTION__, Status));
      } else {
        mInitialized = TRUE;
      }
    }
  }

  if (!EFI_ERROR (Status)) {
    *Mode = &mMode;
  }

  return Status;
}
