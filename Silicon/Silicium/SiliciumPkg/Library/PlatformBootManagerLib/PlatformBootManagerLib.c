/**
  Copyright (C) Microsoft Corporation. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

// TODO: Implement Settings Handler & Silicium Graphics here!

#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DeviceBootManagerLib.h>
#include <Library/PlatformBootManagerLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/MsBootOptionsLib.h>
#include <Library/MsBootPolicyLib.h>
#include <Library/BootGraphicsLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/PerformanceLib.h>
#include <Library/DevicePathLib.h>
#include <Library/BootGraphics.h>
#include <Library/AslUpdateLib.h>
#include <Library/CapsuleLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiLib.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>

#include <Protocol/GraphicsOutput.h>

#include <Configuration/BootDevices.h>

//
// Platform Consoles List
//
BDS_CONSOLE_CONNECT_ENTRY gPlatformConsoles[] = {
  {
    (EFI_DEVICE_PATH_PROTOCOL *)&KeypadDevicePath,
    CONSOLE_IN
  },
  {
    (EFI_DEVICE_PATH_PROTOCOL *)&DisplayDevicePath,
    CONSOLE_OUT | STD_ERROR
  },
  {
    NULL,
    0
  }
};

BOOLEAN
IsGopDevicePath (IN EFI_DEVICE_PATH_PROTOCOL *DevicePath)
{
  // Gp thru each Device Path
  while (!IsDevicePathEndType (DevicePath)) {
    // Compare Device Path Types
    if ((DevicePathType (DevicePath) == ACPI_DEVICE_PATH) && (DevicePathSubType (DevicePath) == ACPI_ADR_DP)) {
      return TRUE;
    }

    // Get Next Device Path Node
    DevicePath = NextDevicePathNode (DevicePath);
  }

  return FALSE;
}

EFI_DEVICE_PATH_PROTOCOL*
UpdateGopDevicePath (
  IN EFI_DEVICE_PATH_PROTOCOL *ConsoleOutDevicePath,
  IN EFI_DEVICE_PATH_PROTOCOL *GopDevicePath)
{
  EFI_DEVICE_PATH_PROTOCOL *NewDevicePath;
  UINTN                     GopDevicePathSize;
  BOOLEAN                   Exists;

  // Get GOP Device Path Size
  GopDevicePathSize = GetDevicePathSize (GopDevicePath);

  do {
    UINTN InstanceDevicePathSize;

    // Get Device Path Instance
    EFI_DEVICE_PATH_PROTOCOL *InstanceDevicePath = GetNextDevicePathInstance (&ConsoleOutDevicePath, &InstanceDevicePathSize);
    if (InstanceDevicePath == NULL) {
      break;
    }

    // Check Device Path
    if (!IsGopDevicePath (InstanceDevicePath) || ((InstanceDevicePathSize == GopDevicePathSize) && (CompareMem (InstanceDevicePath, GopDevicePath, GopDevicePathSize) == 0))) {
      // Compare Device Path
      if ((InstanceDevicePathSize == GopDevicePathSize) && (CompareMem (InstanceDevicePath, GopDevicePath, GopDevicePathSize) == 0)) {
        Exists = TRUE;
      }

      // Append New Device Path
      NewDevicePath = AppendDevicePathInstance (NewDevicePath, InstanceDevicePath);
    }

    // Free Buffer
    FreePool (InstanceDevicePath);
  } while (ConsoleOutDevicePath != NULL);

  // Append New Device Path
  if (!Exists) {
    NewDevicePath = AppendDevicePathInstance (NewDevicePath, GopDevicePath);
  }

  return NewDevicePath;
}

VOID
EFIAPI
PlatformBootManagerBeforeConsole ()
{
  EFI_STATUS                 Status           = EFI_SUCCESS;
  EFI_HANDLE                 Handle           = NULL;
  EFI_DEVICE_PATH_PROTOCOL  *ConsoleOut       = NULL;
  EFI_DEVICE_PATH_PROTOCOL  *GopDevicePath    = (EFI_DEVICE_PATH_PROTOCOL *)&DisplayDevicePath;
  BDS_CONSOLE_CONNECT_ENTRY *PlatformConsoles = (BDS_CONSOLE_CONNECT_ENTRY *)&gPlatformConsoles;

  // Add USB Keyboard to Console In
  EfiBootManagerUpdateConsoleVariable (ConIn, (EFI_DEVICE_PATH_PROTOCOL *)&mUsbKeyboardDevicePath, NULL);

  // Register Boot Options
  MsBootOptionsLibRegisterDefaultBootOptions ();

  // Locate GOP Device Path
  Status = gBS->LocateDevicePath (&gEfiGraphicsOutputProtocolGuid, &GopDevicePath, &Handle);
  if (EFI_ERROR (Status) && !IsDevicePathEnd (GopDevicePath)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Locate GOP Device Path! Status = %r\n", __FUNCTION__, Status));
  }

  // Check Display Handle
  if (Handle != NULL) {
    // Connect GOP Driver
    gBS->ConnectController (Handle, NULL, NULL, TRUE);

    // Get the GOP Device Path
    GopDevicePath = EfiBootManagerGetGopDevicePath (Handle);
  }

  // Get ConOut Variable
  GetEfiGlobalVariable2 (L"ConOut", (VOID *)&ConsoleOut, NULL);

  // Check Handle
  if (Handle != NULL) {
    // Check Device Path
    if (GopDevicePath != NULL) {
      // Get Updated Console Out Device Path
      ConsoleOut = UpdateGopDevicePath (ConsoleOut, GopDevicePath);

      // Free Buffer
      FreePool (GopDevicePath);

      // Check Console Out Device Path
      if (ConsoleOut != NULL) {
        // Set ConOut Variable
        Status = gRT->SetVariable (L"ConOut", &gEfiGlobalVariableGuid, EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_BOOTSERVICE_ACCESS, GetDevicePathSize (ConsoleOut), ConsoleOut);
        if (EFI_ERROR (Status)) {
          DEBUG ((EFI_D_ERROR, "%a: Failed to Set ConOut Variable! Status = %r\n", __FUNCTION__, Status));
        }
      }
    }
  }

  // Free Buffer
  if (ConsoleOut != NULL) {
    FreePool (ConsoleOut);
  }

  // Update Console Variables
  if (PlatformConsoles != NULL) {
    while (PlatformConsoles->DevicePath != NULL) {
      // Update Console In Variable
      if ((PlatformConsoles->ConnectType & CONSOLE_IN) == CONSOLE_IN) {
        EfiBootManagerUpdateConsoleVariable (ConIn, PlatformConsoles->DevicePath, NULL);
      }

      // Update Console Out Variable
      if ((PlatformConsoles->ConnectType & CONSOLE_OUT) == CONSOLE_OUT) {
        EfiBootManagerUpdateConsoleVariable (ConOut, PlatformConsoles->DevicePath, NULL);
      }

      // Update Error Out Variable
      if ((PlatformConsoles->ConnectType & STD_ERROR) == STD_ERROR) {
        EfiBootManagerUpdateConsoleVariable (ErrOut, PlatformConsoles->DevicePath, NULL);
      }

      // Switch to next Console
      PlatformConsoles++;
    }
  }

  // Reset Handle
  Handle = NULL;

  // Perfermonace Start
  PERF_FUNCTION_BEGIN ();

  // Signal EndOfDxe
  EfiEventGroupSignal (&gEfiEndOfDxeEventGroupGuid);

  // Register Ready to Lock Protocol
  Status = gBS->InstallProtocolInterface (&Handle, &gEfiDxeSmmReadyToLockProtocolGuid, EFI_NATIVE_INTERFACE, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Register Ready to Lock Protocol!\n", __FUNCTION__));
    ASSERT_EFI_ERROR (Status);
  }

  // Performance End
  PERF_FUNCTION_END ();

  // Dispatch Deferred Images
  EfiBootManagerDispatchDeferredImages ();
}

VOID
EFIAPI
PlatformBootManagerAfterConsole ()
{
  EFI_STATUS    Status   = EFI_SUCCESS;
  EFI_BOOT_MODE BootMode = GetBootModeHob ();

  // Platform DSDT Table Updates
  PlatformUpdateAcpiTables ();

  // Device DSDT Table Updates
  DeviceUpdateAcpiTables ();

  // Display Boot Logo on Screen
  Status = DisplayBootGraphic (BG_SYSTEM_LOGO);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Display Boot Logo! Status = %r\n", __FUNCTION__, Status));
  }

  // Run Boot Mode Config
  switch (BootMode) {
    case BOOT_WITH_DEFAULT_SETTINGS:
      // Perfermonace Start
      PERF_FUNCTION_BEGIN ();

      // Dispatch
      gDS->Dispatch ();

      // Performance End
      PERF_FUNCTION_END ();
      break;

    case BOOT_ON_S4_RESUME:
    case BOOT_WITH_MINIMAL_CONFIGURATION:
    case BOOT_ASSUMING_NO_CONFIGURATION_CHANGES:
    case BOOT_ON_FLASH_UPDATE:
    case BOOT_IN_RECOVERY_MODE:
    case BOOT_WITH_FULL_CONFIGURATION:
    case BOOT_WITH_FULL_CONFIGURATION_PLUS_DIAGNOSTICS:
    default:
      DEBUG ((EFI_D_ERROR, "%a: Boot Mode %u is not Supported!\n", __FUNCTION__, BootMode));
      ASSERT_EFI_ERROR (EFI_UNSUPPORTED);
  }

  // Proccess Capsules
  ProcessCapsules ();
}

VOID
EFIAPI
PlatformBootManagerOnDemandConInConnect ()
{
  EFI_STATUS  Status;
  EFI_HANDLE *HandleBuffer;
  UINTN       HandleCount;

  // Locate Absolute Pointer Protocol Handles
  Status = gBS->LocateHandleBuffer (ByProtocol, &gEfiAbsolutePointerProtocolGuid, NULL, &HandleCount, &HandleBuffer);
  if (!EFI_ERROR (Status)) {
    // Connect Absolute Pointer Protocol Controllers
    for (UINTN Index = 0; Index < HandleCount; Index++) {
      gBS->ConnectController (HandleBuffer[Index], NULL, NULL, TRUE);
    }
  }
}

VOID
EFIAPI
PlatformBootManagerBdsEntry ()
{
  // Signal that BDS Started
  EfiEventGroupSignal (&gMsStartOfBdsNotifyGuid);
}

VOID
EFIAPI
PlatformBootManagerProcessBootCompletion (OUT EFI_BOOT_MANAGER_LOAD_OPTION *BootOption)
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

VOID
EFIAPI
PlatformBootManagerPriorityBoot (OUT UINT16 **BootNext)
{
  EFI_STATUS                   Status;
  EFI_BOOT_MANAGER_LOAD_OPTION BootOption;
  EFI_BOOT_MANAGER_LOAD_OPTION BootManagerMenu;

  // Get Special Boot Option
  BOOLEAN FfuMode    = MsBootPolicyLibIsSettingsBoot ();
  BOOLEAN SpecialApp = MsBootPolicyLibIsAltBoot ();

  // Clear Boot Requests
  MsBootPolicyLibClearBootRequests ();

  // Get Special Boot Option
  if (FfuMode) {
    // Get UEFI Menu Boot Option
    Status = MsBootOptionsLibGetBootManagerMenu (&BootOption, "VOL+");
    if (EFI_ERROR (Status)) {
      return;
    }
  } else if (SpecialApp) {
    // Get Special App Boot Option
    Status = MsBootOptionsLibGetDefaultBootApp (&BootOption, "VOL-");
    if (EFI_ERROR (Status)) {
      return;
    }
  } else {
    return;
  }

  // Free Buffer
  if (*BootNext != NULL) {
    FreePool (*BootNext);
  }

  // Try Special Boot Option
  EfiBootManagerBoot (&BootOption);

  // Save Status
  Status = BootOption.Status;

  // Free Load Option
  EfiBootManagerFreeLoadOption (&BootOption);

  // Get Boot Manager Menu
  if (!EFI_ERROR (Status)) {
    Status = EfiBootManagerGetBootManagerMenu (&BootManagerMenu);
    if (!EFI_ERROR (Status)) {
      // Try UEFI Menu Boot Option
      EfiBootManagerBoot           (&BootManagerMenu);
      EfiBootManagerFreeLoadOption (&BootManagerMenu);
    }
  }
}

//
// Static Global Variables
//
STATIC EFI_GRAPHICS_OUTPUT_PROTOCOL *mGopProtocol   = NULL;
STATIC CHAR16                       *ComboMessage   = NULL;
STATIC UINTN                         FormXPos       = 0;
STATIC UINTN                         FormYPos       = 0;
STATIC UINTN                         TextXPos       = 0;
STATIC UINTN                         TextYPos       = 0;

VOID
EFIAPI
PlatformBootManagerWaitCallback (UINT16 TimeoutRemain)
{
  EFI_STATUS                    Status;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL Color;

  // Get Timeout Time
  UINT16 Timeout = PcdGet16 (PcdPlatformBootTimeOut);

  // Locate GOP Protocol
  if (!mGopProtocol) {
    Status = gBS->LocateProtocol (&gEfiGraphicsOutputProtocolGuid, NULL, (VOID *)&mGopProtocol);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%a: Failed to Locate GOP Protocol of Primary Display! Status = %r\n", Status));
      return;
    }
  }

  // Get Screen Resolution
  UINT32 ScreenWidth  = mGopProtocol->Mode->Info->HorizontalResolution;
  UINT32 ScreenHeight = mGopProtocol->Mode->Info->VerticalResolution;

  // Set Timeout Bar Size
  UINTN Width  = ScreenWidth * 40 / 100 - 200;
  UINTN Height = Width * 10 / 100;

  // Set Timeout Bar Position
  if ((!FormXPos && !FormYPos) || !TimeoutRemain) {
    FormXPos = (ScreenWidth - Width) / 2;
    FormYPos = (ScreenHeight + Width + 200) / 2;
  }

  // Set Combo Message
  if (ComboMessage == NULL) {
    if (FixedPcdGetPtr (PcdSpecialApp) == "NULL") {
      ComboMessage = L"[Volume Up] FFU Mode";
    } else {
      // Allocate Memory
      ComboMessage = AllocateZeroPool (150);
      if (ComboMessage == NULL) {
        DEBUG ((EFI_D_ERROR, "Failed to Allocate Memory for Combo Message!\n"));
        goto Form;
      }

      UnicodeSPrint (ComboMessage, 150, L"[Volume Up] FFU Mode - [Volume Down] %a", FixedPcdGetPtr (PcdSpecialAppName));
    }

    // Set Combo Message Position
    TextXPos = (ScreenWidth - StrLen (ComboMessage) * EFI_GLYPH_WIDTH) / 2;
    TextYPos = (mGopProtocol->Mode->Info->VerticalResolution - EFI_GLYPH_HEIGHT) * 48 / 50;

    // Animate Combo Message
    for (UINT8 i = 0; i < 255; i++) {
      // Set New Foreground Color
      Color.Blue = Color.Green = Color.Red = i;
      Color.Reserved = 0xFF;

      // Print Combo Message
      PrintXY (TextXPos, TextYPos, &Color, NULL, ComboMessage);

      // Wait a Bit
      gBS->Stall (2500);
    }
  }

Form:
  // Draw Background
  if (TimeoutRemain == Timeout) {
    for (UINT8 i = 0; i < 34; i++) {
      // Set New Background Color
      Color.Blue = Color.Green = Color.Red = i;
      Color.Reserved = 0xFF;

      // Draw New Color
      mGopProtocol->Blt (mGopProtocol, &Color, EfiBltVideoFill, 0, 0, FormXPos, FormYPos, Width, Height, 0);

      // Wait a Bit
      gBS->Stall (23000);
    }
  }

  // Clear Timeout Bar
  if (!TimeoutRemain) {
    for (UINT8 i = 255; i > 0; i--) {
      // Set New Foreground Color
      Color.Blue = Color.Green = Color.Red = i;
      Color.Reserved = 0xFF;

      // Print Combo Message
      if (ComboMessage != NULL) {
        PrintXY (TextXPos, TextYPos, &Color, NULL, ComboMessage);
      }

      // Draw New Color
      mGopProtocol->Blt (mGopProtocol, &Color, EfiBltVideoFill, 0, 0, FormXPos, FormYPos, Width, Height, 0);

      // Wait a Bit
      gBS->Stall (3000);
    }

    if (FixedPcdGetPtr (PcdSpecialApp) != "NULL") {
      if (ComboMessage != NULL) {
        FreePool (ComboMessage);
      }
    }

    return;
  }

  // Set Foreground Color
  Color.Blue = Color.Green = Color.Red = Color.Reserved = 0xFF;

  // Draw Foreground
  for (UINTN i = 0; i < Width / Timeout; i++) {
    mGopProtocol->Blt (mGopProtocol, &Color, EfiBltVideoFill, 0, 0, FormXPos, FormYPos, 1, Height, 0);

    FormXPos++;

    // Wait
    gBS->Stall (Timeout * 1000000 / Width);
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

STATIC
VOID
EFIAPI
PreReadyToBoot (
  IN EFI_EVENT  Event,
  IN VOID      *Context)
{
  gBS->CloseEvent (Event);
}

STATIC
VOID
EFIAPI
PostReadyToBoot (
  IN EFI_EVENT  Event,
  IN VOID      *Context)
{
  // Do Nothing
}

EFI_STATUS
EFIAPI
PlatformBootManagerEntry (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status;
  EFI_EVENT  mPreReadyToBootEvent;
  EFI_EVENT  mPostReadyToBootEvent;

  // Register OnReadyToBoot Event
  Status = gBS->CreateEventEx (EVT_NOTIFY_SIGNAL, TPL_CALLBACK, PreReadyToBoot, NULL, &gEfiEventPreReadyToBootGuid, &mPreReadyToBootEvent);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Register OnReadyToBoot Event! Status = %r\n", __FUNCTION__, Status));
  }

  // Register OnPostReadyToBoot Event
  Status = gBS->CreateEventEx (EVT_NOTIFY_SIGNAL, TPL_CALLBACK, PostReadyToBoot, NULL, &gEfiEventPostReadyToBootGuid, &mPostReadyToBootEvent);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Register OnPostReadyToBoot Event! Status = %r\n", Status));
  }

  return EFI_SUCCESS;
}
