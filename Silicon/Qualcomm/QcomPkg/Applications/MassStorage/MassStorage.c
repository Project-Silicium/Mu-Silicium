#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BootGraphicsLib.h>
#include <Library/DevicePathLib.h>
#include <Library/BootGraphics.h>
#include <Library/UefiLib.h>

#include <Protocol/EFIUsbMsd.h>

#include "MassStorage.h"

//
// Global Variables
//
STATIC EFI_HANDLE mImageHandle;

//
// Global Protocols
//
STATIC EFI_GRAPHICS_OUTPUT_PROTOCOL *mGopProtocol;
STATIC EFI_USB_MSD_PROTOCOL         *mUsbMsdProtocol;

VOID
PrintUI (
  IN CHAR16 *SubBottomMessage,
  IN CHAR16 *BottomMessage)
{
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL  Color[2];
  CHAR16                        *Message[2];
  UINTN                          YPos[2];
  UINTN                          XPos;

  // Get Screen Resolution
  UINT32 ScreenWidth  = mGopProtocol->Mode->Info->HorizontalResolution;
  UINT32 ScreenHeight = mGopProtocol->Mode->Info->VerticalResolution;

  // Calculate Y Positions
  YPos[0] = (ScreenHeight - EFI_GLYPH_HEIGHT) * 47 / 50;
  YPos[1] = (ScreenHeight - EFI_GLYPH_HEIGHT) * 48 / 50;

  // Set Draw Colors
  Color[0].Red = Color[0].Green = Color[0].Blue = 0;
  Color[1].Red = Color[1].Green = Color[1].Blue = 255;

  // Save Messages
  Message[0] = SubBottomMessage;
  Message[1] = BottomMessage;

  // Process both Messages
  for (UINT8 i = 0; i < 2; i++) {
    // Clear Message Row
    mGopProtocol->Blt (mGopProtocol, &Color[0], EfiBltVideoFill, 0, 0, 0, YPos[i], ScreenWidth, 20, 0);

    // Verify Message Content
    if (Message[i] != NULL) {
      // Calculate X Position
      XPos = (ScreenWidth - StrLen (Message[i]) * EFI_GLYPH_WIDTH) / 2;

      // Print Message
      PrintXY (XPos, YPos[i], &Color[1], NULL, Message[i]);
    }
  }
}

EFI_INPUT_KEY
GetPressedKey (IN BOOLEAN Wait)
{
  EFI_INPUT_KEY Key;

  // Wait for Keypress
  if (Wait) {
    gBS->WaitForEvent (1, &gST->ConIn->WaitForKey, NULL);
  }

  // Get Detected Keypress
  gST->ConIn->ReadKeyStroke (gST->ConIn, &Key);

  return Key;
}

VOID
MassStorageStart ()
{
  EFI_STATUS Status;

  // Show Cancel Instruction
  PrintUI (L"[Volume Up] Exit Mass Storage", NULL);

  // Start USB Device
  Status = mUsbMsdProtocol->StartDevice (mUsbMsdProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Start USB Device! Status = %r\n", __FUNCTION__, Status));
    return;
  }

  while (TRUE) {
    // Handle USB Events
    mUsbMsdProtocol->EventHandler (mUsbMsdProtocol);

    // Get current Keypress
    EFI_INPUT_KEY Key = GetPressedKey (FALSE);
    if (Key.ScanCode == SCAN_UP) {
      break;
    }
  }

  // Stop USB Device
  Status = mUsbMsdProtocol->StopDevice (mUsbMsdProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Stop USB Device! Status = %r\n", __FUNCTION__, Status));
  }
}

VOID
MassStorageContinue ()
{
  EFI_STATUS Status;

  // Unassign BLKIO Protocols
  for (UINT8 i = 0; i < MAX_UINT8; i++) {
    Status = mUsbMsdProtocol->AssignBlkIoHandle (mUsbMsdProtocol, NULL, i);
    if (EFI_ERROR (Status)) {
      break;
    }
  }

  // Exit UEFI App
  gBS->Exit (mImageHandle, EFI_SUCCESS, 0, NULL);
}

VOID
MassStoragePowerOff ()
{
  // Power Off
  gRT->ResetSystem (EfiResetShutdown, EFI_SUCCESS, 0, NULL);
}

//
// Main Menu Options
//
STATIC EFI_UI_MENU_DETAILS MainMenu[] = {
  { L"Start Mass Storage", MassStorageStart },
  { L"Continue Boot",      MassStorageContinue },
  { L"Power Off",          MassStoragePowerOff }
};

