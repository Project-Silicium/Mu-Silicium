/**
  Copyright (c) 2006 - 2018, Intel Corporation. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _EFI_COVER_SCREEN_H_
#define _EFI_COVER_SCREEN_H_

#include <Protocol/GraphicsOutput.h>

//
// Global GUID for the Cover Screen Protocol
//
#define EFI_COVER_SCREEN_PROTOCOL_GUID { 0x4C7366E4, 0x089F, 0x4A8F, { 0xA5, 0xCD, 0x39, 0xF0, 0xA7, 0x5E, 0x39, 0xA0 } }

//
// Declare forward reference to the Cover Screen Protocol
//
typedef struct _EFI_COVER_SCREEN_PROTOCOL EFI_COVER_SCREEN_PROTOCOL;

/**
  This Function Draws the defined Pixels on the Cover Screen.

  ...

  @return EFI_SUCCESS                      - Successfully Drew the Pixels on the Cover Screen.
  ...
**/
typedef
EFI_STATUS
(EFIAPI *EFI_COVER_SCREEN_BLT) (
  IN EFI_GRAPHICS_OUTPUT_BLT_PIXEL     *BltBuffer     OPTIONAL,
  IN EFI_GRAPHICS_OUTPUT_BLT_OPERATION  BltOperation,
  IN UINTN                              SourceX,
  IN UINTN                              SourceY,
  IN UINTN                              DestinationX,
  IN UINTN                              DestinationY,
  IN UINTN                              Width,
  IN UINTN                              Height,
  IN UINTN                              Delta         OPTIONAL
  );

/**
  This Function returns the Resolution of the Cover Screen.

  @param[out] ScreenWidth                  - The Width of the Cover Screen.
  @param[out] ScreenHeight                 - The Height of the Cover Screen.
**/
typedef
VOID
(EFIAPI *EFI_COVER_SCREEN_GET_RESOLUTION) (
  OUT UINT32 *ScreenWidth,
  OUT UINT32 *ScreenHeight
  );

//
// Define Protocol Functions
//
struct _EFI_COVER_SCREEN_PROTOCOL {
  EFI_COVER_SCREEN_BLT            Blt;
  EFI_COVER_SCREEN_GET_RESOLUTION GetResolution;
};

extern EFI_GUID gEfiCoverScreenProtocolGuid;

#endif /* _EFI_COVER_SCREEN_H_ */
