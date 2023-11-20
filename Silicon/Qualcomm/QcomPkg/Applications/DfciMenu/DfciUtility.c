/** @file
DfciUtility.c

This module will request new DFCI configuration data from server.

Copyright (C) Microsoft Corporation. All rights reserved.
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>

#include <DfciSystemSettingTypes.h>

#include <Protocol/DfciSettingAccess.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/DfciDeviceIdSupportLib.h>
#include <Library/HiiLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include "DfciUtility.h"

/**
 * ConvertToCHAR16 - Converts a CHAR8 string to a CHAR16 string.
 *
 * @param[in]   Text8          Input CHAR8 String
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
  )
{
  EFI_STATUS  Status;
  CHAR16      *WideString;
  UINTN       WideStringLen;
  UINTN       WideStringSize;

  WideStringLen  = Text8Len + 1;    // WideStringLen has to include NULL
  WideStringSize = WideStringLen * sizeof (CHAR16);

  WideString = AllocatePool (WideStringSize);
  if (NULL == WideString) {
    return EFI_OUT_OF_RESOURCES;
  }

  Status = AsciiStrToUnicodeStrS (Text8, WideString, WideStringLen);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Unable to convert Ascii to Unicode. Code=%r\n", Status));
    FreePool (WideString);
    return Status;
  }

  *Text16 = WideString;
  if (Text16Size != NULL) {
    *Text16Size = WideStringSize;
  }

  return Status;
}

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
  IN UINTN   Text16Len,
  OUT CHAR8  **Text8,
  OUT UINTN  *Text8Size  OPTIONAL
  )
{
  EFI_STATUS  Status;
  CHAR8       *String;
  UINTN       StringLen;
  UINTN       StringSize;

  StringLen  = Text16Len;
  StringSize = StringLen + sizeof (CHAR8);

  String = AllocatePool (StringSize);
  if (NULL == String) {
    return EFI_OUT_OF_RESOURCES;
  }

  Status = UnicodeStrnToAsciiStrS (Text16, Text16Len, String, StringSize, &StringLen);
  if (EFI_ERROR (Status)) {
    FreePool (String);
    DEBUG ((DEBUG_ERROR, "Unable to convert Unicode to Ascii. Code=%r\n", Status));
    return Status;
  }

  *Text8 = String;
  if (Text8Size != NULL) {
    *Text8Size = StringSize;
  }

  return Status;
}

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
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;

  if (IdName != HiiSetString (HiiHandle, IdName, StringValue, NULL)) {
    DEBUG ((DEBUG_ERROR, "%a - Failed to set string for %d: %s. \n", __FUNCTION__, IdName, StringValue));
    Status = EFI_NO_MAPPING;
  }

  return Status;
}

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
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;
  CHAR16      *WideString;

  Status = DfciConvertToCHAR16 (StringValue, AsciiStrLen (StringValue), &WideString, NULL);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = DfciSetString16Entry (HiiHandle, IdName, WideString);
  if (NULL != WideString) {
    FreePool (WideString);
  }

  return Status;
}

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
  )
{
  EFI_STATUS                           Status;
  STATIC DFCI_SETTING_ACCESS_PROTOCOL  *SettingAccess = NULL;
  UINT8                                Dummy;

  *ValuePtr  = NULL;
  *ValueSize = 0;

  if (NULL == SettingAccess) {
    Status = gBS->LocateProtocol (
                    &gDfciSettingAccessProtocolGuid,
                    NULL,
                    (VOID **)&SettingAccess
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a: Unable to obtain the Setting Access protocol. Code = %r\n", __FUNCTION__, Status));
      ASSERT_EFI_ERROR (Status);
      return Status;
    }
  }

  Status = SettingAccess->Get (
                            SettingAccess,
                            IdName,
                            NULL,
                            Type,
                            ValueSize,
                            &Dummy,       // Get doesn't like a NULL pointer
                            NULL
                            );
  if (EFI_ERROR (Status) && (EFI_BUFFER_TOO_SMALL != Status)) {
    DEBUG ((DEBUG_ERROR, "%a - Unable to check %a. %r\n", __FUNCTION__, IdName, Status));
    *ValueSize = 0;
  }

  if (0 == *ValueSize) {
    DEBUG ((DEBUG_ERROR, "%a - Invalid size for %a.\n", __FUNCTION__, IdName));
    goto CLEANUP_SETTING_EXIT;
  }

  *ValuePtr = (UINT8 *)AllocatePool (*ValueSize);
  if (NULL == *ValuePtr) {
    DEBUG ((DEBUG_ERROR, "%a - Unable to allocate memory for %a. %r\n", __FUNCTION__, IdName, Status));
    goto CLEANUP_SETTING_EXIT;
  }

  Status = SettingAccess->Get (
                            SettingAccess,
                            IdName,
                            NULL,
                            Type,
                            ValueSize,
                            *ValuePtr,
                            NULL
                            );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a - Unable to get %a. %r\n", __FUNCTION__, IdName, Status));
    goto CLEANUP_SETTING_EXIT;
  }

  if (Type == DFCI_SETTING_TYPE_STRING) {
    if (*ValueSize == AsciiStrnLenS (*ValuePtr, *ValueSize)) {
      // No terminating NULL
      DEBUG ((DEBUG_ERROR, "%a - No terminating NULL in URL string\n", __FUNCTION__));
      goto CLEANUP_SETTING_EXIT;
    }
  }

  return Status;

CLEANUP_SETTING_EXIT:
  if (*ValuePtr != NULL) {
    FreePool (*ValuePtr);
  }

  return EFI_NOT_FOUND;
}

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
  )
{
  if (NULL != DfciInfo->SerialNumber) {
    FreePool (DfciInfo->SerialNumber);
  }

  if (NULL != DfciInfo->Manufacturer) {
    FreePool (DfciInfo->Manufacturer);
  }

  if (NULL != DfciInfo->ProductName) {
    FreePool (DfciInfo->ProductName);
  }
}

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
  )
{
  EFI_STATUS  Status;

  ZeroMem (DfciInfo, sizeof (DFCI_SYSTEM_INFORMATION));

  Status = DfciIdSupportGetSerialNumber (&DfciInfo->SerialNumber, &DfciInfo->SerialNumberSize);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Unable to get ProductName. Code=%r\n", Status));
    goto Error;
  }

  Status = DfciIdSupportGetManufacturer (&DfciInfo->Manufacturer, &DfciInfo->ManufacturerSize);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Unable to get ProductName. Code=%r\n", Status));
    goto Error;
  }

  Status = DfciIdSupportGetProductName (&DfciInfo->ProductName, &DfciInfo->ProductNameSize);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Unable to get ProductName. Code=%r\n", Status));
    goto Error;
  }

  return EFI_SUCCESS;

Error:
  DfciFreeSystemInfo (DfciInfo);
  return Status;
}
