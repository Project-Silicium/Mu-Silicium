/** @file
  User interaction functions for the FrontPage.

  Copyright (C) Microsoft Corporation. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _FRONT_PAGE_UI_H_
#define _FRONT_PAGE_UI_H_

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
  );

/**
  Determines the current SecureBoot state and updates the status strings accordingly.

  @param[in]  RefreshScreen     BOOLEAN indicating whether to force a screen refresh after updating the strings.

**/
VOID
UpdateSecureBootStatusStrings (
  BOOLEAN  RefreshScreen
  );

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
  );

#endif // _FRONT_PAGE_UI_H_
