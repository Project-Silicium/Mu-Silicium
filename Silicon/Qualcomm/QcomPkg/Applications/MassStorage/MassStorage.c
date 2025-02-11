#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/BootGraphicsLib.h>
#include <Library/BootGraphics.h>

#include <Protocol/EFIUsbMsd.h>
#include <Protocol/EFIChargerEx.h>

#include "MassStorage.h"

// Global Protocols
STATIC EFI_USB_MSD_PROTOCOL         *mUsbMsdProtocol;
STATIC EFI_CHARGER_EX_PROTOCOL      *mChargerExProtocol;
STATIC EFI_GRAPHICS_OUTPUT_PROTOCOL *mConsoleOutHandle;

VOID
PrintGUI (IN CHAR16 *Message)
{
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL Color;

  // Set Message Color
  Color.Blue = Color.Green = Color.Red = Color.Reserved = 0xFF;

  // Set Position of Message
  UINTN XPos = (mConsoleOutHandle->Mode->Info->HorizontalResolution - StrLen (Message) * EFI_GLYPH_WIDTH) / 2;
  UINTN YPos = (mConsoleOutHandle->Mode->Info->VerticalResolution - EFI_GLYPH_HEIGHT) * 48 / 50;

  // Print New Message
  PrintXY (XPos, YPos, &Color, NULL, Message);
}

EFI_STATUS
StartMassStorage ()
{
  EFI_STATUS Status        = EFI_SUCCESS;
  BOOLEAN    Connected     = FALSE;
  UINTN      CurrentSplash = 0;

  // Start Mass Storage
  Status = mUsbMsdProtocol->StartDevice (mUsbMsdProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to set USB Values! Status = %r\n", Status));
    return Status;
  }

  do {
    // Execute Event Handler
    mUsbMsdProtocol->EventHandler (mUsbMsdProtocol);

    // Get Charger Presence
    mChargerExProtocol->GetChargerPresence (&Connected);

    // Display Splash depending on Connect State
    if (Connected && CurrentSplash != BG_MSD_CONNECTED) {
      // Display Connected Splash
      DisplayBootGraphic (BG_MSD_CONNECTED);

      // Set Current Splash Value
      CurrentSplash = BG_MSD_CONNECTED;

      // New Message
      PrintGUI (L"Disconnect your Device to Enable Exit Function.");
    } else if (!Connected && CurrentSplash != BG_MSD_DISCONNECTED) {
      // Display Disconnected Splash
      DisplayBootGraphic (BG_MSD_DISCONNECTED);

      // Set Current Splash Value
      CurrentSplash = BG_MSD_DISCONNECTED;

      // New Message
      PrintGUI (L"Press Volume Up Button to Exit Mass Storage.");
    }

    // Display Confirm Message
    if (!Connected) {
      EFI_INPUT_KEY Key;

      // Get current Key
      gST->ConIn->ReadKeyStroke (gST->ConIn, &Key);

      // Leave Loop
      if (Key.ScanCode == SCAN_UP) {
        // Stop Mass Storage
        mUsbMsdProtocol->StopDevice (mUsbMsdProtocol);

        // Remove Assigned BLK IO Protocol
        mUsbMsdProtocol->AssignBlkIoHandle (mUsbMsdProtocol, NULL, 0);

        // Exit Application
        break;
      }
    }
  } while (TRUE);

  return EFI_SUCCESS;
}

