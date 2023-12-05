/** @file
DfciSARecovery.c

Device Firmware Configuration Interface - Stand Alone driver that can be loaded
at the UEFI Shell

Copyright (C) Microsoft Corporation. All rights reserved.
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>

#include <Uefi/UefiInternalFormRepresentation.h>

#include <Guid/DfciMenuGuid.h>
#include <Guid/MdeModuleHii.h>
#include <Guid/DfciSettingsGuid.h>
#include <Guid/DfciEventGroup.h>
#include <Guid/GlobalVariable.h>
#include <Guid/TlsAuthentication.h>

#include <DfciSystemSettingTypes.h>

#include <Protocol/DevicePath.h>
#include <Protocol/DfciAuthentication.h>
#include <Protocol/DfciSettingPermissions.h>
#include <Protocol/GraphicsOutput.h>
#include <Protocol/HiiConfigAccess.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/DfciUiSupportLib.h>
#include <Library/HiiLib.h>
#include <Library/HttpLib.h>
#include <Library/JsonLiteParser.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiHiiServicesLib.h>
#include <Library/ZeroTouchSettingsLib.h>

#include <Settings/DfciSettings.h>
#include <Settings/DfciPrivateSettings.h>

#include "DfciMenu.h"

#include "DfciPrivate.h"
#include "DfciUtility.h"
#include "DfciUpdate.h"
#include "DfciRequest.h"
#include "DfciUsb.h"

#pragma pack(1)

///
/// HII specific Vendor Device Path definition.
///
typedef struct {
  VENDOR_DEVICE_PATH          VendorDevicePath;
  EFI_DEVICE_PATH_PROTOCOL    End;
} HII_VENDOR_DEVICE_PATH;

#pragma pack()
HII_VENDOR_DEVICE_PATH  mHiiVendorDevicePath = {
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_VENDOR_DP,
      {
        (UINT8)(sizeof (VENDOR_DEVICE_PATH)),
        (UINT8)((sizeof (VENDOR_DEVICE_PATH)) >> 8)
      }
    },
    EFI_CALLER_ID_GUID
  },
  {
    END_DEVICE_PATH_TYPE,
    END_ENTIRE_DEVICE_PATH_SUBTYPE,
    {
      (UINT8)(END_DEVICE_PATH_LENGTH),
      (UINT8)((END_DEVICE_PATH_LENGTH) >> 8)
    }
  }
};

// DFCI HII Package GUID: {  93a27eb8-233a-43d8-b81b-925a38a80988
#define DFCI_HII_PACKAGE_LIST_GUID                                                 \
{                                                                                  \
    0x93a27eb8, 0x233a, 0x43d8, {0xb8, 0x1b, 0x92, 0x5a, 0x38, 0xa8, 0x09, 0x88}   \
}

#define DFCI_MENU_SIGNATURE    SIGNATURE_32 ('i', 'c', 'f', 'D')
#define DEFAULT_USB_FILE_NAME  L"DfciUpdate.Dfi"

// Allow for 3 lines of text that are 100 CHAR16's
#define MAX_MSG_SIZE  600

// *---------------------------------------------------------------------------------------*
// * Application Global Variables                                                          *
// *---------------------------------------------------------------------------------------*
DFCI_NETWORK_REQUEST  mDfciNetworkRequest;

// *---------------------------------------------------------------------------------------*
// * Global Variables                                                                      *
// *---------------------------------------------------------------------------------------*
STATIC DFCI_AUTHENTICATION_PROTOCOL       *mAuthenticationProtocol = NULL;
STATIC DFCI_MENU_CONFIGURATION            mDfciMenuConfiguration;
STATIC DFCI_SETTING_PERMISSIONS_PROTOCOL  *mDfciSettingsPermissionProtocol = NULL;
STATIC DFCI_IDENTITY_MASK                 mIdMask;                // Identities installed
STATIC CHAR8                              *mDfciUrl = NULL;
STATIC UINTN                              mDfciUrlSize;

typedef struct {
  DFCI_IDENTITY_ID     Identity;
  DFCI_CERT_REQUEST    CertRequest;
  DFCI_CERT_FORMAT     CertFormat;
  EFI_STRING_ID        VfrField;
} CERT_INIT_TABLE_ENTRY;

static CERT_INIT_TABLE_ENTRY  mCertInitTable[] = {
  { DFCI_IDENTITY_SIGNER_ZTD,   DFCI_CERT_SUBJECT,    DFCI_CERT_FORMAT_CHAR16,    STR_DFCI_ZTD_SUBJECT_FIELD      },
  { DFCI_IDENTITY_SIGNER_ZTD,   DFCI_CERT_THUMBPRINT, DFCI_CERT_FORMAT_CHAR16_UI, STR_DFCI_ZTD_THUMBPRINT_FIELD   },
  { DFCI_IDENTITY_SIGNER_OWNER, DFCI_CERT_SUBJECT,    DFCI_CERT_FORMAT_CHAR16,    STR_DFCI_OWNER_SUBJECT_FIELD    },
  { DFCI_IDENTITY_SIGNER_OWNER, DFCI_CERT_THUMBPRINT, DFCI_CERT_FORMAT_CHAR16_UI, STR_DFCI_OWNER_THUMBPRINT_FIELD },
  { DFCI_IDENTITY_SIGNER_USER,  DFCI_CERT_SUBJECT,    DFCI_CERT_FORMAT_CHAR16,    STR_DFCI_USER_SUBJECT_FIELD     },
  { DFCI_IDENTITY_SIGNER_USER,  DFCI_CERT_THUMBPRINT, DFCI_CERT_FORMAT_CHAR16_UI, STR_DFCI_USER_THUMBPRINT_FIELD  }
};

#define CERT_TABLE_COUNT  (sizeof(mCertInitTable)/sizeof(CERT_INIT_TABLE_ENTRY))

//
// Private internal data
//
typedef struct {
  UINTN             Signature;
  EFI_HANDLE        DriverHandle;
  EFI_HII_HANDLE    HiiHandle;
} DFCI_MENU_PRIVATE;

DFCI_MENU_PRIVATE  mDfciMenuPrivate = {
  DFCI_MENU_SIGNATURE,
  NULL,
  NULL
};

/**
 * Check if Dfci is enabled
 *
 * @return BOOLEAN      FALSE == No Dfci present
 *                      TRUE  == Dfci Present
 *
 *  Dfci requires more than just the OwnerKey installed.
 *
 */
