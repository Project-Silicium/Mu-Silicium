/** @file
  User interaction functions for the FrontPage.

  Copyright (C) Microsoft Corporation. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "FrontPage.h"
#include "FrontPageUi.h"

#include <PiDxe.h>          // This has to be here so Protocol/FirmwareVolume2.h doesn't puke errors.
#include <UefiSecureBoot.h>

#include <Guid/GlobalVariable.h>
#include <Guid/ImageAuthentication.h>
#include <Guid/SmmVariableCommon.h>
#include <Guid/MdeModuleHii.h>

#include <Protocol/OnScreenKeyboard.h>
#include <Protocol/SimpleWindowManager.h>
#include <Protocol/FirmwareVolume2.h>
#include <Protocol/SmmCommunication.h>
#include <Protocol/SmmVariable.h>
#include <Protocol/VariablePolicy.h>

#include <Library/BaseMemoryLib.h>
#include <Library/SafeIntLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/HiiLib.h>
#include <Library/PrintLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootManagerLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/MsColorTableLib.h>
#include <Library/SwmDialogsLib.h>
#include <Library/SecureBootVariableLib.h>
#include <Library/MuSecureBootKeySelectorLib.h>
#include <Library/SecureBootKeyStoreLib.h>
#include <Library/PasswordPolicyLib.h>

#include <Settings/DfciSettings.h>
#include <Settings/FrontPageSettings.h>

extern EFI_HII_HANDLE                  gStringPackHandle;
extern BOOLEAN                         mResetRequired;
extern DFCI_SETTING_ACCESS_PROTOCOL    *mSettingAccess;
extern UINTN                           mAuthToken;
extern EDKII_VARIABLE_POLICY_PROTOCOL  *mVariablePolicyProtocol;
extern SECURE_BOOT_PAYLOAD_INFO        *mSecureBootKeys;
extern UINT8                           mSecureBootKeysCount;

STATIC
EFI_STATUS
SetSystemPassword (
  IN EFI_IFR_TYPE_VALUE           *Value,
  OUT EFI_BROWSER_ACTION_REQUEST  *ActionRequest
  );

STATIC
EFI_STATUS
HandleInfoPopup (
  IN EFI_IFR_TYPE_VALUE           *Value,
  OUT EFI_BROWSER_ACTION_REQUEST  *ActionRequest
  );

STATIC
EFI_STATUS
HandleSecureBootChange (
  IN EFI_IFR_TYPE_VALUE           *Value,
  OUT EFI_BROWSER_ACTION_REQUEST  *ActionRequest
  );

/**
 * RefreshSecurityForm - Notify browser that the form has changes
 *
 *
 * @return VOID
 */
VOID
RefreshSecurityForm (
  VOID
  )
{
  VOID                *StartOpCodeHandle;
  VOID                *EndOpCodeHandle;
  EFI_IFR_GUID_LABEL  *StartLabel;
  EFI_IFR_GUID_LABEL  *EndLabel;
  STATIC UINT8        NullOpCount = 1;

  DEBUG ((DEBUG_VERBOSE, "VERBOSE [SFP] %a - Attempting to force Security form refresh.\n", __FUNCTION__));

  //
  // Determine how many NULL opcodes we need.
  // This is so that the number of opcodes will vary from execution to
  // execution and force the page CRC to always change.
  // Can toggle back and forth.
  //
  if (1 != NullOpCount) {
    NullOpCount = 1;
  } else {
    NullOpCount = 2;
  }

  //
  // Init OpCode Handle and Allocate space for creation of UpdateData Buffer
  //
  StartOpCodeHandle = HiiAllocateOpCodeHandle ();
  ASSERT (StartOpCodeHandle != NULL);

  EndOpCodeHandle = HiiAllocateOpCodeHandle ();
  ASSERT (EndOpCodeHandle != NULL);

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

  StartLabel->Number = LABEL_UPDATE_SECURITY_START;
  EndLabel->Number   = LABEL_UPDATE_SECURITY_END;

  HiiCreateTextOpCode (StartOpCodeHandle, STRING_TOKEN (STR_NULL_STRING), 0, 0);
  if (NullOpCount > 1) {
    HiiCreateTextOpCode (StartOpCodeHandle, STRING_TOKEN (STR_NULL_STRING), 0, 0);
  }

  HiiUpdateForm (
    mFrontPagePrivate.HiiHandle,
    &gMuFrontPageConfigFormSetGuid,
    FRONT_PAGE_FORM_ID_SECURITY,
    StartOpCodeHandle,
    EndOpCodeHandle
    );

  HiiFreeOpCodeHandle (StartOpCodeHandle);
  HiiFreeOpCodeHandle (EndOpCodeHandle);
}

