/** @file
DfciRequest.c

This module will request new DFCI configuration data from server.

Copyright (C) Microsoft Corporation. All rights reserved.
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>

#include <IndustryStandard/Http11.h>

#include <Guid/DfciPacketHeader.h>
#include <Guid/DfciIdentityAndAuthManagerVariables.h>
#include <Guid/DfciPermissionManagerVariables.h>
#include <Guid/DfciSettingsManagerVariables.h>
#include <Guid/ImageAuthentication.h>
#include <Guid/TlsAuthentication.h>

#include <Protocol/BootManagerPolicy.h>
#include <Protocol/Http.h>
#include <Protocol/Ip4Config2.h>
#include <Protocol/ServiceBinding.h>
#include <Protocol/TlsConfig.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/HttpLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/NetLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>

#include <Settings/DfciSettings.h>
#include <Settings/DfciPrivateSettings.h>

// #include "DfciMenu.h"
#include "DfciPrivate.h"
#include "DfciRequest.h"
#include "DfciUpdate.h"

#define TIMER_PERIOD_1ms  (1000 * 10)       // 1ms value for relative timer.
#define TIMER_PERIOD_1s   (1000 * TIMER_PERIOD_1ms)
#define DHCP_TIMEOUT      (180 * TIMER_PERIOD_1s)
#define HTTP_TIMEOUT      (90 * TIMER_PERIOD_1s)

#define HEADER_AGENT_VALUE   "DFCI-Agent"
#define HEADER_ACCEPT_VALUE  "*/*"
#define HEADER_CONTENT_JSON  "application/json"
#define HEADER_RETRY_AFTER   "Retry-After"
#define HEADER_LOCATION      "Location"

STATIC VOID    *mOldCertificateList = NULL;
STATIC UINTN   mOldCertificateSize;
STATIC UINT32  mOldCertificateAttr;

/**
 * Dump the HTTP Headers
 *
 * @param[in]  Count
 * @param[out] Headers
 *
 * @return VOID
 *
 */
STATIC
VOID
DumpHeaders (
  IN  UINTN            Count,
  IN  EFI_HTTP_HEADER  *Headers
  )
{
  DEBUG_CODE (
    UINTN   Index;

    for (Index = 0; Index < Count; Index++) {
    DEBUG ((
      DEBUG_INFO,
      "  %d - %a: %a\n",
      Index + 1,
      Headers[Index].FieldName,
      Headers[Index].FieldValue
      ));
  }

    );
}

/**
  This function is to display the HTTP error status.

  @param[in]      StatusCode      The status code value in HTTP message.

**/
CHAR8 *
GetHttpErrorMsg (
  EFI_HTTP_STATUS_CODE  StatusCode
  )
{
  CHAR8  *Msg;

  switch (StatusCode) {
    case HTTP_STATUS_100_CONTINUE:
      Msg = "Informational: 100 Continue";
      break;

    case HTTP_STATUS_101_SWITCHING_PROTOCOLS:
      Msg = "Informational: 301 Switching Protocols";
      break;

    case HTTP_STATUS_200_OK:
      Msg = "Success: 200 OK";
      break;

    case HTTP_STATUS_201_CREATED:
      Msg = "Success: 201 Created";
      break;

    case HTTP_STATUS_202_ACCEPTED:
      Msg = "Success: 202 Accepted";
      break;

    case HTTP_STATUS_203_NON_AUTHORITATIVE_INFORMATION:
      Msg = "Success: 203 Non Authoritative Information";
      break;

    case HTTP_STATUS_204_NO_CONTENT:
      Msg = "Success: 3204 No Content";
      break;

    case HTTP_STATUS_205_RESET_CONTENT:
      Msg = "Success: 205 Reset Content";
      break;

    case HTTP_STATUS_206_PARTIAL_CONTENT:
      Msg = "Success: 206 Partial Choices";
      break;

    case HTTP_STATUS_300_MULTIPLE_CHOICES:
      Msg = "Redirection: 300 Multiple Choices";
      break;

    case HTTP_STATUS_301_MOVED_PERMANENTLY:
      Msg = "Redirection: 301 Moved Permanently";
      break;

    case HTTP_STATUS_302_FOUND:
      Msg = "Redirection: 302 Found";
      break;

    case HTTP_STATUS_303_SEE_OTHER:
      Msg = "Redirection: 303 See Other";
      break;

    case HTTP_STATUS_304_NOT_MODIFIED:
      Msg = "Redirection: 304 Not Modified";
      break;

    case HTTP_STATUS_305_USE_PROXY:
      Msg = "Redirection: 305 Use Proxy";
      break;

    case HTTP_STATUS_307_TEMPORARY_REDIRECT:
      Msg = "Redirection: 307 Temporary Redirect";
      break;

    case HTTP_STATUS_308_PERMANENT_REDIRECT:
      Msg = "Redirection: 308 Permanent Redirect";
      break;

    case HTTP_STATUS_400_BAD_REQUEST:
      Msg = "Client Error: 400 Bad Request";
      break;

    case HTTP_STATUS_401_UNAUTHORIZED:
      Msg = "Client Error: 401 Unauthorized";
      break;

    case HTTP_STATUS_402_PAYMENT_REQUIRED:
      Msg = "Client Error: 402 Payment Required";
      break;

    case HTTP_STATUS_403_FORBIDDEN:
      Msg = "Client Error: 403 Forbidden";
      break;

    case HTTP_STATUS_404_NOT_FOUND:
      Msg = "Client Error: 404 Not Found";
      break;

    case HTTP_STATUS_405_METHOD_NOT_ALLOWED:
      Msg = "Client Error: 405 Method Not Allowed";
      break;

    case HTTP_STATUS_406_NOT_ACCEPTABLE:
      Msg = "Client Error: 406 Not Acceptable";
      break;

    case HTTP_STATUS_407_PROXY_AUTHENTICATION_REQUIRED:
      Msg = "Client Error: 407 Proxy Authentication Required";
      break;

    case HTTP_STATUS_408_REQUEST_TIME_OUT:
      Msg = "Client Error: 408 Request Timeout";
      break;

    case HTTP_STATUS_409_CONFLICT:
      Msg = "Client Error: 409 Conflict";
      break;

    case HTTP_STATUS_410_GONE:
      Msg = "Client Error: 410 Gone";
      break;

    case HTTP_STATUS_411_LENGTH_REQUIRED:
      Msg = "Client Error: 411 Length Required";
      break;

    case HTTP_STATUS_412_PRECONDITION_FAILED:
      Msg = "Client Error: 412 Precondition Failed";
      break;

    case HTTP_STATUS_413_REQUEST_ENTITY_TOO_LARGE:
      Msg = "Client Error: 413 Request Entity Too Large";
      break;

    case HTTP_STATUS_414_REQUEST_URI_TOO_LARGE:
      Msg = "Client Error: 414 Request URI Too Long";
      break;

    case HTTP_STATUS_415_UNSUPPORTED_MEDIA_TYPE:
      Msg = "Client Error: 415 Unsupported Media Type";
      break;

    case HTTP_STATUS_416_REQUESTED_RANGE_NOT_SATISFIED:
      Msg = "Client Error: 416 Requested Range Not Satisfiable";
      break;

    case HTTP_STATUS_417_EXPECTATION_FAILED:
      Msg = "Client Error: 417 Expectation Failed";
      break;

    case HTTP_STATUS_429_TOO_MANY_REQUESTS:
      Msg = "Server Error: 429 Too Many Requests";
      break;

    case HTTP_STATUS_500_INTERNAL_SERVER_ERROR:
      Msg = "Server Error: 500 Internal Server Error";
      break;

    case HTTP_STATUS_501_NOT_IMPLEMENTED:
      Msg = "Server Error: 501 Not Implemented";
      break;

    case HTTP_STATUS_502_BAD_GATEWAY:
      Msg = "Server Error: 502 Bad Gateway";
      break;

    case HTTP_STATUS_503_SERVICE_UNAVAILABLE:
      Msg = "Server Error: 503 Service Unavailable";
      break;

    case HTTP_STATUS_504_GATEWAY_TIME_OUT:
      Msg = "Server Error: 504 Gateway Timeout";
      break;

    case HTTP_STATUS_505_HTTP_VERSION_NOT_SUPPORTED:
      Msg = "Server Error: 505 HTTP Version Not Supported";
      break;

    case HTTP_STATUS_UNSUPPORTED_STATUS:
      Msg = "Http Error. Unsupported Status";
      break;

    default:
      Msg = "Unknown HTTP Error";
  }

  return Msg;
}

/**
 * Wait for an event to be signaled.  While waiting, Poll the HTTP operation
 *
 * @param[in]  NetworkRequest - Internal data
 * @param[in]  MainEvent      - The event to wait on
 * @param[in]  Timeout        - Timeout in SetTimer units
 *
 * If the parameter Dfci is NULL, only timeout - no event, no poll
 *
 * @return EFI_STATUS  - EFI_SUCCESS - Wait completed normally
 *                       EFI_TIMEOUT - Event Timed out
 *                       Other       - Wait failed for other reasons
 *
 **/
