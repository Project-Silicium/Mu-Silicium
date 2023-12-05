/** @file
DfciUtility.h

DfciUtility contains useful functions

Copyright (C) Microsoft Corporation. All rights reserved.
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef __DFCI_UTILITY_H__
#define __DFCI_UTILITY_H__

#include <Uefi.h>

#include <DfciSystemSettingTypes.h>

#include <Library/HiiLib.h>

#define DFCI_MAX_STRING_LEN  (1024)

typedef struct {
  CHAR8    *SerialNumber;
  UINTN    SerialNumberSize;
  CHAR8    *Manufacturer;
  UINTN    ManufacturerSize;
  CHAR8    *ProductName;
  UINTN    ProductNameSize;
} DFCI_SYSTEM_INFORMATION;

/**
 * ConvertToCHAR16 - Converts a CHAR8 string to a CHAR16 string.
 *
 * @param[in]   Text           Input CHAR8 String
 * @param[in]   Text8Len       Number of characters in String
 * @param[out]  Text16         Where to store pointer to new CHAR8 string
 * @param[out]  Text16Size     CHAR16 string size (includes NULL)
 *
 * @return     String as CHAR16.  Caller is responsible for freeing returned
 *             string
 */
EFI_STATUS
DfciConvertToCHAR16 (
  IN CHAR8    *Text8,
  IN UINTN    Text8Len,
  OUT CHAR16  **Text16,
  OUT UINTN   *Text16Size  OPTIONAL
  );

/**
 * ConvertToCHAR8 - Converts a CHAR16 string to a CHAR8 string.
 *
 * @param[in]   Text16           Input CHAR16 string
 * @param[in]   Text16Len        Input CHAR16 number of characters
 * @param[out]  Text8            Where to store pointer to new CHAR8 string
 * @param[out]  Text8Size        CHAR8 string size (includes NULL)
 *
 * @return     String as CHAR8.  Caller is responsible for freeing returned
 *             string
 */
EFI_STATUS
DfciConvertToCHAR8 (
  IN CHAR16  *Text16,
  IN UINTN   Test16Len,
  OUT CHAR8  **Text8,
  OUT UINTN  *Text8Len  OPTIONAL
  );

/**
 * SetStringEntry16 - sets the HiiString, and verify that it was accepted.
 *
 * @param[in]  HiiHandle
 * @param[in]  IdName
 * @param[in]  StringValue
 *
 * @return EFI_STATUS
 */
EFI_STATUS
DfciSetString16Entry (
  IN  EFI_HII_HANDLE  HiiHandle,
  IN  EFI_STRING_ID   IdName,
  IN  CHAR16          *StringValue
  );

/**
 * SetStringEntry - Converts the string to CHAR16, and calls SetString16Entry
 *
 * @param[in]  HiiHandle
 * @param[in]  IdName
 * @param[in]  StringValue
 *
 * @return EFI_STATUS
 */
EFI_STATUS
DfciSetStringEntry (
  IN  EFI_HII_HANDLE  HiiHandle,
  IN  EFI_STRING_ID   IdName,
  IN  CHAR8           *StringValue
  );

/**
 * Get A Setting
 *
 * @param[in]   IdName
 * @param[in]   Type
 * @param[in]   ValuePtr
 * @param[out]  ValueSize
 *
 * @return EFI_STATUS
 */
EFI_STATUS
DfciGetASetting (
  IN  DFCI_SETTING_ID_STRING  IdName,
  IN  DFCI_SETTING_TYPE       Type,
  IN  VOID                    **ValuePtr,
  OUT UINTN                   *ValueSize
  );

/**
 * DfciGetSystemInfo
 *
 * Get the system identifier elements
 *
 * @param[in] DfciInfo
 *
 **/
EFI_STATUS
DfciGetSystemInfo (
  IN DFCI_SYSTEM_INFORMATION  *DfciInfo
  );

/**
 * DfciFreeInfo
 *
 * @param[in]  DfciInfo
 *
 * Free the system identifier elements
 **/
VOID
DfciFreeSystemInfo (
  IN DFCI_SYSTEM_INFORMATION  *DfciInfo
  );

#endif
