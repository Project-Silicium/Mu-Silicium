/**
  Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PlatformBootManagerLib.h>
#include <Library/DevicePathLib.h>
#include <Library/UefiLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/CapsuleLib.h>
#include <Library/DeviceBootManagerLib.h>
#include <Library/HobLib.h>
#include <Library/PerformanceLib.h>
#include <Library/PcdLib.h>

#include <Guid/MemoryTypeInformation.h>
#include <Guid/MemoryOverwriteControl.h>

#include "BdsPlatform.h"

VOID
ExitPmAuth ()
{
  EFI_STATUS Status = EFI_SUCCESS;
  EFI_HANDLE Handle = NULL;

  PERF_FUNCTION_BEGIN (); // MS_CHANGE

  // Signal EndOfDxe
  EfiEventGroupSignal (&gEfiEndOfDxeEventGroupGuid);

  // Install Read to Lock Protocol
  Status = gBS->InstallProtocolInterface (&Handle, &gEfiDxeSmmReadyToLockProtocolGuid, EFI_NATIVE_INTERFACE, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Install Ready to Lock Protocol! Status = %r\n"));
    ASSERT (FALSE);
  }

  PERF_FUNCTION_END (); // MS_CHANGE
}

VOID
ConnectRootBridge (BOOLEAN Recursive)
{
  EFI_HANDLE *RootBridgeHandleBuffer = NULL;
  UINTN       RootBridgeIndex        = 0;
  UINTN       RootBridgeHandleCount  = 0;

  PERF_FUNCTION_BEGIN (); // MS_CHANGE

  // Locate PCI Root Bridge Handle Buffer
  gBS->LocateHandleBuffer (ByProtocol, &gEfiPciRootBridgeIoProtocolGuid, NULL, &RootBridgeHandleCount, &RootBridgeHandleBuffer);

  // Connect PCI Root Bridge Controller
  for (RootBridgeIndex = 0; RootBridgeIndex < RootBridgeHandleCount; RootBridgeIndex++) {
    gBS->ConnectController (RootBridgeHandleBuffer[RootBridgeIndex], NULL, NULL, Recursive);
  }

  PERF_FUNCTION_END (); // MS_CHANGE
}

BOOLEAN
IsGopDevicePath (IN EFI_DEVICE_PATH_PROTOCOL *DevicePath)
{
  while (!IsDevicePathEndType (DevicePath)) {
    if ((DevicePathType (DevicePath) == ACPI_DEVICE_PATH) && (DevicePathSubType (DevicePath) == ACPI_ADR_DP)) {
      return TRUE;
    }

    // Get Next Device Path
    DevicePath = NextDevicePathNode (DevicePath);
  }

  return FALSE;
}

EFI_DEVICE_PATH_PROTOCOL*
UpdateGopDevicePath (
  EFI_DEVICE_PATH_PROTOCOL *DevicePath,
  EFI_DEVICE_PATH_PROTOCOL *Gop)
{
  EFI_DEVICE_PATH_PROTOCOL *Return   = NULL;
  EFI_DEVICE_PATH_PROTOCOL *Instance = NULL;
  UINTN                     Size     = 0;
  UINTN                     GopSize  = 0;
  BOOLEAN                   Exist    = 0;

  // Get GOP Device Path Size
  GopSize = GetDevicePathSize (Gop);

  do {
    // Get Next Device Path
    Instance = GetNextDevicePathInstance (&DevicePath, &Size);

    if (Instance == NULL) {
      break;
    }

    if (!IsGopDevicePath (Instance) || ((Size == GopSize) && (CompareMem (Instance, Gop, GopSize) == 0))) {
      if ((Size == GopSize) && (CompareMem (Instance, Gop, GopSize) == 0)) {
        Exist = TRUE;
      }

      // Append New Device Path
      Return = AppendDevicePathInstance (Return, Instance);
    }

    FreePool (Instance);
  } while (DevicePath != NULL);

  if (!Exist) {
    // Append New Device Path
    Return = AppendDevicePathInstance (Return, Gop);
  }

  return Return;
}

VOID
EFIAPI
PlatformBootManagerBeforeConsole ()
{
  EFI_STATUS                 Status           = EFI_SUCCESS;
  EFI_HANDLE                 Handle           = NULL;
  EFI_DEVICE_PATH_PROTOCOL  *TempDevicePath   = NULL;
  EFI_DEVICE_PATH_PROTOCOL  *ConsoleOut       = NULL;
  BDS_CONSOLE_CONNECT_ENTRY *PlatformConsoles = NULL;

  // Get Current Boot Mode
  mBootMode = GetBootModeHob ();

  // Add USB Keyboard to ConIn
  EfiBootManagerUpdateConsoleVariable (ConIn, (EFI_DEVICE_PATH_PROTOCOL *)&mUsbClassKeyboardDevicePath, NULL);

  // Connect Root Bridge
  ConnectRootBridge (FALSE);

  // Get ConOut Handle
  Handle = DeviceBootManagerBeforeConsole (&TempDevicePath, &PlatformConsoles);

  // Update ConOut Variable
  GetEfiGlobalVariable2 (L"ConOut", (VOID **)&ConsoleOut, NULL);

  if (Handle != NULL) {
    if (TempDevicePath != NULL) {
      // Update & Get ConOut Device Path
      ConsoleOut = UpdateGopDevicePath (ConsoleOut, TempDevicePath);

      FreePool (TempDevicePath);

      if (ConsoleOut != NULL) {
        // Set ConOut Variable
        Status = gRT->SetVariable (L"ConOut", &gEfiGlobalVariableGuid, EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_BOOTSERVICE_ACCESS, GetDevicePathSize (ConsoleOut), ConsoleOut);

        if (EFI_ERROR (Status)) {
          DEBUG ((EFI_D_ERROR, "%a: Failed to Set ConOut Variable! Status = %r\n", __FUNCTION__, Status));
        }
      }
    }
  }

  if (ConsoleOut != NULL) {
    FreePool (ConsoleOut);
  }

  // Update Console Variables
  if (PlatformConsoles != NULL) {
    while (PlatformConsoles->DevicePath != NULL) {
      if ((PlatformConsoles->ConnectType & CONSOLE_IN) == CONSOLE_IN) {
        EfiBootManagerUpdateConsoleVariable (ConIn, PlatformConsoles->DevicePath, NULL);
      }

      if ((PlatformConsoles->ConnectType & CONSOLE_OUT) == CONSOLE_OUT) {
        EfiBootManagerUpdateConsoleVariable (ConOut, PlatformConsoles->DevicePath, NULL);
      }

      if ((PlatformConsoles->ConnectType & STD_ERROR) == STD_ERROR) {
        EfiBootManagerUpdateConsoleVariable (ErrOut, PlatformConsoles->DevicePath, NULL);
      }

      PlatformConsoles++;
    }
  }

  ExitPmAuth ();
  EfiBootManagerDispatchDeferredImages ();
}

VOID
ConnectSequence ()
{
  EFI_STATUS                 Status;
  EFI_HANDLE                 DeviceHandle;
  EFI_DEVICE_PATH_PROTOCOL **PlatformConnectSequence;

  PERF_FUNCTION_BEGIN (); // MS_CHANGE

  PlatformConnectSequence = mPlatformConnectSequence;

  if (PlatformConnectSequence != NULL) {
    while (*PlatformConnectSequence != NULL) {
      // Connect Device Path
      Status = EfiBootManagerConnectDevicePath (*PlatformConnectSequence, &DeviceHandle);

      if (!EFI_ERROR (Status)) {
        // Connect Controller
        gBS->ConnectController (DeviceHandle, NULL, NULL, TRUE);
      }

      PlatformConnectSequence++;
    }
  }

  // Dispatch
  gDS->Dispatch ();

  PERF_FUNCTION_END (); // MS_CHANGE
}

STATIC
VOID
SetMorControl ()
{
  EFI_STATUS Status       = EFI_SUCCESS;
  UINT8      MorControl   = 1;
  UINTN      VariableSize = sizeof (MorControl);

  // Set Memory Overwrite Variable
  Status = gRT->SetVariable (MEMORY_OVERWRITE_REQUEST_VARIABLE_NAME, &gEfiMemoryOverwriteControlDataGuid, EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS, VariableSize, &MorControl);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Set Memory Overwrite Control Variable! Status = %r\n", __FUNCTION__, Status));
  }
}

VOID
EFIAPI
PlatformBootManagerAfterConsole ()
{
  EFI_STATUS Status       = EFI_SUCCESS;
  UINT32     Attributes   = 0;
  UINTN      VariableSize = 0;

  // Get Platform Connect Sequence
  mPlatformConnectSequence = DeviceBootManagerAfterConsole ();

  switch (mBootMode) {
    case BOOT_ON_S4_RESUME:
    case BOOT_WITH_MINIMAL_CONFIGURATION:
    case BOOT_ASSUMING_NO_CONFIGURATION_CHANGES:
      ConnectSequence ();

      break;

    case BOOT_ON_FLASH_UPDATE:
      // Connect all EFI Controller Drivers
      EfiBootManagerConnectAll ();

      // Get Memory Type Variable
      Status = gRT->GetVariable (EFI_MEMORY_TYPE_INFORMATION_VARIABLE_NAME, &gEfiMemoryTypeInformationGuid, &Attributes, &VariableSize, NULL);
      if (Status == EFI_BUFFER_TOO_SMALL) {
        Status = gRT->SetVariable (EFI_MEMORY_TYPE_INFORMATION_VARIABLE_NAME, &gEfiMemoryTypeInformationGuid, Attributes, 0, NULL);
        if (EFI_ERROR (Status)) {
          DEBUG ((EFI_D_ERROR, "%a: Failed to Set Memory Type Variable! Status = %r\n", __FUNCTION__, Status));
          ASSERT (FALSE);
        }
      }

      // Signal Read to Process Capsules Signal
      EfiEventGroupSignal (&gMuReadyToProcessCapsulesNotifyGuid);

      // Process Capsules
      Status = ProcessCapsules ();
      if (EFI_ERROR (Status)) {
        DEBUG ((EFI_D_ERROR, "%a: Failed to Process Capsules! Status = %r\n", __FUNCTION__, Status));

        // Clear MOR
        SetMorControl ();
      }

      // Reboot System
      gRT->ResetSystem (EfiResetCold, EFI_SUCCESS, 0, NULL);
      break;

    case BOOT_IN_RECOVERY_MODE:
      break;

    case BOOT_WITH_FULL_CONFIGURATION:
    case BOOT_WITH_FULL_CONFIGURATION_PLUS_DIAGNOSTICS:
    case BOOT_WITH_DEFAULT_SETTINGS:
    default:
      ConnectSequence ();

      break;
  }

  // Proccess Capsules
  ProcessCapsules ();
}

STATIC UINTN                         XPos         = 0;
STATIC EFI_GRAPHICS_OUTPUT_PROTOCOL *mGOPProtocol = NULL;

VOID
EFIAPI
PlatformBootManagerWaitCallback (UINT16 TimeoutRemain)
{
  EFI_STATUS                    Status;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL Color;

  // Locate GOP Protocol
  if (mGOPProtocol == NULL) {
    Status = gBS->HandleProtocol (gST->ConsoleOutHandle, &gEfiGraphicsOutputProtocolGuid, (VOID *)&mGOPProtocol);

    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%a: Failed to Locate GOP Protocol! Status = %r\n", __FUNCTION__, Status));
      return;
    }
  }

  // Get Screen Size
  UINT32 ScreenWidth  = mGOPProtocol->Mode->Info->HorizontalResolution;
  UINT32 ScreenHeight = mGOPProtocol->Mode->Info->VerticalResolution;

  // Default Y Position
  UINTN YPos = ScreenHeight * 48 / 50;

  // Get Timeout Time
  UINT16 Timeout = PcdGet16(PcdPlatformBootTimeOut);

  if (!TimeoutRemain) {
    SetMem (&Color, sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL), 0x0);

    // Clear Combo Message & Timeout Bar
    Status = mGOPProtocol->Blt (mGOPProtocol, &Color, EfiBltVideoFill, 0, 0, 0, YPos - EFI_GLYPH_HEIGHT, ScreenWidth, ScreenHeight - (YPos - EFI_GLYPH_HEIGHT), ScreenHeight * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL));
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%a: Failed to Clear Combo Message! Status = %r\n", __FUNCTION__, Status));
    }
  } else {
    // Set Timeout Bar Color
    Color.Blue = Color.Green = Color.Red = Color.Reserved = 0xFF;

    for (UINTN i = 0; i < ScreenWidth / Timeout; i++) {
      // Draw Timeout Bar
      mGOPProtocol->Blt (mGOPProtocol, &Color, EfiBltVideoFill, 0, 0, XPos, YPos, 1, ScreenHeight / 50, (ScreenHeight / 50) * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL));

      XPos++;

      // Wait
      gBS->Stall (Timeout * 1000000 / ScreenWidth);
    }
  }
}