STATIC
EFI_STATUS
EventWait (
  IN  DFCI_NETWORK_REQUEST  *NetworkRequest  OPTIONAL,
  IN  EFI_EVENT             MainEvent,
  IN  UINTN                 Timeout
  )
{
  BOOLEAN     Failed;
  EFI_STATUS  Status;
  UINTN       Step;
  EFI_EVENT   TimeOutEvent;

  Failed = TRUE;
  do {
    Step         = 1;
    TimeOutEvent = NULL;
    Status       = gBS->CreateEvent (EVT_TIMER, 0, NULL, NULL, &TimeOutEvent);
    if (EFI_ERROR (Status)) {
      break;
    }

    Step = 2;
    //
    // Set the timeout event
    //
    Status = gBS->SetTimer (
                    TimeOutEvent,
                    TimerRelative,
                    Timeout
                    );
    if (EFI_ERROR (Status)) {
      break;
    }

    Step = 3;
    do {
      if (NULL != NetworkRequest) {
        if (NULL != NetworkRequest->Http.HttpProtocol) {
          NetworkRequest->Http.HttpProtocol->Poll (NetworkRequest->Http.HttpProtocol);
        }

        if (EFI_SUCCESS == gBS->CheckEvent (MainEvent)) {
          Status = EFI_SUCCESS;
          break;
        }
      }

      if (EFI_SUCCESS == gBS->CheckEvent (TimeOutEvent)) {
        Status = EFI_TIMEOUT;
        break;
      }
    } while (TRUE);

    Failed = FALSE;        // Once we get here, the processed is complete

    Step = 4;
    gBS->SetTimer (TimeOutEvent, TimerCancel, 0);
    gBS->CloseEvent (TimeOutEvent);
  } while (FALSE);

  if (Failed) {
    DEBUG ((DEBUG_ERROR, "Wait error at step %d - code=%r\n", Step, Status));
  }

  return Status;
}

/**
 *  Timer Tick handler  - Update the list of devices
 *
 *  @param[in] Event      Event that signaled the callback.
 *  @param[in] Context    Pointer to an optional event context.
 *
 *  @retval None.
 *
 **/
STATIC
VOID
EFIAPI
TimerTick (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  UINTN                           DataSize;
  EFI_IP4_CONFIG2_INTERFACE_INFO  *Info;
  EFI_IP4_CONFIG2_PROTOCOL        *Ip4Config2;
  DFCI_NETWORK_REQUEST            *NetworkRequest;
  EFI_STATUS                      Status;

  NetworkRequest = (DFCI_NETWORK_REQUEST *)Context;

  Status = gBS->HandleProtocol (
                  NetworkRequest->HttpNic.NicHandle,
                  &gEfiIp4Config2ProtocolGuid,
                  (VOID **)&Ip4Config2
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Tick error locating IPv4 Config2 protocol. Code=%r\n", Status));
    if (NULL != NetworkRequest->HttpNic.WaitEvent) {
      gBS->SignalEvent (NetworkRequest->HttpNic.WaitEvent);
    }

    return;
  }

  DataSize = 0;
  Status   = Ip4Config2->GetData (
                           Ip4Config2,
                           Ip4Config2DataTypeInterfaceInfo,
                           &DataSize,
                           &Info
                           );
  if (EFI_BUFFER_TOO_SMALL != Status) {
    DEBUG ((DEBUG_ERROR, "Error obtaining IP4 Interface Info size. Code=%r\n", Status));
  } else {
    Info = AllocatePool (DataSize);
    if (NULL == Info) {
      DEBUG ((DEBUG_ERROR, "Error allocating %d bytes for Info\n", DataSize));
    } else {
      Info->StationAddress.Addr[0] = 0;
      Status                       = Ip4Config2->GetData (
                                                   Ip4Config2,
                                                   Ip4Config2DataTypeInterfaceInfo,
                                                   &DataSize,
                                                   Info
                                                   );
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "Error obtaining IP4 Interface Info. Code=%r\n", Status));
        DEBUG ((
          DEBUG_ERROR,
          " DataSize=%d, StructSize=%d\n",
          DataSize,
          sizeof (EFI_IP4_CONFIG2_INTERFACE_INFO)
          ));
      } else {
        if (Info->StationAddress.Addr[0] != 0) {
          if (NULL != NetworkRequest->HttpNic.WaitEvent) {
            gBS->SignalEvent (NetworkRequest->HttpNic.WaitEvent);
          }

          DEBUG ((
            DEBUG_INFO,
            "DHCP Local Address is %d.%d.%d.%d.\n",
            Info->StationAddress.Addr[0],
            Info->StationAddress.Addr[1],
            Info->StationAddress.Addr[2],
            Info->StationAddress.Addr[3]
            ));
        }
      }

      FreePool (Info);
    }
  }
}

/**
 * Unconfigure the NIC
 *
 * @param NetworkRequest  - Internal data
 *
 * @retval EFI_STATUS
 *
 **/
STATIC
EFI_STATUS
UnconfigureNIC (
  IN DFCI_NETWORK_REQUEST  *NetworkRequest
  )
{
  EFI_IP4_CONFIG2_PROTOCOL  *Ip4Config2;
  EFI_IP4_CONFIG2_POLICY    Policy;
  EFI_STATUS                Status;

  Status = gBS->HandleProtocol (
                  NetworkRequest->HttpNic.NicHandle,
                  &gEfiIp4Config2ProtocolGuid,
                  (VOID **)&Ip4Config2
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Error locating IPv4 Config2 protocol. Code=%r\n", Status));
    return Status;
  }

  //
  // 1. Set Policy to Static IP
  //
  Policy = Ip4Config2PolicyStatic;
  Status = Ip4Config2->SetData (
                         Ip4Config2,
                         Ip4Config2DataTypePolicy,
                         sizeof (EFI_IP4_CONFIG2_POLICY),
                         &Policy
                         );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Unable to set policy to static.. Code=%r\n", Status));
    return Status;
  }

  //
  // 2. Set IP Addr and Subnet to 0.0.0.0 0.0.0.0
  //

  Status = Ip4Config2->SetData (
                         Ip4Config2,
                         Ip4Config2DataTypeManualAddress,
                         0,
                         NULL
                         );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Unable to clear IP4 Configuration. Code=%r\n", Status));
  }

  //
  // 3. Set Gateway address to 0.0.0.0
  //
  if (!EFI_ERROR (Status)) {
    Status = Ip4Config2->SetData (
                           Ip4Config2,
                           Ip4Config2DataTypeGateway,
                           0,
                           NULL
                           );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Error setting GateWay address. Code=%r\n", Status));
    }
  }

  return Status;
}

/**
 * Configure DHCP address
 *
 * @param[in]  NetworkRequest
 *
 * @return EFI_STATUS
 *
 **/
STATIC
EFI_STATUS
ConfigureDHCP (
  IN DFCI_NETWORK_REQUEST  *NetworkRequest
  )
{
  EFI_IP4_CONFIG2_PROTOCOL  *Ip4Config2;
  EFI_IP4_CONFIG2_POLICY    Policy;
  EFI_STATUS                Status;
  EFI_EVENT                 TimerEvent;

  if (NULL == NetworkRequest) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // 1. Set state back to Static
  //
  Status = gBS->CreateEvent (0, 0, NULL, NULL, &NetworkRequest->HttpNic.WaitEvent);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Unable to create wait event. Code=%r\n", Status));
    return Status;
  }

  do {
    Status = UnconfigureNIC (NetworkRequest);

    if (EFI_ERROR (Status)) {
      break;
    }

    Status = gBS->HandleProtocol (
                    NetworkRequest->HttpNic.NicHandle,
                    &gEfiIp4Config2ProtocolGuid,
                    (VOID **)&Ip4Config2
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Error locating IPv4 Config2 protocol. Code=%r\n", Status));
      return Status;
    }

    //
    // Set policy to DHCP - this should start a DHCP DORA
    //
    Policy = Ip4Config2PolicyDhcp;
    Status = Ip4Config2->SetData (
                           Ip4Config2,
                           Ip4Config2DataTypePolicy,
                           sizeof (EFI_IP4_CONFIG2_POLICY),
                           &Policy
                           );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Unable to set DHCP address. Code=%r\n", Status));
    }

    NetworkRequest->HttpNic.DhcpRequested = TRUE;        // Remember to set back to STATIC

    //
    // 3. Poll the IP4 Address until valid
    //
    Status = gBS->CreateEvent (
                    EVT_TIMER | EVT_NOTIFY_SIGNAL,
                    TPL_CALLBACK,
                    TimerTick,
                    (VOID *)NetworkRequest,
                    &TimerEvent
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Unable to create event DHCP Completion. Code=%r\n", Status));
      break;
    }

    //
    // Set the timer event to poll for an IP4 address every 1 second
    //
    Status = gBS->SetTimer (
                    TimerEvent,
                    TimerPeriodic,
                    TIMER_PERIOD_1s
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Unable to set timer for DHCP Completion. Code=%r\n", Status));
      gBS->CloseEvent (TimerEvent);
      break;
    }

    Status = EventWait (NetworkRequest, NetworkRequest->HttpNic.WaitEvent, DHCP_TIMEOUT);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Error from wait on DHCP address. Code=%r\n", Status));
    } else {
      DEBUG ((DEBUG_INFO, "DHCP Address satisfied.\n"));
    }

    gBS->SetTimer (
           TimerEvent,
           TimerCancel,
           0
           );
    gBS->CloseEvent (TimerEvent);
  } while (FALSE);

  gBS->CloseEvent (NetworkRequest->HttpNic.WaitEvent);
  NetworkRequest->HttpNic.WaitEvent = NULL;

  return Status;
}

