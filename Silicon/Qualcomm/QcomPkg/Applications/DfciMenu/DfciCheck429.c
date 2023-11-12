/** @file
DfciCheck429.c

Device Firmware Configuration Interface - Stand Alone driver that can be loaded
at the UEFI Shell

Copyright (C) Microsoft Corporation. All rights reserved.
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>

#include <Protocol/ShellParameters.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/HttpLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>

#include "DfciPrivate.h"
#include "DfciRequest.h"

// *---------------------------------------------------------------------------------------*
// * Application Global Variables                                                          *
// *---------------------------------------------------------------------------------------*
DFCI_NETWORK_REQUEST  mDfciNetworkRequest = { 0 };

#define host_name_prefix  "http://"
#define host_name_suffix  "/return_429"

/**
*  This function is the main entry of the DfciCheck429 application.
*
* @param[in]   ImageHandle
* @param[in]   SystemTable
*
**/
EFI_STATUS
EFIAPI
DfciCheck429Entry (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  UINTN                          host_name_len;
  CHAR8                          *host_name_url;
  UINTN                          offset;
  EFI_SHELL_PARAMETERS_PROTOCOL  *Parameters;
  EFI_STATUS                     Status;

  ZeroMem (&mDfciNetworkRequest, sizeof (mDfciNetworkRequest));
  Parameters = NULL;

  Status = gBS->HandleProtocol (
                  ImageHandle,
                  &gEfiShellParametersProtocolGuid,
                  (VOID **)&Parameters
                  );

  if (EFI_ERROR (Status) || (Parameters == NULL)) {
    // This UEFI_DRIVER application must run under the UEFI Shell
    DEBUG ((DEBUG_ERROR, "%a - Shell Parameters protocol not available. %r\n", __FUNCTION__, Status));
    return 8;
  }

  if (Parameters->Argc != 2) {
    AsciiPrint ("Incorrect parameters:\n");
    AsciiPrint ("\n");
    AsciiPrint ("  DfciCheck429  host-name-or-ip-address\n");
    AsciiPrint ("\n");
    AsciiPrint ("  Examples:\n");
    AsciiPrint ("    DfciCheck host.docker.internal.com\n");
    AsciiPrint ("    DfciCheck 192.168.1.115\n");
    return 8;
  }

  host_name_len = 1;
  host_name_len = StrLen (Parameters->Argv[1]);
  if (host_name_len < 1) {
    AsciiPrint ("Invalid or missing host-name-or-ip-address\n");
    return 8;
  }

  AsciiPrint ("This test takes at least 40 seconds, and may take over a minute with network retires.\n");

  //
  // Build the CHAR8 URL from the CHAR16 parameter + the CHA8 host_name_suffix, host_name_prefix, and a '\0'.
  // Note that sizeof returns the size including the NULL, so we only need one of those NULL characters.
  //
  //    'http://' + host-name-or-ip-address + '/return_429'
  //
  host_name_len += sizeof (host_name_prefix) + sizeof (host_name_suffix) - sizeof (CHAR8);
  host_name_url  = AllocatePool (host_name_len);
  if (host_name_url == NULL) {
    AsciiPrint ("TEST FAILED. Out of memory\n");
    return 8;
  }

  Status = AsciiStrCpyS (host_name_url, host_name_len, host_name_prefix);
  if (EFI_ERROR (Status)) {
    AsciiPrint ("TEST FAILED. AsciiStrCpyS failed\n");
    return 8;
  }

  offset = AsciiStrLen (host_name_prefix);
  Status = UnicodeStrToAsciiStrS (Parameters->Argv[1], &host_name_url[offset], host_name_len - offset);
  if (EFI_ERROR (Status)) {
    AsciiPrint ("TEST FAILED. UnicodeStrToAsciiStrS failed\n");
    return 8;
  }

  Status = AsciiStrCatS (host_name_url, host_name_len, host_name_suffix);
  if (EFI_ERROR (Status)) {
    AsciiPrint ("TEST FAILED. AsciiStrCatS failed\n");
    return 8;
  }

  AsciiPrint ("Processing url=%a\n", host_name_url);

  mDfciNetworkRequest.HttpRequest.Url = host_name_url;
  mDfciNetworkRequest.MainLogic       = Check429Logic;

  // Try every NIC in the system until one fills the first part of the request.
  Status = TryEachNICThenProcessRequest (&mDfciNetworkRequest);

  if (EFI_ERROR (Status)) {
    AsciiPrint ("TEST FAILED.  No NIC satisfied the request.\n");
  } else {
    DEBUG ((DEBUG_INFO, "Url   %a\n", mDfciNetworkRequest.HttpRequest.Url));
    DEBUG ((DEBUG_INFO, "HttpStatus = %d, Status-%r\n", mDfciNetworkRequest.HttpStatus.HttpStatus, Status));
    DEBUG ((DEBUG_INFO, "HttpStatus = %a\n", GetHttpErrorMsg (mDfciNetworkRequest.HttpStatus.HttpStatus)));

    if (mDfciNetworkRequest.HttpStatus.HttpStatus == HTTP_STATUS_429_TOO_MANY_REQUESTS) {
      AsciiPrint ("TEST PASSED. Network stack returned 429 as expected.\n");
    } else {
      if (mDfciNetworkRequest.HttpStatus.HttpStatus == HTTP_STATUS_UNSUPPORTED_STATUS) {
        AsciiPrint ("TEST FAILED.  Http status could not be retrieved.\n");
      } else {
        AsciiPrint ("TEST FAILED.  Unexpected status = %a\n", GetHttpErrorMsg (mDfciNetworkRequest.HttpStatus.HttpStatus));
      }
    }
  }

  return EFI_SUCCESS;
}
