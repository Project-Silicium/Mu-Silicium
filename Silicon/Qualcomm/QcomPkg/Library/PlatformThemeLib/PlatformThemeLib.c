/**
  Copyright (C) Microsoft Corporation. All rights reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Protocol/MsUiThemeProtocol.h>

#include <Library/PlatformThemeLib.h>

#include "PlatformTheme.h"

MS_UI_THEME_DESCRIPTION*
EFIAPI
PlatformThemeGet ()
{
  FixedFont.Package               = FONT_PTR_SET  &mMsUiFontPackageHdr_Selawik_Regular_8pt_Fixed;
  FixedFont.Glyphs                = GLYPH_PTR_SET &mMsUiFontPackageGlyphs_Selawik_Regular_8pt_Fixed;

  SmallOSKFont.Package            = FONT_PTR_SET  &mMsUiFontPackageHdr_Selawik_Regular_9pt;
  SmallOSKFont.Glyphs             = GLYPH_PTR_SET &mMsUiFontPackageGlyphs_Selawik_Regular_9pt;

  SmallFont.Package               = FONT_PTR_SET  &mMsUiFontPackageHdr_Selawik_Regular_10pt;
  SmallFont.Glyphs                = GLYPH_PTR_SET &mMsUiFontPackageGlyphs_Selawik_Regular_10pt;

  StandardFont.Package            = FONT_PTR_SET  &mMsUiFontPackageHdr_Selawik_Regular_13pt;
  StandardFont.Glyphs             = GLYPH_PTR_SET &mMsUiFontPackageGlyphs_Selawik_Regular_13pt;

  MediumFont.Package              = FONT_PTR_SET  &mMsUiFontPackageHdr_Selawik_Regular_14pt;
  MediumFont.Glyphs               = GLYPH_PTR_SET &mMsUiFontPackageGlyphs_Selawik_Regular_14pt;

  LargeFont.Package               = FONT_PTR_SET  &mMsUiFontPackageHdr_Selawik_Regular_18pt;
  LargeFont.Glyphs                = GLYPH_PTR_SET &mMsUiFontPackageGlyphs_Selawik_Regular_18pt;

  gMsUiPlatformTheme.FixedFont    = FONT_PTR_SET  &FixedFont;
  gMsUiPlatformTheme.SmallOSKFont = FONT_PTR_SET  &SmallOSKFont;
  gMsUiPlatformTheme.SmallFont    = FONT_PTR_SET  &SmallFont;
  gMsUiPlatformTheme.StandardFont = FONT_PTR_SET  &StandardFont;
  gMsUiPlatformTheme.MediumFont   = FONT_PTR_SET  &MediumFont;
  gMsUiPlatformTheme.LargeFont    = FONT_PTR_SET  &LargeFont;

  return &gMsUiPlatformTheme;
}