/**
  Handle a request to reboot back into FrontPage.

  @retval EFI_SUCCESS

**/
STATIC
EFI_STATUS
HandleRebootToFrontPage (
  IN  EFI_IFR_TYPE_VALUE          *Value,
  OUT EFI_BROWSER_ACTION_REQUEST  *ActionRequest
  )
{
  EFI_STATUS  Status;
  UINT32      Attributes = EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE;
  UINTN       DataSize;
  UINT64      OsIndications;

  DEBUG ((DEBUG_INFO, "[%a:%a]\n", gEfiCallerBaseName, __FUNCTION__));

  //
  // Step 1: Read the current OS indications variable.
  DataSize = sizeof (OsIndications);
  Status   = gRT->GetVariable (
                    EFI_OS_INDICATIONS_VARIABLE_NAME,
                    &gEfiGlobalVariableGuid,
                    &Attributes,
                    &DataSize,
                    (VOID *)&OsIndications
                    );

  //
  // Step 2: Update OS indications variable to enable the boot to FrontPage.
  if (!EFI_ERROR (Status) || (Status == EFI_NOT_FOUND)) {
    if (Status == EFI_NOT_FOUND) {
      OsIndications = EFI_OS_INDICATIONS_BOOT_TO_FW_UI;
      Attributes    = (EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE);
    } else {
      OsIndications |= EFI_OS_INDICATIONS_BOOT_TO_FW_UI;
    }

    Status = gRT->SetVariable (
                    EFI_OS_INDICATIONS_VARIABLE_NAME,
                    &gEfiGlobalVariableGuid,
                    Attributes,
                    DataSize,
                    (VOID *)&OsIndications
                    );
  }

  //
  // Step 3: Reboot!
  if (!EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "[%a:%a] Requesting reboot...\n", gEfiCallerBaseName, __FUNCTION__));
    *ActionRequest = EFI_BROWSER_ACTION_REQUEST_EXIT;
    mResetRequired = TRUE;
  } else {
    DEBUG ((DEBUG_ERROR, "[%a:%a] Status = %r\n", gEfiCallerBaseName, __FUNCTION__, Status));
  }

  return Status;
}

