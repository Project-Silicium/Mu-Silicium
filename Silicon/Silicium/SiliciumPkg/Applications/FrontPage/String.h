/** @file String.h

  Copyright (c) 2004 - 2009, Intel Corporation. All rights reserved.
  Copyright (C) Microsoft Corporation. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent

  String support

**/

#ifndef _STRING_H_
#define _STRING_H_

extern EFI_HII_HANDLE  gStringPackHandle;

//
// This is the VFR compiler generated header file which defines the
// string identifiers.
//

extern UINT8  FrontPageStrings[];

/**
  Get string by string id from HII Interface


  @param Id              String ID.

  @retval  CHAR16 *  String from ID.
  @retval  NULL      If error occurs.

**/
CHAR16 *
GetStringById (
  IN  EFI_STRING_ID  Id
  );

/**
  Initialize HII global accessor for string support.

**/
VOID
InitializeStringSupport (
  VOID
  );

/**
  Call the browser and display the front page

  @return   Status code that will be returned by
            EFI_FORM_BROWSER2_PROTOCOL.SendForm ().

**/
EFI_STATUS
CallFrontPage (
  IN UINT32  FormIndex
  );

#endif // _STRING_H_