/**
 * Configure the NIC for an IPv4 or IPv6 address
 *
 *
 * @param NetworkRequest
 *
 * @return EFI_STATUS
 *
 */
STATIC
EFI_STATUS
ConfigureHTTP (
  IN DFCI_NETWORK_REQUEST  *NetworkRequest
  )
{
  EFI_IP4_CONFIG2_MANUAL_ADDRESS  Address;
  UINTN                           DataSize;
  EFI_IP4_CONFIG2_PROTOCOL        *Ip4Config2;
  EFI_STATUS                      Status;

  if (NULL == NetworkRequest) {
    return EFI_INVALID_PARAMETER;
  }

  if (NetworkRequest->Http.ConfigData.LocalAddressIsIPv6) {
    DEBUG ((DEBUG_ERROR, "IPv6 is not supported yet\n"));
    Status = EFI_UNSUPPORTED;
  } else {
    //
    // Initialize HTTP Config Data
    //
    NetworkRequest->Http.ConfigData.HttpVersion        = HttpVersion11;
    NetworkRequest->Http.ConfigData.TimeOutMillisec    = 0;              // Indicates default timeout period
    NetworkRequest->Http.ConfigData.LocalAddressIsIPv6 = FALSE;

    ZeroMem (&NetworkRequest->HttpNic.IPv4Node, sizeof (NetworkRequest->HttpNic.IPv4Node));
    NetworkRequest->HttpNic.IPv4Node.UseDefaultAddress   = TRUE;            // Use address configured already
    NetworkRequest->Http.ConfigData.AccessPoint.IPv4Node = &NetworkRequest->HttpNic.IPv4Node;

    //
    // Check the current IP Address.  If an IP address not present, ConfigureDHCP
    //
    Status = gBS->HandleProtocol (
                    NetworkRequest->HttpNic.NicHandle,
                    &gEfiIp4Config2ProtocolGuid,
                    (VOID **)&Ip4Config2
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Error locating IPv4 Config2 protocol. Code=%r\n", Status));
      return Status;
    }

    DataSize = sizeof (EFI_IP4_CONFIG2_MANUAL_ADDRESS);
    Status   = Ip4Config2->GetData (
                             Ip4Config2,
                             Ip4Config2DataTypeManualAddress,
                             &DataSize,
                             &Address
                             );
    if (EFI_ERROR (Status) || (Address.Address.Addr[0] == 0)) {
      DEBUG ((DEBUG_ERROR, "Configuring DHCP for DFCI. Code=%r\n", Status));
      Status = ConfigureDHCP (NetworkRequest);
    }
  }

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Error configuring HTTP. Code=%r\n", Status));
    return Status;
  }

  //
  // Continue with Common Configuration.
  //

  Status = NetworkRequest->HttpNic.HttpSbProtocol->CreateChild (
                                                     NetworkRequest->HttpNic.HttpSbProtocol,
                                                     &NetworkRequest->Http.HttpChildHandle
                                                     );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Error creating worker child. Code=%r\n", Status));
    return Status;
  }

  NetworkRequest->Http.HttpProtocol = NULL;
  Status                            = gBS->HandleProtocol (
                                             NetworkRequest->Http.HttpChildHandle,
                                             &gEfiHttpProtocolGuid,
                                             (VOID **)&NetworkRequest->Http.HttpProtocol
                                             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Unable to locate HTTP protocol. Code=%r\n", Status));
    return Status;
  }

  Status = NetworkRequest->Http.HttpProtocol->Configure (
                                                NetworkRequest->Http.HttpProtocol,
                                                &NetworkRequest->Http.ConfigData
                                                );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Unable to configure HTTP Protocol. Code=%r\n", Status));
    return Status;
  }

  return Status;
}

/**
 * DfciBuildRequestHeaders
 *
 * @param[in]  Url
 * @param[in]  BodyLength
 * @param[in]  ContentType
 * @param[out] Headers
 * @param[out] Count
 *
 *
 * @return EFI_STATUS
 *
 **/
STATIC
EFI_STATUS
DfciBuildRequestHeaders (
  IN  CHAR8            *Url,
  IN  UINTN            BodyLength,
  IN  CONST CHAR8      *ContentType,
  OUT EFI_HTTP_HEADER  **Headers,
  OUT UINTN            *Count
  )
{
  EFI_STATUS       Status;
  VOID             *UrlParser;
  EFI_HTTP_HEADER  *RequestHeaders;
  UINTN            HeaderCount;
  CHAR8            ContentLengthString[21];           // 2**64 is 1.8E19, or 20 digits. +1 for a NULL.

  if ((NULL == Url) ||
      (NULL == ContentType) ||
      (NULL == Headers) ||
      (NULL == Count))
  {
    return EFI_INVALID_PARAMETER;
  }

  *Headers = NULL;
  *Count   = 0;

  if (0 != BodyLength) {
    HeaderCount = 5;
  } else {
    HeaderCount = 4;
  }

  RequestHeaders = AllocateZeroPool (sizeof (EFI_HTTP_HEADER) * HeaderCount);   // Allocate headers
  if (NULL == RequestHeaders) {
    return EFI_OUT_OF_RESOURCES;
  }

  *Headers = RequestHeaders;
  *Count   = HeaderCount;

  UrlParser = NULL;

  Status = HttpParseUrl (Url, (UINT32)AsciiStrLen (Url), FALSE, &UrlParser);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Don't check each AllocateCopyPool.  All of the pointers are initialized as NULL, and those that work
  // will be freed by HttpFreeHeaderFields.  Leave the failure detection to the HTTP operation failing.
  //
  RequestHeaders[0].FieldName  = AllocateCopyPool (AsciiStrSize (HTTP_HEADER_HOST), HTTP_HEADER_HOST);
  RequestHeaders[1].FieldName  = AllocateCopyPool (AsciiStrSize (HTTP_HEADER_USER_AGENT), HTTP_HEADER_USER_AGENT);
  RequestHeaders[1].FieldValue = AllocateCopyPool (AsciiStrSize (HEADER_AGENT_VALUE), HEADER_AGENT_VALUE);
  RequestHeaders[2].FieldName  = AllocateCopyPool (AsciiStrSize (HTTP_HEADER_ACCEPT), HTTP_HEADER_ACCEPT);
  RequestHeaders[2].FieldValue = AllocateCopyPool (AsciiStrSize (HEADER_ACCEPT_VALUE), HEADER_ACCEPT_VALUE);
  RequestHeaders[3].FieldName  = AllocateCopyPool (AsciiStrSize (HTTP_HEADER_CONTENT_LENGTH), HTTP_HEADER_CONTENT_LENGTH);
  AsciiSPrint (ContentLengthString, sizeof (ContentLengthString), "%ld", BodyLength);
  RequestHeaders[3].FieldValue = AllocateCopyPool (AsciiStrSize (ContentLengthString), ContentLengthString);

  if ((RequestHeaders[0].FieldName == NULL) ||
      (RequestHeaders[1].FieldName == NULL) ||
      (RequestHeaders[1].FieldValue == NULL) ||
      (RequestHeaders[2].FieldName == NULL) ||
      (RequestHeaders[2].FieldValue == NULL) ||
      (RequestHeaders[3].FieldName == NULL) ||
      (RequestHeaders[3].FieldValue == NULL))
  {
    Status = EFI_OUT_OF_RESOURCES;
    goto Exit;
  }

  Status = HttpUrlGetHostName (Url, UrlParser, &RequestHeaders[0].FieldValue);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Unable to get Host Name from URL\n"));
    goto Exit;
  }

  if (0 != BodyLength) {
    RequestHeaders[4].FieldName  = AllocateCopyPool (AsciiStrSize (HTTP_HEADER_CONTENT_TYPE), HTTP_HEADER_CONTENT_TYPE);
    RequestHeaders[4].FieldValue = AllocateCopyPool (AsciiStrSize (ContentType), ContentType);
    if ((RequestHeaders[4].FieldName == NULL) ||
        (RequestHeaders[4].FieldValue == NULL))
    {
      Status = EFI_OUT_OF_RESOURCES;
      goto Exit;
    }
  }

Exit:
  if (UrlParser != NULL) {
    HttpUrlFreeParser (UrlParser);
  }

  return Status;
}

/**
 * DfciIssueRequest
 *
 *
 * @param[in]  NetworkRequest
 * @param[in]  RequestToken
 *
 * @return EFI_STATUS
 *
 **/
