#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BootGraphicsLib.h>
#include <Library/BootGraphics.h>
#include <Library/UefiLib.h>

#include <Protocol/EFIUsbMsd.h>
#include <Protocol/EFIChargerEx.h>

#include "MassStorage.h"

//
// Global Protocols
//
STATIC EFI_GRAPHICS_OUTPUT_PROTOCOL *mGopProtocol;

VOID
PrintGUI (
  IN CHAR16  *Message,
  IN UINT8    MessageArg,
  IN BOOLEAN  UseMessageArg)
{
  EFI_STATUS                    Status = EFI_SUCCESS;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL Color  = {0xFF, 0xFF, 0xFF, 0xFF};

  // Locate GOP Protocol
  if (mGopProtocol == NULL) {
    Status = gBS->LocateProtocol (&gEfiGraphicsOutputProtocolGuid, NULL, (VOID *)&mGopProtocol);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "Failed to Locate Console Out Protocol! Status = %r\n", Status));
      return;
    }
  }

  // Set Position X & Y Position
  UINTN XPos = (mGopProtocol->Mode->Info->HorizontalResolution - StrLen (Message) * EFI_GLYPH_WIDTH) / 2;
  UINTN YPos = (mGopProtocol->Mode->Info->VerticalResolution - EFI_GLYPH_HEIGHT) * 48 / 50;

  // Print New Message
  if (UseMessageArg) {
    PrintXY (XPos, YPos, &Color, NULL, Message, MessageArg);
  } else {
    PrintXY (XPos, YPos, &Color, NULL, Message);
  }
}

