/** @file
 *BootManger - Menu to select and manage boot options.

  Copyright (C) Microsoft Corporation. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>

#include "BootMenu.h"

#include <Uefi/UefiInternalFormRepresentation.h>
#include <Uefi/MsUefiInternalFormRepresentationEx.h>

#include <Guid/MdeModuleHii.h>
#include <Guid/GlobalVariable.h>

#include <DfciSystemSettingTypes.h>

#include <Protocol/HiiConfigAccess.h>
#include <Protocol/DevicePath.h>
#include <Protocol/FormBrowserEx2.h>
#include <Protocol/SimpleWindowManager.h>
#include <Protocol/DfciSettingAccess.h>
#include <Protocol/MsFrontPageAuthTokenProtocol.h>

#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DevicePathLib.h>
#include <Library/HiiLib.h>
#include <Library/MsBootOptionsLib.h>
#include <Library/PrintLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootManagerLib.h>
#include <Library/UefiHiiServicesLib.h>
#include <Library/BootGraphicsProviderLib.h>
#include <Library/BootGraphicsLib.h>
#include <Library/GraphicsConsoleHelperLib.h>
#include <Library/SwmDialogsLib.h>

#include <Settings/BootMenuSettings.h>

#define BOOT_MENU_SIGNATURE  SIGNATURE_32 ('u', 'n', 'm', 'B')

#define MAX_MSG_SIZE_CAPTION  100
#define MAX_MSG_SIZE_WARNING  200

#pragma pack(1)
///
/// HII specific Vendor Device Path definition.
///
typedef struct {
  VENDOR_DEVICE_PATH          VendorDevicePath;
  EFI_DEVICE_PATH_PROTOCOL    End;
} HII_VENDOR_DEVICE_PATH;

#pragma pack()

// Global variables
EFI_BOOT_MANAGER_LOAD_OPTION  *mBootOptions           = NULL;
UINTN                         mBootOptionCount        = 0;
EFI_BOOT_MANAGER_LOAD_OPTION  *mDefaultLoadOptions    = NULL;
UINTN                         mDefaultLoadOptionCount = 0;

// VarStore for each of the section in the VFR
ORDER_MENU_CONFIGURATION                mOrderConfiguration;
SETTINGS_MENU_CONFIGURATION             mSettingsConfiguration;
SETTINGS_GRAYOUT_CONFIGURATION          mSettingsGrayoutConfiguration;
SETTINGS_SUPPRESS_CONFIGURATION         mSettingsSuppressConfiguration;
BOOLEAN                                 mForcingExit = FALSE;
MS_SIMPLE_WINDOW_MANAGER_PROTOCOL       *mSWMProtocol;
EFI_EVENT                               mSWMRegisterEvent;
VOID                                    *mSWMRegistration;
EFI_EVENT                               mSettingAccessRegisterEvent;
VOID                                    *mSettingAccessRegistration;
DFCI_AUTH_TOKEN                         mAuthToken;
EDKII_FORM_BROWSER_EXTENSION2_PROTOCOL  *mBrowserEx2;
EFI_EVENT                               mAuthTokenRegisterEvent;
VOID                                    *mAuthTokenRegistration;
FRONT_PAGE_AUTH_TOKEN_PROTOCOL          *mAuthTokenProtocol;

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

#define mBootOptionLimit  ((UINT8) MIN (mBootOptionCount, MAX_BOOT_OPTIONS_SUPPORTED))

EFI_STATUS
EFIAPI
ExtractConfig (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL  *This,
  IN  CONST EFI_STRING                      Request,
  OUT EFI_STRING                            *Progress,
  OUT EFI_STRING                            *Results
  );

EFI_STATUS
EFIAPI
RouteConfig (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL  *This,
  IN  CONST EFI_STRING                      Configuration,
  OUT EFI_STRING                            *Progress
  );

EFI_STATUS
EFIAPI
DriverCallback (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL  *This,
  IN  EFI_BROWSER_ACTION                    Action,
  IN  EFI_QUESTION_ID                       QuestionId,
  IN  UINT8                                 Type,
  IN  EFI_IFR_TYPE_VALUE                    *Value,
  OUT EFI_BROWSER_ACTION_REQUEST            *ActionRequest
  );

typedef struct {
  UINTN                             Signature;
  EFI_HANDLE                        DriverHandle;
  EFI_HII_HANDLE                    HiiHandle;
  EFI_HII_CONFIG_ACCESS_PROTOCOL    ConfigAccess;
} BOOT_MENU_PRIVATE;

BOOT_MENU_PRIVATE  mBootMenuPrivate = {
  BOOT_MENU_SIGNATURE,
  NULL,
  NULL,
  {
    ExtractConfig,
    RouteConfig,
    DriverCallback
  }
};

/**
  SWM registration notification callback

  @param[in] Event      Event that signalled the callback.
  @param[in] Context    Pointer to an optional event contxt.

  @retval None.

**/
VOID
EFIAPI
SWMRegisteredCallback (
  IN  EFI_EVENT  Event,
  IN  VOID       *Context
  )
{
  EFI_STATUS  Status;

  Status = gBS->LocateProtocol (
                  &gMsSWMProtocolGuid,
                  mSWMRegistration,
                  (VOID **)&mSWMProtocol
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Unable to locate SWM. Code=%r\n", Status));
  }

  return;
}