STATIC
EFI_STATUS
DfciIssueRequest (
  IN  DFCI_NETWORK_REQUEST  *NetworkRequest,
  IN  EFI_HTTP_TOKEN        *RequestToken
  )
{
  EFI_HTTP_REQUEST_DATA  *RequestData;
  EFI_HTTP_MESSAGE       *RequestMessage;
  EFI_STATUS             Status;
  EFI_STATUS             Status2;

  if ((NULL == NetworkRequest) ||
      (NULL == RequestToken))
  {
    return EFI_INVALID_PARAMETER;
  }

  Status = gBS->CreateEvent (0, 0, NULL, NULL, &RequestToken->Event);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Unable to create callback event. Code=%r\n", Status));
    return Status;
  }

  RequestMessage = RequestToken->Message;
  RequestData    = RequestMessage->Data.Request;

  DEBUG ((DEBUG_INFO, "Making Request - Headers:\n"));
  DumpHeaders (RequestMessage->HeaderCount, RequestMessage->Headers);
  DEBUG ((DEBUG_INFO, "HttpRequestToken:\n"));
  DUMP_HEX (DEBUG_INFO, 0, RequestToken, sizeof (EFI_HTTP_TOKEN), "");
  DUMP_HEX (DEBUG_INFO, 0, RequestMessage, sizeof (EFI_HTTP_MESSAGE), "");
  DUMP_HEX (DEBUG_INFO, 0, RequestData, sizeof (EFI_HTTP_REQUEST_DATA), "");
  DEBUG ((DEBUG_INFO, "%p Url=%s\n", RequestData->Url, RequestData->Url));

  Status = NetworkRequest->Http.HttpProtocol->Request (NetworkRequest->Http.HttpProtocol, RequestToken);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Http Request failed. Code=%r\n", Status));
    gBS->CloseEvent (RequestToken->Event);
    return Status;
  }

  Status = EventWait (NetworkRequest, RequestToken->Event, HTTP_TIMEOUT);
  gBS->CloseEvent (RequestToken->Event);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Http request timed out\n"));
    Status2 = NetworkRequest->Http.HttpProtocol->Cancel (NetworkRequest->Http.HttpProtocol, RequestToken);
    if (EFI_ERROR (Status2)) {
      DEBUG ((DEBUG_ERROR, "Http Cancel failed. Code=%r\n", Status));
    }
  }

  DEBUG ((DEBUG_INFO, "Request Token status = %r\n", RequestToken->Status));
  DEBUG ((DEBUG_INFO, "DfciIssueRequest status = %r\n", Status));

  return Status;
}

/**
 * DfciGetResponse
 *
 *
 * @param[in]  NetworkRequest
 * @param[in]  ResponseToken
 *
 * @return EFI_STATUS
 *
 **/
STATIC
EFI_STATUS
DfciGetResponse (
  IN  DFCI_NETWORK_REQUEST  *NetworkRequest,
  IN  EFI_HTTP_TOKEN        *ResponseToken
  )
{
  EFI_HTTP_RESPONSE_DATA  *ResponseData;
  EFI_HTTP_MESSAGE        *ResponseMessage;
  EFI_STATUS              Status;
  EFI_STATUS              Status2;

  if ((NULL == NetworkRequest) ||
      (NULL == ResponseToken))
  {
    return EFI_INVALID_PARAMETER;
  }

  Status = gBS->CreateEvent (0, 0, NULL, NULL, &ResponseToken->Event);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Unable to create callback event. Code=%r\n", Status));
    return Status;
  }

  ResponseMessage = ResponseToken->Message;
  ResponseData    = ResponseMessage->Data.Response;

  DEBUG ((DEBUG_INFO, "HttpResponseToken:\n"));
  DUMP_HEX (DEBUG_INFO, 0, ResponseToken, sizeof (EFI_HTTP_TOKEN), "");
  DUMP_HEX (DEBUG_INFO, 0, ResponseMessage, sizeof (EFI_HTTP_MESSAGE), "");
  if (NULL != ResponseData) {
    DUMP_HEX (DEBUG_INFO, 0, ResponseData, sizeof (EFI_HTTP_RESPONSE_DATA), "");
  }

  Status = NetworkRequest->Http.HttpProtocol->Response (NetworkRequest->Http.HttpProtocol, ResponseToken);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Http Response failed. Code=%r\n", Status));
    return Status;
  }

  Status = EventWait (NetworkRequest, ResponseToken->Event, HTTP_TIMEOUT);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Http Response timeout.\n"));
    Status2 = NetworkRequest->Http.HttpProtocol->Cancel (NetworkRequest->Http.HttpProtocol, ResponseToken);
    if (EFI_ERROR (Status2)) {
      DEBUG ((DEBUG_ERROR, "Http HttpCancel failed. Code=%r", Status));
    }
  }

  if (NULL != ResponseData) {
    DEBUG ((DEBUG_INFO, "Response status is %d, %a\n", ResponseData->StatusCode, GetHttpErrorMsg (ResponseData->StatusCode)));
    NetworkRequest->HttpStatus.HttpStatus = ResponseData->StatusCode;
  }

  DEBUG ((DEBUG_INFO, "Received %d headers\n", ResponseMessage->HeaderCount));

  DumpHeaders (ResponseMessage->HeaderCount, ResponseMessage->Headers);
  DEBUG ((DEBUG_INFO, "HttpResponseData:\n"));
  DUMP_HEX (DEBUG_INFO, 0, ResponseToken, sizeof (EFI_HTTP_TOKEN), "");
  if ((ResponseMessage->Body != NULL) && (ResponseMessage->BodyLength != 0)) {
    DUMP_HEX (DEBUG_INFO, 0, ResponseMessage->Body, ResponseMessage->BodyLength, "");
  }

  return Status;
}

#define CLEANUP_REQUEST   0x01
#define CLEANUP_RESPONSE  0x02
#define CLEANUP_STATUS    0x04
#define CLEANUP_HTTP      0x08
#define CLEANUP_NIC       0x10
#define CLEANUP_MAIN      0x20
#define CLEAR_ALL         0x80

#define CLEANUP_ALL  (CLEANUP_REQUEST | CLEANUP_RESPONSE | CLEANUP_STATUS | CLEANUP_HTTP | CLEANUP_NIC | CLEANUP_MAIN)

/**
 * Cleanup Network Request.
 *
 * @param[in]  NetworkRequest
 * @param[in]  CleanupMask
 *
 * Free caller responsible items in:
 *
 *    CLEANUP_xxx   Just clean up the requested zone(s) then zero the zone
 *    CLEAR_ALL     Just zero all of the zones
**/
EFI_STATUS
CleanupNetworkRequest (
  IN DFCI_NETWORK_REQUEST  *NetworkRequest,
  IN UINT8                 CleanupMask
  )
{
  DEBUG ((DEBUG_INFO, "Cleanup on aisle %x\n", CleanupMask));
  //
  // Request Zone
  //
  if (CleanupMask & CLEANUP_REQUEST) {
    if (NetworkRequest->HttpRequest.Url != NULL) {
      FreePool (NetworkRequest->HttpRequest.Url);
    }

    if (NetworkRequest->HttpRequest.BootstrapUrl != NULL) {
      FreePool (NetworkRequest->HttpRequest.BootstrapUrl);
    }
  }

  if (CleanupMask & (CLEANUP_REQUEST | CLEAR_ALL)) {
    ZeroMem (&NetworkRequest->HttpRequest, sizeof (NetworkRequest->HttpRequest));
  }

  //
  // Response Zone
  //
  if (CleanupMask & CLEANUP_RESPONSE) {
    if (NetworkRequest->HttpResponse.Body != NULL) {
      FreePool (NetworkRequest->HttpResponse.Body);
    }

    if (NetworkRequest->HttpResponse.Headers != NULL) {
      HttpFreeHeaderFields (NetworkRequest->HttpResponse.Headers, NetworkRequest->HttpResponse.HeaderCount);
    }
  }

  if (CleanupMask & (CLEANUP_RESPONSE | CLEAR_ALL)) {
    ZeroMem (&NetworkRequest->HttpResponse, sizeof (NetworkRequest->HttpResponse));
  }

  //
  // Status Zone
  //
  if (CleanupMask & CLEANUP_STATUS) {
    if (NULL != NetworkRequest->HttpStatus.HttpMessage) {
      FreePool (NetworkRequest->HttpStatus.HttpMessage);
    }

    if (NULL != NetworkRequest->HttpStatus.HttpReturnCode) {
      FreePool (NetworkRequest->HttpStatus.HttpReturnCode);
    }
  }

  if (CleanupMask & (CLEANUP_STATUS | CLEAR_ALL)) {
    ZeroMem (&NetworkRequest->HttpStatus, sizeof (NetworkRequest->HttpStatus));
  }

  //
  // Http zone
  //
  if (CleanupMask & (CLEANUP_HTTP | CLEAR_ALL)) {
    ZeroMem (&NetworkRequest->Http, sizeof (NetworkRequest->Http));
  }

  //
  // NIC Zone
  //
  if (CleanupMask & (CLEANUP_NIC | CLEAR_ALL)) {
    ZeroMem (&NetworkRequest->HttpNic, sizeof (NetworkRequest->HttpNic));
  }

  //
  // Main Zone
  //
  if (CleanupMask & CLEANUP_MAIN) {
    if (NULL != NetworkRequest->Main.RegistrationEvent) {
      gBS->CloseEvent (NetworkRequest->Main.RegistrationEvent);
    }
  }

  if (CleanupMask & (CLEANUP_MAIN | CLEAR_ALL)) {
    ZeroMem (&NetworkRequest->Main, sizeof (NetworkRequest->Main));
  }

  DEBUG ((DEBUG_INFO, "Cleanup on aisle %x complete.\n", CleanupMask));
  return EFI_SUCCESS;
}