STATIC
BOOLEAN
CheckIfDfciEnrolled (
  VOID
  )
{
  EFI_STATUS  Status;
  BOOLEAN     IsDfciMenuEnabled = FALSE;
  UINT8       *Cert;
  UINTN       CertSize;

  mDfciMenuConfiguration.DfciZeroTouchOptGrayOut    = MENU_FALSE;
  mDfciMenuConfiguration.DfciZeroTouchCertAvailable = MENU_FALSE;
  mDfciMenuConfiguration.DfciZeroTouchEnabled       = MENU_FALSE;
  mDfciMenuConfiguration.DfciOwnerEnabled           = MENU_FALSE;
  mDfciMenuConfiguration.DfciUserEnabled            = MENU_FALSE;

  Status =  mAuthenticationProtocol->GetEnrolledIdentities (mAuthenticationProtocol, &mIdMask);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a - Failed to get owner ids. %r\n", __FUNCTION__, Status));
    return FALSE;
  }

  Status = GetZeroTouchCertificate (&Cert, &CertSize);
  if (!EFI_ERROR (Status)) {
    mDfciMenuConfiguration.DfciZeroTouchCertAvailable = MENU_TRUE;
    FreePool (Cert);
    DEBUG ((DEBUG_INFO, "%a: Zero Touch certificate is available\n", __FUNCTION__));
  }

  DEBUG ((DEBUG_INFO, "IdMask=%x\n", mIdMask));
  if (IS_ZTD_IDENTITY_ENROLLED (mIdMask)) {
    Status =  mAuthenticationProtocol->GetCertInfo (
                                         mAuthenticationProtocol,
                                         DFCI_IDENTITY_SIGNER_ZTD,
                                         NULL,
                                         0,
                                         DFCI_CERT_THUMBPRINT,
                                         DFCI_CERT_FORMAT_CHAR8,
                                         (VOID **)&mDfciNetworkRequest.ZeroTouchThumbprint,
                                         &mDfciNetworkRequest.ZeroTouchThumbprintSize
                                         );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a - Failed to get ZTD cert. %r\n", __FUNCTION__, Status));
    } else {
      if (NULL != mDfciNetworkRequest.ZeroTouchThumbprint) {
        mDfciMenuConfiguration.DfciZeroTouchEnabled = MENU_TRUE;
        IsDfciMenuEnabled                           = TRUE;
      }
    }
  }

  if (IS_OWNER_IDENTITY_ENROLLED (mIdMask)) {
    Status =  mAuthenticationProtocol->GetCertInfo (
                                         mAuthenticationProtocol,
                                         DFCI_IDENTITY_SIGNER_OWNER,
                                         NULL,
                                         0,
                                         DFCI_CERT_THUMBPRINT,
                                         DFCI_CERT_FORMAT_CHAR8,
                                         (VOID **)&mDfciNetworkRequest.OwnerThumbprint,
                                         &mDfciNetworkRequest.OwnerThumbprintSize
                                         );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a - Failed to get owner cert. %r\n", __FUNCTION__, Status));
    } else {
      if (NULL != mDfciNetworkRequest.OwnerThumbprint) {
        mDfciMenuConfiguration.DfciOwnerEnabled = MENU_TRUE;
      }
    }
  }

  if (IS_USER_IDENTITY_ENROLLED (mIdMask)) {
    mDfciMenuConfiguration.DfciUserEnabled = MENU_TRUE;
    IsDfciMenuEnabled                      = TRUE;
  }

  Status = DfciGetSystemInfo (&mDfciNetworkRequest.DfciInfo);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Unable to get Dfci System Info. %r\n", __FUNCTION__, Status));
  }

  DEBUG ((
    DEBUG_INFO,
    "%a - IsDfci=%d, ZtdEnabled=%d, OwnerEnabled=%d, UserEnabled=%d\n",
    __FUNCTION__,
    IsDfciMenuEnabled,
    mDfciMenuConfiguration.DfciZeroTouchEnabled,
    mDfciMenuConfiguration.DfciOwnerEnabled,
    mDfciMenuConfiguration.DfciUserEnabled
    ));
  return IsDfciMenuEnabled;
}

