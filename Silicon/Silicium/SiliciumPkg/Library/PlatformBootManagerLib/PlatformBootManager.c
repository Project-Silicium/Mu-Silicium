/**
  Copyright (C) DuoWoA authors. All rights reserved.
  Copyright (C) Microsoft Corporation. All rights reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/AcpiTableUpdateLib.h>
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

#include "PlatformBootManager.h"

//
// Global Variables
//
STATIC CHAR16 *ComboMessage = NULL;
STATIC UINTN   XPos         = 0;
STATIC UINTN   YPos         = 0;

VOID
EFIAPI
PlatformBootManagerBeforeConsole ()
{
  EFI_STATUS Status;

  // Signal End Of DXE
  EfiEventGroupSignal (&gEfiEndOfDxeEventGroupGuid);

  // Dispatch Deferred Images
  EfiBootManagerDispatchDeferredImages ();

  // Setup Secure Boot
  Status = SetupSecureBoot ();
  ASSERT_EFI_ERROR (Status);

  // Register Boot Options
  MsBootOptionsLibRegisterDefaultBootOptions ();

  // Register Console Variables
  EfiBootManagerUpdateConsoleVariable (ConIn,  (EFI_DEVICE_PATH_PROTOCOL *)&KeypadDevicePath,  NULL);
  EfiBootManagerUpdateConsoleVariable (ConOut, (EFI_DEVICE_PATH_PROTOCOL *)&DisplayDevicePath, NULL);
  EfiBootManagerUpdateConsoleVariable (ErrOut, (EFI_DEVICE_PATH_PROTOCOL *)&DisplayDevicePath, NULL);

  // Update ACPI Tables
  UpdateAcpiTables ();
}

VOID
CreateComboMessage (
  IN UINT32 ScreenWidth,
  IN UINT32 ScreenHeight)
{
  UINTN MessageLength;

  // Set Combo Message Parts
  CHAR8 *InitalMessage = "[Volume Up] FFU Mode";
  CHAR8 *AltAppName    = (CHAR8 *)FixedPcdGetPtr (PcdAlternativeAppFileName);

  // Set Combo Message Length
  MessageLength = AsciiStrLen (InitalMessage) + 1;
  if (AltAppName != "NULL") {
    MessageLength += AsciiStrLen (AltAppName) + 17;
  }

  // Fix Buffer Size
  MessageLength *= sizeof (CHAR16);

  // Alocate Memory
  ComboMessage = AllocateZeroPool (MessageLength);
  if (ComboMessage == NULL) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Allocate Memory for Combo Message!\n", __FUNCTION__));
    return;
  }

  // Set Combo Message
  if (AltAppName == "NULL") {
    UnicodeSPrint (ComboMessage, MessageLength, L"%a", InitalMessage);
  } else {
    UnicodeSPrint (ComboMessage, MessageLength, L"%a - [Volume Down] %a", InitalMessage, AltAppName);
  }

  // Set Combo Message Position
  XPos = (ScreenWidth - StrLen (ComboMessage) * EFI_GLYPH_WIDTH) / 2;
  YPos = ScreenHeight * 48 / 50;
}

VOID
SetMaxConsoleMode (
  IN  EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *ConOut,
  OUT UINT32                          *ScreenWidth,
  OUT UINT32                          *ScreenHeight)
{
  EFI_STATUS Status    = EFI_SUCCESS;
  UINTN      MaxMode   = 0;
  UINTN      Colums[2] = {0};
  UINTN      Rows[2]   = {0};

  // Go thru each Console Mode
  for (INT32 i = 0; i < ConOut->Mode->MaxMode; i++) {
    // Get Specified Console Mode Details
    Status = ConOut->QueryMode (ConOut, i, &Colums[0], &Rows[0]);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%a: Failed to get Console Resolution of Mode %d! Status = %r\n", __FUNCTION__, i, Status));
      continue;
    }

    // Compare Console Resolutions
    if ((Colums[0] > Colums[1]) && (Rows[0] > Rows[1])) {
      // Save Console Mode
      MaxMode = i;

      // Save Console Resolution
      Colums[1] = Colums[0];
      Rows[1]   = Rows[0];
    }
  }

  // Set Max Console Mode
  Status = ConOut->SetMode (ConOut, MaxMode);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to set Max Console Mode! Status = %r\n", __FUNCTION__, Status));
    return;
  }

  // Pass Screen Resolution
  *ScreenWidth  = Colums[1] * EFI_GLYPH_WIDTH;
  *ScreenHeight = Rows[1] * EFI_GLYPH_HEIGHT;
}

VOID
EFIAPI
PlatformBootManagerAfterConsole ()
{
  EFI_STATUS Status;
  UINT32     ScreenWidth;
  UINT32     ScreenHeight;

  // Set Max Console Mode
  SetMaxConsoleMode (gST->ConOut, &ScreenWidth, &ScreenHeight);

  // Display Boot Logo
  Status = DisplayBootGraphic (BG_SYSTEM_LOGO);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Display Boot Logo! Status = %r\n", __FUNCTION__, Status));
  }

  // Verify Screen Resolution
  if (!ScreenWidth || !ScreenHeight) {
    return;
  }

  // Register Key Callback
  Status = RegisterKeyCallback ((EFI_DEVICE_PATH_PROTOCOL *)&KeypadDevicePath);
  if (!EFI_ERROR (Status)) {
    // Create Combo Message
    CreateComboMessage (ScreenWidth, ScreenHeight);
  }
}

VOID
EFIAPI
PlatformBootManagerOnDemandConInConnect ()
{
  return;
}

VOID
EFIAPI
PlatformBootManagerBdsEntry ()
{
  return;
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
  BOOLEAN                      EnterFfuMode;
  BOOLEAN                      EnterAlternativeApp;

  // Get current Key States
  GetKeyStates (&EnterFfuMode, &EnterAlternativeApp);

  // Unregister Key Callback
  UnregisterKeyCallback ();

  // Verify Key States
  if (!EnterFfuMode && !EnterAlternativeApp) {
    return;
  }

  // Check FFU Mode & Alternative App
  if (EnterFfuMode) {
    // Get FFU Mode Load Option
    Status = MsBootOptionsLibGetBootManagerMenu (&LoadOption, NULL);
  } else if (EnterAlternativeApp) {
    // Get Alternative App Load Option
    Status = MsBootOptionsLibGetDefaultBootApp (&LoadOption, NULL);
  }

  // Check for Errors
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to get Specified Load Option! Status = %r", __FUNCTION__, Status));
    return;
  }

  // Wipe next Boot Option
  if (EnterFfuMode && *BootNext != NULL) {
    FreePool (*BootNext);
  }

  // Execute Specified Load Option
  EfiBootManagerBoot           (&LoadOption);
  EfiBootManagerFreeLoadOption (&LoadOption);
}

VOID
EFIAPI
PlatformBootManagerWaitCallback (IN UINT16 TimeoutRemain)
{
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL Color;

  // Verify Combo Message
  if (ComboMessage == NULL) {
    return;
  }

  // Get Timeout Time
  UINT16 Timeout = PcdGet16 (PcdPlatformBootTimeOut);

  // Check Remaining Time
  if (TimeoutRemain == Timeout) {
    // Set Text Color
    Color.Red = Color.Green = Color.Blue = 0xFF;

    // Print Combo Message
    PrintXY (XPos, YPos, &Color, NULL, ComboMessage);
  } else if (!TimeoutRemain) {
    // Set Text Color
    Color.Red = Color.Green = Color.Blue = 0x00;

    // Clear Combo Message
    PrintXY (XPos, YPos, &Color, NULL, ComboMessage);

    // Free Buffer
    FreePool (ComboMessage);
  }
}

VOID
EFIAPI
PlatformBootManagerUnableToBoot ()
{
  EFI_STATUS Status;

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