/**
Auth Token registration notification callback

@param[in] Event      Event that signalled the callback.
@param[in] Context    Pointer to an optional event contxt.

@retval None.

**/
VOID
EFIAPI
AuthTokenCallback (
  IN  EFI_EVENT  Event,
  IN  VOID       *Context
  )
{
  EFI_STATUS  Status;

  Status = gBS->LocateProtocol (
                  &gMsFrontPageAuthTokenProtocolGuid,
                  mAuthTokenRegistration,
                  (VOID **)&mAuthTokenProtocol
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Unable to locate FrontPageAuthTokenProtocol in Boot Menu application. Code=%r\n", Status));
  } else {
    mAuthToken = mAuthTokenProtocol->AuthToken;
    DEBUG ((DEBUG_INFO, "AuthToken value in Bootmenu %x\n", mAuthToken));
  }

  return;
}

BOOLEAN
IsDefaultBootOption (
  EFI_BOOT_MANAGER_LOAD_OPTION  *BootOption
  )
{
  UINT32   CurrentAttributes;
  BOOLEAN  IsDefault;

  mDefaultLoadOptions = MsBootOptionsLibGetDefaultOptions (&mDefaultLoadOptionCount);
  if (NULL == mDefaultLoadOptions) {
    DEBUG ((DEBUG_ERROR, "%a Error obtaining default boot options\n", __FUNCTION__));
    return FALSE;
  }

  // The actual boot option may have LOAD_OPTION_ACTIVE set or off.  Check for both.
  CurrentAttributes       = BootOption->Attributes;
  BootOption->Attributes |= LOAD_OPTION_ACTIVE;
  IsDefault               = (-1 != EfiBootManagerFindLoadOption (BootOption, mDefaultLoadOptions, mDefaultLoadOptionCount));
  if (!IsDefault) {
    BootOption->Attributes &= ~LOAD_OPTION_ACTIVE;
    IsDefault               = (-1 != EfiBootManagerFindLoadOption (BootOption, mDefaultLoadOptions, mDefaultLoadOptionCount));
  }

  // Restore attributes
  BootOption->Attributes = CurrentAttributes;

  EfiBootManagerFreeLoadOptions (mDefaultLoadOptions, mDefaultLoadOptionCount);

  return IsDefault;
}

