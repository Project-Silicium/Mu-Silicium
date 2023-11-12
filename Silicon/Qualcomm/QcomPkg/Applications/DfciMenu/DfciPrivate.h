/** @file
DfciPrivate.h

Header file for Dfci Private Data

Copyright (C) Microsoft Corporation. All rights reserved.
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef __DFCI_PRIVATE_H__
#define __DFCI_PRIVATE_H__

#include <Protocol/ServiceBinding.h>
#include <Protocol/Http.h>
#include <Protocol/Ip4Config2.h>

#include "DfciUtility.h"

typedef struct _DFCI_NETWORK_REQUEST DFCI_NETWORK_REQUEST;

typedef enum {
  DFCI_PRE_BOOTSTRAP,
  DFCI_BOOTSTRAP,
  DFCI_BOOTSTRAP_COMPLETE,
  DFCI_PRE_RECOVERY,
  DFCI_RECOVERY,
  DFCI_RECOVERY_COMPLETE,
  DFCI_NO_RECOVERY_AVAILABLE,
  DFCI_CHECK_429,
  DFCI_CHECK_429_COMPLETE
} DFCI_MAIN_LOGIC_STATE;

/**
 *  Function to process the main logic of the Dfci network provider
 *
 * @param[in]  Network Request
 *
 * @retval EFI_SUCCESS -       Packet processed normally
 * @retval Error -             Error processing packet
 */
typedef
EFI_STATUS
(EFIAPI *DFCI_MAIN_LOGIC)(
  IN  DFCI_NETWORK_REQUEST   *NetworkRequest,
  OUT BOOLEAN                *DoneProcessing
  );

/**
 * DFCI Private Data
 **/
struct _DFCI_NETWORK_REQUEST {
  //
  // Shared input from DfciMenu Initialization
  //
  CHAR8                      *ZeroTouchThumbprint;
  UINTN                      ZeroTouchThumbprintSize;
  CHAR8                      *OwnerThumbprint;
  UINTN                      OwnerThumbprintSize;
  CHAR8                      *HttpsThumbprint;
  UINTN                      HttpsThumbprintSize;
  CONST UINT8                *HttpsCert;
  UINTN                      HttpsCertSize;
  CHAR8                      *RegistrationId;
  UINTN                      RegistrationIdSize;
  CHAR8                      *TenantId;
  UINTN                      TenantIdSize;
  DFCI_SYSTEM_INFORMATION    DfciInfo;
  DFCI_MAIN_LOGIC            MainLogic;
  DFCI_MAIN_LOGIC_STATE      LogicState;

  struct {
    //
    // Managed by ProcessDfciNetworkRequest
    //
    // This section is managed by MainLogic
    VOID         *Registration;
    EFI_EVENT    RegistrationEvent;
  } Main;

  struct {
    //
    // Input parameters for network operations
    //
    // This section is cleared by a CleanupNetworkRequest (CLEANUP_REQUEST)
    CHAR8    *Url;
    UINTN    UrlSize;
    CHAR8    *BootstrapUrl;
    UINTN    BootstrapUrlSize;
    CHAR8    *Body;
    UINTN    BodySize;
  } HttpRequest;

  struct {
    //
    // Output Parameters  Caller to free Body and Headers
    //
    // This section is cleared by a CleanupNetworkRequest (CLEANUP_RESPONSE)
    CHAR8              *Body;
    UINTN              BodySize;
    EFI_HTTP_HEADER    *Headers;
    UINTN              HeaderCount;
  } HttpResponse;

  struct {
    //
    // Http Request Status
    //
    // This section is cleared by a CleanupNetworkRequest (CLEANUP_STATUS)
    CHAR8                   *HttpReturnCode;
    UINTN                   HttpReturnCodeSize;
    CHAR8                   *HttpMessage;
    UINTN                   HttpMessageSize;
    EFI_HTTP_STATUS_CODE    HttpStatus;
  } HttpStatus;

  struct {
    //
    // Http service section
    //
    EFI_HTTP_PROTOCOL       *HttpProtocol;
    EFI_HANDLE              HttpChildHandle;
    EFI_HTTP_CONFIG_DATA    ConfigData;
  } Http;

  struct {
    //
    // NIC Section
    //
    // This section is managed by the function TryEachNICThenProcessRequest
    EFI_HANDLE                      NicHandle;
    BOOLEAN                         DhcpRequested;
    EFI_SERVICE_BINDING_PROTOCOL    *HttpSbProtocol;

    //
    // IPv4 Specific section
    //
    EFI_HTTPv4_ACCESS_POINT         IPv4Node;

    //
    // IPv6 Specific section
    //
    EFI_HTTPv6_ACCESS_POINT         IPv6Node;

    //
    // Fields valid during DHCP delay
    //
    EFI_EVENT                       WaitEvent;
  } HttpNic;
};

extern  DFCI_NETWORK_REQUEST  mDfciNetworkRequest;

#endif // __DFCI_PRIVATE_H__
