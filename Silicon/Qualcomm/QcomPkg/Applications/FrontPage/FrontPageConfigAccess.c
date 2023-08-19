/** @file
  HiiConfigAccess definitions for FrontPage.

  Copyright (C) Microsoft Corporation. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "FrontPageConfigAccess.h"
#include "FrontPage.h"
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>

// For the post-security locks UI controls.
#include <Guid/MuVarPolicyFoundationDxe.h>

extern EFI_HII_CONFIG_ROUTING_PROTOCOL  *mHiiConfigRouting;

/**
  Quick helper function to see if ReadyToBoot has already been signalled.

  @retval     TRUE    ReadyToBoot has been signalled.
  @retval     FALSE   Otherwise...

**/
STATIC
BOOLEAN
IsPostReadyToBoot (
  VOID
  )
{
  EFI_STATUS       Status;
  UINT32           Attributes;
  PHASE_INDICATOR  Indicator;
  UINTN            Size;
  static BOOLEAN   Result, Initialized = FALSE;

  Size = sizeof (Indicator);

  if (!Initialized) {
    Status = gRT->GetVariable (
                    READY_TO_BOOT_INDICATOR_VAR_NAME,
                    &gMuVarPolicyDxePhaseGuid,
                    &Attributes,
                    &Size,
                    &Indicator
                    );
    Result      = (!EFI_ERROR (Status) && (Attributes == READY_TO_BOOT_INDICATOR_VAR_ATTR));
    Initialized = TRUE;
  }

  return Result;
} // IsPostReadyToBoot()

/**
  This function allows a caller to extract the current configuration for the
  FRONT_PAGE_HACK structure from the target driver.
  @param Request         A null-terminated Unicode string in <ConfigRequest> format.
  @param Progress        On return, points to a character in the Request string.
                         Points to the string's null terminator if request was successful.
                         Points to the most recent '&' before the first failing name/value
                         pair (or the beginning of the string if the failure is in the
                         first name/value pair) if the request was not successful.
  @param Results         A null-terminated Unicode string in <ConfigAltResp> format which
                         has all values filled in for the names in the Request string.
                         String to be allocated by the called function.

**/
VOID
EFIAPI
GetFrontPageUiControls (
  IN  CONST EFI_STRING  Request,
  OUT EFI_STRING        *Progress,
  OUT EFI_STRING        *Results
  )
{
  EFI_STATUS              Status;
  FRONT_PAGE_UI_CONTROLS  FrontPageUiControls;

  // For Mu, our threat model is generally that of a vertically-integrated platform.
  // As such, our security lock for UI purposes can be ReadyToBoot.
  FrontPageUiControls.PostSecurityLocks = IsPostReadyToBoot ();

  //
  // Convert buffer data to <ConfigResp> by helper function BlockToConfig()
  //
  Status = mHiiConfigRouting->BlockToConfig (
                                mHiiConfigRouting,
                                Request,
                                (UINT8 *)&FrontPageUiControls,
                                sizeof (FrontPageUiControls),
                                Results,
                                Progress
                                );

  //
  // Set Progress string...
  // NOTE: This may need some more figuring out for error handling.
  if (!EFI_ERROR (Status)) {
    DEBUG ((DEBUG_VERBOSE, "[%a:%a] - Result=\n\t%s\n", gEfiCallerBaseName, __FUNCTION__, *Results));
    *Progress = Request + StrLen (Request);
  }
}

/**
  This function allows a caller to extract the current configuration for one
  or more named elements from the target driver.


  @param This            Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param Request         A null-terminated Unicode string in <ConfigRequest> format.
  @param Progress        On return, points to a character in the Request string.
                         Points to the string's null terminator if request was successful.
                         Points to the most recent '&' before the first failing name/value
                         pair (or the beginning of the string if the failure is in the
                         first name/value pair) if the request was not successful.
  @param Results         A null-terminated Unicode string in <ConfigAltResp> format which
                         has all values filled in for the names in the Request string.
                         String to be allocated by the called function.

  @retval  EFI_SUCCESS            The Results is filled with the requested values.
  @retval  EFI_OUT_OF_RESOURCES   Not enough memory to store the results.
  @retval  EFI_INVALID_PARAMETER  Request is illegal syntax, or unknown name.
  @retval  EFI_NOT_FOUND          Routing data doesn't match any storage in this driver.

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
  //
  // First, let's sanitize the input...
  if ((Progress == NULL) || (Results == NULL)) {
    ASSERT (Progress != NULL);
    ASSERT (Results != NULL);
    return EFI_INVALID_PARAMETER;
  }

  //
  // Let's just ignore NULL requests.
  if (Request == NULL) {
    ASSERT (Request != NULL);
    // NOTE: In theory, we might create a new, complete request here.
    // But NAH!
    return EFI_NOT_FOUND;
  }

  //
  // Initialize the local variables.
  DEBUG ((
    DEBUG_VERBOSE,
    "[%a:%a] - Request=\n\t%s\n",
    gEfiCallerBaseName,
    __FUNCTION__,
    Request
    ));
  *Progress = Request;

  if (HiiIsConfigHdrMatch (Request, &gMuFrontPageConfigFormSetGuid, L"FrontPageUiControls")) {
    GetFrontPageUiControls (Request, Progress, Results);
  } else {
    return EFI_UNSUPPORTED;
  }

  // SetupBrowser considers an error from ExtractConfig to be a fatal error.  Just ignore
  // errors up to the SetupBrowser.
  return EFI_SUCCESS;
}

/**
  This function processes the results of changes in configuration.


  @param This            Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param Configuration   A null-terminated Unicode string in <ConfigResp> format.
  @param Progress        A pointer to a string filled in with the offset of the most
                         recent '&' before the first failing name/value pair (or the
                         beginning of the string if the failure is in the first
                         name/value pair) or the terminating NULL if all was successful.

  @retval  EFI_SUCCESS            The Results is processed successfully.
  @retval  EFI_INVALID_PARAMETER  Configuration is NULL.
  @retval  EFI_NOT_FOUND          Routing data doesn't match any storage in this driver.

**/
EFI_STATUS
EFIAPI
RouteConfig (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL  *This,
  IN  CONST EFI_STRING                      Configuration,
  OUT EFI_STRING                            *Progress
  )
{
  return EFI_NOT_FOUND;
}