/**
  This function processes the results of changes in configuration.

  @param This            Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param Action          Specifies the type of action taken by the browser.
  @param QuestionId      A unique value which is sent to the original exporting driver
                         so that it can identify the type of data to expect.
  @param Type            The type of value for the question.
  @param Value           A pointer to the data being sent to the original exporting driver.
  @param ActionRequest   On return, points to the action requested by the callback function.

  @retval  EFI_SUCCESS           The callback successfully handled the action.
  @retval  EFI_OUT_OF_RESOURCES  Not enough storage is available to hold the variable and its data.
  @retval  EFI_DEVICE_ERROR      The variable could not be saved.
  @retval  EFI_UNSUPPORTED       The specified Action is not supported by the callback.

**/
EFI_STATUS
EFIAPI
UiCallback (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL  *This,
  IN  EFI_BROWSER_ACTION                    Action,
  IN  EFI_QUESTION_ID                       QuestionId,
  UINT8                                     Type,
  IN  EFI_IFR_TYPE_VALUE                    *Value,
  OUT EFI_BROWSER_ACTION_REQUEST            *ActionRequest
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;

  DEBUG ((DEBUG_INFO, "FrontPage:UiCallback() - Question ID=0x%08x Type=0x%04x Action=0x%04x ShortValue=0x%02x\n", QuestionId, Type, Action, *(UINT8 *)Value));

  //
  // Sanitize input values.
  if ((Value == NULL) || (ActionRequest == NULL)) {
    DEBUG ((DEBUG_INFO, "FrontPage:UiCallback - Bailing from invalid input.\n"));
    return EFI_INVALID_PARAMETER;
  }

  //
  // Filter responses.
  // NOTE: For now, let's only consider elements that have CHANGED.
  if (Action != EFI_BROWSER_ACTION_CHANGED) {
    DEBUG ((DEBUG_INFO, "FrontPage:UiCallback - Bailing from unimportant input.\n"));
    return EFI_UNSUPPORTED;
  }

  //
  // Set a default action request.
  *ActionRequest = EFI_BROWSER_ACTION_REQUEST_NONE;

  //
  // Handle the specific callback.
  // We'll record the callback event as mCallbackKey so that other processes can make decisions
  // on how we exited the run loop (if that occurs).
  mCallbackKey = QuestionId;
  switch (mCallbackKey) {
    case FRONT_PAGE_ACTION_SEC_DISPLAY_SB_WHAT_IS:
      Status = HandleInfoPopup (Value, ActionRequest);
      break;

    /////////////////////////////////////////////////////////////////////////////
    // SECURITY CALLBACKS
    //
    case FRONT_PAGE_ACTION_SEC_SET_SYSTEM_PASSWORD:
      Status = SetSystemPassword (Value, ActionRequest);
      break;

    case FRONT_PAGE_ACTION_SEC_CHANGE_SB_CONFIG:
      Status = HandleSecureBootChange (Value, ActionRequest);
      break;
    case FRONT_PAGE_ACTION_EXIT_FRONTPAGE:
      *ActionRequest = EFI_BROWSER_ACTION_REQUEST_EXIT;
      break;

    case FRONT_PAGE_ACTION_REBOOT_TO_FRONTPAGE:
      Status = HandleRebootToFrontPage (Value, ActionRequest);
      break;

    default:
      DEBUG ((DEBUG_INFO, "FrontPage:UiCallback - Unknown event passed.\n"));
      Status       = EFI_UNSUPPORTED;
      mCallbackKey = 0;
      break;
  }

  return Status;
}