/**
 * Get Dfci Parameters.
 *
 * @param    NONE
 *
 * @return EFI_STATUS
 */
STATIC
EFI_STATUS
GetDfciParameters (
  VOID
  )
{
  STATIC BOOLEAN      AlreadyRun = FALSE;
  CHAR16              *Field;
  UINTN               i;
  CHAR8               *HostName;
  CHAR8               *Name;
  UINTN               NameSize;
  VOID                *Parser;
  DFCI_IDENTITY_MASK  RecoveryMask;
  EFI_HII_HANDLE      *RecoveryHandle;
  EFI_STATUS          Status;

  if (!AlreadyRun) {
    AlreadyRun = TRUE;

    mDfciMenuConfiguration.DfciHttpRecoveryEnabled = MENU_FALSE;
    mDfciMenuConfiguration.DfciRecoveryEnabled     = MENU_FALSE;

    //
    // Populate Cert information
    //
    for (i = 0; i < CERT_TABLE_COUNT; i++) {
      Status =  mAuthenticationProtocol->GetCertInfo (
                                           mAuthenticationProtocol,
                                           mCertInitTable[i].Identity,
                                           NULL,
                                           0,
                                           mCertInitTable[i].CertRequest,
                                           mCertInitTable[i].CertFormat,
                                           (VOID **)&Field,
                                           NULL
                                           );
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "%a - Failed to get %x cert. %r\n", __FUNCTION__, mCertInitTable[i].Identity, Status));
      } else {
        if (NULL != Field) {
          DEBUG ((DEBUG_INFO, "String is %s", Field));
          DEBUG ((DEBUG_INFO, "\n"));
          FreePool (Field);
        }
      }
    }

    //
    // Check if hard unenroll is enabled
    //
    RecoveryMask = 0;
    Status       = mDfciSettingsPermissionProtocol->GetPermission (mDfciSettingsPermissionProtocol, DFCI_PRIVATE_SETTING_ID__DFCI_RECOVERY, &RecoveryMask);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a - Failed to get permission for recovery %r\n", __FUNCTION__, Status));
    }

    DEBUG ((DEBUG_INFO, "%a - mIdMask=%x, RecoveryMask=%x\n", __FUNCTION__, mIdMask, RecoveryMask));
    RecoveryMask &= mIdMask;

    if (RecoveryMask == 0) {
      DEBUG ((DEBUG_INFO, "%a - No Identities have DFCI Recovery Permissions\n", __FUNCTION__));
    }

    RecoveryHandle = HiiGetHiiHandles (&gDfciRecoveryFormsetGuid);
    if (RecoveryHandle != NULL) {
      mDfciMenuConfiguration.DfciRecoveryEnabled = MENU_TRUE;
      DEBUG ((DEBUG_INFO, "Dfci Recovery is enabled\n"));
      FreePool (RecoveryHandle);
    }

    RecoveryMask = 0;
    Status       = mDfciSettingsPermissionProtocol->GetPermission (mDfciSettingsPermissionProtocol, DFCI_PRIVATE_SETTING_ID__ZTD_RECOVERY, &RecoveryMask);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a - Failed to get permission for recovery %r\n", __FUNCTION__, Status));
    }

    if (RecoveryMask != 0) {
      mDfciMenuConfiguration.DfciRecoveryEnabled = MENU_TRUE;
      DEBUG ((DEBUG_INFO, "%a - Ztd Recovery enabled\n", __FUNCTION__));
    }

    // To enable Http recovery, the URL, Https Cert, TenantId, and Registration Id are
    // required.  These settings may be at the default state, which is NULL (size 0) for
    // the Https Cert, or a NULL string. For this simple test, to be valid, they need to
    // have a size of 2 or better.

    Status = DfciGetASetting (
               DFCI_PRIVATE_SETTING_ID__DFCI_RECOVERY_URL,
               DFCI_SETTING_TYPE_STRING,
               (VOID **)&mDfciUrl,
               &mDfciUrlSize
               );
    if (EFI_ERROR (Status) || (mDfciUrlSize <= sizeof (CHAR8))) {
      DEBUG ((DEBUG_ERROR, "%a: Unable to obtain Recovery Url\n", __FUNCTION__));
      goto NO_HTTP_RECOVERY;
    }

    Status = DfciGetASetting (
               DFCI_PRIVATE_SETTING_ID__DFCI_HTTPS_CERT,
               DFCI_SETTING_TYPE_CERT,
               (VOID **)&mDfciNetworkRequest.HttpsCert,
               &mDfciNetworkRequest.HttpsCertSize
               );
    if (EFI_ERROR (Status) || (mDfciNetworkRequest.HttpsCertSize <= sizeof (CHAR8))) {
      DEBUG ((DEBUG_ERROR, "%a: Unable to obtain Https Certificate\n", __FUNCTION__));
      goto NO_HTTP_RECOVERY;
    }

    Status = DfciGetASetting (
               DFCI_PRIVATE_SETTING_ID__DFCI_TENANT_ID,
               DFCI_SETTING_TYPE_STRING,
               (VOID **)&mDfciNetworkRequest.TenantId,
               &mDfciNetworkRequest.TenantIdSize
               );
    if (EFI_ERROR (Status) || (mDfciNetworkRequest.TenantIdSize <= sizeof (CHAR8))) {
      DEBUG ((DEBUG_ERROR, "%a: Unable to obtain TenantId\n", __FUNCTION__));
      goto NO_HTTP_RECOVERY;
    }

    Status = DfciGetASetting (
               DFCI_PRIVATE_SETTING_ID__DFCI_REGISTRATION_ID,
               DFCI_SETTING_TYPE_STRING,
               (VOID **)&mDfciNetworkRequest.RegistrationId,
               &mDfciNetworkRequest.RegistrationIdSize
               );
    if (EFI_ERROR (Status) || (mDfciNetworkRequest.RegistrationIdSize <= sizeof (CHAR8))) {
      DEBUG ((DEBUG_ERROR, "%a: Unable to obtain RegistrationId\n", __FUNCTION__));
      goto NO_HTTP_RECOVERY;
    }

    Status =  mAuthenticationProtocol->GetCertInfo (
                                         mAuthenticationProtocol,
                                         0,
                                         mDfciNetworkRequest.HttpsCert,
                                         mDfciNetworkRequest.HttpsCertSize,
                                         DFCI_CERT_THUMBPRINT,
                                         DFCI_CERT_FORMAT_CHAR8,
                                         (VOID **)&mDfciNetworkRequest.HttpsThumbprint,
                                         &mDfciNetworkRequest.HttpsThumbprintSize
                                         );
    if (EFI_ERROR (Status) || (mDfciNetworkRequest.HttpsThumbprint == NULL)) {
      DEBUG ((DEBUG_ERROR, "Error getting Https certificate info. Status = %r\n", Status));
      goto NO_HTTP_RECOVERY;
    }

    Status = HttpParseUrl (mDfciUrl, (UINT32)mDfciUrlSize, FALSE, &Parser);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a: Unable to parse host Url\n", __FUNCTION__));
      goto NO_HTTP_RECOVERY;
    }

    Status = HttpUrlGetHostName (mDfciUrl, Parser, &HostName);
    if (EFI_ERROR (Status)) {
      FreePool (Parser);
      DEBUG ((DEBUG_ERROR, "%a: Unable to parse host Url\n", __FUNCTION__));
      goto NO_HTTP_RECOVERY;
    }

    FreePool (Parser);
    FreePool (HostName);
    mDfciMenuConfiguration.DfciHttpRecoveryEnabled = MENU_TRUE;
    DEBUG ((DEBUG_INFO, "Dfci Http Recovery is enabled\n"));

