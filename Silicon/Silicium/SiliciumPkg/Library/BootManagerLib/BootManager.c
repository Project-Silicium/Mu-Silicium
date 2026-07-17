/**
  Copyright (C) DuoWoA authors. All rights reserved.
  Copyright (C) Microsoft Corporation. All rights reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/AcpiTableUpdateLib.h>
#include <Library/DeviceBootManagerLib.h>
#include <Library/PlatformBootManagerLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/SecureBootHandlerLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/MsBootOptionsLib.h>
#include <Library/BootGraphicsLib.h>
#include <Library/BootGraphics.h>
#include <Library/PrintLib.h>

#include <Configuration/BootDevices.h>

#include "BootManager.h"

//
// Global Variables
//
STATIC UINTN XPos = 0;
STATIC UINTN YPos = 0;

//
// BDS Combo Message
//
STATIC CONST CHAR8 *ComboMessage = "[Volume Up] Boot Manager";

VOID
EFIAPI
PlatformBootManagerBeforeConsole ()
{
  // Signal End Of DXE
  EfiEventGroupSignal (&gEfiEndOfDxeEventGroupGuid);

  // Dispatch Deferred Images
  EfiBootManagerDispatchDeferredImages ();

  // Register Boot Options
  MsBootOptionsLibRegisterDefaultBootOptions ();

  // Register Console Variables
  EfiBootManagerUpdateConsoleVariable (ConIn,  (EFI_DEVICE_PATH_PROTOCOL *)&KeypadDevicePath,  NULL);
  EfiBootManagerUpdateConsoleVariable (ConOut, (EFI_DEVICE_PATH_PROTOCOL *)&DisplayDevicePath, NULL);
  EfiBootManagerUpdateConsoleVariable (ErrOut, (EFI_DEVICE_PATH_PROTOCOL *)&DisplayDevicePath, NULL);

  // Update ACPI Tables
  UpdateAcpiTables ();

  // Execute Secondary Before Console
  DeviceBootManagerBeforeConsole (NULL, NULL);
}

VOID
PostGopSetup (OUT EFI_GRAPHICS_OUTPUT_PROTOCOL **GopProtocol)
{
  EFI_STATUS                    Status;
  EFI_GRAPHICS_OUTPUT_PROTOCOL *LocalGopProtocol;

  // Verify Parameters
  if (GopProtocol == NULL) {
    return;
  }

  // Locate GOP Protocol
  Status = gBS->HandleProtocol (gST->ConsoleOutHandle, &gEfiGraphicsOutputProtocolGuid, (VOID *)&LocalGopProtocol);
  if (EFI_ERROR (Status) && Status != EFI_UNSUPPORTED) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Locate GOP Protocol from Handle! Status = %r\n", __FUNCTION__, Status));
    return;
  }

  // Check Status
  if (Status == EFI_UNSUPPORTED) {
    // Locate GOP Protocol
    Status = gBS->LocateProtocol (&gEfiGraphicsOutputProtocolGuid, NULL, (VOID *)&LocalGopProtocol);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%a: Failed to Locate GOP Protocol! Status = %r\n", __FUNCTION__, Status));
      return;
    }

    // Install GOP Protocol
    Status = gBS->InstallProtocolInterface (&gST->ConsoleOutHandle, &gEfiGraphicsOutputProtocolGuid, EFI_NATIVE_INTERFACE, LocalGopProtocol);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%a: Failed to Install GOP Protocol onto \"ConsoleOutHandle\"! Status = %r\n", __FUNCTION__, Status));
    }
  }

  // Pass GOP Protocol
  *GopProtocol = LocalGopProtocol;
}

VOID
EFIAPI
PlatformBootManagerAfterConsole ()
{
  EFI_STATUS                    Status      = EFI_SUCCESS;
  EFI_GRAPHICS_OUTPUT_PROTOCOL *GopProtocol = NULL;

  // Post Setup GOP
  PostGopSetup (&GopProtocol);

  // Display Boot Logo
  Status = DisplayBootGraphic (BG_SYSTEM_LOGO);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Display Boot Logo! Status = %r\n", __FUNCTION__, Status));
  }

  // Register Key Callback
  Status = RegisterKeyCallback ((EFI_DEVICE_PATH_PROTOCOL *)&KeypadDevicePath);
  if (!EFI_ERROR (Status) && GopProtocol != NULL) {
    // Set Screen Resolution
    UINT32 ScreenWidth  = GopProtocol->Mode->Info->HorizontalResolution;
    UINT32 ScreenHeight = GopProtocol->Mode->Info->VerticalResolution;

    // Set Combo Message Position
    XPos = (ScreenWidth - AsciiStrLen (ComboMessage) * EFI_GLYPH_WIDTH) / 2;
    YPos = ScreenHeight * 48 / 50;
  }

  // Execute Secondary After Console
  DeviceBootManagerAfterConsole ();
}

VOID
EFIAPI
PlatformBootManagerOnDemandConInConnect ()
{
  // Execute Secondary On Demand
  DeviceBootManagerOnDemandConInConnect ();
}

VOID
EFIAPI
PlatformBootManagerProcessBootCompletion (OUT EFI_BOOT_MANAGER_LOAD_OPTION *BootOption)
{
  return;
}

VOID
EFIAPI
PlatformBootManagerPriorityBoot (IN OUT UINT16 **BootNext)
{
  EFI_STATUS                   Status;
  EFI_BOOT_MANAGER_LOAD_OPTION LoadOption;
  BOOLEAN                      BootManagerFlag;

  // Unregister Key Callback
  UnregisterKeyCallback ();

  // Get Boot Manager Flag
  BootManagerFlag = EnterBootManager ();
  if (BootManagerFlag == FALSE) {
    return;
  }

  // Get Boot Manager Load Option
  Status = MsBootOptionsLibGetBootManagerMenu (&LoadOption, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to get Boot Manager Load Option! Status = %r", __FUNCTION__, Status));
    return;
  }

  // Execute Boot Manager Load Option
  EfiBootManagerBoot           (&LoadOption);
  EfiBootManagerFreeLoadOption (&LoadOption);
}

VOID
EFIAPI
PlatformBootManagerWaitCallback (IN UINT16 TimeoutRemain)
{
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL Color;

  // Get Timeout Time
  UINT16 Timeout = PcdGet16 (PcdPlatformBootTimeOut);

  // Check Remaining Timeout
  if (TimeoutRemain == Timeout || TimeoutRemain == 0) {
    // Set Draw Color
    UINT8 DrawColor = (TimeoutRemain == Timeout) ? 0xFF : 0x00;

    // Set GOP Draw Color
    Color.Red = Color.Green = Color.Blue = DrawColor;

    // Print Combo Message
    AsciiPrintXY (XPos, YPos, &Color, NULL, ComboMessage);
  }
}

VOID
EFIAPI
PlatformBootManagerUnableToBoot ()
{
  EFI_STATUS Status;

  // Execute Secondary Unable To Boot
  DeviceBootManagerUnableToBoot ();

  // Display No Boot OS Logo
  Status = DisplayBootGraphic (BG_NO_BOOT_OS);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Display No Boot OS Logo! Status = %r\n", __FUNCTION__, Status));
    Print (L"No Operating System Found! Please make sure that an OS is Installed.\n");
  }

  // Wait 10s
  gBS->Stall (10000000);

  // Shutdown
  gRT->ResetSystem (EfiResetShutdown, EFI_SUCCESS, 0, NULL);

  // Do Cpu Dead Loop
  CpuDeadLoop ();
}

VOID
EFIAPI
PreReadyToBoot (
  IN EFI_EVENT  Event,
  IN VOID      *Context)
{
  EFI_STATUS Status;

  // Setup Secure Boot
  Status = SetupSecureBoot ();
  ASSERT_EFI_ERROR (Status);

  // Close Event
  gBS->CloseEvent (Event);
}

VOID
EFIAPI
PlatformBootManagerBdsEntry ()
{
  EFI_STATUS Status;
  EFI_EVENT  PreReadyToBootEvent;

  // Register Pre-ReadyToBoot Event
  Status = gBS->CreateEventEx (EVT_NOTIFY_SIGNAL, TPL_CALLBACK, PreReadyToBoot, NULL, &gEfiEventPreReadyToBootGuid, &PreReadyToBootEvent);
  ASSERT_EFI_ERROR (Status);

  // Execute Secondary BDS Entry
  DeviceBootManagerBdsEntry ();
}
