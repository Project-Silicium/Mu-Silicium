/**
  Copyright (C) Microsoft Corporation. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/DebugLib.h>
#include <Library/DeviceBootManagerLib.h>
#include <Library/MsBootOptionsLib.h>
#include <Library/MsBootPolicyLib.h>
#include <Library/BootGraphicsLib.h>
#include <Library/BootGraphics.h>
#include <Library/MsPlatformDevicesLib.h>
#include <Library/AcpiPlatformUpdateLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiLib.h>

STATIC EFI_EVENT mPreReadyToBootEvent;
STATIC EFI_EVENT mPostReadyToBootEvent;

STATIC
VOID
EFIAPI
PreReadyToBoot (
  IN EFI_EVENT Event,
  IN VOID     *Context)
{
  gBS->CloseEvent (Event);
}

STATIC
VOID
EFIAPI
PostReadyToBoot (
  IN EFI_EVENT Event,
  IN VOID     *Context)
{
  // Do Nothing
}

EFI_STATUS
EFIAPI
DeviceBootManagerConstructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status;

  // Register OnReadyToBoot Event
  Status = gBS->CreateEventEx (EVT_NOTIFY_SIGNAL, TPL_CALLBACK, PreReadyToBoot, NULL, &gEfiEventPreReadyToBootGuid, &mPreReadyToBootEvent);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Register OnReadyToBoot Event! Status = %r\n", __FUNCTION__, Status));
  }

  // Register OnPostReadyToBoot Event
  Status = gBS->CreateEventEx (EVT_NOTIFY_SIGNAL, TPL_CALLBACK, PostReadyToBoot, NULL, &gEfiEventPostReadyToBootGuid, &mPostReadyToBootEvent);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Eegister OnPostReadyToBoot Event! Status = %r\n", Status));
  }

  return EFI_SUCCESS;
}

EFI_DEVICE_PATH_PROTOCOL**
EFIAPI
DeviceBootManagerOnDemandConInConnect () { return GetPlatformConnectOnConInList (); }

VOID
EFIAPI
DeviceBootManagerBdsEntry ()
{
  // Signal that BDS Started
  EfiEventGroupSignal (&gMsStartOfBdsNotifyGuid);
}

EFI_HANDLE
EFIAPI
DeviceBootManagerBeforeConsole (
  EFI_DEVICE_PATH_PROTOCOL  **DevicePath,
  BDS_CONSOLE_CONNECT_ENTRY **PlatformConsoles)
{
  // Register Boot Options
  MsBootOptionsLibRegisterDefaultBootOptions ();

  // Get Console List
  *PlatformConsoles = GetPlatformConsoleList ();

  // Get Preferred Console
  return GetPlatformPreferredConsole (DevicePath);
}

EFI_DEVICE_PATH_PROTOCOL**
EFIAPI
DeviceBootManagerAfterConsole ()
{
  EFI_STATUS Status;

  // Update the ACPI Tables
  PlatformUpdateAcpiTables ();

  // Display Boot Logo on Screen
  Status = DisplayBootGraphic (BG_SYSTEM_LOGO);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Display Boot Logo! Status = %r\n", __FUNCTION__, Status));
  }

  return NULL;
}

VOID
EFIAPI
DeviceBootManagerProcessBootCompletion (IN EFI_BOOT_MANAGER_LOAD_OPTION *BootOption)
{
  EFI_STATUS Status;
  BOOLEAN    MsBootNext;

  // Get MsBootNext Variable
  Status = gRT->GetVariable (L"MsBootNext", &gMsBootMenuFormsetGuid, NULL, 0, &MsBootNext);
  if (!EFI_ERROR (Status)) {
    // Set MsBootNext Variable
    Status = gRT->SetVariable (L"MsBootNext", &gMsBootMenuFormsetGuid, EFI_VARIABLE_BOOTSERVICE_ACCESS, 0, NULL);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%a: Failed to Set MsBootNext Variable! Status = %r\n", __FUNCTION__, Status));
    }
  }
}

EFI_STATUS
EFIAPI
DeviceBootManagerPriorityBoot (EFI_BOOT_MANAGER_LOAD_OPTION *BootOption)
{
  // Get Special Boot Option
  BOOLEAN XenonGUI   = MsBootPolicyLibIsSettingsBoot ();
  BOOLEAN SpecialApp = MsBootPolicyLibIsAltBoot ();

  // Clear Boot Requests
  MsBootPolicyLibClearBootRequests ();

  // Boot Special Option
  if (XenonGUI) {
    return MsBootOptionsLibGetBootManagerMenu (BootOption, "VOL+");
  } else if (SpecialApp) {
    return MsBootOptionsLibGetDefaultBootApp (BootOption, "VOL-");
  }

  return EFI_NOT_FOUND;
}

VOID
EFIAPI
DeviceBootManagerUnableToBoot ()
{
  EFI_STATUS Status;

  // Display No Boot OS Logo
  Status = DisplayBootGraphic (BG_NO_BOOT_OS);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Display No Boot OS Logo! Status = %r\n", __FUNCTION__, Status));
    Print (L"No Operating System Found! Please make sure that an OS is Installed.\n");
  }

  // Wait 10s
  gBS->Stall(10000000);

  // Shutdown
  gRT->ResetSystem (EfiResetShutdown, EFI_SUCCESS, 0, NULL);

  // Do Cpu Dead Loop
  CpuDeadLoop ();
}
