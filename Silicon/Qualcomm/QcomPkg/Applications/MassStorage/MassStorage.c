#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BootGraphicsLib.h>
#include <Library/DevicePathLib.h>
#include <Library/BootGraphics.h>
#include <Library/UefiLib.h>

#include <Protocol/EFIUsbMsd.h>
#include <Protocol/EFIChargerEx.h>
#include <Protocol/EFIChg.h>

#include "MassStorage.h"
#include "eMMC.h"
#include "UFS.h"

//
// Global Protocols
//
STATIC EFI_USB_MSD_PROTOCOL         *mUsbMsdProtocol;
STATIC EFI_CHARGER_EX_PROTOCOL      *mChargerExProtocol;
STATIC EFI_CHG_PROTOCOL             *mChgProtocol;
STATIC EFI_GRAPHICS_OUTPUT_PROTOCOL *mGopProtocol;

//
// Global Storage Handles
//
STATIC EFI_UFS_LUN_DATA       UfsLunData[8];
STATIC EFI_BLOCK_IO_PROTOCOL *EmmcBlockIoProtocol;

EFI_STATUS
LocateNeededProtocols ()
{
  EFI_STATUS Status;

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
  }

  // Locate Samsung Charger Protocol
  Status = gBS->LocateProtocol (&gEfiChgProtocolGuid, NULL, (VOID *)&mChgProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate Samsung Charger Protocol! Status = %r\n", Status));
  }

  // Check for Errors
  if (mChargerExProtocol == NULL && mChgProtocol == NULL) {
    return Status;
  }

  return EFI_SUCCESS;
}

VOID
GetUfsStorageHandles ()
{
  EFI_STATUS Status;

  // Clear UFS LUN Data
  ZeroMem (&UfsLunData, sizeof (EFI_UFS_LUN_DATA) * 8);

  // Set UFS LUN Device Paths
  UfsLunData[0].DevicePath = (EFI_DEVICE_PATH_PROTOCOL *)&UfsLun0DevicePath;
  UfsLunData[1].DevicePath = (EFI_DEVICE_PATH_PROTOCOL *)&UfsLun1DevicePath;
  UfsLunData[2].DevicePath = (EFI_DEVICE_PATH_PROTOCOL *)&UfsLun2DevicePath;
  UfsLunData[3].DevicePath = (EFI_DEVICE_PATH_PROTOCOL *)&UfsLun3DevicePath;
  UfsLunData[4].DevicePath = (EFI_DEVICE_PATH_PROTOCOL *)&UfsLun4DevicePath;
  UfsLunData[5].DevicePath = (EFI_DEVICE_PATH_PROTOCOL *)&UfsLun5DevicePath;
  UfsLunData[6].DevicePath = (EFI_DEVICE_PATH_PROTOCOL *)&UfsLun6DevicePath;
  UfsLunData[7].DevicePath = (EFI_DEVICE_PATH_PROTOCOL *)&UfsLun7DevicePath;

  // Get Block IO Protocols
  for (UINT8 i = 0; i < 8; i++) {
    EFI_HANDLE ProtocolHandle;

    // Locate Device Path
    Status = gBS->LocateDevicePath (&gEfiBlockIoProtocolGuid, &UfsLunData[i].DevicePath, &ProtocolHandle);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "Failed to Locate Device Path of UFS LUN %u! Status = %r\n", i, Status));
      continue;
    }

    // Save Block IO Protocol
    Status = gBS->HandleProtocol (ProtocolHandle, &gEfiBlockIoProtocolGuid, (VOID *)&UfsLunData[i].BlockIoProtocol);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "Failed to Get Block IO Protocol from UFS LUN %u Device Path! Status = %r\n", i, Status));
      continue;
    }
  }
}

