/** @file
DfciUpdate.c

This module will request and apply a new DFCI configuration.

Copyright (C) Microsoft Corporation. All rights reserved.
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

// A file on a USB key is limited to 255 characters.  This code will generate a filename
// based on the Serial Number, Model, and Manufacturer strings concatenated with "_",
// and truncated to 250 characters.  The file name extensions will be
//
//   .Dfi  -- Dfci Update Package
//
//  After assembling the filename, each character is inspected for invalid characters.  The
//  following are invalid characters:
//
//  Any binary value of 0x01-0x1f, and any of    " * / : < > ? \ |
//
//  All invalid characters are changed to @
//
// The packet from a network contains the exact same contents as the USB update.

#include <Uefi.h>

#include <Guid/DfciPacketHeader.h>
#include <Guid/DfciIdentityAndAuthManagerVariables.h>
#include <Guid/DfciPermissionManagerVariables.h>
#include <Guid/DfciSettingsManagerVariables.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/DfciDeviceIdSupportLib.h>
#include <Library/JsonLiteParser.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>

#include "DfciPrivate.h"
#include "DfciUtility.h"
#include "DfciUsb.h"
#include "DfciRequest.h"
#include "DfciUpdate.h"

STATIC JSON_SET_VARIABLE_TABLE_ENTRY  mJsonSetVariableEntryMailbox[] = {
  {   //  JSON_SET_IDENTITY
    DFCI_IDENTITY_APPLY_VAR_NAME,
    &gDfciAuthProvisionVarNamespace,
    DFCI_IDENTITY_VAR_ATTRIBUTES,
    DFCI_IDENTITY_APPLY_VAR_SIGNATURE
  },

  {   // JSON_SET_IDENTITY2
    DFCI_IDENTITY2_APPLY_VAR_NAME,
    &gDfciAuthProvisionVarNamespace,
    DFCI_IDENTITY_VAR_ATTRIBUTES,
    DFCI_IDENTITY_APPLY_VAR_SIGNATURE
  },

  {   // JSON_SET_PERMISSIONS
    DFCI_PERMISSION_POLICY_APPLY_VAR_NAME,
    &gDfciPermissionManagerVarNamespace,
    DFCI_PERMISSION_POLICY_APPLY_VAR_ATTRIBUTES,
    DFCI_PERMISSION_POLICY_APPLY_VAR_SIGNATURE
  },

  {   // JSON_SET_PERMISSIONS2
    DFCI_PERMISSION2_POLICY_APPLY_VAR_NAME,
    &gDfciPermissionManagerVarNamespace,
    DFCI_PERMISSION_POLICY_APPLY_VAR_ATTRIBUTES,
    DFCI_PERMISSION_POLICY_APPLY_VAR_SIGNATURE
  },

  {   // JSON_SET_SETTINGS
    DFCI_SETTINGS_APPLY_INPUT_VAR_NAME,
    &gDfciSettingsManagerVarNamespace,
    DFCI_SECURED_SETTINGS_VAR_ATTRIBUTES,
    DFCI_SECURED_SETTINGS_APPLY_VAR_SIGNATURE
  },

  {   // JSON_SET_SETTINGS2
    DFCI_SETTINGS2_APPLY_INPUT_VAR_NAME,
    &gDfciSettingsManagerVarNamespace,
    DFCI_SECURED_SETTINGS_VAR_ATTRIBUTES,
    DFCI_SECURED_SETTINGS_APPLY_VAR_SIGNATURE
  }
};

#define KEYWORD_MFG         "OemManufacturer"
#define KEYWORD_MODEL       "ModelName"
#define KEYWORD_SERIAL      "SerialNumber"
#define KEYWORD_THUMBPRINT  "Thumbprint"

#define KEYWORD_PROVISIONING      "ProvisioningPacket"
#define KEYWORD_PROVISIONING2     "ProvisioningPacket2"
#define KEYWORD_PERMISSIONS       "PermissionsPacket"
#define KEYWORD_PERMISSIONS2      "PermissionsPacket2"
#define KEYWORD_SETTINGS          "SettingsPacket"
#define KEYWORD_SETTINGS2         "SettingsPacket2"
#define KEYWORD_TRANSITIONING1    "TransitionPacket1"
#define KEYWORD_TRANSITIONING2    "TransitionPacket2"
#define KEYWORD_RESULT_MESSAGE    "ResultMessage"
#define KEYWORD_RESULT_CODE       "ResultCode"
#define KEYWORD_OWNER_THUMBPRINT  "DdsWildcardCertificateThumbprint"
#define KEYWORD_HTTPS_THUMBPRINT  "DdsEncryptionCertificateThumbprint"
#define KEYWORD_TENANTID          "TenantId"
#define KEYWORD_REGISTRATIONID    "RegistrationId"

//
// DFCI RecoveryBootstrap response
//
JSON_RESPONSE_TO_ACTION_ENTRY  mRecoveryBootstrapResponse[] = {
  {
    KEYWORD_TRANSITIONING1,
    NULL,
    NULL,
    JSON_ACTION_SET_VARIABLE,
    JSON_SET_IDENTITY,
    TRUE
  },
  {
    KEYWORD_TRANSITIONING2,
    NULL,
    NULL,
    JSON_ACTION_SET_VARIABLE,
    JSON_SET_IDENTITY2,
    TRUE
  },
  {
    KEYWORD_SETTINGS,
    NULL,
    NULL,
    JSON_ACTION_SET_VARIABLE,
    JSON_SET_SETTINGS,
    TRUE
  },
  {
    KEYWORD_RESULT_MESSAGE,
    &mDfciNetworkRequest.HttpStatus.HttpMessage,
    &mDfciNetworkRequest.HttpStatus.HttpMessageSize,
    JSON_ACTION_SET_HTTP_MESSAGE,
    0,
    FALSE
  },
  {
    KEYWORD_RESULT_CODE,
    &mDfciNetworkRequest.HttpStatus.HttpReturnCode,
    &mDfciNetworkRequest.HttpStatus.HttpReturnCodeSize,
    JSON_ACTION_SET_RETURN_CODE,
    0,
    FALSE
  },
  {
    NULL,
    NULL,
    NULL,
    0,
    0,
    FALSE
  }
};

//
// DFCI Recovery response
//
JSON_RESPONSE_TO_ACTION_ENTRY  mRecoveryResponse[] = {
  { KEYWORD_PROVISIONING,
    NULL,
    NULL,
    JSON_ACTION_SET_VARIABLE,
    JSON_SET_IDENTITY,
    TRUE },
  { KEYWORD_PERMISSIONS,
    NULL,
    NULL,
    JSON_ACTION_SET_VARIABLE,
    JSON_SET_PERMISSIONS,
    TRUE },
  { KEYWORD_SETTINGS,
    NULL,
    NULL,
    JSON_ACTION_SET_VARIABLE,
    JSON_SET_SETTINGS,
    TRUE },
  { KEYWORD_RESULT_MESSAGE,
    &mDfciNetworkRequest.HttpStatus.HttpMessage,
    &mDfciNetworkRequest.HttpStatus.HttpMessageSize,
    JSON_ACTION_SET_HTTP_MESSAGE,
    0,
    FALSE },
  { KEYWORD_RESULT_CODE,
    &mDfciNetworkRequest.HttpStatus.HttpReturnCode,
    &mDfciNetworkRequest.HttpStatus.HttpReturnCodeSize,
    JSON_ACTION_SET_RETURN_CODE,
    0,
    FALSE },
  { NULL,
    NULL,
    NULL,
    0,
    0,
    FALSE }
};

//
// DFCI USB Recovery response
//
//
// The standard Dfci Json string is of the format:
//    { "ProvisioningPacket",  "b64-encoded-dfci-identity-packet",
//      "ProvisioningPacket2", "b64-encoded-dfci-identity-packet",
//      "Permissions",         "b64-encoded-dfci-permissions-packet",
//      "Permissions2",        "b64-encoded-dfci-permissions-packet",
//      "SettingsPacket",      "b64-encoded-dfci-settings-packet",
//      "SettingsPacket2",     "b64-encoded-dfci-settings-packet" }
//
JSON_RESPONSE_TO_ACTION_ENTRY  mUsbRecovery[] = {
  { KEYWORD_PROVISIONING,
    NULL,
    NULL,
    JSON_ACTION_SET_VARIABLE,
    JSON_SET_IDENTITY,
    TRUE },
  { KEYWORD_PROVISIONING2,
    NULL,
    NULL,
    JSON_ACTION_SET_VARIABLE,
    JSON_SET_IDENTITY2,
    TRUE },
  { KEYWORD_PERMISSIONS,
    NULL,
    NULL,
    JSON_ACTION_SET_VARIABLE,
    JSON_SET_PERMISSIONS,
    TRUE, },
  { KEYWORD_PERMISSIONS2,
    NULL,
    NULL,
    JSON_ACTION_SET_VARIABLE,
    JSON_SET_PERMISSIONS2,
    TRUE },
  { KEYWORD_SETTINGS,
    NULL,
    NULL,
    JSON_ACTION_SET_VARIABLE,
    JSON_SET_SETTINGS,
    TRUE, },
  { KEYWORD_SETTINGS2,
    NULL,
    NULL,
    JSON_ACTION_SET_VARIABLE,
    JSON_SET_SETTINGS2,
    TRUE },
  { NULL,
    NULL,
    NULL,
    0,
    0,
    FALSE }
};

/**
 * BuildUsbRequest
 *
 * @param[in]   FileNameExtension - Extension for file name
 * @param[out]  filename          - Name of the file on USB to retrieve
 *
 **/