NO_HTTP_RECOVERY:

    Status = DfciGetASetting (
               DFCI_PRIVATE_SETTING_ID__MDM_FRIENDLY_NAME,
               DFCI_SETTING_TYPE_STRING,
               (VOID **)&Name,
               &NameSize
               );
    if (!EFI_ERROR (Status) && (NameSize >= 1)) {
      mDfciMenuConfiguration.DfciFriendlyName = MENU_TRUE;
      DEBUG ((DEBUG_INFO, "Dfci MDM.FriendlyName is enabled\n"));
    }

    Status = DfciGetASetting (
               DFCI_PRIVATE_SETTING_ID__MDM_TENANT_NAME,
               DFCI_SETTING_TYPE_STRING,
               (VOID **)&Name,
               &NameSize
               );
    if (!EFI_ERROR (Status) && (NameSize >= 1)) {
      mDfciMenuConfiguration.DfciTenantName = MENU_TRUE;
      DEBUG ((DEBUG_INFO, "Dfci MDM.Tenant is enabled\n"));
    }
  }

  return EFI_SUCCESS;
}

/**
 * Display Message Box - Displays a message box with the status of the Dfci Request. If the
 *                       Dfci request appears normal, allow a restart to apply the new settings
 *
 * @param StatusIn       What kind of failure
 * @param Restart        Display the Restart Now button
 * @param MessageText    Used when a specific message is required
 *
 * @return EFI_STATUS
 */