EFI_STATUS
InitializeNetworkRequest (
  IN DFCI_NETWORK_REQUEST  *NetworkRequest
  )
{
  EFI_STATUS  Status;

  CleanupNetworkRequest (NetworkRequest, CLEAR_ALL);

  Status = DfciGetASetting (
             DFCI_PRIVATE_SETTING_ID__DFCI_RECOVERY_URL,
             DFCI_SETTING_TYPE_STRING,
             (VOID **)&NetworkRequest->HttpRequest.Url,
             &NetworkRequest->HttpRequest.UrlSize
             );
  if (EFI_ERROR (Status) || (NetworkRequest->HttpRequest.UrlSize <= 1)) {
    DEBUG ((DEBUG_ERROR, "%a: Unable to get RecoveryURL. Code=%r\n", __FUNCTION__, Status));
    goto INITIALIZE_CLEANUP;
  }

  Status = DfciGetASetting (
             DFCI_PRIVATE_SETTING_ID__DFCI_BOOTSTRAP_URL,
             DFCI_SETTING_TYPE_STRING,
             (VOID **)&NetworkRequest->HttpRequest.BootstrapUrl,
             &NetworkRequest->HttpRequest.BootstrapUrlSize
             );
  if (EFI_ERROR (Status) || (NetworkRequest->HttpRequest.BootstrapUrlSize <= 1)) {
    DEBUG ((DEBUG_ERROR, "%a: Unable to get BootstrapURL. Code=%r\n", __FUNCTION__, Status));
    goto INITIALIZE_CLEANUP;
  }

INITIALIZE_CLEANUP:

  return Status;
}

STATIC
EFI_STATUS
ProcessHttpRequest (
  IN  DFCI_NETWORK_REQUEST  *NetworkRequest,
  IN  EFI_HTTP_METHOD       HttpMethod,
  IN  CHAR8                 *Url
  )
{
  UINTN                   ContentLength;
  EFI_HTTP_HEADER         *ContentLengthHeader;
  UINTN                   CurrentLength;
  CHAR8                   *Packet;
  EFI_HTTP_REQUEST_DATA   RequestData;
  EFI_HTTP_MESSAGE        RequestMessage;
  EFI_HTTP_TOKEN          RequestToken;
  EFI_HTTP_RESPONSE_DATA  ResponseData;
  EFI_HTTP_MESSAGE        ResponseMessage;
  EFI_HTTP_TOKEN          ResponseToken;
  EFI_STATUS              Status;

  RequestData.Method = HttpMethod;

  Status = DfciConvertToCHAR16 (Url, AsciiStrLen (Url), &RequestData.Url, NULL);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Send request without the terminating NULL.
  if (NetworkRequest->HttpRequest.BodySize == 0) {
    RequestMessage.BodyLength = 0;
  } else {
    RequestMessage.BodyLength = NetworkRequest->HttpRequest.BodySize - sizeof (CHAR8);
  }

  RequestMessage.Body         = NetworkRequest->HttpRequest.Body;
  RequestMessage.Data.Request = &RequestData;
  RequestMessage.Headers      = NULL;
  RequestMessage.HeaderCount  = 0;

  RequestToken.Event   = NULL;
  RequestToken.Status  = EFI_SUCCESS;
  RequestToken.Message = &RequestMessage;

  Status = DfciBuildRequestHeaders (
             Url,
             RequestMessage.BodyLength,
             HEADER_CONTENT_JSON,
             &RequestMessage.Headers,
             &RequestMessage.HeaderCount
             );
  if (EFI_ERROR (Status)) {
    goto S_EXIT1;
  }

  Status = DfciIssueRequest (NetworkRequest, &RequestToken);
  if (EFI_ERROR (Status)) {
    goto S_EXIT1;
  }

  ResponseData.StatusCode = HTTP_STATUS_UNSUPPORTED_STATUS;

  ResponseMessage.Data.Response = &ResponseData;
  ResponseMessage.HeaderCount   = 0;
  ResponseMessage.Headers       = NULL;

  ResponseMessage.BodyLength = 0;
  ResponseMessage.Body       = NULL;

  ResponseToken.Event   = NULL;
  ResponseToken.Status  = EFI_SUCCESS;
  ResponseToken.Message = &ResponseMessage;

  Status = DfciGetResponse (NetworkRequest, &ResponseToken);
  if (EFI_ERROR (Status)) {
    goto S_EXIT1;
  }

  ContentLengthHeader = HttpFindHeader (
                          ResponseMessage.HeaderCount,
                          ResponseMessage.Headers,
                          HTTP_HEADER_CONTENT_LENGTH
                          );
  ContentLength = 0;
  if (NULL != ContentLengthHeader) {
    ContentLength = AsciiStrDecimalToUintn (ContentLengthHeader->FieldValue);
  }

  DEBUG ((DEBUG_INFO, "ContentLength=%d, ActualLength=%d\n", ContentLength, ResponseMessage.BodyLength));

  NetworkRequest->HttpResponse.Headers     = ResponseMessage.Headers;
  NetworkRequest->HttpResponse.HeaderCount = ResponseMessage.HeaderCount;

  ResponseMessage.HeaderCount   = 0;
  ResponseMessage.Headers       = NULL;
  ResponseMessage.Data.Response = NULL;

  if (0 == ContentLength) {
    DEBUG ((DEBUG_INFO, "No content available\n"));
    Status = EFI_SUCCESS;
    goto S_EXIT1;
  }

  Packet = AllocatePool (ContentLength + sizeof (CHAR8));   // Add 1 for a NULL
  if (NULL == Packet) {
    DEBUG ((DEBUG_ERROR, "Unable to allocate return buffer\n"));
    Status = EFI_OUT_OF_RESOURCES;
    goto S_EXIT1;
  }

  CurrentLength = 0;

  while (CurrentLength < ContentLength) {
    ResponseMessage.Body       = &Packet[CurrentLength];
    ResponseMessage.BodyLength = ContentLength - CurrentLength;
    Status                     = DfciGetResponse (NetworkRequest, &ResponseToken);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Error from additional response data. Code=%r\n", Status));
      FreePool (Packet);
      goto S_EXIT1;
    }

    CurrentLength += ResponseMessage.BodyLength;
  }

  Packet[ContentLength]                 = '\0'; // Add terminating NULL
  NetworkRequest->HttpResponse.Body     = Packet;
  NetworkRequest->HttpResponse.BodySize = ContentLength + sizeof (CHAR8);

S_EXIT1:
  FreePool (RequestData.Url);
  HttpFreeHeaderFields (RequestMessage.Headers, RequestMessage.HeaderCount);
  return Status;
}

/**
 * ProcessHttpRequestInternal
 *
 * Process an Http request, and honor 429 and possibly 202 as retry events
 *
 * @param  [in]    NetworkRequest
 * @param  [in]    HttpMethod
 * @param  [in]    Url
 * &param  [in]    RetryOn202
 *
 * return  EFI_STATUS
 **/