EFI_STATUS
EFIAPI
BuildUsbRequest (
  IN  CHAR16  *FileExtension,
  OUT CHAR16  **FileName
  )
{
  DFCI_SYSTEM_INFORMATION  DfciInfo;
  UINTN                    i;
  CHAR16                   *PktFileName = NULL;
  UINTN                    PktNameLen;
  EFI_STATUS               Status;

  PktFileName = NULL;

  Status = DfciGetSystemInfo (&DfciInfo);
  if (EFI_ERROR (Status)) {
    goto Error;
  }

  PktFileName = (CHAR16 *)AllocatePool (MAX_USB_FILE_NAME_LENGTH * sizeof (CHAR16));
  if (PktFileName == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto Error;
  }

  // The maximum file name length is 255 characters and a NULL.  Leave room for
  // the four character file name extension.  Create the base PktFileName out of
  // the first 251 characters of SerialNumber_ProductName_Manufacturer then add the
  // file name extension.

  PktNameLen = UnicodeSPrintAsciiFormat (
                 PktFileName,
                 (MAX_USB_FILE_NAME_LENGTH - 4) * sizeof (CHAR16),
                 "%a_%a_%a",
                 DfciInfo.SerialNumber,
                 DfciInfo.ProductName,
                 DfciInfo.Manufacturer
                 );
  DfciFreeSystemInfo (&DfciInfo);
  if ((PktNameLen == 0) || (PktNameLen >= (MAX_USB_FILE_NAME_LENGTH - 4))) {
    DEBUG ((DEBUG_ERROR, "Invalid file name length %d\n", PktNameLen));
    Status = EFI_BAD_BUFFER_SIZE;
    goto Error;
  }

  //
  //  Any binary value of 0x01-0x1f, and any of    " * / : < > ? \ |
  //  are not allowed in the file name.  If any of these exist, then
  //  replace the invalid character with an '@'.
  //
  for (i = 0; i < PktNameLen; i++) {
    if (((PktFileName[i] >= 0x00) &&
         (PktFileName[i] <= 0x1F)) ||
        (PktFileName[i] == L'\"') ||
        (PktFileName[i] == L'*')  ||
        (PktFileName[i] == L'/')  ||
        (PktFileName[i] == L':')  ||
        (PktFileName[i] == L'<')  ||
        (PktFileName[i] == L'>')  ||
        (PktFileName[i] == L'?')  ||
        (PktFileName[i] == L'\\') ||
        (PktFileName[i] == L'|'))
    {
      PktFileName[i] = L'@';
    }
  }

  Status = StrCatS (PktFileName, MAX_USB_FILE_NAME_LENGTH * sizeof (CHAR16), FileExtension);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Unable to append the file name ext. Code=%r\n", Status));
    goto Error;
  }

  *FileName = PktFileName;

  return EFI_SUCCESS;