EFI_STATUS
PrepareMassStorage ()
{
  EFI_STATUS                Status            = EFI_SUCCESS;
  EFI_DEVICE_PATH_PROTOCOL *UFSDevicePath     = (EFI_DEVICE_PATH_PROTOCOL *)&UFSLun0DevicePath;
  EFI_DEVICE_PATH_PROTOCOL *eMMCDevicePath    = (EFI_DEVICE_PATH_PROTOCOL *)&eMMCUserPartitionDevicePath;
  EFI_BLOCK_IO_PROTOCOL    *eMMCBlkIoProtocol = NULL;
  EFI_BLOCK_IO_PROTOCOL    *UFSBlkIoProtocol  = NULL;
  EFI_HANDLE                UFSHandle         = NULL;
  EFI_HANDLE                eMMCHandle        = NULL;

  // Locate USB Mass Storage Protocol
  Status = gBS->LocateProtocol (&gEfiUsbMsdProtocolGuid, NULL, (VOID *)&mUsbMsdProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate USB Mass Storage Protocol! Status = %r\n", Status));
    return Status;
  }

  // Locate Charger Protocol
  Status = gBS->LocateProtocol (&gChargerExProtocolGuid, NULL, (VOID *)&mChargerExProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate Charger Ex Protocol! Status = %r\n", Status));
    return Status;
  }

  // Locate Console Out Protocol
  Status = gBS->HandleProtocol (gST->ConsoleOutHandle, &gEfiGraphicsOutputProtocolGuid, (VOID *)&mConsoleOutHandle);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate Console Out Protocol! Status = %r\n", Status));
    return Status;
  }

  // Locate UFS & eMMC Device Path
  Status  = gBS->LocateDevicePath (&gEfiBlockIoProtocolGuid, &UFSDevicePath,  &UFSHandle);
  Status |= gBS->LocateDevicePath (&gEfiBlockIoProtocolGuid, &eMMCDevicePath, &eMMCHandle);
  if (EFI_ERROR (Status) && Status != EFI_NOT_FOUND) {
    DEBUG ((EFI_D_ERROR, "Failed to Get Device Path of UFS/eMMC! Status = %r\n", Status));
    return EFI_NOT_FOUND;
  }

  // Open & Assign UFS BLK IO Protocol
  if (UFSHandle != NULL) {
    // Get Protocol
    Status = gBS->OpenProtocol (UFSHandle, &gEfiBlockIoProtocolGuid, (VOID *)&UFSBlkIoProtocol, NULL, NULL, EFI_OPEN_PROTOCOL_GET_PROTOCOL);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "Failed to Open BLK IO Protocol of UFS! Status = %r\n", Status));
      return Status;
    }

    // Assign Protocol
    Status = mUsbMsdProtocol->AssignBlkIoHandle (mUsbMsdProtocol, UFSBlkIoProtocol, 0);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "Failed to Assign UFS BLK IO Protocol! Status = %r\n", Status));
      return Status;
    }
  }
  
  // Open & Assign eMMC BLK IO Protocol
  if (eMMCHandle != NULL) {
    // Get Protocol
    Status = gBS->OpenProtocol (eMMCHandle, &gEfiBlockIoProtocolGuid, (VOID *)&eMMCBlkIoProtocol, NULL, NULL, EFI_OPEN_PROTOCOL_GET_PROTOCOL);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "Failed to Open BLK IO Protocol of eMMC! Status = %r\n", Status));
      return Status;
    }

    // Assign Protocol
    Status = mUsbMsdProtocol->AssignBlkIoHandle (mUsbMsdProtocol, eMMCBlkIoProtocol, 0);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "Failed to Assign eMMC BLK IO Protocol! Status = %r\n", Status));
      return Status;
    }
  }

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
InitMassStorage (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status;

  // Check for Sony & Google
  if (FixedPcdGetPtr (PcdSmbiosSystemManufacturer) == "Sony Group Corporation" || FixedPcdGetPtr (PcdSmbiosSystemManufacturer) == "Google LLC") {
    return EFI_UNSUPPORTED;
  }

  // Reset Input Protocol
  gST->ConIn->Reset (gST->ConIn, TRUE);

  // Display Warning Message
  DisplayBootGraphic (BG_MSD_WARNING);

  // Check For Key Press
  do {
    EFI_INPUT_KEY Key;

    // Get Current Key Press
    gST->ConIn->ReadKeyStroke (gST->ConIn, &Key);

    // Leave Loop
    if (Key.UnicodeChar == CHAR_CARRIAGE_RETURN) {
      break;
    }

    // Exit Mass Storage
    if (Key.ScanCode == SCAN_UP) {
      return EFI_ABORTED;
    }
  } while (TRUE);

  // Disable WatchDog Timer
  gBS->SetWatchdogTimer (0, 0, 0, (CHAR16 *)NULL);

  // Prepare Mass Storage
  Status = PrepareMassStorage ();
  if (EFI_ERROR (Status)) {
    goto error;
  }

  // Start Mass Storage
  Status = StartMassStorage ();
  if (EFI_ERROR (Status)) {
    goto error;
  }

  return EFI_SUCCESS;

error:
  // Display Failed Splash
  DisplayBootGraphic (BG_MSD_ERROR);

  // Check for Any Button
  gBS->WaitForEvent (1, gST->ConIn->WaitForKey, 0);

  return Status;
}