/**
 *This function rebuilds the list of boot options for the menu.

**/
VOID
RebuildOrderList (
  VOID
  )
{
  EFI_STATUS          Status;
  VOID                *StartOpCodeHandle;
  VOID                *EndOpCodeHandle;
  VOID                *OptionsOpCodeHandle;
  EFI_IFR_GUID_LABEL  *StartLabel;
  EFI_IFR_GUID_LABEL  *EndLabel;
  EFI_STRING_ID       Prompt;
  UINTN               Index;
  UINT32              OptionValue;
  UINT8               *OpcodeBuffer;

  if (mBootOptions != NULL) {
    EfiBootManagerFreeLoadOptions (mBootOptions, mBootOptionCount);
    mBootOptions = NULL;
  }

  ZeroMem (&mOrderConfiguration.OrderOptions, sizeof (mOrderConfiguration.OrderOptions));

  //
  // Init OpCode Handle and Allocate space for creation of UpdateData Buffer
  //
  StartOpCodeHandle = HiiAllocateOpCodeHandle ();
  ASSERT (StartOpCodeHandle != NULL);

  EndOpCodeHandle = HiiAllocateOpCodeHandle ();
  ASSERT (EndOpCodeHandle != NULL);

  OptionsOpCodeHandle = HiiAllocateOpCodeHandle ();
  ASSERT (OptionsOpCodeHandle != NULL);

  //
  // Create Hii Extend Label OpCode as the start opcode
  //
  StartLabel               = (EFI_IFR_GUID_LABEL *)HiiCreateGuidOpCode (StartOpCodeHandle, &gEfiIfrTianoGuid, NULL, sizeof (EFI_IFR_GUID_LABEL));
  StartLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;

  //
  // Create Hii Extend Label OpCode as the end opcode
  //
  EndLabel               = (EFI_IFR_GUID_LABEL *)HiiCreateGuidOpCode (EndOpCodeHandle, &gEfiIfrTianoGuid, NULL, sizeof (EFI_IFR_GUID_LABEL));
  EndLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;

  StartLabel->Number = LABEL_ORDER_OPTIONS;
  EndLabel->Number   = LABEL_ORDER_END;

  mBootOptions = EfiBootManagerGetLoadOptions (&mBootOptionCount, LoadOptionTypeBoot);

  for (Index = 0; Index < mBootOptionLimit; Index++) {
    //
    // Don't display the hidden/inactive boot options.
    //
    if (((mBootOptions[Index].Attributes & LOAD_OPTION_HIDDEN) != 0) ||
        ((mBootOptions[Index].Attributes & LOAD_OPTION_CATEGORY_APP) != 0))
    {
      continue;
    }

    ASSERT (mBootOptions[Index].Description != NULL);

    Prompt = HiiSetString (
               mBootMenuPrivate.HiiHandle,
               0,
               mBootOptions[Index].Description,
               NULL
               );

    DEBUG ((DEBUG_INFO, "%a Indx=%d, Hash=%x, Attr=%x, %s\n", __FUNCTION__, Index, mBootOptions[Index].OptionNumber, mBootOptions[Index].Attributes, mBootOptions[Index].Description));

    OptionValue = (UINT32)(mBootOptions[Index].OptionNumber + 1);
    if ((mBootOptions[Index].Attributes & LOAD_OPTION_ACTIVE) == LOAD_OPTION_ACTIVE) {
      OptionValue |= ORDERED_LIST_CHECKBOX_VALUE_32;
    }

    if (!IsDefaultBootOption (&mBootOptions[Index])) {
      OptionValue |= ORDERED_LIST_ALLOW_DELETE_VALUE_32;
    }

    OpcodeBuffer = HiiCreateOneOfOptionOpCode (
                     OptionsOpCodeHandle,
                     Prompt,
                     EFI_IFR_FLAG_CALLBACK,
                     EFI_IFR_TYPE_NUM_SIZE_32,
                     OptionValue
                     );
    ASSERT (OpcodeBuffer != NULL);
    mOrderConfiguration.OrderOptions[Index] = OptionValue;
  }

  OpcodeBuffer = HiiCreateOrderedListOpCode (
                   StartOpCodeHandle,
                   (EFI_QUESTION_ID)(MS_BOOT_ORDER_QUESTION_ID),
                   MS_BOOT_ORDER_VARID,
                   OFFSET_OF (ORDER_MENU_CONFIGURATION, OrderOptions),
                   STRING_TOKEN (STR_BOOT_ORDER_LIST),
                   0,
                   EFI_IFR_FLAG_CALLBACK,        // OPTIONS_ONLY is unused - means combo ListBox
                   EFI_IFR_UNIQUE_SET | EMBEDDED_CHECKBOX | EMBEDDED_DELETE,
                   EFI_IFR_NUMERIC_SIZE_4,
                   mBootOptionLimit,
                   OptionsOpCodeHandle,
                   NULL                        // Default Op Code is NULL
                   );
  ASSERT (OpcodeBuffer != NULL);

  Status = HiiUpdateForm (
             mBootMenuPrivate.HiiHandle,
             &gMsBootMenuFormsetGuid,
             MS_BOOT_ORDER_FORM_ID,
             StartOpCodeHandle,
             EndOpCodeHandle
             );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a Error in HiiUpdateform.  Code=%r\n", __FUNCTION__, Status));
  }

  if (StartOpCodeHandle != NULL) {
    HiiFreeOpCodeHandle (StartOpCodeHandle);
  }

  if (StartOpCodeHandle != NULL) {
    HiiFreeOpCodeHandle (EndOpCodeHandle);
  }

  if (OptionsOpCodeHandle != NULL) {
    HiiFreeOpCodeHandle (OptionsOpCodeHandle);
  }
}