STATIC
EFI_STATUS
DisplayMessageBox (
  EFI_STRING_ID  MsgToken,
  IN EFI_STATUS  StatusIn,
  IN BOOLEAN     Restart,
  IN CHAR16      *MessageText  OPTIONAL
  )
{
  EFI_STRING  pTitle;
  EFI_STRING  pCaption;
  EFI_STRING  pBody;
  EFI_STRING  pTmp;

  pTitle = HiiGetString (mDfciMenuPrivate.HiiHandle, STRING_TOKEN (STR_DFCI_MB_TITLE), NULL);

  if (EFI_SUCCESS == StatusIn) {
    pCaption = HiiGetString (mDfciMenuPrivate.HiiHandle, STRING_TOKEN (STR_DFCI_MB_CAPTION), NULL);
  } else {
    pCaption = HiiGetString (mDfciMenuPrivate.HiiHandle, STRING_TOKEN (STR_DFCI_MB_CAPTION_FAIL), NULL);
  }

  switch (StatusIn) {
    case EFI_NOT_FOUND:
      pBody = HiiGetString (mDfciMenuPrivate.HiiHandle, STRING_TOKEN (STR_DFCI_MB_NOT_FOUND), NULL);
      if ((NULL != pBody) && (NULL != MessageText)) {
        pTmp = AllocatePool (MAX_MSG_SIZE);
        if (NULL != pTmp) {
          UnicodeSPrint (pTmp, MAX_MSG_SIZE, pBody, MessageText);
          FreePool (pBody);
          pBody = pTmp;
        }
      }

      break;

    case EFI_NO_MEDIA:
      pBody = HiiGetString (mDfciMenuPrivate.HiiHandle, STRING_TOKEN (STR_DFCI_MB_NO_MEDIA), NULL);
      break;

    default:
      pBody = HiiGetString (mDfciMenuPrivate.HiiHandle, MsgToken, NULL);
      if ((NULL != pBody) && (NULL != MessageText)) {
        pTmp = AllocatePool (MAX_MSG_SIZE);
        if (NULL != pTmp) {
          UnicodeSPrint (pTmp, MAX_MSG_SIZE, pBody, MessageText);
          FreePool (pBody);
          pBody = pTmp;
        }
      }

      break;
  }

  if ((NULL == pTitle) || (NULL == pCaption) || (NULL == pBody)) {
    DEBUG ((DEBUG_ERROR, "Invalid message parameters. pTitle=%p, pCaption=%p, pBody=%p\n", pTitle, pCaption, pBody));
  }

  DEBUG ((DEBUG_ERROR, "pTitle   = %s\n", pTitle));
  DEBUG ((DEBUG_ERROR, "pCaption = %s\n", pCaption));
  DEBUG ((DEBUG_ERROR, "pBody    = %s\n", pBody));

  if (NULL != pTitle) {
    FreePool (pTitle);
  }

  if (NULL != pCaption) {
    FreePool (pCaption);
  }

  if (NULL != pBody) {
    FreePool (pBody);
  }

  return EFI_SUCCESS;
}

