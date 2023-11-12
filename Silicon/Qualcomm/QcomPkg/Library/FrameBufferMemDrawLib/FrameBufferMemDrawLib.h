/** @file
Library used to display things on the Frame buffer by memory copying

Copyright (C) Microsoft Corporation. All rights reserved.
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef FRAMEBUFFER_MEM_DRAW_LIB_INTERNAL_H_
#define FRAMEBUFFER_MEM_DRAW_LIB_INTERNAL_H_

/**
  Get pertenant information about the frame buffer

  @param[out]  - Pointer where to store the framebuffer address.
  @param[out]  - Pointer where to store the mode info data.
**/
EFI_STATUS
GetGraphicsInfo (
  EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE  **Mode
  );

#endif