/**
  This function is the main entry of the platform setup entry.
  The function will present the main menu of the system setup,
  this is the platform reference part and can be customize.
**/
EFI_STATUS
EFIAPI
BootMenuEntry (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status;

  // Locate the Simple Window Manager protocol.
  //

  mSWMProtocol = NULL;
  Status       = gBS->LocateProtocol (
                        &gMsSWMProtocolGuid,
                        NULL,
                        (VOID **)&mSWMProtocol
                        );

  if (EFI_ERROR (Status)) {
    Status = gBS->CreateEvent (
                    EVT_NOTIFY_SIGNAL,
                    TPL_CALLBACK,
                    SWMRegisteredCallback,
                    NULL,
                    &mSWMRegisterEvent
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "%a: Failed to create SWM registration event (%r).\r\n", __FUNCTION__, Status));
    } else {
      Status = gBS->RegisterProtocolNotify (
                      &gMsSWMProtocolGuid,
                      mSWMRegisterEvent,
                      &mSWMRegistration
                      );
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_INFO, "%a: Failed to register for SWM registration notifications (%r).\r\n", __FUNCTION__, Status));
      }
    }
  }

  // Auth token acquisition
  Status = gBS->LocateProtocol (
                  &gMsFrontPageAuthTokenProtocolGuid,
                  NULL,
                  (VOID **)&mAuthTokenProtocol
                  );

  if (EFI_ERROR (Status)) {
    Status = gBS->CreateEvent (
                    EVT_NOTIFY_SIGNAL,
                    TPL_CALLBACK,
                    AuthTokenCallback,
                    NULL,
                    &mAuthTokenRegisterEvent
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "%a: Failed to create SettingAccess registration event (%r).\r\n", __FUNCTION__, Status));
    } else {
      Status = gBS->RegisterProtocolNotify (
                      &gMsFrontPageAuthTokenProtocolGuid,
                      mAuthTokenRegisterEvent,
                      &mAuthTokenRegistration
                      );
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_INFO, "%a: Failed to register for SWM registration notifications (%r).\r\n", __FUNCTION__, Status));
      }
    }
  } else {
    // if you get lucky the first time
    mAuthToken = mAuthTokenProtocol->AuthToken;
    DEBUG ((DEBUG_INFO, "AuthToken value in Bootmenu %x\n", mAuthToken));
  }

  //
  // Install Device Path Protocol and Config Access protocol to driver handle
  //
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &mBootMenuPrivate.DriverHandle,
                  &gEfiDevicePathProtocolGuid,
                  &mHiiVendorDevicePath,
                  &gEfiHiiConfigAccessProtocolGuid,
                  &mBootMenuPrivate.ConfigAccess,
                  NULL
                  );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Error on InstallMultipleProtocol. Code=%r\n", __FUNCTION__, Status));
  }

  //
  // Publish our HII data
  //
  mBootMenuPrivate.HiiHandle = HiiAddPackages (
                                 &gMsBootMenuFormsetGuid,
                                 mBootMenuPrivate.DriverHandle,
                                 BootMenuVfrBin,
                                 BootMenuStrings,
                                 NULL
                                 );
  if (mBootMenuPrivate.HiiHandle == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: Error on HiiAddPackages. Code=%r\n", __FUNCTION__, Status));
  } else {
    Status = gBS->LocateProtocol (&gEdkiiFormBrowserEx2ProtocolGuid, NULL, (VOID **)&mBrowserEx2);
    ASSERT_EFI_ERROR (Status);
  }

  return EFI_SUCCESS;
}

/**
  This function processes the results of changes in configuration.

  @param  This                   Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param  Action                 Specifies the type of action taken by the browser.
  @param  QuestionId             A unique value which is sent to the original
                                 exporting driver so that it can identify the type
                                 of data to expect.
  @param  Type                   The type of value for the question.
  @param  Value                  A pointer to the data being sent to the original
                                 exporting driver.
  @param  ActionRequest          On return, points to the action requested by the
                                 callback function.

  @retval EFI_SUCCESS            The callback successfully handled the action.
  @retval EFI_OUT_OF_RESOURCES   Not enough storage is available to hold the
                                 variable and its data.
  @retval EFI_DEVICE_ERROR       The variable could not be saved.
  @retval EFI_UNSUPPORTED        The specified Action is not supported by the
                                 callback.

**/
EFI_STATUS
EFIAPI
DriverCallback (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL  *This,
  IN  EFI_BROWSER_ACTION                    Action,
  IN  EFI_QUESTION_ID                       QuestionId,
  IN  UINT8                                 Type,
  IN  EFI_IFR_TYPE_VALUE                    *Value,
  OUT EFI_BROWSER_ACTION_REQUEST            *ActionRequest
  )
{
  EFI_STATUS  Status;
  UINT32      *BootOrder;
  UINTN       Index;

  DEBUG ((DEBUG_INFO, "*HII* - Action=%d, Question=%d, Type=%d, Value=%d\n", Action, QuestionId, Type, Value->u64));

  *ActionRequest = EFI_BROWSER_ACTION_REQUEST_NONE;

  Status = EFI_UNSUPPORTED;

  switch (Action) {
    case EFI_BROWSER_ACTION_FORM_OPEN:
      switch (QuestionId) {
        case MS_BOOT_ORDER_INIT_KEY:
          RebuildOrderList ();
          Status = EFI_SUCCESS;
          break;
        default:
          break;
      }

      break;

    case EFI_BROWSER_ACTION_FORM_CLOSE:
      if (mForcingExit) {
        mForcingExit = FALSE;
        mBrowserEx2->SetScope (SystemLevel);
        mBrowserEx2->ExecuteAction (BROWSER_ACTION_EXIT, 0);      // Tell browser to Exit completely to follow the boot next action
        Status = EFI_SUCCESS;
        SetGraphicsConsoleMode (GCM_NATIVE_RES);
        DisplayBootGraphic (BG_SYSTEM_LOGO);
      }

      break;

    case EFI_BROWSER_ACTION_CHANGED:
      switch (QuestionId) {
        case MS_BOOT_ORDER_QUESTION_ID:
          // Ordered ListBox - value points to an array of N U32's(element size), May be terminated by a value of 0.
          if (Type == EFI_IFR_TYPE_BUFFER) {
            BootOrder = (UINT32 *)Value;
            for (Index = 0; Index < mBootOptionLimit; Index++) {
              if (*BootOrder == 0) {
                break;
              }

              DEBUG ((DEBUG_INFO, "%a Order is %4.4x\n", __FUNCTION__, *BootOrder - 1));
              BootOrder++;
            }
          }

          *ActionRequest = EFI_BROWSER_ACTION_REQUEST_FORM_APPLY;
          Status         = EFI_SUCCESS;
          break;

        case MS_ENABLE_IPV6_QUESTION_ID:
        case MS_ENABLE_ALT_BOOT_QUESTION_ID:
        case MS_ENABLE_BOOT_ORDER_LOCK_QUESTION_ID:
        case MS_ENABLE_USB_BOOT_QUESTION_ID:
          *ActionRequest = EFI_BROWSER_ACTION_REQUEST_FORM_APPLY;
          Status         = EFI_SUCCESS;
          break;

        default:
          break;
      }

    default:
      break;
  }

  return Status;
}

