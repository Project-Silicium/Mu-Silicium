/** @file
A shared place for all colors used in this package

Copyright (C) Microsoft Corporation. All rights reserved.
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>

#include <Protocol/GraphicsOutput.h>
#include <Library/MsColorTableLib.h>

// Predefined colors
#define MS_GRAPHICS_WHITE_COLOR               {.Blue = 0xFF, .Green = 0xFF, .Red = 0xFF, .Reserved = 0xFF}
#define MS_GRAPHICS_BLACK_COLOR               {.Blue = 0x00, .Green = 0x00, .Red = 0x00, .Reserved = 0xFF}
#define MS_GRAPHICS_GREEN_COLOR               {.Blue = 0x00, .Green = 0xFF, .Red = 0x00, .Reserved = 0xFF}
#define MS_GRAPHICS_YELLOW_COLOR              {.Blue = 0x00, .Green = 0xFF, .Red = 0xFF, .Reserved = 0xFF}
#define MS_GRAPHICS_LIGHT_GRAY_1_COLOR        {.Blue = 0xF2, .Green = 0xF2, .Red = 0xF2, .Reserved = 0xFF}
#define MS_GRAPHICS_LIGHT_GRAY_2_COLOR        {.Blue = 0xE6, .Green = 0xE6, .Red = 0xE6, .Reserved = 0xFF}
#define MS_GRAPHICS_LIGHT_GRAY_3_COLOR        {.Blue = 0xCC, .Green = 0xCC, .Red = 0xCC, .Reserved = 0xFF}
#define MS_GRAPHICS_MED_GRAY_1_COLOR          {.Blue = 0x3A, .Green = 0x3A, .Red = 0x3A, .Reserved = 0xFF}
#define MS_GRAPHICS_MED_GRAY_2_COLOR          {.Blue = 0x99, .Green = 0x99, .Red = 0x99, .Reserved = 0xFF}
#define MS_GRAPHICS_MED_GRAY_3_COLOR          {.Blue = 0x7A, .Green = 0x7A, .Red = 0x7A, .Reserved = 0xFF}
#define MS_GRAPHICS_DARK_GRAY_1_COLOR         {.Blue = 0x73, .Green = 0x73, .Red = 0x73, .Reserved = 0xFF}
#define MS_GRAPHICS_DARK_GRAY_2_COLOR         {.Blue = 0x33, .Green = 0x33, .Red = 0x33, .Reserved = 0xFF}
#define MS_GRAPHICS_CYAN_COLOR                {.Blue = 0xD7, .Green = 0x78, .Red = 0x00, .Reserved = 0xFF}
#define MS_GRAPHICS_RED_COLOR                 {.Blue = 0x0D, .Green = 0x00, .Red = 0xAE, .Reserved = 0xFF}
#define MS_GRAPHICS_TEXT_RED_COLOR            {.Blue = 0x00, .Green = 0x33, .Red = 0xFF, .Reserved = 0xFF}
#define MS_GRAPHICS_LIGHT_RED_COLOR           {.Blue = 0x21, .Green = 0x11, .Red = 0xE8, .Reserved = 0xFF}
#define MS_GRAPHICS_MED_GREEN_COLOR           {.Blue = 0x50, .Green = 0x9D, .Red = 0x45, .Reserved = 0xFF}
#define MS_GRAPHICS_LIGHT_CYAN_COLOR          {.Blue = 0xF7, .Green = 0xE4, .Red = 0xCC, .Reserved = 0xFF}
#define MS_GRAPHICS_LIGHT_BLUE_COLOR          {.Blue = 0xE8, .Green = 0xA2, .Red = 0x00, .Reserved = 0xFF}
#define MS_GRAPHICS_BRIGHT_BLUE_COLOR         {.Blue = 0xEA, .Green = 0xD9, .Red = 0x99, .Reserved = 0xFF}
#define MS_GRAPHICS_MED_BLUE_COLOR            {.Blue = 0xE7, .Green = 0xC1, .Red = 0x91, .Reserved = 0xFF}
#define MS_GRAPHICS_SKY_BLUE_COLOR            {.Blue = 0xB2, .Green = 0x67, .Red = 0x20, .Reserved = 0xFF}
#define MS_GRAPHICS_NEAR_BLACK_COLOR          {.Blue = 0x1A, .Green = 0x1A, .Red = 0x1A, .Reserved = 0xFF}
#define MS_GRAPHICS_GRAY_KEY_FILL_COLOR       {.Blue = 0x4D, .Green = 0x4D, .Red = 0x4D, .Reserved = 0xFF}
#define MS_GRAPHICS_DARK_GRAY_KEY_FILL_COLOR  {.Blue = 0x33, .Green = 0x33, .Red = 0x33, .Reserved = 0xFF}

MS_COLOR_TABLE  gMsColorTable = {
  //// Color definitions per view
  // Label
  .LabelTextNormalColor     = MS_GRAPHICS_WHITE_COLOR,
  .LabelTextLargeColor      = MS_GRAPHICS_CYAN_COLOR,
  .LabelTextRedColor        = MS_GRAPHICS_TEXT_RED_COLOR,
  .LabelTextGrayoutColor    = MS_GRAPHICS_LIGHT_GRAY_3_COLOR,
  .LabelTextBackgroundColor = MS_GRAPHICS_BLACK_COLOR,

  // ListBox
  .ListBoxNormalColor                  = MS_GRAPHICS_LIGHT_GRAY_1_COLOR,
  .ListBoxHoverColor                   = MS_GRAPHICS_LIGHT_GRAY_2_COLOR,
  .ListBoxSelectColor                  = MS_GRAPHICS_MED_BLUE_COLOR,
  .ListBoxGrayoutColor                 = MS_GRAPHICS_LIGHT_GRAY_2_COLOR,
  .ListBoxHighlightBoundColor          = MS_GRAPHICS_BLACK_COLOR,
  .ListBoxSelectFGColor                = MS_GRAPHICS_BLACK_COLOR,
  .ListBoxNormalFGColor                = MS_GRAPHICS_WHITE_COLOR,
  .ListBoxGrayoutFGColor               = MS_GRAPHICS_MED_GRAY_3_COLOR,
  .ListBoxCheckBoxBackgroundColor      = MS_GRAPHICS_WHITE_COLOR,
  .ListBoxCheckBoxBoundGrayoutColor    = MS_GRAPHICS_MED_GRAY_3_COLOR,
  .ListBoxCheckBoxSelectBGGrayoutColor = MS_GRAPHICS_MED_GRAY_3_COLOR,
  .ListBoxCheckBoxNormalBGGrayoutColor = MS_GRAPHICS_BLACK_COLOR,
  .ListBoxTranshanGrayoutColor         = MS_GRAPHICS_MED_GRAY_3_COLOR,
  .ListBoxTranshanSelectColor          = MS_GRAPHICS_DARK_GRAY_1_COLOR,
  .ListBoxTranshanNormalColor          = MS_GRAPHICS_DARK_GRAY_1_COLOR,

  // EditBox
  .EditBoxNormalColor         = MS_GRAPHICS_LIGHT_GRAY_1_COLOR,
  .EditBoxSelectColor         = MS_GRAPHICS_RED_COLOR,
  .EditBoxGrayoutColor        = MS_GRAPHICS_LIGHT_GRAY_1_COLOR,
  .EditBoxTextColor           = MS_GRAPHICS_BLACK_COLOR,
  .EditBoxTextGrayoutColor    = MS_GRAPHICS_BLACK_COLOR,
  .EditBoxHighlightBGColor    = MS_GRAPHICS_WHITE_COLOR,
  .EditBoxHighlightBoundColor = MS_GRAPHICS_BLACK_COLOR,
  .EditBoxWaterMarkFGColor    = MS_GRAPHICS_LIGHT_GRAY_3_COLOR,

  // Button
  .ButtonNormalColor         = MS_GRAPHICS_WHITE_COLOR,
  .ButtonHoverColor          = MS_GRAPHICS_WHITE_COLOR,
  .ButtonSelectColor         = MS_GRAPHICS_WHITE_COLOR,
  .ButtonGrayoutColor        = MS_GRAPHICS_MED_GRAY_2_COLOR,
  .ButtonRingColor           = MS_GRAPHICS_WHITE_COLOR,
  .ButtonTextNormalColor     = MS_GRAPHICS_CYAN_COLOR,
  .ButtonTextSelectColor     = MS_GRAPHICS_MED_GRAY_2_COLOR,
  .ButtonHighlightBoundColor = MS_GRAPHICS_BLACK_COLOR,

  .ButtonLinkNormalColor     = MS_GRAPHICS_LIGHT_GRAY_3_COLOR,
  .ButtonLinkHoverColor      = MS_GRAPHICS_LIGHT_GRAY_3_COLOR,
  .ButtonLinkSelectColor     = MS_GRAPHICS_MED_GRAY_2_COLOR,
  .ButtonLinkGrayoutColor    = MS_GRAPHICS_MED_GRAY_2_COLOR,
  .ButtonLinkRingColor       = MS_GRAPHICS_LIGHT_GRAY_3_COLOR,
  .ButtonLinkTextNormalColor = MS_GRAPHICS_BLACK_COLOR,
  .ButtonLinkTextSelectColor = MS_GRAPHICS_WHITE_COLOR,

  // ToggleSwitch
  .ToggleSwitchOnColor            = MS_GRAPHICS_CYAN_COLOR,
  .ToggleSwitchOffColor           = MS_GRAPHICS_DARK_GRAY_2_COLOR,
  .ToggleSwitchHoverColor         = MS_GRAPHICS_LIGHT_GRAY_2_COLOR,
  .ToggleSwitchGrayoutColor       = MS_GRAPHICS_LIGHT_GRAY_3_COLOR,
  .ToggleSwitchTextFGColor        = MS_GRAPHICS_BLACK_COLOR,
  .ToggleSwitchTextBGColor        = MS_GRAPHICS_WHITE_COLOR,
  .ToggleSwitchHighlightBGColor   = MS_GRAPHICS_BLACK_COLOR,
  .ToggleSwitchBackgroundColor    = MS_GRAPHICS_WHITE_COLOR,
  .ToggleSwitchCircleGrayoutColor = MS_GRAPHICS_MED_GRAY_1_COLOR,

  // Message Box
  .MessageBoxTextColor               = MS_GRAPHICS_WHITE_COLOR,
  .MessageBoxTitleBarTextColor       = MS_GRAPHICS_BLACK_COLOR,
  .MessageBoxBackgroundColor         = MS_GRAPHICS_CYAN_COLOR,
  .MessageBoxBackgroundAlert1Color   = MS_GRAPHICS_RED_COLOR,
  .MessageBoxBackgroundAlert2Color   = MS_GRAPHICS_CYAN_COLOR,
  .MessageBoxDialogFrameColor        = MS_GRAPHICS_LIGHT_GRAY_3_COLOR,
  .MessageBoxButtonHoverColor        = MS_GRAPHICS_LIGHT_BLUE_COLOR,
  .MessageBoxButtonSelectColor       = MS_GRAPHICS_BRIGHT_BLUE_COLOR,
  .MessageBoxButtonSelectAlert1Color = MS_GRAPHICS_LIGHT_RED_COLOR,
  .MessageBoxButtonGrayoutColor      = MS_GRAPHICS_MED_GRAY_1_COLOR,
  .MessageBoxButtonRingColor         = MS_GRAPHICS_WHITE_COLOR,
  .MessageBoxButtonTextColor         = MS_GRAPHICS_WHITE_COLOR,
  .MessageBoxButtonSelectTextColor   = MS_GRAPHICS_WHITE_COLOR,

  // Password
  .PasswordDialogTextColor               = MS_GRAPHICS_WHITE_COLOR,
  .PasswordDialogTitleBarTextColor       = MS_GRAPHICS_BLACK_COLOR,
  .PasswordDialogErrorTextColor          = MS_GRAPHICS_YELLOW_COLOR,
  .PasswordDialogEditBoxBackgroundColor  = MS_GRAPHICS_WHITE_COLOR,
  .PasswordDialogEditBoxTextColor        = MS_GRAPHICS_BLACK_COLOR,
  .PasswordDialogEditBoxGrayoutColor     = MS_GRAPHICS_LIGHT_GRAY_3_COLOR,
  .PasswordDialogEditBoxGrayoutTextColor = MS_GRAPHICS_MED_GRAY_3_COLOR,
  .PasswordDialogTextSelectColor         = MS_GRAPHICS_RED_COLOR,
  .PasswordDialogBackGroundColor         = MS_GRAPHICS_RED_COLOR,
  .PasswordDialogFrameColor              = MS_GRAPHICS_LIGHT_GRAY_3_COLOR,
  .PasswordDialogButtonHoverColor        = MS_GRAPHICS_LIGHT_RED_COLOR,
  .PasswordDialogButtonSelectColor       = MS_GRAPHICS_LIGHT_RED_COLOR,
  .PasswordDialogButtonGrayOutColor      = MS_GRAPHICS_MED_GRAY_1_COLOR,
  .PasswordDialogButtonRingColor         = MS_GRAPHICS_WHITE_COLOR,
  .PasswordDialogButtonTextColor         = MS_GRAPHICS_WHITE_COLOR,
  .PasswordDialogButtonSelectTextColor   = MS_GRAPHICS_WHITE_COLOR,

  // SemmUserAuth
  .UserAuthDialogTextColor             = MS_GRAPHICS_WHITE_COLOR,
  .UserAuthTitleBarTextColor           = MS_GRAPHICS_BLACK_COLOR,
  .UserAuthErrorTextColor              = MS_GRAPHICS_YELLOW_COLOR,
  .UserAuthEditBoxBackGroundColor      = MS_GRAPHICS_LIGHT_CYAN_COLOR,
  .UserAuthEditBoxTextColor            = MS_GRAPHICS_BLACK_COLOR,
  .UserAuthEditBoxGrayOutColor         = MS_GRAPHICS_LIGHT_GRAY_3_COLOR,
  .UserAuthEditBoxGrayOutTextColor     = MS_GRAPHICS_MED_GRAY_3_COLOR,
  .UserAuthDialogTextSelectColor       = MS_GRAPHICS_RED_COLOR,
  .UserAuthDialogBackGroundColor       = MS_GRAPHICS_RED_COLOR,
  .UserAuthDialogFrameColor            = MS_GRAPHICS_LIGHT_GRAY_3_COLOR,
  .UserAuthDialogButtonHoverColor      = MS_GRAPHICS_LIGHT_RED_COLOR,
  .UserAuthDialogButtonSelectColor     = MS_GRAPHICS_LIGHT_RED_COLOR,
  .UserAuthDialogButtonGrayOutColor    = MS_GRAPHICS_MED_GRAY_1_COLOR,
  .UserAuthDialogButtonRingColor       = MS_GRAPHICS_WHITE_COLOR,
  .UserAuthDialogButtonTextColor       = MS_GRAPHICS_WHITE_COLOR,
  .UserAuthDialogButtonSelectTextColor = MS_GRAPHICS_WHITE_COLOR,

  // Default
  .DefaultDialogTextColor             = MS_GRAPHICS_WHITE_COLOR,
  .DefaultTitleBarTextColor           = MS_GRAPHICS_BLACK_COLOR,
  .DefaultErrorTextColor              = MS_GRAPHICS_YELLOW_COLOR,
  .DefaultEditBoxBackGroundColor      = MS_GRAPHICS_LIGHT_CYAN_COLOR,
  .DefaultEditBoxTextColor            = MS_GRAPHICS_BLACK_COLOR,
  .DefaultEditBoxGrayOutColor         = MS_GRAPHICS_LIGHT_GRAY_3_COLOR,
  .DefaultEditBoxGrayOutTextColor     = MS_GRAPHICS_MED_GRAY_3_COLOR,
  .DefaultDialogTextSelectColor       = MS_GRAPHICS_CYAN_COLOR,
  .DefaultDialogBackGroundColor       = MS_GRAPHICS_CYAN_COLOR,
  .DefaultDialogFrameColor            = MS_GRAPHICS_LIGHT_GRAY_3_COLOR,
  .DefaultDialogButtonHoverColor      = MS_GRAPHICS_LIGHT_BLUE_COLOR,
  .DefaultDialogButtonSelectColor     = MS_GRAPHICS_BRIGHT_BLUE_COLOR,
  .DefaultDialogButtonGrayOutColor    = MS_GRAPHICS_MED_GRAY_1_COLOR,
  .DefaultDialogButtonRingColor       = MS_GRAPHICS_WHITE_COLOR,
  .DefaultDialogButtonTextColor       = MS_GRAPHICS_WHITE_COLOR,
  .DefaultDialogButtonSelectTextColor = MS_GRAPHICS_WHITE_COLOR,

  // Single Select
  .SingleSelectDialogTextColor             = MS_GRAPHICS_WHITE_COLOR,
  .SingleSelectDialogTitleBarTextColor     = MS_GRAPHICS_BLACK_COLOR,
  .SingleSelectDialogDialogBackGroundColor = MS_GRAPHICS_CYAN_COLOR,
  .SingleSelectDialogDialogFrameColor      = MS_GRAPHICS_LIGHT_GRAY_3_COLOR,
  .SingleSelectDialogButtonHoverColor      = MS_GRAPHICS_LIGHT_BLUE_COLOR,
  .SingleSelectDialogButtonSelectColor     = MS_GRAPHICS_MED_BLUE_COLOR,
  .SingleSelectDialogButtonGrayoutColor    = MS_GRAPHICS_MED_GRAY_1_COLOR,
  .SingleSelectDialogButtonRingColor       = MS_GRAPHICS_WHITE_COLOR,
  .SingleSelectDialogButtonTextColor       = MS_GRAPHICS_WHITE_COLOR,
  .SingleSelectDialogButtonSelectTextColor = MS_GRAPHICS_WHITE_COLOR,
  .SingleSelectDialogListBoxGreyoutColor   = MS_GRAPHICS_LIGHT_GRAY_2_COLOR,

  // Test Background Colors
  .TestBackground1Color = MS_GRAPHICS_BLACK_COLOR,
  .TestBackground2Color = MS_GRAPHICS_SKY_BLUE_COLOR,
  .TestBackground3Color = MS_GRAPHICS_RED_COLOR,

  // On Screen Keyboard
  .KeyLabelColor                     = MS_GRAPHICS_WHITE_COLOR,
  .KeyShiftnNavFillColor             = MS_GRAPHICS_GRAY_KEY_FILL_COLOR,
  .KeyDefaultFillColor               = MS_GRAPHICS_DARK_GRAY_KEY_FILL_COLOR,
  .KeyboardSizeChangeBackgroundColor = MS_GRAPHICS_NEAR_BLACK_COLOR,
  .KeyboardDocknCloseForegroundColor = MS_GRAPHICS_WHITE_COLOR,
  .KeyboardDocknCloseBackgroundColor = MS_GRAPHICS_NEAR_BLACK_COLOR,
  .KeyboardShiftStateKeyColor        = MS_GRAPHICS_WHITE_COLOR,
  .KeyboardShiftStateFGColor         = MS_GRAPHICS_CYAN_COLOR,
  .KeyboardShiftStateBGColor         = MS_GRAPHICS_WHITE_COLOR,
  .KeyboardCapsLockStateKeyColor     = MS_GRAPHICS_CYAN_COLOR,
  .KeyboardCapsLockStateFGColor      = MS_GRAPHICS_WHITE_COLOR,
  .KeyboardCapsLockStateBGColor      = MS_GRAPHICS_CYAN_COLOR,
  .KeyboardNumSymStateKeyColor       = MS_GRAPHICS_CYAN_COLOR,
  .KeyboardNumSymStateFGColor        = MS_GRAPHICS_GRAY_KEY_FILL_COLOR,
  .KeyboardNumSymA0StateFGColor      = MS_GRAPHICS_WHITE_COLOR,
  .KeyboardNumSymA0StateBGColor      = MS_GRAPHICS_CYAN_COLOR,
  .KeyboardFunctionStateKeyColor     = MS_GRAPHICS_CYAN_COLOR,
  .KeyboardFunctionStateFGColor      = MS_GRAPHICS_WHITE_COLOR,
  .KeyboardFunctionStateBGColor      = MS_GRAPHICS_CYAN_COLOR,
  .KeyboardSelectedStateKeyColor     = MS_GRAPHICS_WHITE_COLOR,
  .KeyboardSelectedStateFGColor      = MS_GRAPHICS_BLACK_COLOR,
  .KeyboardSelectedStateBGColor      = MS_GRAPHICS_WHITE_COLOR,

  // Display Engine
  .TitleBarBackgroundColor     = MS_GRAPHICS_DARK_GRAY_2_COLOR,
  .TitleBarTextColor           = MS_GRAPHICS_WHITE_COLOR,
  .MasterFrameBackgroundColor  = MS_GRAPHICS_DARK_GRAY_2_COLOR,
  .MasterFrameCellNormalColor  = MS_GRAPHICS_DARK_GRAY_2_COLOR,
  .MasterFrameCellHoverColor   = MS_GRAPHICS_DARK_GRAY_2_COLOR,
  .MasterFrameCellSelectColor  = MS_GRAPHICS_SKY_BLUE_COLOR,
  .MasterFrameCellGrayoutColor = MS_GRAPHICS_DARK_GRAY_2_COLOR,
  .FormCanvasBackgroundColor   = MS_GRAPHICS_BLACK_COLOR
};
