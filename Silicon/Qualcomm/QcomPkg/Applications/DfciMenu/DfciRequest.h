/** @file
DfciRequest.h

Defines the Request function to get the configuration from the server

Copyright (C) Microsoft Corporation. All rights reserved.
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef __DFCI_REQUEST_H__
#define __DFCI_REQUEST_H__

#define MAX_DELAY_BEFORE_RETRY  24
#define MIN_DELAY_BEFORE_RETRY  1

/**
 * Process Http Recovery
 *
 * @param NetworkRequest     - Private data
 * @param Message            - Where to store the status message
 *
 * @return EFI_STATUS EFIAPI
 */
EFI_STATUS
EFIAPI
ProcessDfciNetworkRequest (
  IN  DFCI_NETWORK_REQUEST  *NetworkRequest,
  OUT CHAR16                **Message
  );

/**
 * Process Simple Http Recovery
 *
 * @param NetworkRequest     - Private data
 * @param Message            - Where to store the status message
 *
 * @return EFI_STATUS EFIAPI
 */
EFI_STATUS
EFIAPI
ProcessSimpleNetworkRequest (
  IN  DFCI_NETWORK_REQUEST  *NetworkRequest,
  OUT CHAR16                **Message
  );

/**
 * TryEachNICThenProcessRequest
 *
 * @param[in]   NetworkRequest
 *
 * Returns      EFI_STATUS
 *
 **/
EFI_STATUS
TryEachNICThenProcessRequest (
  DFCI_NETWORK_REQUEST  *NetworkRequest
  );

/**
 * Check network stack for capability to receive HTTP error 429.
 *
 * @param[in]  NetworkRequest
 * @param[out] Done Processing    - Inform caller processing is complete
 *
 * This is a really simple request to a test server that will return 429.
 **/
EFI_STATUS
EFIAPI
Check429Logic (
  IN  DFCI_NETWORK_REQUEST  *NetworkRequest,
  OUT BOOLEAN               *DoneProcessing
  );

/**
  This function is to display the HTTP error status.

  @param[in]      StatusCode      The status code value in HTTP message.

**/
CHAR8 *
GetHttpErrorMsg (
  EFI_HTTP_STATUS_CODE  StatusCode
  );

#endif // __DFCI_REQUEST_H__