VOID
GetEmmcStorageHandle ()
{
  EFI_STATUS                Status;
  EFI_DEVICE_PATH_PROTOCOL *EmmcDevicePath;
  EFI_HANDLE                ProtocolHandle;

  // Set eMMC Device Path
  EmmcDevicePath = (EFI_DEVICE_PATH_PROTOCOL *)&EmmcUserDevicePath;

  // Locate Device Path
  Status = gBS->LocateDevicePath (&gEfiBlockIoProtocolGuid, &EmmcDevicePath, &ProtocolHandle);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate Device Path of eMMC! Status = %r\n", Status));
    return;
  }

  // Save Block IO Protocol
  Status = gBS->HandleProtocol (ProtocolHandle, &gEfiBlockIoProtocolGuid, (VOID *)&EmmcBlockIoProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Get Block IO Protocol from eMMC Device Path! Status = %r\n", Status));
    return;
  }
}

EFI_STATUS
GetInternalStorageHandles ()
{
  // Get UFS Storage Handles
  GetUfsStorageHandles ();

  // Get eMMC Storage Handle
  GetEmmcStorageHandle ();

  // Check for Errors
  if (UfsLunData[0].BlockIoProtocol == NULL && EmmcBlockIoProtocol == NULL) {
    return EFI_NOT_FOUND;
  }

  return EFI_SUCCESS;
}

VOID
PrintUI (
  IN CHAR16 *Message,
  IN UINT8   MessageArg)
{
  EFI_STATUS                    Status;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL Color;

  // Locate GOP Protocol
  if (mGopProtocol == NULL) {
    Status = gBS->LocateProtocol (&gEfiGraphicsOutputProtocolGuid, NULL, (VOID *)&mGopProtocol);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "Failed to Locate GOP Protocol! Status = %r\n", Status));
      return;
    }
  }

  // Get Screen Resolution
  UINT32 ScreenWidth  = mGopProtocol->Mode->Info->HorizontalResolution;
  UINT32 ScreenHeight = mGopProtocol->Mode->Info->VerticalResolution;

  // Set Message X & Y Position
  UINTN XPos = (ScreenWidth - StrLen (Message) * EFI_GLYPH_WIDTH) / 2;
  UINTN YPos = (ScreenHeight - EFI_GLYPH_HEIGHT) * 48 / 50;

  // Set Message Color
  Color.Red   = 0xFF;
  Color.Green = 0xFF;
  Color.Blue  = 0xFF;

  // Draw New Message
  if (MessageArg != -1) {
    PrintXY (XPos, YPos, &Color, NULL, Message, MessageArg);
  } else {
    PrintXY (XPos, YPos, &Color, NULL, Message);
  }
}