/**
 * Issue DfciRequest to the network
 *
 * @param   NONE
 *
 * @return -- This routine never returns to the caller
 */
STATIC
VOID
IssueDfciNetworkRequest (
  VOID
  )
{
  //
  //  TODO: Verify requirement for on prem network
  //

  BOOLEAN     OnPrem = FALSE;
  EFI_STATUS  NetworkStatus;
  CHAR16      *Msg = NULL;

  if (OnPrem) {
    NetworkStatus = ProcessSimpleNetworkRequest (&mDfciNetworkRequest, &Msg);
  } else {
    NetworkStatus = ProcessDfciNetworkRequest (&mDfciNetworkRequest, &Msg);
  }

  // Success also includes
  if (EFI_MEDIA_CHANGED == NetworkStatus) {
    NetworkStatus = EFI_SUCCESS;
  }

  //
  // Inform user that operation is complete - then restart the system to return to the trusted code
  //
  DisplayMessageBox (STRING_TOKEN (STR_DFCI_MB_NEW_SETTINGS), NetworkStatus, TRUE, Msg);
}

/**
*  This function is the main entry of the Dfci Menu application.
*
* @param[in]   ImageHandle
* @param[in]   SystemTable
*
**/
EFI_STATUS
EFIAPI
DfciSARecoveryEntry (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS      Status;
  EFI_HII_HANDLE  *DfciHandles;

  AsciiPrint ("DfciSARecovery V0.1\n");

  DfciHandles = HiiGetHiiHandles (&gDfciMenuFormsetGuid);
  if (DfciHandles == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: Unable to locate Dfci Menu.\n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }

  mDfciMenuPrivate.HiiHandle = DfciHandles[0];
  FreePool (DfciHandles);

  if (NULL == mDfciMenuPrivate.HiiHandle) {
    DEBUG ((DEBUG_ERROR, "Unable to locate DfciMenu\n"));
    return EFI_INVALID_PARAMETER;
  }

  Status = gBS->LocateProtocol (
                  &gDfciAuthenticationProtocolGuid,
                  NULL,
                  (VOID **)&mAuthenticationProtocol
                  );
  if (EFI_ERROR (Status) || (mAuthenticationProtocol == NULL)) {
    DEBUG ((DEBUG_ERROR, "%a -  DfciAuthentication protocol not available. %r\n", __FUNCTION__, Status));
    ASSERT (FALSE);     // Fatal error - There is a Depex for this protocol
    return FALSE;
  }

  // Get all Id's that have Dfci Recovery permission.
  Status = gBS->LocateProtocol (
                  &gDfciSettingPermissionsProtocolGuid,
                  NULL,
                  (VOID **)&mDfciSettingsPermissionProtocol
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a - DfciSettingPermissionsProtocolGuid not available. %r\n", __FUNCTION__, Status));
    ASSERT (FALSE);    // Fatal error - again, there is a Depex for this protocol
    return FALSE;
  }

  Status = GetDfciParameters ();

  if (!CheckIfDfciEnrolled ()) {
    // Check if system is managed by DFCI
    DEBUG ((DEBUG_INFO, "%a - Error getting Cert Information.\n", __FUNCTION__));
    return EFI_SUCCESS;
  }

  if (mDfciMenuConfiguration.DfciHttpRecoveryEnabled == MENU_TRUE) {
    DEBUG ((DEBUG_INFO, "%a - Processing Network Request.\n", __FUNCTION__));
    IssueDfciNetworkRequest ();
  } else {
    DEBUG ((DEBUG_ERROR, "%a - Unable to attempt network request.\n", __FUNCTION__));
  }

  // Right now, this is a driver due to the libraries used.  So, never load.
  return EFI_NOT_FOUND;
}