STATIC
EFI_STATUS
ProcessHttpRequestInternal (
  IN  DFCI_NETWORK_REQUEST  *NetworkRequest,
  IN  EFI_HTTP_METHOD       HttpMethod,
  IN  CHAR8                 *Url,
  IN  BOOLEAN               RetryOn202
  )
{
  //
  //  Retry delay table.  Retry after 1 second.  If that fails, 2 seconds.
  //  So on and so forth up to 16 second delay.
  //
  STATIC UINT64    DelayTable[] = { 0, 16, 8, 4, 2, 1 };
  EFI_HTTP_HEADER  *Header;
  UINTN            RetryCount;
  EFI_STATUS       Status;
  UINT64           DelayInSeconds;
  BOOLEAN          Retry;

  RetryCount = 6;
  CleanupNetworkRequest (NetworkRequest, CLEANUP_STATUS);
  do {
    RetryCount--;
    Retry          = FALSE;
    DelayInSeconds = DelayTable[RetryCount];

    // Issue HTTP Request here
    Status = ProcessHttpRequest (NetworkRequest, HttpMethod, Url);
    if (!EFI_ERROR (Status)) {
      if (RetryOn202 && (NetworkRequest->HttpStatus.HttpStatus == HTTP_STATUS_202_ACCEPTED)) {
        DelayInSeconds = DelayTable[RetryCount];
        Retry          = TRUE;
      }

      if (NetworkRequest->HttpStatus.HttpStatus == HTTP_STATUS_429_TOO_MANY_REQUESTS) {
        Header = HttpFindHeader (
                   NetworkRequest->HttpResponse.HeaderCount,
                   NetworkRequest->HttpResponse.Headers,
                   (CHAR8 *)&HEADER_RETRY_AFTER
                   );

        //
        // Override retry after delay to value supplied by the server
        //
        if (NULL != Header) {
          AsciiStrDecimalToUintnS (Header->FieldValue, NULL, &DelayInSeconds);
        }

        DEBUG ((DEBUG_INFO, "Requested delay is %d\n", DelayInSeconds));

        // Limit retry delay to reasonable values
        if (DelayInSeconds < MIN_DELAY_BEFORE_RETRY) {
          DelayInSeconds = MIN_DELAY_BEFORE_RETRY;
        } else if (DelayInSeconds > MAX_DELAY_BEFORE_RETRY) {
          DelayInSeconds = MAX_DELAY_BEFORE_RETRY;
        }

        Retry = TRUE;
      }

      if (Retry && (RetryCount > 0)) {
        //
        // EventWait in 100ns units. 10 * 100ns ->us.  1000us->ms.  1000->ms->S
        //
        // Wait will return TIMEOUT or it will not wait.  So, just let the retry counter
        // exit the loop if wait fails.
        Status = EventWait (NULL, 0, DelayInSeconds * 10 * 1000 * 1000);
        if (EFI_TIMEOUT != Status) {
          DEBUG ((DEBUG_ERROR, "Http EventWait unexpected error %r\n", Status));
        }

        CleanupNetworkRequest (NetworkRequest, CLEANUP_RESPONSE | CLEANUP_STATUS);
        DEBUG ((DEBUG_INFO, "Processing retry. Count=%d\n", RetryCount));
      }
    }
  } while (Retry && (RetryCount > 0));

  return Status;
}

STATIC
EFI_STATUS
ProcessHttpRequestWithRetries (
  IN  DFCI_NETWORK_REQUEST  *NetworkRequest,
  IN  EFI_HTTP_METHOD       HttpMethod,
  IN  CHAR8                 *Url,
  IN  BOOLEAN               RetryOn202
  )
{
  EFI_STATUS  Status;
  EFI_STATUS  Status2;

  NetworkRequest->Http.ConfigData.LocalAddressIsIPv6 = FALSE;
  NetworkRequest->Http.HttpChildHandle               = gImageHandle; // Place HttpChild on our image handle
  Status                                             = ConfigureHTTP (NetworkRequest);
  if (EFI_ERROR (Status)) {
    goto EARLY_EXIT;
  }

  Status = ProcessHttpRequestInternal (
             NetworkRequest,
             HttpMethod,
             Url,
             RetryOn202
             );
  //
  // Don't let success or fail of cleanup from disturbing the return status.
  //
  if (NULL != NetworkRequest->Http.HttpProtocol) {
    Status2 = NetworkRequest->Http.HttpProtocol->Configure (NetworkRequest->Http.HttpProtocol, NULL);
    if (EFI_ERROR (Status2)) {
      DEBUG ((DEBUG_ERROR, "Unable to cleanup HTTP Protocol. Code=%r\n", Status2));
    }
  }

  Status2 = NetworkRequest->HttpNic.HttpSbProtocol->DestroyChild (
                                                      NetworkRequest->HttpNic.HttpSbProtocol,
                                                      NetworkRequest->Http.HttpChildHandle
                                                      );
  if (EFI_ERROR (Status2)) {
    DEBUG ((DEBUG_ERROR, "Error destroying worker child. Code=%r\n", Status2));
  }

  CleanupNetworkRequest (NetworkRequest, CLEANUP_HTTP);

EARLY_EXIT:
  return Status;
}

/**
 * Process Async Request
 *
 * @param[in]   NetworkRequest
 * @param[in]   Url
 * @param[in]   DoneProcessing
 *
 * Uses Url to contact a server.  The server will respond with a new URL.  Contact that
 * server for the packet data, and the apply the packet data.
 **/
STATIC
EFI_STATUS
ProcessAsyncRequest (
  IN  DFCI_NETWORK_REQUEST  *NetworkRequest,
  IN  CHAR8                 *Url,
  OUT BOOLEAN               *DoneProcessing
  )
{
  EFI_HTTP_HEADER  *Header;
  EFI_STATUS       Status;

  Status = ProcessHttpRequestWithRetries (
             NetworkRequest,
             HttpMethodPost,
             Url,
             FALSE
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Process Http Post failed.  Status = %r\n", Status));
    goto PROTOCOL_ERROR;
  }

  //
  // Only 202 is expected here (except retries)
  //
  if (NetworkRequest->HttpStatus.HttpStatus != HTTP_STATUS_202_ACCEPTED) {
    Status = EFI_PROTOCOL_ERROR;
    DEBUG ((
      DEBUG_ERROR,
      "Http request 1 failed.  Http error is %d, %a\n",
      NetworkRequest->HttpStatus.HttpStatus,
      GetHttpErrorMsg (NetworkRequest->HttpStatus.HttpStatus)
      ));
    goto PROTOCOL_ERROR;
  }

  //
  // Current NIC was able to talk to server.  Do not process any more NICs.
  //
  *DoneProcessing = TRUE;

  //
  // Server provides the location from where to receive the updates in the location header.
  //
  Header = HttpFindHeader (
             NetworkRequest->HttpResponse.HeaderCount,
             NetworkRequest->HttpResponse.Headers,
             (CHAR8 *)&HEADER_LOCATION
             );
  if (NULL == Header) {
    Status = EFI_PROTOCOL_ERROR;
    DEBUG ((DEBUG_ERROR, "Request 1 should have returned Location header\n"));
    goto PROTOCOL_ERROR;
  }

  //
  // Get the first set of update certificates
  //
  Status = ProcessHttpRequestWithRetries (
             NetworkRequest,
             HttpMethodGet,
             Header->FieldValue,
             TRUE
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Http Get failed.  Status = %r\n", Status));
    goto PROTOCOL_ERROR;
  }

  //
  // Only 200 is expected here
  //
  if (NetworkRequest->HttpStatus.HttpStatus != HTTP_STATUS_200_OK) {
    Status = EFI_PROTOCOL_ERROR;
    DEBUG ((
      DEBUG_ERROR,
      "Http request 2 failed.  Http error is %d, %a\n",
      NetworkRequest->HttpStatus.HttpStatus,
      GetHttpErrorMsg (NetworkRequest->HttpStatus.HttpStatus)
      ));

    goto PROTOCOL_ERROR;
  }

  //
  // A response is required
  //
  if ((NULL == NetworkRequest->HttpResponse.Body) || (NetworkRequest->HttpResponse.BodySize == 0)) {
    Status = EFI_PROTOCOL_ERROR;
    goto PROTOCOL_ERROR;
  }

PROTOCOL_ERROR:
  return Status;
}

/**
 * MainLogic for processing the recovery contract with on premise recovery provider
 *
 * @param[in]  NetworkRequest
 * @param[out] Done Processing    - Inform caller processing is complete
 *
 * This is a simple request that retrieves a packet exactly like the USB Request.
 **/
STATIC
EFI_STATUS
EFIAPI
SimpleMainLogic (
  IN  DFCI_NETWORK_REQUEST  *NetworkRequest,
  OUT BOOLEAN               *DoneProcessing
  )
{
  return EFI_UNSUPPORTED;
}

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
  )
{
  EFI_STATUS  Status;

  *DoneProcessing            = FALSE;
  NetworkRequest->LogicState = DFCI_CHECK_429;

  //
  // Step 1. Ping server with Bootstrap URL provided in DFCI settings
  //

  Status = ProcessHttpRequestWithRetries (
             NetworkRequest,
             HttpMethodGet,
             NetworkRequest->HttpRequest.Url,
             FALSE
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Process Http Get failed.  Status = %r\n", Status));
    goto CHK_429_CLEANUP;
  }

  NetworkRequest->LogicState = DFCI_CHECK_429_COMPLETE;
  *DoneProcessing            = TRUE;

  // Let the caller verify the HTTP status

CHK_429_CLEANUP:

  return Status;
}

/**
 * MainLogic for processing the recovery contract with recovery provider
 *
 * @param[in]  NetworkRequest
 * @param[out] Done Processing    - Inform caller processing is complete
 *
 * There are two parts of the recovery.  First, the certs need to be updated.  After
 * the certs are updated, the new settings are obtained.
 *
 * Step 1. Bootstrap request.
 *
 * The Bootstrap is the action of requesting updates to the certificates being
 * used. It starts with an HTTP POST with the current DDS and ZTD certificate
 * thumbprints.  The response will be a new URL to request the packets.
 *
 * After a short delay, the new URL is used to request data.  It is expected that
 * there will be HTTP return code 429 to request additional delay.
 * The requests are asynchronous.  That is, the first HTTP action starts the server
 * action of building the required packets - cert update, settings, etc.  The second
 * request is to obtain some or all of the packet data.
 *
 * This has to be done twice.  The first HTTP request is to ask for updated certs.
 * The response has the URL to process for the actual data.  Then, using the response
 * URL, the cert updates are requested until there are no more.  The second HTTP
 * request is for the actual permission and setting update, or unenroll operation.
 *
 * If no certs are needed, the Step 2 is processed immediately.  Otherwise, the system
 * need to be rebooted to apply the updated certs.  When this occurs, the used must manually
 * start another refresh from network request.  Depending on how old the certificates are,
 * this may have to be repeated a few times.
 *
 * Step2. Recovery Request
 *
 * This is exactly the same as Step 1, except the host sends the packets that un-enro the system.
 * After the reboot, DFCI will not be enrolled.
 *
 **/
