/**
  Copyright (C) Microsoft Corporation. All rights reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PLATFORM_THEME_LIB_H_
#define _PLATFORM_THEME_LIB_H_

#include <Resources/FontPackage_Selawik_Regular_8pt_Fixed.h>
#include <Resources/FontPackage_Selawik_Regular_9pt.h>
#include <Resources/FontPackage_Selawik_Regular_10pt.h>
#include <Resources/FontPackage_Selawik_Regular_13pt.h>
#include <Resources/FontPackage_Selawik_Regular_14pt.h>
#include <Resources/FontPackage_Selawik_Regular_18pt.h>

#define FILLED_AT_RUNTIME  0

#define FONT_DECL(TABLE, NAME)                           \
  STATIC MS_UI_FONT_DESCRIPTION TABLE = {                \
    MS_UI_CUSTOM_FONT_ ## NAME ## _CELL_HEIGHT,          \
    MS_UI_CUSTOM_FONT_ ## NAME ## _CELL_WIDTH,           \
    MS_UI_CUSTOM_FONT_ ## NAME ## _MAX_ADVANCE,          \
    sizeof (mMsUiFontPackageHdr_ ## NAME),               \
    sizeof (mMsUiFontPackageGlyphs_ ## NAME),            \
    FILLED_AT_RUNTIME,                                   \
    FILLED_AT_RUNTIME                                    \
  };

// Scale is a percentage of 3000x2000 (long story)
#define SCALE  39

// The fonts for this platform are:
FONT_DECL (FixedFont,    Selawik_Regular_8pt_Fixed)
FONT_DECL (SmallOSKFont, Selawik_Regular_9pt)
FONT_DECL (SmallFont,    Selawik_Regular_10pt)
FONT_DECL (StandardFont, Selawik_Regular_13pt)
FONT_DECL (MediumFont,   Selawik_Regular_14pt)
FONT_DECL (LargeFont,    Selawik_Regular_18pt)

STATIC MS_UI_THEME_DESCRIPTION gMsUiPlatformTheme = {
  MS_UI_THEME_PROTOCOL_SIGNATURE,
  MS_UI_THEME_PROTOCOL_VERSION,
  SCALE,
  0,
  FILLED_AT_RUNTIME,
  FILLED_AT_RUNTIME,
  FILLED_AT_RUNTIME,
  FILLED_AT_RUNTIME,
  FILLED_AT_RUNTIME,
  FILLED_AT_RUNTIME
};

#endif /* _PLATFORM_THEME_LIB_H_ */
