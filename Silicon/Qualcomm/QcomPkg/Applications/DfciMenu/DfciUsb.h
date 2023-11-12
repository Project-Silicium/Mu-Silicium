/** @file
DfciUsb.h

DfciUsb loads Dfci Configuration from a USB drive

Copyright (C) Microsoft Corporation. All rights reserved.
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef __DFCI_USB_H__
#define __DFCI_USB_H__

// MAX_USB_FILE_NAME_LENGTH Includes the terminating NULL
#define MAX_USB_FILE_NAME_LENGTH  256

/**
*
*  Request a Json Dfci settings packet.
*
*  @param[in]     FileName        What file to read.
*  @param[out]    JsonString      Where to store the Json String
*  @param[out]    JsonStringSize  Size of Json String
*
*  @retval   Status               Always returns success.
*
**/
EFI_STATUS
EFIAPI
DfciRequestJsonFromUSB (
  IN  CHAR16  *FileName,
  OUT CHAR8   **JsonString,
  OUT UINTN   *JsonStringSize
  );

#endif // __DFCI_USB_H__
