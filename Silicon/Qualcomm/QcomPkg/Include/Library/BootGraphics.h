/** @file
This BootGraphicsLib  is only intended to be used by BDS to configure
the console mode and set an image on the screen.

Copyright (C) Microsoft Corporation. All rights reserved.
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _BOOT_GRAPHICS_H_
#define _BOOT_GRAPHICS_H_

typedef UINT8 BOOT_GRAPHIC;
#define BG_NO_BOOT_OS            (4)

/**
  Display Main System Boot Graphic

**/
EFI_STATUS
EFIAPI
DisplayPostBootGraphic (
  BOOT_GRAPHIC  graphic,
  INTN DestX, INTN DestY
  );

#endif /* _BOOT_GRAPHICS_H_ */
