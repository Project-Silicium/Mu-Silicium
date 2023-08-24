/** @file
 *Header file for Ms Boot Policy implementation

Copyright (C) Microsoft Corporation. All rights reserved.
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _MS_BOOT_POLICY_H_
#define _MS_BOOT_POLICY_H_

#include <Uefi.h>

#include <Settings/BootMenuSettings.h>

#include <Guid/ConnectConInEvent.h>
#include <Guid/GlobalVariable.h>
#include <Guid/StatusCodeDataTypeVariable.h>

#include <Protocol/Bds.h>
#include <Protocol/DevicePath.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/LoadFile.h>
#include <Protocol/SimpleFileSystem.h>
#include <Protocol/GraphicsOutput.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/MsBootManagerSettingsLib.h>
#include <Library/MsBootPolicyLib.h>
#include <Library/BootGraphicsProviderLib.h>
#include <Library/BootGraphicsLib.h>
#include <Library/GraphicsConsoleHelperLib.h>
#include <Library/MsNetworkDependencyLib.h>
#include <Library/PcdLib.h>
#include <Library/PrintLib.h>
#include <Library/ReportStatusCodeLib.h>
#include <Library/UefiBootManagerLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/MsAltBootLib.h>

#endif