VOID
MassStorageMain ()
{
  UINT8 MaxMainOption     = ARRAY_SIZE (MainMenu) - 1;
  UINT8 CurrentMainOption = 0;

  while (TRUE) {
    // Show current Selected Option & Instructions
    PrintUI (L"[Volume Keys] Switch Option - [Power Button] Select Option", MainMenu[CurrentMainOption].Label);

    // Get current Keypress
    EFI_INPUT_KEY Key = GetPressedKey (TRUE);

    // Execute Specified Action
    if (Key.UnicodeChar == CHAR_CARRIAGE_RETURN) {
      MainMenu[CurrentMainOption].Action ();
      continue;
    }

    // Select Different Option
    if (Key.ScanCode == SCAN_UP && CurrentMainOption < MaxMainOption) {
      CurrentMainOption++;
    } else if (Key.ScanCode == SCAN_DOWN && CurrentMainOption > 0) {
      CurrentMainOption--;
    }
  }
}

EFI_STATUS
AssignStorageHandles ()
{
  EFI_STATUS                Status            = EFI_SUCCESS;
  EFI_DEVICE_PATH_PROTOCOL *StorageDevicePath = NULL;
  EFI_BLOCK_IO_PROTOCOL    *BlkIoProtocol     = NULL;
  EFI_HANDLE                ProtocolHandle    = NULL;
  UINT8                     AssignableMsdLuns = 0;
  UINT8                     AssignedMsdLuns   = 0;

  // Set Storage GUIDs
  EFI_GUID StorageGuid[] = {
    gEfiUfsLU0Guid,            gEfiUfsLU1Guid,
    gEfiUfsLU2Guid,            gEfiUfsLU3Guid,
    gEfiUfsLU4Guid,            gEfiUfsLU5Guid,
    gEfiUfsLU6Guid,            gEfiUfsLU7Guid,
    gEfiEmmcUserPartitionGuid, gEfiSdRemovableGuid
  };

  // Get Max Number of Assignable LUNs
  Status = mUsbMsdProtocol->QueryMaxLun (mUsbMsdProtocol, &AssignableMsdLuns);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to get Number of Max Assignable LUNs! Status = %r\n", __FUNCTION__, Status));
    AssignableMsdLuns = 2;
  }

  // Go thru each Storage
  for (UINT8 i = 0; i < ARRAY_SIZE (StorageGuid); i++) {
    // Compare Assigned and Assignable LUNs
    if (AssignedMsdLuns + 1 == AssignableMsdLuns) {
      break;
    }

    // Set Storage GUID
    StorageVendorDevicePath.VendorDevicePath.Guid = StorageGuid[i];

    // Convert Storage Device Path
    StorageDevicePath = (EFI_DEVICE_PATH_PROTOCOL *)&StorageVendorDevicePath;

    // Locate Storage Device Path
    Status = gBS->LocateDevicePath (&gEfiBlockIoProtocolGuid, &StorageDevicePath, &ProtocolHandle);
    if (EFI_ERROR (Status)) {
      continue;
    }

    // Get Storage BLKIO Protocol
    Status = gBS->HandleProtocol (ProtocolHandle, &gEfiBlockIoProtocolGuid, (VOID *)&BlkIoProtocol);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%a: Failed to Get BLKIO Protocol from Storage %u Device Path!\n", __FUNCTION__, i));
      return Status;
    }

    // Assign Storage BLKIO Protocol
    Status = mUsbMsdProtocol->AssignBlkIoHandle (mUsbMsdProtocol, BlkIoProtocol, AssignedMsdLuns);
    if (!EFI_ERROR (Status)) {
      AssignedMsdLuns++;
    }
  }

  // Verify Assigned MSD LUNs
  if (!AssignedMsdLuns) {
    DEBUG ((EFI_D_ERROR, "%a: No BLKIO Protocol was Assigned!\n", __FUNCTION__));
    return EFI_NOT_FOUND;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
MassStorageEntry (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status;

  // Save Image Handle
  mImageHandle = ImageHandle;

  // Locate GOP Protocol
  Status = gBS->LocateProtocol (&gEfiGraphicsOutputProtocolGuid, NULL, (VOID *)&mGopProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Locate GOP Protocol! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  // Locate USB MSD Protocol
  Status = gBS->LocateProtocol (&gEfiUsbMsdProtocolGuid, NULL, (VOID *)&mUsbMsdProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Locate USB MSD Protocol! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  // Assign Storage Handles
  Status = AssignStorageHandles ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Disable Watchdog Timer
  gBS->SetWatchdogTimer (0, 0, 0, (CHAR16 *)NULL);

  // Show Mass Storage Splash
  DisplayBootGraphic (BG_MASS_STORAGE);

  // Enter Main Menu
  MassStorageMain ();

  return EFI_SUCCESS;
}
