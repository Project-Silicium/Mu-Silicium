/* Copyright (c) 2015-2016, The Linux Foundation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * * Redistributions of source code must retain the above copyright
 *  notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided
 *  with the distribution.
 *   * Neither the name of The Linux Foundation nor the names of its
 * contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef __USB_EX_H__
#define __USB_EX_H__

#pragma pack(1)
/// Standard Device Qualifier Descriptor
/// USB 2.0 spec, Section 9.6.2
///
typedef struct {
  UINT8 Length;
  UINT8 DescriptorType;
  UINT16 BcdUSB;
  UINT8 DeviceClass;
  UINT8 DeviceSubClass;
  UINT8 DeviceProtocol;
  UINT8 MaxPacketSize0;
  UINT8 NumConfigurations;
  UINT8 Reserved;
} USB_DEVICE_QUALIFIER_DESCRIPTOR;

///
/// Standard Binary Object Store Descriptor
/// USB 3.0 spec, Section 9.6.2
///
typedef struct {
  UINT8 Length;
  UINT8 DescriptorType;
  UINT16 TotalLength;
  UINT8 NumDeviceCaps;
} USB_BOS_DESCRIPTOR;

///
/// Standard Device Capability Descriptor
/// USB 3.0 spec, Section 9.6.2
///
typedef struct {
  UINT8 Length;
  UINT8 DescriptorType;
  UINT8 DevCapabilityType;
  UINT8 DevCapabilityData[1];
} USB_DEVICE_CAPABILITY_DESCRIPTOR;

///
/// Standard Device Capability Descriptor, USB 2.0 Extension
/// USB 3.0 spec, Section 9.6.2.1
///
typedef struct {
  UINT8 Length;
  UINT8 DescriptorType;
  UINT8 DevCapabilityType;
  UINT32 Attributes;
} USB_USB_20_EXTENSION_DESCRIPTOR;

///
/// Standard Device Capability Descriptor, SuperSpeed USB
/// USB 3.0 spec, Section 9.6.2.2
///
typedef struct {
  UINT8 Length;
  UINT8 DescriptorType;
  UINT8 DevCapabilityType;
  UINT8 Attributes;
  UINT16 SpeedsSupported;
  UINT8 FunctionalitySupport;
  UINT8 U1DevExitLat;
  UINT16 U2DevExitLat;
} USB_SUPERSPEED_USB_DESCRIPTOR;

///
/// Standard Device Capability Descriptor, Container ID
/// USB 3.0 spec, Section 9.6.2.3
///
typedef struct {
  UINT8 Length;
  UINT8 DescriptorType;
  UINT8 DevCapabilityType;
  UINT8 Reserved;
  UINT8 UUID[16];
} USB_CONTAINER_ID_DESCRIPTOR;

///
/// Standard SuperSpeed Endpoint Companion Descriptor
/// USB 3.0 spec, Section 9.6.7
///
typedef struct {
  UINT8 Length;
  UINT8 DescriptorType;
  UINT8 MaxBurst;
  UINT8 Attributes;
  UINT16 BytesPerInterval;
} USB_SS_ENDPOINT_COMPANION_DESCRIPTOR;
#pragma pack()

/** USB Binary Object Store descriptor. **/
typedef USB_BOS_DESCRIPTOR EFI_USB_BOS_DESCRIPTOR;
/** USB Generic Device Capability descriptor. **/
typedef USB_DEVICE_CAPABILITY_DESCRIPTOR EFI_USB_DEVICE_CAPABILITY_DESCRIPTOR;
/** USB 2.0 Extension Device Capability descriptor. **/
typedef USB_USB_20_EXTENSION_DESCRIPTOR EFI_USB_USB_20_EXTENSION_DESCRIPTOR;
/** USB Super-speed USB Device Capability descriptor. **/
typedef USB_SUPERSPEED_USB_DESCRIPTOR EFI_USB_SUPERSPEED_USB_DESCRIPTOR;
/** USB Container ID Device Capability descriptor. **/
typedef USB_CONTAINER_ID_DESCRIPTOR EFI_USB_CONTAINER_ID_DESCRIPTOR;
/** USB Interface Association descriptor. **/
typedef USB_INTERFACE_ASSOCIATION_DESCRIPTOR
    EFI_USB_INTERFACE_ASSOCIATION_DESCRIPTOR;
/** USB Super-speed Endpoint Companion descriptor **/
typedef USB_SS_ENDPOINT_COMPANION_DESCRIPTOR
    EFI_USB_SS_ENDPOINT_COMPANION_DESCRIPTOR;
/** USB device qualifier descriptor **/
typedef USB_DEVICE_QUALIFIER_DESCRIPTOR EFI_USB_DEVICE_QUALIFIER_DESCRIPTOR;

#endif