// TODO: Handle this Better!
VOID
StartMassStorage (IN EFI_BLOCK_IO_PROTOCOL *BlockIo)
{
  EFI_STATUS Status        = EFI_SUCCESS;
  UINT8      CurrentSplash = 0;
  BOOLEAN    Connected     = FALSE;
  BOOLEAN    Running       = FALSE;

  // Clear LUN Message
  PrintUI (L"      ", -1);

  // Assing Block IO Protocol
  Status = mUsbMsdProtocol->AssignBlkIoHandle (mUsbMsdProtocol, BlockIo, 0);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Assing Block IO Protocol! Status = %r\n", Status));
    return;
  }

  // Handle USB Events & Input
  while (TRUE) {
    // Get USB Connect State
    if (mChargerExProtocol != NULL) {
      mChargerExProtocol->GetChargerPresence (&Connected);
    } else {
      mChgProtocol->GetChargerStatus (&Connected);
    }

    if (Connected) {
      if (!Running) {
        // Start USB Device
        Status = mUsbMsdProtocol->StartDevice (mUsbMsdProtocol);
        if (EFI_ERROR (Status)) {
          DEBUG ((EFI_D_ERROR, "Failed to Start USB Device! Status = %r\n", Status));
          return;
        }

        // Set Running State
        Running = TRUE;
      }

      // Update Splash
      if (CurrentSplash != BG_MSD_CONNECTED) {
        // Display Connected Splash
        DisplayBootGraphic (BG_MSD_CONNECTED);

        // Clear Input Options
        PrintUI (L"                                           ", -1);

        // Set Current Splash
        CurrentSplash = BG_MSD_CONNECTED;
      }

      // Execute Event Handler
      mUsbMsdProtocol->EventHandler (mUsbMsdProtocol);
    } else {
      EFI_INPUT_KEY Key;

      if (Running) {
        // Stop USB Device
        Status = mUsbMsdProtocol->StopDevice (mUsbMsdProtocol);
        if (EFI_ERROR (Status)) {
          DEBUG ((EFI_D_ERROR, "Failed to Stop USB Device! Status = %r\n", Status));
        }

        // Set Running State
        Running = FALSE;

        // Reset Input Protocol
        gST->ConIn->Reset (gST->ConIn, TRUE);
      }

      // Update Splash
      if (CurrentSplash != BG_MSD_DISCONNECTED) {
        // Display Connected Splash
        DisplayBootGraphic (BG_MSD_DISCONNECTED);

        // Print Input Options
        if (BlockIo != EmmcBlockIoProtocol) {
          PrintUI (L"[Volume Up] Return - [Volume Down] Shutdown", -1);
        } else {
          PrintUI (L"[Volume Down] Shutdown", -1);
        }

        // Set Current Splash
        CurrentSplash = BG_MSD_DISCONNECTED;
      }

      // Get current Key
      gST->ConIn->ReadKeyStroke (gST->ConIn, &Key);

      // Exit Loop
      if ((Key.ScanCode == SCAN_UP || Key.ScanCode == SCAN_ESC) && BlockIo != EmmcBlockIoProtocol) {
        // Clear Input Options
        PrintUI (L"                                           ", -1);

        // Unassign Block IO Protocol
        Status = mUsbMsdProtocol->AssignBlkIoHandle (mUsbMsdProtocol, NULL, 0);
        if (EFI_ERROR (Status)) {
          DEBUG ((EFI_D_ERROR, "Failed to Unassign Block IO Protocol! Status = %r\n"));
        }

        break;
      }

      // Power Off Device
      if (Key.ScanCode == SCAN_DOWN || Key.ScanCode == SCAN_DELETE) {
        gRT->ResetSystem (EfiResetShutdown, EFI_SUCCESS, 0, NULL);
      }
    }
  }
}

EFI_STATUS
MassStorageHandler ()
{
  UINT8 CurrentLun = 0;

  // Check Storage Type
  if (EmmcBlockIoProtocol != NULL) {
    // Start Mass Storage
    StartMassStorage (EmmcBlockIoProtocol);
  }

  // Reset Input Protocol
  gST->ConIn->Reset (gST->ConIn, TRUE);

  // Display Default Splash
  DisplayBootGraphic (BG_MSD_DEFAULT);

  // Handle Input
  while (TRUE) {
    EFI_INPUT_KEY Key;

    // Get Current Key Press
    gST->ConIn->ReadKeyStroke (gST->ConIn, &Key);

    // Start Mass Storage
    if (Key.UnicodeChar == CHAR_CARRIAGE_RETURN) {
      StartMassStorage (UfsLunData[CurrentLun].BlockIoProtocol);

      // Display Default Splash
      DisplayBootGraphic (BG_MSD_DEFAULT);
    }

    // Select Next LUN
    if (Key.ScanCode == SCAN_UP && CurrentLun < 7) {
      // Verify LUN Data
      if (UfsLunData[CurrentLun + 1].BlockIoProtocol != NULL) {
        CurrentLun++;
      }
    }

    // Select Previous LUN
    if (Key.ScanCode == SCAN_DOWN && CurrentLun > 0) {
      // Verify LUN Data
      if (UfsLunData[CurrentLun - 1].BlockIoProtocol != NULL) {
        CurrentLun--;
      }
    }

    // Print Current LUN
    PrintUI (L"LUN %u", CurrentLun);
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

  // Locate Needed Protocols
  Status = LocateNeededProtocols ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Get Internal Storage Handles
  Status = GetInternalStorageHandles ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Disable WatchDog Timer
  gBS->SetWatchdogTimer (0, 0, 0, (CHAR16 *)NULL);

  // Enter Mass Storage Handler
  Status = MassStorageHandler ();
  if (EFI_ERROR (Status)) {
    return Status;
  }


  return EFI_SUCCESS;
}