STATIC
EFI_STATUS
EFIAPI
DfciMainLogic (
  IN  DFCI_NETWORK_REQUEST  *NetworkRequest,
  OUT BOOLEAN               *DoneProcessing
  )
{
  EFI_STATUS  Status;

  *DoneProcessing            = FALSE;
  NetworkRequest->LogicState = DFCI_PRE_BOOTSTRAP;

  //
  // Step 1. Ping server with Bootstrap URL provided in DFCI settings
  //
  Status = BuildJsonBootstrapRequest (NetworkRequest);
  if (EFI_ERROR (Status)) {
    goto MAIN_CLEANUP;
  }

  Status = ProcessAsyncRequest (
             NetworkRequest,
             NetworkRequest->HttpRequest.BootstrapUrl,
             DoneProcessing
             );
  if (!EFI_ERROR (Status)) {
    NetworkRequest->LogicState = DFCI_BOOTSTRAP;
    //
    // Process the response JSON
    //
    Status = DfciUpdateFromJson (
               NetworkRequest->HttpResponse.Body,
               NetworkRequest->HttpResponse.BodySize,
               mRecoveryBootstrapResponse
               );
  }

  switch (Status) {
    case EFI_SUCCESS:
    case EFI_NOT_FOUND:
      //
      //  EFI_SUCCESS and EFI_NOT_FOUND both indicate no updates occurred, and that
      //  processing should continue to the next step.
      //
      Status = EFI_SUCCESS;
      DEBUG ((DEBUG_INFO, "No Bootstrap updates\n"));
      NetworkRequest->LogicState = DFCI_PRE_RECOVERY;
      break;

    case EFI_MEDIA_CHANGED:
      //
      //  EFI_MEDIA_CHANGED indicates that Dfci Update Variables were set.  Exit to
      //  caller to display a status message and then reboot to apply the updates.
      //
      DEBUG ((DEBUG_INFO, "Bootstrap JSON updated\n"));
      NetworkRequest->LogicState = DFCI_BOOTSTRAP_COMPLETE;
      goto MAIN_CLEANUP;
      break;

    default:
      //
      //  An error occurred processing the first request.  Exit to the caller to display
      //  a status message and then reboot the system.
      //
      DEBUG ((DEBUG_ERROR, "Bootstrap JSON failed.  Status = %r\n", Status));
      goto MAIN_CLEANUP;
      break;
  }

  Status = BuildJsonRecoveryRequest (NetworkRequest);
  if (EFI_ERROR (Status)) {
    goto MAIN_CLEANUP;
  }

  Status = ProcessAsyncRequest (
             NetworkRequest,
             NetworkRequest->HttpRequest.Url,
             DoneProcessing
             );
  if (!EFI_ERROR (Status)) {
    NetworkRequest->LogicState = DFCI_RECOVERY;
    //
    // Process the response JSON
    //
    Status = DfciUpdateFromJson (
               NetworkRequest->HttpResponse.Body,
               NetworkRequest->HttpResponse.BodySize,
               mRecoveryResponse
               );
  }

  switch (Status) {
    case EFI_SUCCESS:
    case EFI_NOT_FOUND:
      //
      //  EFI_SUCCESS and EFI_NOT_FOUND both indicate no updates occurred, and that
      //  processing is complete.
      //
      Status                     = EFI_SUCCESS;
      NetworkRequest->LogicState = DFCI_NO_RECOVERY_AVAILABLE;
      DEBUG ((DEBUG_INFO, "No Recovery updates\n"));
      break;

    case EFI_MEDIA_CHANGED:
      //
      //  EFI_MEDIA_CHANGED indicates that Dfci Update Variables were set.  Exit to
      //  caller to display a status message and then reboot to apply the updates.
      //
      NetworkRequest->LogicState = DFCI_RECOVERY_COMPLETE;
      DEBUG ((DEBUG_INFO, "Recovery updates applied\n"));
      break;

    default:
      //
      //  An error occurred processing the first request.  Exit to the caller to display
      //  a status message and then reboot the system.
      //
      DEBUG ((DEBUG_ERROR, "Recovery updates failed.  Status = %r\n", Status));
      break;
  }

MAIN_CLEANUP:

  return Status;
}

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
  )
{
  EFI_BOOT_MANAGER_POLICY_PROTOCOL  *BootPolicy;
  BOOLEAN                           DoneProcessing;
  EFI_HANDLE                        *HandleBuffer;
  UINTN                             HandleCount;
  BOOLEAN                           MediaPresent;
  UINTN                             NicIndex;
  EFI_STATUS                        Status;

  Status = gBS->LocateProtocol (
                  &gEfiBootManagerPolicyProtocolGuid,
                  NULL,
                  (VOID **)&BootPolicy
                  );

  // If the platform chose to publish a Boot Manager Policy, ask it to start the
  // networking stack.  Ignore any errors, and attempt to access the network even if
  // the BootPolicy is not found, or BootPolicy returns an error.
  if (!EFI_ERROR (Status)) {
    Status = BootPolicy->ConnectDeviceClass (BootPolicy, &gEfiBootManagerPolicyNetworkGuid);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Error starting the network. Code = %r\n", Status));
    }
  }

  //
  // Try each connected NIC until a successful settings transfer.
  //

  DoneProcessing = FALSE;
  HandleBuffer   = NULL;
  //
  // Find NICs with HTTP ServiceBinding protocol.  These are the available HTTP devices.
  //
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiHttpServiceBindingProtocolGuid,
                  NULL,
                  &HandleCount,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status) || (0 == HandleCount)) {
    DEBUG ((DEBUG_ERROR, "Unable to locate any NICs for HTTP file access\n"));
    Status = EFI_NOT_FOUND;
    goto CLEANUP;
  }

  for (NicIndex = 0; (NicIndex < HandleCount) && !DoneProcessing; NicIndex++) {
    //
    // Clear the working section of DFCI_NETWORK_REQUEST
    //
    DEBUG ((DEBUG_INFO, "Attempting NicIndex=%d\n", NicIndex));

    CleanupNetworkRequest (NetworkRequest, CLEANUP_NIC);

    NetworkRequest->HttpNic.NicHandle = HandleBuffer[NicIndex];

    Status = gBS->HandleProtocol (
                    NetworkRequest->HttpNic.NicHandle,
                    &gEfiHttpServiceBindingProtocolGuid,
                    (VOID **)&NetworkRequest->HttpNic.HttpSbProtocol
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Error locating HttpServiceBinding protocol. Code=%r\n", Status));
      continue;
    }

    // Verify Media is present before doing any work.  We don't really care about the error cases.  On
    // error cases, assume Media is Present.
    MediaPresent = TRUE;
    Status       = NetLibDetectMedia (NetworkRequest->HttpNic.NicHandle, &MediaPresent);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "NetLibDetectMedi returned %r. Assuming Media Present\n", Status));
    }

    if (!MediaPresent) {
      Status = EFI_NO_MEDIA;
      continue;
    }

    //
    // Process HTTP Recovery flow on this NIC. If it fails, check the next NIC
    //
    Status = NetworkRequest->MainLogic (NetworkRequest, &DoneProcessing);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "MainLogic error. Code=%r\n", Status));
    }

    if (NetworkRequest->HttpNic.DhcpRequested) {
      UnconfigureNIC (NetworkRequest);
    }
  }

CLEANUP:
  if (NULL != HandleBuffer) {
    FreePool (HandleBuffer);
  }

  CleanupNetworkRequest (NetworkRequest, CLEANUP_NIC);
  DEBUG ((DEBUG_INFO, "Done processing\n"));

  return Status;
}

/**
 * Restore Certificates
 *
 * Any TSL certificates loaded on the system are restored.
 *
 *
**/
STATIC
EFI_STATUS
RestoreCertificates (
  VOID
  )
{
  EFI_STATUS  Status;

  if (NULL == mOldCertificateList) {
    return EFI_SUCCESS;
  }

  Status = gRT->SetVariable (
                  EFI_TLS_CA_CERTIFICATE_VARIABLE,
                  &gEfiTlsCaCertificateGuid,
                  mOldCertificateAttr,
                  mOldCertificateSize,
                  mOldCertificateList
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Unable to restore Tls Certificates\n"));
  }

  FreePool (mOldCertificateList);

  mOldCertificateList = NULL;

  return EFI_SUCCESS;
}