STATIC
EFI_STATUS
SetSystemPassword (
  IN  EFI_IFR_TYPE_VALUE          *Value,
  OUT EFI_BROWSER_ACTION_REQUEST  *ActionRequest
  )
{
  EFI_STATUS          Status          = EFI_SUCCESS;
  SWM_MB_RESULT       Result          = 0;
  PW_TEST_BITMAP      PwdValidBitmap  = 0;
  CHAR16              *pErrorMessage  = (CHAR16 *)HiiGetString (gStringPackHandle, STRING_TOKEN (STR_NULL_STRING), NULL);
  CHAR16              *PasswordBuffer = NULL;  // This will be allocated by PasswordDialog(). Needs to be tracked, wiped, and freed.
  PASSWORD_HASH       PasswordHash;
  UINTN               PasswordHashSize;
  DFCI_SETTING_FLAGS  Flags = 0;

  DEBUG ((DEBUG_INFO, "INFO: [FP] SetSystemPassword: ENTER\r\n"));

  // Present a dialog to the user for setting the password.
  //
  do {
    Status = SwmDialogsPasswordPrompt (
               HiiGetString (gStringPackHandle, STRING_TOKEN (STR_PWD_ENTER_PWD_TITLEBARTEXT), NULL),                               // Dialog titlebar text.
               HiiGetString (gStringPackHandle, STRING_TOKEN (STR_PWD_CAPTION), NULL),                                              // Dialog caption text.
               HiiGetString (gStringPackHandle, STRING_TOKEN (STR_PWD_SET_BODYTEXT), NULL),                                         // Dialog body text.
               pErrorMessage,
               SWM_PWD_TYPE_SET_PASSWORD,
               &Result,
               &PasswordBuffer
               );

    // If the user selects cancel or there's a dialog error, abort.
    //
    if (EFI_ERROR (Status) || (SWM_MB_IDCANCEL == Result)) {
      break;
    }

    // If the user selects OK, make sure both the password and the confirmation match then set it.
    //
    if (SWM_MB_IDOK == Result) {
      // If the new password string is invalid *and* it's not a null string (clear password), free the password buffer returned by the password dialog and try again.
      //
      if ((FALSE == PasswordPolicyIsPwStringValid (PasswordBuffer, &PwdValidBitmap)) && ((PwdValidBitmap & PW_TEST_STRING_NULL) != PW_TEST_STRING_NULL)) {
        // Select an appropriate error message.
        //
        if (PwdValidBitmap & PW_TEST_STRING_TOO_SHORT) {
          // Password is too short.
          //
          pErrorMessage = (CHAR16 *)HiiGetString (gStringPackHandle, STRING_TOKEN (STR_PWD_ERRORMSG_TOOSHORT), NULL);
        } else if (PwdValidBitmap & PW_TEST_STRING_TOO_LONG) {
          // Password is too long.
          //
          pErrorMessage = (CHAR16 *)HiiGetString (gStringPackHandle, STRING_TOKEN (STR_PWD_ERRORMSG_TOOLONG), NULL);
        } else if (PwdValidBitmap & PW_TEST_STRING_INVALID_CHAR) {
          // Password contains invalid characters.
          //
          pErrorMessage = (CHAR16 *)HiiGetString (gStringPackHandle, STRING_TOKEN (STR_PWD_ERRORMSG_INVALID_CHAR), NULL);
        } else {
          // Some other (non-specific) failure.
          //
          pErrorMessage = (CHAR16 *)HiiGetString (gStringPackHandle, STRING_TOKEN (STR_PWD_ERRORMSG_SET_GENFAILURE), NULL);
        }

        // If password buffer was used, make sure it's freed.
        //
        if (NULL != PasswordBuffer) {
          ZeroMem ((UINT8 *)PasswordBuffer, StrLen (PasswordBuffer) * sizeof (CHAR16));
          FreePool (PasswordBuffer);
          PasswordBuffer = NULL;
        }

        continue;
      }

      //
      // Otherwise, try setting the password.  If it fails, free the password buffer and try again.
      //
      Status = PasswordPolicyGeneratePasswordHash (NULL, PasswordBuffer, &PasswordHash, &PasswordHashSize);

      if (!EFI_ERROR (Status)) {
        Status = mSettingAccess->Set (
                                   mSettingAccess,
                                   DFCI_SETTING_ID__PASSWORD,
                                   &mAuthToken,
                                   DFCI_SETTING_TYPE_PASSWORD,
                                   PasswordHashSize,
                                   (VOID *)PasswordHash,
                                   &Flags
                                   );
        FreePool (PasswordHash);
      }

      if (EFI_ERROR (Status)) {
        // Select an appropriate error message.
        //
        if (EFI_SECURITY_VIOLATION == Status) {
          // Password authentication error.
          //
          pErrorMessage = (CHAR16 *)HiiGetString (gStringPackHandle, STRING_TOKEN (STR_PWD_ERRORMSG_AUTHERROR), NULL);
        } else {
          // Some other (non-specific) failure.
          //
          pErrorMessage = (CHAR16 *)HiiGetString (gStringPackHandle, STRING_TOKEN (STR_PWD_ERRORMSG_SET_GENFAILURE), NULL);
        }

        // If password buffer was used, make sure it's freed.
        //
        if (NULL != PasswordBuffer) {
          ZeroMem ((UINT8 *)PasswordBuffer, StrLen (PasswordBuffer) * sizeof (CHAR16));
          FreePool (PasswordBuffer);
          PasswordBuffer = NULL;
        }

        continue;
      }

      break;
    }
  } while (TRUE);

  // If password buffer was used, make sure it's freed.
  //
  if (NULL != PasswordBuffer) {
    ZeroMem ((UINT8 *)PasswordBuffer, StrLen (PasswordBuffer) * sizeof (CHAR16));
    FreePool (PasswordBuffer);
    PasswordBuffer = NULL;
  }

  DEBUG ((DEBUG_INFO, "INFO: [FP] SetSystemPassword: EXIT\r\n"));

  return Status;
}

