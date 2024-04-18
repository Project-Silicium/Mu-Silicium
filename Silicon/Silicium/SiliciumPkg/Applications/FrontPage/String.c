/** @file String.c

  Copyright (c) 2004 - 2010, Intel Corporation. All rights reserved.<BR>
  Copyright (C) Microsoft Corporation. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent

  String support

**/

#include "FrontPage.h"

#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/HiiLib.h>

EFI_HII_HANDLE  gStringPackHandle;

EFI_GUID  mFrontPageStringPackGuid = {
  // {9CA9EC7A-BC96-45E4-A500-1D4B79141553}
  0x9ca9ec7a, 0xbc96, 0x45e4, { 0xa5, 0x0, 0x1d, 0x4b, 0x79, 0x14, 0x15, 0x53 }
};

/**
  Initialize HII global accessor for string support.

**/
VOID
InitializeStringSupport (
  VOID
  )
{
  gStringPackHandle = HiiAddPackages (
                        &mFrontPageStringPackGuid,
                        gImageHandle,
                        FrontPageStrings,
                        NULL
                        );
}

/**
  Get string by string id from HII Interface


  @param Id              String ID.

  @retval  CHAR16 *  String from ID.
  @retval  NULL      If error occurs.

**/
CHAR16 *
GetStringById (
  IN  EFI_STRING_ID  Id
  )
{
  return HiiGetString (gStringPackHandle, Id, NULL);
}