/**
 * EnableDfciCertificates
 *
 * Certificates are loaded in the UEFI variable EFI_TLS_CA_CERTIFICATE_VARIABLE.
 *
 * The normal flow would be:
 * 1. Save the current variable, if it exists
 * 2. Package the DFCI HTTPS certificate, and set the certificate variable
 * 3. Process HTTPS requests.
 * 4. Restore the EFI_TLS_CA_CERTIFICATE_VARIABLE
 *
**/
STATIC
EFI_STATUS
EnableDfciCertificate (
  IN    DFCI_NETWORK_REQUEST  *NetworkRequest
  )
{
  EFI_SIGNATURE_LIST  *Cert;
  EFI_SIGNATURE_DATA  *CertData;
  UINTN               CertSize;
  EFI_STATUS          Status;

  //
  // Try to read the TlsCaCertificate variable.
  //
  Status = gRT->GetVariable (
                  EFI_TLS_CA_CERTIFICATE_VARIABLE,
                  &gEfiTlsCaCertificateGuid,
                  &mOldCertificateAttr,
                  &mOldCertificateSize,
                  NULL
                  );

  if (Status == EFI_BUFFER_TOO_SMALL) {
    //
    // Allocate buffer and read the current certificate variable.
    //
    mOldCertificateList = AllocatePool (mOldCertificateSize);
    if (mOldCertificateList == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }

    Status = gRT->GetVariable (
                    EFI_TLS_CA_CERTIFICATE_VARIABLE,
                    &gEfiTlsCaCertificateGuid,
                    &mOldCertificateAttr,
                    &mOldCertificateSize,
                    mOldCertificateList
                    );
    if (EFI_ERROR (Status)) {
      //
      // Couldn't save the existing variable.
      //
      FreePool (mOldCertificateList);
      mOldCertificateList = NULL;
      DEBUG ((DEBUG_ERROR, "Error reading old certificate list. Code=%r\n", Status));
      // Go ahead and try to use our certificate anyway
    }

    // Delete current variable first in case the attributes change.
    Status = gRT->SetVariable (
                    EFI_TLS_CA_CERTIFICATE_VARIABLE,
                    &gEfiTlsCaCertificateGuid,
                    0,
                    0,
                    NULL
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Unable to erase old cert data. Code=%r\n", Status));
      // Go ahead and try to use our certificate anyway
    }
  }

  CertSize = sizeof (EFI_SIGNATURE_LIST) + sizeof (EFI_SIGNATURE_DATA) - 1 + NetworkRequest->HttpsCertSize;

  Cert = (EFI_SIGNATURE_LIST *)AllocateZeroPool (CertSize);
  if (Cert == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    return Status;
  }

  Cert->SignatureListSize   = (UINT32)CertSize;
  Cert->SignatureHeaderSize = 0;
  Cert->SignatureSize       = (UINT32)(sizeof (EFI_SIGNATURE_DATA) - 1 + NetworkRequest->HttpsCertSize);
  CopyGuid (&Cert->SignatureType, &gEfiCertX509Guid);

  CertData = (EFI_SIGNATURE_DATA *)((UINT8 *)Cert + sizeof (EFI_SIGNATURE_LIST));
  CopyGuid (&CertData->SignatureOwner, &gEfiCallerIdGuid);
  CopyMem ((UINT8 *)(CertData->SignatureData), NetworkRequest->HttpsCert, NetworkRequest->HttpsCertSize);

  Status = gRT->SetVariable (
                  EFI_TLS_CA_CERTIFICATE_VARIABLE,
                  &gEfiTlsCaCertificateGuid,
                  (EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS),
                  CertSize,
                  Cert
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Unable to set Dfci Https certificate. Code=%r\n", Status));
    RestoreCertificates ();
  }

  FreePool (Cert);

  return Status;
}

STATIC
UINTN
GetResponseMsgLength (
  IN  CONST CHAR8  *FormatMessage,
  ...
  )
{
  VA_LIST  Marker;
  UINTN    MsgLen;

  VA_START (Marker, FormatMessage);

  MsgLen = SPrintLengthAsciiFormat (FormatMessage, Marker);

  VA_END (Marker);
  return MsgLen;
}

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
  )
{
  CHAR16       *Msg = NULL;
  EFI_STATUS   Status;
  UINTN        MsgLen;
  UINTN        MsgSize;
  CONST CHAR8  *MsgFormat;

  Status = InitializeNetworkRequest (NetworkRequest);
  if (EFI_ERROR (Status)) {
    goto EXIT_NETWORK_REQUEST;
  }

  Status = EnableDfciCertificate (NetworkRequest);
  if (EFI_ERROR (Status)) {
    goto EXIT_NETWORK_REQUEST;
  }

  NetworkRequest->MainLogic = DfciMainLogic;

  // Try every NIC in the system until one fills the first part of the request.
  Status = TryEachNICThenProcessRequest (NetworkRequest);

  DEBUG ((DEBUG_INFO, "Url   %a\n", NetworkRequest->HttpRequest.Url));
  DEBUG ((DEBUG_INFO, "HttpStatus = %d\n", NetworkRequest->HttpStatus.HttpStatus));
  DEBUG ((DEBUG_INFO, "HttpStatus = %a\n", GetHttpErrorMsg (NetworkRequest->HttpStatus.HttpStatus)));
  DEBUG ((DEBUG_INFO, "HttpRc  = %a\n", NetworkRequest->HttpStatus.HttpReturnCode));
  DEBUG ((DEBUG_INFO, "HttpMsg = %a\n", NetworkRequest->HttpStatus.HttpMessage));

  // The proper message format depends on:
  //
  //  1. UEFI Status
  //  2. Main Logic State.

  #define MSG_PARAMETERS   \
      NetworkRequest->HttpRequest.Url, \
      Status, \
      GetHttpErrorMsg (NetworkRequest->HttpStatus.HttpStatus), \
      NetworkRequest->HttpStatus.HttpReturnCode, \
      NetworkRequest->HttpStatus.HttpMessage, \
      NetworkRequest->LogicState

  #define MSG_FORMAT_ERROR_OCCURRED   \
      "%a\nStatus = %r, HttpStatus=%a\n Code=%a, Msg=%a"

  #define MSG_FORMAT_NO_UPDATE_NEEDED   \
      "%a\nNo updates are available from the server."

  #define MSG_FORMAT_MORE_UPDATES_NEEDED   \
      "%a\nCertificate updates have been accepted from the server.  A restart is needed\n\
to apply these new certificates. After restarting the system, repeat this\n\
operation."

  #define MSG_FORMAT_RECOVERY_COMPLETE   \
      "%a\nUpdates have been applied from the server."

  // An error occurred. Display the full error message.
  MsgFormat = "%a\nStatus = %r, HttpStatus=%a\nCode=%a, Msg=%a\nState=%d";

  if (EFI_SUCCESS == Status) {
    if (NetworkRequest->LogicState == DFCI_NO_RECOVERY_AVAILABLE) {
      // Done here.  No update packets are available.
      MsgFormat = MSG_FORMAT_NO_UPDATE_NEEDED;
    }
  } else if (EFI_MEDIA_CHANGED == Status) {
    if (NetworkRequest->LogicState == DFCI_BOOTSTRAP_COMPLETE) {
      // Intermediate step complete
      MsgFormat = MSG_FORMAT_MORE_UPDATES_NEEDED;
    } else if (NetworkRequest->LogicState == DFCI_RECOVERY_COMPLETE) {
      // Done here - recovery complete
      MsgFormat = MSG_FORMAT_RECOVERY_COMPLETE;
    }
  }

  DEBUG ((DEBUG_INFO, "Assembling error message\n"));
  MsgLen = GetResponseMsgLength (MsgFormat, MSG_PARAMETERS);

  ASSERT (MsgLen > 0);
  if (MsgLen > 0) {
    MsgSize = (MsgLen + 1) * sizeof (CHAR16);
    Msg     = AllocatePool (MsgSize);
    if (NULL != Msg) {
      MsgLen = UnicodeSPrintAsciiFormat (Msg, MsgSize, MsgFormat, MSG_PARAMETERS);
      ASSERT (MsgLen > 0);
      if (MsgLen > 0) {
        *Message = Msg;
      } else {
        FreePool (Msg);
      }
    }
  }

  if (NULL != Msg) {
    DEBUG ((DEBUG_INFO, "Error message of %d characters generated\n", MsgLen));
    DEBUG ((DEBUG_INFO, "%s\n", Msg));
  }

EXIT_NETWORK_REQUEST:
  RestoreCertificates ();
  CleanupNetworkRequest (NetworkRequest, CLEANUP_ALL);
  return Status;
}

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
  )
{
  EFI_STATUS  Status;

  Status = InitializeNetworkRequest (NetworkRequest);
  if (EFI_ERROR (Status)) {
    goto EXIT_NETWORK_REQUEST;
  }

  NetworkRequest->MainLogic = SimpleMainLogic;

  // Try every NIC in the system until one fills the first part of the request.
  Status = TryEachNICThenProcessRequest (NetworkRequest);

EXIT_NETWORK_REQUEST:

  CleanupNetworkRequest (NetworkRequest, CLEANUP_ALL);
  return Status;
}