/**
  Presents the user with a (hopefully) helpful dialog
  with more info about a particular subject.

  NOTE: Subject is determined by the state of mCallbackKey.

  @retval   EFI_SUCCESS     Message successfully displayed.
  @retval   EFI_NOT_FOUND   mCallbackKey not recognized or string could not be loaded.
  @retval   Others          Return value of SwmDialogsMessageBox().

**/
STATIC
EFI_STATUS
HandleInfoPopup (
  IN  EFI_IFR_TYPE_VALUE          *Value,
  OUT EFI_BROWSER_ACTION_REQUEST  *ActionRequest
  )
{
  EFI_STATUS     Status = EFI_SUCCESS;
  EFI_STRING_ID  TitleId = 0, MessageId = 0, CaptionId = 0;
  CHAR16         *TitleBarText = NULL, *InfoMessage = NULL, *CaptionText = NULL;
  SWM_MB_RESULT  SwmResult;

  //
  // First, we need to determine which info message to display.
  switch (mCallbackKey) {
    case FRONT_PAGE_ACTION_SEC_DISPLAY_SB_WHAT_IS:
      TitleId   = STRING_TOKEN (STR_SEC_SB_WHAT_IS_TITLE);
      CaptionId = STRING_TOKEN (STR_SEC_SB_WHAT_IS_LINK);
      MessageId = STRING_TOKEN (STR_SEC_SB_WHAT_IS_TEXT);
      break;

    default:
      Status = EFI_NOT_FOUND;
      break;
  }

  //
  // Next, attempt to load the string.
  if (!EFI_ERROR (Status)) {
    TitleBarText = (CHAR16 *)HiiGetString (mFrontPagePrivate.HiiHandle, TitleId, NULL);
    CaptionText  = (CHAR16 *)HiiGetString (mFrontPagePrivate.HiiHandle, CaptionId, NULL);
    InfoMessage  = (CHAR16 *)HiiGetString (mFrontPagePrivate.HiiHandle, MessageId, NULL);
    if ((NULL == InfoMessage) || (NULL == TitleBarText)) {
      Status = EFI_NOT_FOUND;
    }
  }

  //
  // Finally, display the message to the user.
  if (!EFI_ERROR (Status)) {
    Status = SwmDialogsMessageBox (
               TitleBarText,                                // Dialog title bar text.
               InfoMessage,                                 // Dialog body text.
               CaptionText,                                 // Dialog caption text.
               SWM_MB_OK,                                   // Show only Ok button.
               0,                                           // No timeout
               &SwmResult
               );                                           // Return result.
  }

  return Status;
}