Error:
  if (NULL != PktFileName) {
    FreePool (PktFileName);
  }

  DfciFreeSystemInfo (&DfciInfo);

  return Status;
}

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
  )
{
  #define JSON_RECOVERY_BOOTSTRAP_COUNT  2

  JSON_REQUEST_ELEMENT  JsonRequest[JSON_RECOVERY_BOOTSTRAP_COUNT];
  CHAR8                 *JsonRequestString;
  UINTN                 JsonRequestStringSize;
  EFI_STATUS            Status;

  JsonRequest[0].FieldName = KEYWORD_HTTPS_THUMBPRINT;
  JsonRequest[0].FieldLen  = sizeof (KEYWORD_HTTPS_THUMBPRINT) - sizeof (CHAR8);
  JsonRequest[0].Value     = NetworkRequest->HttpsThumbprint;
  JsonRequest[0].ValueLen  = NetworkRequest->HttpsThumbprintSize - sizeof (CHAR8);

  JsonRequest[1].FieldName = KEYWORD_OWNER_THUMBPRINT;
  JsonRequest[1].FieldLen  = sizeof (KEYWORD_OWNER_THUMBPRINT) - sizeof (CHAR8);
  JsonRequest[1].Value     = NetworkRequest->OwnerThumbprint;
  JsonRequest[1].ValueLen  = NetworkRequest->OwnerThumbprintSize - sizeof (CHAR8);

  Status = JsonLibEncode (JsonRequest, JSON_RECOVERY_BOOTSTRAP_COUNT, &JsonRequestString, &JsonRequestStringSize);
  if (!EFI_ERROR (Status)) {
    NetworkRequest->HttpRequest.Body     = JsonRequestString;
    NetworkRequest->HttpRequest.BodySize = JsonRequestStringSize;
  }

  return Status;
}

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
  )
{
  #define JSON_RECOVERY_REQUEST_COUNT  6

  JSON_REQUEST_ELEMENT  JsonRequest[JSON_RECOVERY_REQUEST_COUNT];
  CHAR8                 *JsonRequestString;
  UINTN                 JsonRequestStringSize;
  EFI_STATUS            Status;

  JsonRequest[0].FieldName = KEYWORD_MFG;
  JsonRequest[0].FieldLen  = sizeof (KEYWORD_MFG) - sizeof (CHAR8);
  JsonRequest[0].Value     = NetworkRequest->DfciInfo.Manufacturer;
  JsonRequest[0].ValueLen  = NetworkRequest->DfciInfo.ManufacturerSize - sizeof (CHAR8);

  JsonRequest[1].FieldName = KEYWORD_MODEL;
  JsonRequest[1].FieldLen  = sizeof (KEYWORD_MODEL) - sizeof (CHAR8);
  JsonRequest[1].Value     = NetworkRequest->DfciInfo.ProductName;
  JsonRequest[1].ValueLen  = NetworkRequest->DfciInfo.ProductNameSize - sizeof (CHAR8);

  JsonRequest[2].FieldName = KEYWORD_SERIAL;
  JsonRequest[2].FieldLen  = sizeof (KEYWORD_SERIAL) - sizeof (CHAR8);
  JsonRequest[2].Value     = NetworkRequest->DfciInfo.SerialNumber;
  JsonRequest[2].ValueLen  = NetworkRequest->DfciInfo.SerialNumberSize - sizeof (CHAR8);

  JsonRequest[3].FieldName = KEYWORD_OWNER_THUMBPRINT;
  JsonRequest[3].FieldLen  = sizeof (KEYWORD_OWNER_THUMBPRINT) - sizeof (CHAR8);
  JsonRequest[3].Value     = NetworkRequest->OwnerThumbprint;
  JsonRequest[3].ValueLen  = NetworkRequest->OwnerThumbprintSize - sizeof (CHAR8);

  JsonRequest[4].FieldName = KEYWORD_TENANTID;
  JsonRequest[4].FieldLen  = sizeof (KEYWORD_TENANTID) - sizeof (CHAR8);
  JsonRequest[4].Value     = NetworkRequest->TenantId;
  JsonRequest[4].ValueLen  = NetworkRequest->TenantIdSize - sizeof (CHAR8);

  JsonRequest[5].FieldName = KEYWORD_REGISTRATIONID;
  JsonRequest[5].FieldLen  = sizeof (KEYWORD_REGISTRATIONID) - sizeof (CHAR8);
  JsonRequest[5].Value     = NetworkRequest->RegistrationId;
  JsonRequest[5].ValueLen  = NetworkRequest->RegistrationIdSize - sizeof (CHAR8);

  Status = JsonLibEncode (JsonRequest, JSON_RECOVERY_REQUEST_COUNT, &JsonRequestString, &JsonRequestStringSize);
  if (!EFI_ERROR (Status)) {
    if (NetworkRequest->HttpRequest.Body == NULL) {
      FreePool (NetworkRequest->HttpRequest.Body);
    }

    NetworkRequest->HttpRequest.Body     = JsonRequestString;
    NetworkRequest->HttpRequest.BodySize = JsonRequestStringSize;
  }

  return Status;
}