EFI_STATUS
StartMassStorage (
  IN EFI_USB_MSD_PROTOCOL    *mUsbMsdProtocol,
  IN EFI_CHARGER_EX_PROTOCOL *mChargerExProtocol,
  IN EFI_BLOCK_IO_PROTOCOL   *mDiskIoProtocol)
{
  EFI_STATUS Status        = EFI_SUCCESS;
  BOOLEAN    Connected     = FALSE;
  UINT8      CurrentSplash = 0;

  // Reset Input Protocol
  gST->ConIn->Reset (gST->ConIn, TRUE);

  // Assing DiskIO Protocol
  Status = mUsbMsdProtocol->AssignBlkIoHandle (mUsbMsdProtocol, mDiskIoProtocol, 0);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Assing DiskIO Protocol! Status = %r\n", Status));
    return Status;
  }

  // Start Mass Storage
  Status = mUsbMsdProtocol->StartDevice (mUsbMsdProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to set USB Attributes! Status = %r\n", Status));
    return Status;
  }

  if (mChargerExProtocol == NULL) {
    // Display Unknown USB State Splash
    DisplayBootGraphic (BG_MSD_UNKNOWN_STATE);
  }

  // Print Exit Message
  PrintGUI (L"Press Volume Up Button to Exit Mass Storage.", 0, FALSE);

  do {
    EFI_INPUT_KEY Key;

    // Execute Event Handler
    mUsbMsdProtocol->EventHandler (mUsbMsdProtocol);

    if (mChargerExProtocol != NULL) {
      // Get Charger Presence
      mChargerExProtocol->GetChargerPresence (&Connected);

      // Display Splash depending on Connect State
      if (Connected && CurrentSplash != BG_MSD_CONNECTED) {
        // Display Connected Splash
        DisplayBootGraphic (BG_MSD_CONNECTED);

        // Set Current Splash Value
        CurrentSplash = BG_MSD_CONNECTED;

        // Print Exit Message
        PrintGUI (L"Press Volume Up Button to Exit Mass Storage.", 0, FALSE);
      } else if (!Connected && CurrentSplash != BG_MSD_DISCONNECTED) {
        // Display Disconnected Splash
        DisplayBootGraphic (BG_MSD_DISCONNECTED);

        // Set Current Splash Value
        CurrentSplash = BG_MSD_DISCONNECTED;

        // Print Exit Message
        PrintGUI (L"Press Volume Up Button to Exit Mass Storage.", 0, FALSE);
      }
    }

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
  } while (TRUE);

  // CLear Message
  PrintGUI (L"                                            ", 0, FALSE);

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
InitMassStorage (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS               Status;
  EFI_USB_MSD_PROTOCOL    *mUsbMsdProtocol;
  EFI_CHARGER_EX_PROTOCOL *mChargerExProtocol;
  EFI_HANDLE               PartitionTableHandle;

  // Check for Sony & Google
  if (FixedPcdGetPtr (PcdSmbiosSystemManufacturer) == "Sony Group Corporation" || FixedPcdGetPtr (PcdSmbiosSystemManufacturer) == "Google LLC") {
    return EFI_UNSUPPORTED;
  }

  // Locate USB Mass Storage Protocol
  Status = gBS->LocateProtocol (&gEfiUsbMsdProtocolGuid, NULL, (VOID *)&mUsbMsdProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate USB Mass Storage Protocol! Status = %r\n", Status));
    return Status;
  }

  // Disable WatchDog Timer
  gBS->SetWatchdogTimer (0, 0, 0, (CHAR16 *)NULL);

  // Locate Charger Protocol
  Status = gBS->LocateProtocol (&gChargerExProtocolGuid, NULL, (VOID *)&mChargerExProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate Charger Ex Protocol! Status = %r\n", Status));
  }

  if (FixedPcdGetBool (PcdDeviceHasEmmc)) {
    EFI_DEVICE_PATH_PROTOCOL *EmmcUserDevicePath = (EFI_DEVICE_PATH_PROTOCOL *)&EmmcUserPartitionDevicePath;
    EFI_BLOCK_IO_PROTOCOL    *EmmcDiskIoProtocol = NULL;

    // Locate eMMC User Device Path
    Status = gBS->LocateDevicePath (&gEfiBlockIoProtocolGuid, &EmmcUserDevicePath, &PartitionTableHandle);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "Failed to Locate eMMC User Device Path! Status = %r\n", Status));
      return Status;
    }

    // Locate eMMC User DiskIO Protocol
    Status = gBS->OpenProtocol (PartitionTableHandle, &gEfiBlockIoProtocolGuid, (VOID *)&EmmcDiskIoProtocol, NULL, NULL, EFI_OPEN_PROTOCOL_GET_PROTOCOL);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "Failed to Locate eMMC User DiskIO Protocol! Status = %r\n", Status));
      return Status;
    }

    // Start Mass Storage
    Status = StartMassStorage (mUsbMsdProtocol, mChargerExProtocol, EmmcDiskIoProtocol);
    if (EFI_ERROR (Status)) {
      return Status;
    }
  } else {
    UFS_LUN_DATA UfsLunData[8] = {0};
    UINT8        CurrentLun    = 0;

    // Set UFS LUN Device Paths
    UfsLunData[0].LunDevicePath = (EFI_DEVICE_PATH_PROTOCOL *)&UfsLun0DevicePath;
    UfsLunData[1].LunDevicePath = (EFI_DEVICE_PATH_PROTOCOL *)&UfsLun1DevicePath;
    UfsLunData[2].LunDevicePath = (EFI_DEVICE_PATH_PROTOCOL *)&UfsLun2DevicePath;
    UfsLunData[3].LunDevicePath = (EFI_DEVICE_PATH_PROTOCOL *)&UfsLun3DevicePath;
    UfsLunData[4].LunDevicePath = (EFI_DEVICE_PATH_PROTOCOL *)&UfsLun4DevicePath;
    UfsLunData[5].LunDevicePath = (EFI_DEVICE_PATH_PROTOCOL *)&UfsLun5DevicePath;
    UfsLunData[6].LunDevicePath = (EFI_DEVICE_PATH_PROTOCOL *)&UfsLun6DevicePath;
    UfsLunData[7].LunDevicePath = (EFI_DEVICE_PATH_PROTOCOL *)&UfsLun7DevicePath;

    // Locate UFS LUN Device Paths
    for (UINT8 i = 0; i < 8; i++) {
      gBS->LocateDevicePath (&gEfiBlockIoProtocolGuid, &UfsLunData[i].LunDevicePath, &PartitionTableHandle);

      // Get UFS LUN iskIO Protocol
      if (PartitionTableHandle == NULL) {
        DEBUG ((EFI_D_ERROR, "Failed to Locate UFS LUN %u Device Path! Status = %r\n", i, Status));
      } else {
        Status = gBS->OpenProtocol (PartitionTableHandle, &gEfiBlockIoProtocolGuid, (VOID *)&UfsLunData[i].LunDiskIoProtocol, NULL, NULL, EFI_OPEN_PROTOCOL_GET_PROTOCOL);
        if (EFI_ERROR (Status)) {
          DEBUG ((EFI_D_ERROR, "Failed to Locate DiskIO Protocol of UFS LUN %u! Status = %r\n", i, Status));
        }
      }
    }

    // Check for LUN0 & LUN1
    if (UfsLunData[0].LunDiskIoProtocol == NULL) {
      return Status;
    } else if (UfsLunData[1].LunDiskIoProtocol != NULL) {
  LunMenu:
      // Display Select LUN Splash
      DisplayBootGraphic (BG_MSD_SELECT_LUN);

      // Reset Input Protocol
      gST->ConIn->Reset (gST->ConIn, TRUE);

      do {
        EFI_INPUT_KEY Key;

        // Get Current Key Press
        gST->ConIn->ReadKeyStroke (gST->ConIn, &Key);

        // Exit to Mass Storage
        if (Key.UnicodeChar == CHAR_CARRIAGE_RETURN) {
          break;
        }

        // Increase Number
        if (Key.ScanCode == SCAN_UP) {
          if (CurrentLun < 7 && UfsLunData[CurrentLun + 1].LunDiskIoProtocol != NULL) {
            CurrentLun++;
          }
        }

        // Decrese Number
        if (Key.ScanCode == SCAN_DOWN) {
          if (CurrentLun > 0 && UfsLunData[CurrentLun - 1].LunDiskIoProtocol != NULL) {
            CurrentLun--;
          } else if (CurrentLun == 0) {
            PrintGUI (L"Continue Boot", 0, FALSE);

            do {
              EFI_INPUT_KEY Key;

              // Get Current Key Press
              gST->ConIn->ReadKeyStroke (gST->ConIn, &Key);

              // Exit Mass Storage
              if (Key.UnicodeChar == CHAR_CARRIAGE_RETURN) {
                goto exit;
              }

              // Go Back to LUN Select
              if (Key.ScanCode == SCAN_UP) {
                PrintGUI (L"             ", 0, FALSE);
                break;
              }
            } while (TRUE);
          }
        }

        // Print Current Lun
        PrintGUI (L"Current LUN: %u\r", CurrentLun, TRUE);
      } while (TRUE);
    }

    // Start Mass Storage
    Status = StartMassStorage (mUsbMsdProtocol, mChargerExProtocol, UfsLunData[CurrentLun].LunDiskIoProtocol);
    if (EFI_ERROR (Status)) {
      return Status;
    }

    // Reenter the LUN Menu
    if (UfsLunData[0].LunDevicePath != NULL && UfsLunData[1].LunDevicePath != NULL) {
      goto LunMenu;
    }
  }

exit:
  return EFI_SUCCESS;
}
