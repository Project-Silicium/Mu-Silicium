/** @file
DfciUpdate.h

DfciUpdate parses the Json String and applies each element

Copyright (C) Microsoft Corporation. All rights reserved.
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef __DFCI_UPDATE_H__
#define __DFCI_UPDATE_H__

typedef struct {
  CHAR16      *MailboxName;
  EFI_GUID    *MailboxNamespace;
  UINT32      MailboxAttributes;
  UINT32      Signature;
} JSON_SET_VARIABLE_TABLE_ENTRY;

#define JSON_SET_IDENTITY      0
#define JSON_SET_IDENTITY2     1
#define JSON_SET_PERMISSIONS   2
#define JSON_SET_PERMISSIONS2  3
#define JSON_SET_SETTINGS      4
#define JSON_SET_SETTINGS2     5

#define JSON_ACTION_SET_VARIABLE      0
#define JSON_ACTION_SET_RETURN_CODE   1
#define JSON_ACTION_SET_HTTP_MESSAGE  2

typedef struct {
  CHAR8      *FieldName;
  CHAR8      **Message;
  UINTN      *MessageSize;
  UINTN      Action;
  UINTN      VariableIndex;    // Only for action SET_VARIABLE
  BOOLEAN    DecodeBase64;
} JSON_RESPONSE_TO_ACTION_ENTRY;

//
// Expected JSON Results
//
extern JSON_RESPONSE_TO_ACTION_ENTRY  mRecoveryBootstrapResponse[];
extern JSON_RESPONSE_TO_ACTION_ENTRY  mRecoveryResponse[];
extern JSON_RESPONSE_TO_ACTION_ENTRY  mUsbRecovery[];

/**
 * BuildUsbRequest
 *
 * @param[in]   FileNameExtension - Extension for file name
 * @param[out]  filename   - Name of the file on USB to retrieve
 *
 **/
EFI_STATUS
EFIAPI
BuildUsbRequest (
  IN  CHAR16  *FileExtension,
  OUT CHAR16  **FileName
  );

/**
 * BuildJsonBootstrapRequest
 *
 * Build the payload for a Bootstrap request
 *
 * @Param [in]    NetworkRequest
 *
 **/
EFI_STATUS
EFIAPI
BuildJsonBootstrapRequest (
  IN DFCI_NETWORK_REQUEST  *NetworkRequest
  );

/**
 * BuildJsonRecoveryRequest
 *
 * Build the payload for a Recovery request
 *
 * @Param [in]    NetworkRequest
 *
 **/
EFI_STATUS
EFIAPI
BuildJsonRecoveryRequest (
  IN DFCI_NETWORK_REQUEST  *NetworkRequest
  );

/**
 * DfciUpdateFromJson
 *
 * Parse the Json String and update DFCI with each element parsed.
 *
 * @param [in]  JsonString      - Dfci Update Json string
 * @param [in]  JsonStringSize  - Size of Json String
 * @param [in]  ResponseTable   - Expected JSON elements
 *
 **/
EFI_STATUS
EFIAPI
DfciUpdateFromJson (
  IN  CHAR8                          *JsonString,
  IN  UINTN                          JsonStringSize,
  IN  JSON_RESPONSE_TO_ACTION_ENTRY  *ResponseTable
  );

#endif // __DFCI_UPDATE_H__