/**
  Handle a request to change the SecureBoot configuration.

  @retval EFI_SUCCESS         Successfully updated SecureBoot default variables or user cancelled.
  @retval Others              Failed to update. SecureBoot state remains unchanged.

**/
STATIC
EFI_STATUS
HandleSecureBootChange (
  IN  EFI_IFR_TYPE_VALUE          *Value,
  OUT EFI_BROWSER_ACTION_REQUEST  *ActionRequest
  )
{
  EFI_STATUS          Status;
  CHAR16              *DialogTitleBarText, *DialogCaptionText, *DialogBodyText;
  CHAR16              **Options = NULL;
  SWM_MB_RESULT       SwmResult;
  UINTN               SelectedIndex;
  UINT8               IndexSetValue;
  UINT8               Index;
  DFCI_SETTING_FLAGS  Flags = 0;
  UINTN               OptionsCount;

  //
  // Load UI dialog strings.
  DialogTitleBarText = (CHAR16 *)HiiGetString (mFrontPagePrivate.HiiHandle, STRING_TOKEN (STR_SB_CONFIG_TITLEBARTEXT), NULL);
  DialogCaptionText  = (CHAR16 *)HiiGetString (mFrontPagePrivate.HiiHandle, STRING_TOKEN (STR_SB_CONFIG_CAPTION), NULL);
  DialogBodyText     = (CHAR16 *)HiiGetString (mFrontPagePrivate.HiiHandle, STRING_TOKEN (STR_SB_CONFIG_BODY), NULL);

  OptionsCount = mSecureBootKeysCount + 1;
  Options      = AllocatePool (OptionsCount * sizeof (CHAR16 *));
  if (Options == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Load UI option strings.
  for (Index = 0; Index < mSecureBootKeysCount; Index++) {
    Options[Index] = (CHAR16 *)mSecureBootKeys[Index].SecureBootKeyName;
  }

  Options[mSecureBootKeysCount] = (CHAR16 *)HiiGetString (mFrontPagePrivate.HiiHandle, STRING_TOKEN (STR_GENERIC_TEXT_NONE), NULL);

  //
  // Display the dialog to the user.
  Status = SwmDialogsSelectPrompt (
             DialogTitleBarText,
             DialogCaptionText,
             DialogBodyText,
             Options,
             OptionsCount,
             &SwmResult,
             &SelectedIndex
             );
  DEBUG ((DEBUG_INFO, "INFO [SFP] %a - SingleSelectDialog returning: Status = %r, SwmResult = 0x%X, SelectedIndex = %d\n", __FUNCTION__, Status, (UINTN)SwmResult, SelectedIndex));

  //
  // If the form was submitted, process the update.
  if (!EFI_ERROR (Status) && (SWM_MB_IDOK == SwmResult) && !EFI_ERROR (SafeUintnToUint8 (SelectedIndex, &IndexSetValue))) {
    mVariablePolicyProtocol->DisableVariablePolicy ();

    if (IndexSetValue == mSecureBootKeysCount) {
      IndexSetValue = MU_SB_CONFIG_NONE;
    }

    Status = mSettingAccess->Set (
                               mSettingAccess,
                               DFCI_SETTING_ID__SECURE_BOOT_KEYS_ENUM,
                               &mAuthToken,
                               DFCI_SETTING_TYPE_SECUREBOOTKEYENUM,
                               sizeof (IndexSetValue),
                               &IndexSetValue,
                               &Flags
                               );
    //
    // If successful, update the display.
    if (!EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "%a - Successfully changed keys. Updating form browser and requesting restart.\n", __FUNCTION__));

      *ActionRequest = EFI_BROWSER_ACTION_REQUEST_FORM_APPLY;

      //
      // Indicate that this change should also trigger a reboot.
      mResetRequired = TRUE;

      //
      // Update the display strings.
      UpdateSecureBootStatusStrings (TRUE);
    } else {
      DEBUG ((DEBUG_ERROR, "ERROR [SFP] %a - Failed to update SecureBoot config! %r\n", __FUNCTION__, Status));
      DialogTitleBarText = (CHAR16 *)HiiGetString (mFrontPagePrivate.HiiHandle, STRING_TOKEN (STR_SB_UPDATE_FAILURE_TITLE), NULL);
      DialogBodyText     = (CHAR16 *)HiiGetString (mFrontPagePrivate.HiiHandle, STRING_TOKEN (STR_SB_UPDATE_FAILURE), NULL);
      SwmDialogsMessageBox (
        DialogTitleBarText,                            // Dialog title bar text.
        DialogBodyText,                                // Dialog body text.
        L"",                                           // Dialog caption text.
        SWM_MB_OK,                                     // Show Ok button.
        0,                                             // No timeout
        &SwmResult
        );                                             // Return result.
    }
  }

  // IMPORTANT NOTE: Until further review, all attempts to set the SecureBoot keys should result in a reboot.
  //                  This is to account for possible edge cases in the suspension of VariablePolicy enforcement.
  mResetRequired = TRUE;

  if (Options != NULL) {
    FreePool (Options);
  }

  return Status;
}

