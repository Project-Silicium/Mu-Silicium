/** @file
Library used to display things on the Frame buffer by memory copying

Copyright (C) Microsoft Corporation. All rights reserved.
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Base.h>
#include <Uefi.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/GraphicsOutput.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/FrameBufferMemDrawLib.h>
#include <Library/FrameBufferBltLib.h>

STATIC EFI_GRAPHICS_OUTPUT_PROTOCOL  *mGraphicsOutput = NULL;

/***
 Get Graphics Information
*/
EFI_STATUS
GetGraphicsInfo (
  EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE  **Mode
  )
{
  EFI_STATUS  Status;

  Status = EFI_SUCCESS;

  //
  // Try to open GOP first
  //
  if (mGraphicsOutput == NULL) {
    Status = gBS->HandleProtocol (gST->ConsoleOutHandle, &gEfiGraphicsOutputProtocolGuid, (VOID **)&mGraphicsOutput);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_WARN, "%a - Failed to find GOP on ConsoleOutHandle. %r\n", __FUNCTION__, Status));
      // failed on console out.  Try globally within system
      Status = gBS->LocateProtocol (&gEfiGraphicsOutputProtocolGuid, NULL, (VOID **)&mGraphicsOutput);
    }

    // if we don't find it
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a - Failed to find GOP globally. %r\n", __FUNCTION__, Status));
      return Status;
    }
  }

  *Mode = mGraphicsOutput->Mode;
  return Status;
}