/**
  This function processes the results of changes in configuration.

  @param  This                   Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param  Configuration          A null-terminated Unicode string in <ConfigResp>
                                 format.
  @param  Progress               A pointer to a string filled in with the offset of
                                 the most recent '&' before the first failing
                                 name/value pair (or the beginning of the string if
                                 the failure is in the first name/value pair) or
                                 the terminating NULL if all was successful.

  @retval EFI_SUCCESS            The Results is processed successfully.
  @retval EFI_INVALID_PARAMETER  Configuration is NULL.
  @retval EFI_NOT_FOUND          Routing data doesn't match any storage in this
                                 driver.

**/
EFI_STATUS
EFIAPI
RouteConfig (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL  *This,
  IN  CONST EFI_STRING                      Configuration,
  OUT EFI_STRING                            *Progress
  )
{
  UINTN                            BufferSize;
  EFI_STATUS                       Status;
  UINTN                            Index;
  UINTN                            Index2;
  UINTN                            NewBootOrderCount;
  UINT16                           *NewBootOrder;
  UINT16                           ThisOption;
  BOOLEAN                          PrevActive;
  BOOLEAN                          ThisActive;
  EFI_STRING                       pCaption;
  EFI_STRING                       pTempCaption;
  EFI_STRING                       pConfirm;
  EFI_STRING                       pTempConfirm;
  EFI_STRING                       pMsgBox;
  EFI_STRING                       pTitle;
  SWM_MB_RESULT                    SwmResult = 0;
  CHAR16                           OptionName[sizeof ("Boot####")];
  BOOLEAN                          AllowSetBootorder = TRUE;
  BOOLEAN                          MsBootNext;

  DEBUG ((DEBUG_INFO, "%a - Configuration=%s\n", __FUNCTION__, Configuration));
  DEBUG ((DEBUG_INFO, "%s", Configuration));
  DEBUG ((DEBUG_INFO, "\n"));

  if ((Configuration == NULL) || (Progress == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  if (Configuration == NULL) {
    return EFI_UNSUPPORTED;
  }

  if (StrStr (Configuration, L"OFFSET") == NULL) {
    return EFI_UNSUPPORTED;
  }

  if (HiiIsConfigHdrMatch (Configuration, &gMsBootMenuFormsetGuid, L"BootOrderConfig")) {
    BufferSize = sizeof (mOrderConfiguration);

    Status = gHiiConfigRouting->ConfigToBlock (
                                  gHiiConfigRouting,
                                  Configuration,
                                  (UINT8 *)&mOrderConfiguration,
                                  &BufferSize,
                                  Progress
                                  );

    if (!EFI_ERROR (Status)) {
      NewBootOrderCount = 0;
      NewBootOrder      = (UINT16 *)AllocateZeroPool (sizeof (UINT16) * mBootOptionCount);
      ASSERT (NewBootOrder != NULL);

      if (NULL == NewBootOrder) {
        return EFI_UNSUPPORTED;
      }

      for (Index = 0; Index < mBootOptionCount; Index++) {
        if (Index < mBootOptionLimit) {
          if (0 != mOrderConfiguration.OrderOptions[Index]) {
            ThisOption = (UINT16)(mOrderConfiguration.OrderOptions[Index] - 1);
            if (mOrderConfiguration.OrderOptions[Index] & ORDERED_LIST_BOOT_VALUE_32) {
              pTitle   = HiiGetString (mBootMenuPrivate.HiiHandle, STRING_TOKEN (STR_BOOT_BOOT_TITLE), NULL);
              pCaption = HiiGetString (mBootMenuPrivate.HiiHandle, STRING_TOKEN (STR_BOOT_BOOT_CAPTION), NULL);
              pConfirm = HiiGetString (mBootMenuPrivate.HiiHandle, STRING_TOKEN (STR_BOOT_BOOT_WARNING), NULL);

              pMsgBox = pConfirm;
              if (NULL == pMsgBox) {
                // Just in case HiiMsg is not available
                pMsgBox = L"Are you sure you want to boot %s?";
              }

              pTempCaption = AllocatePool (MAX_MSG_SIZE_CAPTION);
              if ((NULL != pTempCaption) && (NULL != pCaption)) {
                UnicodeSPrint (pTempCaption, MAX_MSG_SIZE_CAPTION, pCaption, mBootOptions[Index].Description);
              }

              pTempConfirm = AllocatePool (MAX_MSG_SIZE_WARNING);
              if ((NULL != pTempConfirm) && (NULL != pConfirm)) {
                UnicodeSPrint (pTempConfirm, MAX_MSG_SIZE_WARNING, pMsgBox, mBootOptions[Index].Description);
              }

              SwmResult = SWM_MB_IDCANCEL;
              if (NULL != mSWMProtocol) {
                // Ignore delete when SWM not found.
                Status = SwmDialogsMessageBox (
                            pTitle,
                            pTempConfirm,                                          // Dialog body text.
                            pTempCaption,                                          // Dialog caption text.
                            SWM_MB_OKCANCEL,                                       // Show OK and CANCEL buttons.
                            0,                                                     // No timeout
                            &SwmResult
                            );                                                     // Return result.
              }

              if (NULL != pCaption) {
                FreePool (pCaption);
              }

              if (NULL != pConfirm) {
                FreePool (pConfirm);
              }

              if (NULL != pTempCaption) {
                FreePool (pTempCaption);
              }

              if (NULL != pTempConfirm) {
                FreePool (pTempConfirm);
              }

              if (SWM_MB_IDOK == SwmResult) {
                Status = gRT->SetVariable (
                                L"BootNext",
                                &gEfiGlobalVariableGuid,
                                EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                                sizeof (ThisOption),
                                &ThisOption
                                );
                if (EFI_ERROR (Status)) {
                  DEBUG ((DEBUG_ERROR, "%a: Error setting BootNext. Code=%r\n", __FUNCTION__, Status));
                } else {
                  MsBootNext = TRUE;
                  Status     = gRT->SetVariable (
                                      L"MsBootNext",
                                      &gMsBootMenuFormsetGuid,
                                      EFI_VARIABLE_BOOTSERVICE_ACCESS,
                                      sizeof (MsBootNext),
                                      &MsBootNext
                                      );
                  DEBUG ((DEBUG_INFO, "%a BootNext set to BOOT%4.4x\n", __FUNCTION__, ThisOption));
                }

                mBrowserEx2->SetScope (SystemLevel);
                mBrowserEx2->ExecuteAction (BROWSER_ACTION_EXIT, 0);                    // Tell browser to Exit completely to follow the boot next action
                Status = EFI_SUCCESS;
                SetGraphicsConsoleMode (GCM_NATIVE_RES);
                DisplayBootGraphic (BG_SYSTEM_LOGO);
                return EFI_SUCCESS;                    // On a boot request, return immediately
              }

              goto Exit1;                   // Terminate processing, and don't update the boot order
            } else {
              NewBootOrder[NewBootOrderCount++] = ThisOption;
              for (Index2 = 0; Index2 < mBootOptionLimit; Index2++) {
                if (ThisOption == mBootOptions[Index2].OptionNumber) {
                  PrevActive = ((mBootOptions[Index2].Attributes & LOAD_OPTION_ACTIVE) == LOAD_OPTION_ACTIVE);
                  ThisActive = ((mOrderConfiguration.OrderOptions[Index] & ORDERED_LIST_CHECKBOX_VALUE_32) != 0);
                  if (ThisActive != PrevActive) {
                    mBootOptions[Index2].Attributes ^= LOAD_OPTION_ACTIVE;
                    EfiBootManagerLoadOptionToVariable (&mBootOptions[Index2]);
                  }

                  break;
                }
              }
            }
          }
        } else {
          NewBootOrder[NewBootOrderCount++] = (UINT16)mBootOptions[Index].OptionNumber;
        }
      }

      // Any deleted options?
      if (mBootOptionCount != NewBootOrderCount) {
        // On the delete path, only allow setting the boot order if confirmation is YES.
        AllowSetBootorder = FALSE;

        // First, mark entries as Unassigned that are in the new BootOrder list
        for (Index = 0; Index < mBootOptionLimit; Index++) {
          for (Index2 = 0; Index2 < NewBootOrderCount; Index2++) {
            if (mBootOptions[Index].OptionNumber == NewBootOrder[Index2]) {
              mBootOptions[Index].OptionNumber = LoadOptionNumberUnassigned;
              break;
            }
          }
        }

        // Then, delete every boot option with an OptionNumber that is left
        for (Index = 0; Index < mBootOptionLimit; Index++) {
          if (mBootOptions[Index].OptionNumber != LoadOptionNumberUnassigned) {
            pTitle   = HiiGetString (mBootMenuPrivate.HiiHandle, STRING_TOKEN (STR_BOOT_DELETE_TITLE), NULL);
            pCaption = HiiGetString (mBootMenuPrivate.HiiHandle, STRING_TOKEN (STR_BOOT_DELETE_CAPTION), NULL);
            pConfirm = HiiGetString (mBootMenuPrivate.HiiHandle, STRING_TOKEN (STR_BOOT_DELETE_WARNING), NULL);

            pMsgBox = pConfirm;
            if (NULL == pMsgBox) {
              // Just in case HiiMsg is not available
              pMsgBox = L"Are you sure you want to delete %s?";
            }

            pTempCaption = AllocatePool (MAX_MSG_SIZE_CAPTION);
            if ((NULL != pTempCaption) && (NULL != pCaption)) {
              UnicodeSPrint (pTempCaption, MAX_MSG_SIZE_CAPTION, pCaption, mBootOptions[Index].Description);
            }

            pTempConfirm = AllocatePool (MAX_MSG_SIZE_WARNING);
            if ((NULL != pTempConfirm) && (NULL != pConfirm)) {
              UnicodeSPrint (pTempConfirm, MAX_MSG_SIZE_WARNING, pMsgBox, mBootOptions[Index].Description);
            }

            SwmResult = SWM_MB_IDCANCEL;
            if (NULL != mSWMProtocol) {
              // Ignore delete when SWM not found.
              Status = SwmDialogsMessageBox (
                          pTitle,
                          pTempConfirm,                                        // Dialog body text.
                          pTempCaption,                                        // Dialog caption text.
                          SWM_MB_OKCANCEL,                                     // Show Yes and No buttons.
                          0,                                                   // No timeout
                          &SwmResult
                          );                                                   // Return result.
            }

            if (SWM_MB_IDOK == SwmResult) {
              AllowSetBootorder = TRUE;
              EfiBootManagerDeleteLoadOptionVariable (mBootOptions[Index].OptionNumber, LoadOptionTypeBoot);
              UnicodeSPrint (OptionName, sizeof (OptionName), L"Boot%04x", mBootOptions[Index].OptionNumber);
              Status = gRT->SetVariable (
                              OptionName,
                              &gEfiGlobalVariableGuid,
                              EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                              0,
                              NULL
                              );
              if (EFI_ERROR (Status)) {
                DEBUG ((DEBUG_ERROR, "%a Error deleting %s. Code=%r\n", __FUNCTION__, OptionName, Status));
              } else {
                DEBUG ((DEBUG_INFO, "%a Variable %s deleted. Code=%r\n", __FUNCTION__, OptionName, Status));
              }
            }

            if (NULL != pCaption) {
              FreePool (pCaption);
            }

            if (NULL != pConfirm) {
              FreePool (pConfirm);
            }

            if (NULL != pTempCaption) {
              FreePool (pTempCaption);
            }

            if (NULL != pTempConfirm) {
              FreePool (pTempConfirm);
            }
          }
        }
      }

      if (AllowSetBootorder) {
        Status = gRT->SetVariable (
                        L"BootOrder",
                        &gEfiGlobalVariableGuid,
                        EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                        sizeof (UINT16) * NewBootOrderCount,
                        NewBootOrder
                        );
      }

Exit1:
      FreePool (NewBootOrder);
      RebuildOrderList ();
    }

    DEBUG ((DEBUG_INFO, "%a Size is %d, Requested is %d. Code=%r\n", __FUNCTION__, sizeof (mOrderConfiguration), BufferSize, Status));
  }

  DEBUG ((DEBUG_INFO, "%a complete. Code = %r\n", __FUNCTION__, Status));
  return Status;
}

/**
  This function allows a caller to extract the current configuration for one
  or more named elements from the target driver.

  @param  This                   Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param  Request                A null-terminated Unicode string in
                                 <ConfigRequest> format.
  @param  Progress               On return, points to a character in the Request
                                 string. Points to the string's null terminator if
                                 request was successful. Points to the most recent
                                 '&' before the first failing name/value pair (or
                                 the beginning of the string if the failure is in
                                 the first name/value pair) if the request was not
                                 successful.
  @param  Results                A null-terminated Unicode string in
                                 <ConfigAltResp> format which has all values filled
                                 in for the names in the Request string. String to
                                 be allocated by the called function.

  @retval EFI_SUCCESS            The Results is filled with the requested values.
  @retval EFI_OUT_OF_RESOURCES   Not enough memory to store the results.
  @retval EFI_INVALID_PARAMETER  Request is illegal syntax, or unknown name.
  @retval EFI_NOT_FOUND          Routing data doesn't match any storage in this
                                 driver.

**/
EFI_STATUS
EFIAPI
ExtractConfig (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL  *This,
  IN  CONST EFI_STRING                      Request,
  OUT EFI_STRING                            *Progress,
  OUT EFI_STRING                            *Results
  )
{
  EFI_STATUS  Status;

  if ((Progress == NULL) || (Results == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  if (Request == NULL) {
    return EFI_UNSUPPORTED;
  }

  if (StrStr (Request, L"OFFSET") == NULL) {
    return EFI_UNSUPPORTED;
  }

  DEBUG ((DEBUG_INFO, "%a - Request=%s\n", __FUNCTION__, Request));
  DEBUG ((DEBUG_INFO, "%s", Request));
  DEBUG ((DEBUG_INFO, "\n"));

  //
  // Convert buffer data to <ConfigResp> by helper function BlockToConfig()
  //
  if (HiiIsConfigHdrMatch (Request, &gMsBootMenuFormsetGuid, L"BootOrderConfig")) {
    Status = gHiiConfigRouting->BlockToConfig (
                                  gHiiConfigRouting,
                                  Request,
                                  (UINT8 *)&mOrderConfiguration,
                                  sizeof (mOrderConfiguration),
                                  Results,
                                  Progress
                                  );
    DEBUG ((DEBUG_INFO, "%a Size is %d, Code=%r\n", __FUNCTION__, sizeof (mOrderConfiguration), Status));
  } else if (HiiIsConfigHdrMatch (Request, &gMsBootMenuFormsetGuid, L"BootSettingsConfig")) {
    // Do a similar process for both BootSettings and BootGrayout - cannot predict which will
    // come first.

    DEBUG ((DEBUG_INFO, "%a for Menu Settings\n", __FUNCTION__));

    Status = gHiiConfigRouting->BlockToConfig (
                                  gHiiConfigRouting,
                                  Request,
                                  (UINT8 *)&mSettingsConfiguration,
                                  sizeof (SETTINGS_MENU_CONFIGURATION),
                                  Results,
                                  Progress
                                  );
  } else if (HiiIsConfigHdrMatch (Request, &gMsBootMenuFormsetGuid, L"BootGrayoutConfig")) {
    // Do the similar thing for both BootSettings and BootGrayout - cannot predict which will
    // come first.
    mSettingsGrayoutConfiguration.RestrictedAccessString = FALSE;
    DEBUG ((DEBUG_INFO, "%a for Grayout Settings\n", __FUNCTION__));

    Status = gHiiConfigRouting->BlockToConfig (
                                  gHiiConfigRouting,
                                  Request,
                                  (UINT8 *)&mSettingsGrayoutConfiguration,
                                  sizeof (SETTINGS_GRAYOUT_CONFIGURATION),
                                  Results,
                                  Progress
                                  );
  } else if (HiiIsConfigHdrMatch (Request, &gMsBootMenuFormsetGuid, L"BootSuppressConfig")) {
    // Do the similar thing for both BootSettings and BootGrayout - cannot predict which will
    // come first.
    DEBUG ((DEBUG_INFO, "%a for Suppress Settings\n", __FUNCTION__));

    Status = gHiiConfigRouting->BlockToConfig (
                                  gHiiConfigRouting,
                                  Request,
                                  (UINT8 *)&mSettingsSuppressConfiguration,
                                  sizeof (SETTINGS_SUPPRESS_CONFIGURATION),
                                  Results,
                                  Progress
                                  );
  } else {
    Status = EFI_UNSUPPORTED;
  }

  DEBUG ((DEBUG_INFO, "%a complete. Code = %r\n", __FUNCTION__, Status));
  return Status;
}