/**
  Determines the current SecureBoot state and updates the status strings accordingly.

  @param  RefreshScreen     BOOLEAN indicating whether to force a screen refresh after updating the strings.

**/
VOID
UpdateSecureBootStatusStrings (
  BOOLEAN  RefreshScreen
  )
{
  BOOLEAN  IsEnabled;
  UINTN    CurrentConfig;
  CHAR16   StateString[256];                // This is a somewhat arbitrary limit. Just needs to be large enough to encompass the largest possible string.
  CHAR16   *PreambleSubstring = NULL;
  CHAR16   *StateSubstring    = NULL;
  CHAR16   *ConfigSubstring   = NULL;
  CHAR16   *SuffixSubstring   = NULL;

  //
  // No matter what the mode is, we need the preamble.
  PreambleSubstring = (CHAR16 *)HiiGetString (mFrontPagePrivate.HiiHandle, STRING_TOKEN (STR_SEC_SB_STATE_PREAMBLE), NULL);

  //
  // Determine whether SecureBoot is enabled.
  IsEnabled = IsSecureBootEnabled ();

  //
  // If enabled, determine the current config.
  if (IsEnabled) {
    // Use the "Enabled" substring.
    StateSubstring  = (CHAR16 *)HiiGetString (mFrontPagePrivate.HiiHandle, STRING_TOKEN (STR_SEC_SB_STATE_ENABLED), NULL);
    SuffixSubstring = (CHAR16 *)HiiGetString (mFrontPagePrivate.HiiHandle, STRING_TOKEN (STR_SEC_SB_KEY_CONFIG_TEXT), NULL);

    // Determine the ConfigSubstring.
    CurrentConfig = GetCurrentSecureBootConfig ();
    if (MU_SB_CONFIG_NONE == CurrentConfig) {
      ConfigSubstring = (CHAR16 *)HiiGetString (mFrontPagePrivate.HiiHandle, STRING_TOKEN (STR_GENERIC_TEXT_NONE), NULL);
    } else if (mSecureBootKeysCount <= CurrentConfig) {
      ConfigSubstring = (CHAR16 *)HiiGetString (mFrontPagePrivate.HiiHandle, STRING_TOKEN (STR_SEC_SB_CUSTOM_CONFIG_TEXT), NULL);
    } else {
      ConfigSubstring = (CHAR16 *)mSecureBootKeys[CurrentConfig].SecureBootKeyName;
    }

    UnicodeSPrint (
      StateString,
      sizeof (StateString),
      L"%s %s %s %s",
      PreambleSubstring,
      StateSubstring,
      ConfigSubstring,
      SuffixSubstring
      );
  }
  //
  // If disabled, just present the state string.
  else {
    StateSubstring = (CHAR16 *)HiiGetString (mFrontPagePrivate.HiiHandle, STRING_TOKEN (STR_SEC_SB_STATE_DISABLED), NULL);
    UnicodeSPrint (StateString, sizeof (StateString), L"%s %s", PreambleSubstring, StateSubstring);
  }

  //
  // Finally, push the updates into the HiiDatabase.
  HiiSetString (mFrontPagePrivate.HiiHandle, STRING_TOKEN (STR_SEC_SB_STATE_TEXT), StateString, NULL);

  //
  // And notify the form that it needs to refresh.
  if (RefreshScreen) {
    RefreshSecurityForm ();
  }
} // UpdateSecureBootStatusStrings()