/**
 *  Function to process a Json Element
 *
 * @param[in]  Json Request Element   Element Being Processed
 * @param[in]  Context                Context for Apply Function
 *
 * @retval EFI_SUCCESS -       Packet processed normally, no variable set
 * @retval EFI_MEDIA_CHANGED - Packet processed normally, variable set
 * @retval Error -             Severe error processing packet
 */
STATIC
EFI_STATUS
EFIAPI
ProcessFunction (
  IN  JSON_REQUEST_ELEMENT  *Rqst,
  IN  VOID                  *Context
  )
{
  UINTN                          ActionIndex;
  UINTN                          j;
  DFCI_PACKET_HEADER             *Pkt;
  JSON_RESPONSE_TO_ACTION_ENTRY  *ResponseTable;
  EFI_STATUS                     Status;
  CHAR8                          *StringValue;
  BOOLEAN                        Valid;
  BOOLEAN                        VariableChanged;
  UINTN                          ValueSize;

  //
  // The FieldName and Value must be specified
  //
  if ((NULL == Rqst) ||
      (NULL == Rqst->FieldName) ||
      (NULL == Rqst->Value) ||
      (NULL == Context))
  {
    DEBUG ((DEBUG_ERROR, "Invalid or missing ProcessFunction parameter\n"));
    return EFI_INVALID_PARAMETER;
  }

  ResponseTable   = (JSON_RESPONSE_TO_ACTION_ENTRY *)Context;
  Valid           = FALSE;
  VariableChanged = FALSE;
  for (j = 0; ResponseTable[j].FieldName != NULL; j++) {
    if ((Rqst->FieldLen == AsciiStrLen (ResponseTable[j].FieldName)) &&
        (0 == AsciiStrnCmp (ResponseTable[j].FieldName, Rqst->FieldName, Rqst->FieldLen)))
    {
      if (ResponseTable[j].DecodeBase64) {
        ValueSize = 0;
        Status    = Base64Decode (Rqst->Value, Rqst->ValueLen, NULL, &ValueSize);
        if (Status != EFI_BUFFER_TOO_SMALL) {
          DEBUG ((DEBUG_ERROR, "Cannot query binary blob size. Code = %r\n", Status));
          return EFI_INVALID_PARAMETER;
        }

        StringValue = (CHAR8 *)AllocatePool (ValueSize + sizeof (CHAR8));       // Allow for extra NULL
        if (NULL == StringValue) {
          DEBUG ((DEBUG_ERROR, "Cannot allocate StringValue\n"));
          return EFI_OUT_OF_RESOURCES;
        }

        if (NULL == StringValue) {
          DEBUG ((DEBUG_ERROR, "Failed to allocate pool\n"));
          return EFI_OUT_OF_RESOURCES;
        }

        Status = Base64Decode (Rqst->Value, Rqst->ValueLen, (UINT8 *)StringValue, &ValueSize);
        if (EFI_ERROR (Status)) {
          FreePool (StringValue);
          DEBUG ((DEBUG_ERROR, "Cannot decode Value data. Code=%r\n", Status));
          return Status;
        }

        StringValue[ValueSize] = '\0';        // Add a NULL in case it is not part of the string.
      } else {
        StringValue = AllocateZeroPool (Rqst->ValueLen + sizeof (CHAR8));
        if (NULL == StringValue) {
          return EFI_OUT_OF_RESOURCES;
        }

        CopyMem (StringValue, Rqst->Value, Rqst->ValueLen);
        ValueSize = Rqst->ValueLen + sizeof (CHAR8);
      }

      ActionIndex = ResponseTable[j].VariableIndex;

      switch (ResponseTable[j].Action) {
        case JSON_ACTION_SET_VARIABLE:
          Pkt = (DFCI_PACKET_HEADER *)StringValue;
          if (mJsonSetVariableEntryMailbox[ActionIndex].Signature != Pkt->Sig.Hdr.Signature) {
            DEBUG ((
              DEBUG_ERROR,
              "Invalid binary signature %4.4x, Indx=%d, Rqst %.*a. Expected %4.4x for %a.\n",
              Pkt->Sig.Hdr.Signature,
              j,
              Rqst->FieldLen,
              Rqst->FieldName,
              mJsonSetVariableEntryMailbox[ActionIndex].Signature,
              ResponseTable[j].FieldName
              ));
            FreePool (StringValue);
            return EFI_INVALID_PARAMETER;
          }

          Status = gRT->SetVariable (
                          mJsonSetVariableEntryMailbox[ActionIndex].MailboxName,
                          mJsonSetVariableEntryMailbox[ActionIndex].MailboxNamespace,
                          mJsonSetVariableEntryMailbox[ActionIndex].MailboxAttributes,
                          ValueSize,
                          (VOID *)Pkt
                          );
          if (EFI_ERROR (Status)) {
            DEBUG ((
              DEBUG_ERROR,
              "Unable to set mailbox %s. Code = %r\n",
              mJsonSetVariableEntryMailbox[ActionIndex].MailboxName,
              Status
              ));
            FreePool (StringValue);
            return Status;
          } else {
            VariableChanged = TRUE;
            DEBUG ((DEBUG_INFO, "Mailbox %s setup\n", mJsonSetVariableEntryMailbox[ActionIndex].MailboxName));
          }

          Valid = TRUE;
          break;

        case JSON_ACTION_SET_RETURN_CODE:
        case JSON_ACTION_SET_HTTP_MESSAGE:
          *ResponseTable[j].Message     = StringValue;
          *ResponseTable[j].MessageSize = ValueSize;
          StringValue                   = NULL;
          Valid                         = TRUE;
          break;

        default:
          break;
      }

      if (NULL != StringValue) {
        FreePool (StringValue);
      }

      break;
    }
  }

  if (Valid) {
    if (VariableChanged) {
      Status = EFI_MEDIA_CHANGED;
      DEBUG ((DEBUG_INFO, "Media Change detected in DfciUpdate\n"));
    } else {
      Status = EFI_SUCCESS;
    }
  } else {
    Status = EFI_INVALID_PARAMETER;
    DEBUG ((DEBUG_ERROR, "Rqst not found in ResponseTable. Rqst=%a\n", Rqst->FieldName));
  }

  return Status;
}

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
  )
{
  EFI_STATUS  Status;

  Status = JsonLibParse (JsonString, JsonStringSize, ProcessFunction, (VOID *)ResponseTable);
  return Status;
}