/**
  Present user with password prompt and attempt to validate password.

  NOTE: If user enters password incorrectly too many times, return FALSE.

  @param  MaxAttempts   The number of invalid password attempts before the
                        system will halt with an appropriate message.
                        If 0, user receives unlimited opportunites.

  @retval     TRUE    User entered the password correctly.
  @retval     FALSE   User cancelled password attempt or failed to authenticate.

**/
BOOLEAN
ChallengeUserPassword (
  UINT8  MaxAttempts
  )
{
  EFI_STATUS     Status;
  SWM_MB_RESULT  SwmResult = 0;
  CHAR16         *pErrorMessage = (CHAR16 *)HiiGetString (gStringPackHandle, STRING_TOKEN (STR_NULL_STRING), NULL);
  CHAR16         *PasswordBuffer = NULL;           // This will be allocated by PasswordPrompt(). Needs to be tracked, wiped, and freed.
  BOOLEAN        Result = FALSE, AttemptsExpired = FALSE;

  // Primary UI loop.
  // Display prompt. Process results.
  // This will loop forever if MaxAttempts is 0 and user continues to enter invalid passwords.
  //
  do {
    // Present the password dialog to prompt the user.
    //
    Status = SwmDialogsPasswordPrompt (
               HiiGetString (gStringPackHandle, STRING_TOKEN (STR_PWD_ENTER_PWD_TITLEBARTEXT), NULL),                               // Dialog titlebar text.
               HiiGetString (gStringPackHandle, STRING_TOKEN (STR_PWD_CAPTION), NULL),                                              // Dialog caption text.
               HiiGetString (gStringPackHandle, STRING_TOKEN (STR_PWD_ENTER_BODYTEXT), NULL),                                       // Dialog body text.
               pErrorMessage,
               SWM_PWD_TYPE_PROMPT_PASSWORD,
               &SwmResult,
               &PasswordBuffer
               );

    // Check for errors and whether the user selected cancel.
    //
    if (EFI_ERROR (Status) || (SWM_MB_IDCANCEL == SwmResult)) {
      break;        // Return FALSE.
    }

    // If the user selected "OK", check whether the password provided is valid.
    //
    if (SWM_MB_IDOK == SwmResult) {
      if (GetAuthToken (PasswordBuffer) == EFI_SUCCESS) {
        // Password authentication successful.  Display the full menu.
        //
        Result = TRUE;
        break;
      } else {
        // Password authentication error.  Display error text and ask the user for the password again.
        //
        pErrorMessage = (CHAR16 *)HiiGetString (gStringPackHandle, STRING_TOKEN (STR_PWD_ERRORMSG_AUTHERROR), NULL);

        // If password buffer was used, make sure it's freed.
        //
        if (PasswordBuffer) {
          ZeroMem ((UINT8 *)PasswordBuffer, StrLen (PasswordBuffer) * sizeof (CHAR16));
          FreePool (PasswordBuffer);
          PasswordBuffer = NULL;
        }

        // If MaxAttempts were specified, determine whether we've used our last attempt.
        //
        if (0 != MaxAttempts) {
          // If we've just used our last attempt, make a note of it and get out of here.
          //
          if (0 == --MaxAttempts) {
            AttemptsExpired = TRUE;
            break;
          }
        }
      }
    }
  } while (TRUE);

  // Handle the case where the attempts have expired.
  // Inform the user and return FALSE.
  //
  if (TRUE == AttemptsExpired) {
    DEBUG ((DEBUG_INFO, "FrontPage::%a - Max password attempts elapsed!!\n", __FUNCTION__));
    Status = SwmDialogsMessageBox (
               (CHAR16 *)HiiGetString (gStringPackHandle, STRING_TOKEN (STR_PWD_ATTEMPTS_EXPIRED_TITLE), NULL),    // Dialog titlebar text.
               (CHAR16 *)HiiGetString (gStringPackHandle, STRING_TOKEN (STR_PWD_ATTEMPTS_EXPIRED_BODYTEXT), NULL), // Dialog body text.
               (CHAR16 *)HiiGetString (gStringPackHandle, STRING_TOKEN (STR_PWD_ATTEMPTS_EXPIRED_CAPTION), NULL),  // Dialog caption text.
               SWM_MB_OK,                                                                                          // Show only Ok button.
               0,                                                                                                  // No timeout
               &SwmResult
               );                                           // Return result.
    Result = FALSE;
  }

  if (Result == TRUE) {
    // By now the token has been acquired, but let's confirm it's valid
    if (mAuthToken == DFCI_AUTH_TOKEN_INVALID) {
      Result = FALSE;
    }
  }

  // If password buffer was used, make sure it's freed.
  //
  if (NULL != PasswordBuffer) {
    ZeroMem ((UINT8 *)PasswordBuffer, StrLen (PasswordBuffer) * sizeof (CHAR16));
    FreePool (PasswordBuffer);
    PasswordBuffer = NULL;
  }

  return Result;
} // ChallengeUserPassword()
