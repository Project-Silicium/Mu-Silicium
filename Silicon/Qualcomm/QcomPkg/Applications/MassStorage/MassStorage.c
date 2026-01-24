#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BootGraphicsLib.h>
#include <Library/DevicePathLib.h>
#include <Library/BootGraphics.h>
#include <Library/UefiLib.h>

#include <Protocol/EFIUsbMsd.h>

#include "MassStorage.h"
#include "eMMC.h"
#include "UFS.h"

//
// Global Protocols
//
STATIC EFI_USB_MSD_PROTOCOL         *mUsbMsdProtocol;
STATIC EFI_GRAPHICS_OUTPUT_PROTOCOL *mGopProtocol;

//
// Global Storage Handles
//
STATIC EFI_UFS_LUN_DATA       UfsLunData[8];
STATIC EFI_BLOCK_IO_PROTOCOL *EmmcBlockIoProtocol;

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
      DEBUG ((EFI_D_ERROR, "%a: Failed to Locate Device Path of UFS LUN %u! Status = %r\n", __FUNCTION__, i, Status));
      continue;
    }

    // Save Block IO Protocol
    Status = gBS->HandleProtocol (ProtocolHandle, &gEfiBlockIoProtocolGuid, (VOID *)&UfsLunData[i].BlockIoProtocol);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%a: Failed to Get Block IO Protocol from UFS LUN %u Device Path! Status = %r\n", __FUNCTION__, i, Status));
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
    DEBUG ((EFI_D_ERROR, "%a: Failed to Locate Device Path of eMMC! Status = %r\n", __FUNCTION__, Status));
    return;
  }

  // Save Block IO Protocol
  Status = gBS->HandleProtocol (ProtocolHandle, &gEfiBlockIoProtocolGuid, (VOID *)&EmmcBlockIoProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Get Block IO Protocol from eMMC Device Path! Status = %r\n", __FUNCTION__, Status));
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
  IN CHAR16  *Message,
  IN UINT8    MessageArg,
  IN BOOLEAN  AltPosition)
{
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL Color;
  UINTN                         XPos;
  UINTN                         YPos;

  // Get Screen Resolution
  UINT32 ScreenWidth  = mGopProtocol->Mode->Info->HorizontalResolution;
  UINT32 ScreenHeight = mGopProtocol->Mode->Info->VerticalResolution;

  // Set Message Y Position
  if (AltPosition) {
    YPos = (ScreenHeight - EFI_GLYPH_HEIGHT) * 47 / 50;
  } else {
    YPos = (ScreenHeight - EFI_GLYPH_HEIGHT) * 48 / 50;
  }

  // Set Background Color
  Color.Red = Color.Green = Color.Blue = 0;

  // Clear Message Position
  mGopProtocol->Blt (mGopProtocol, &Color, EfiBltVideoFill, 0, 0, 0, YPos, ScreenWidth, 20, 0);

  // Verify Message
  if (Message == NULL) {
    return;
  }

  // Set Message X Position
  XPos = (ScreenWidth - StrLen (Message) * EFI_GLYPH_WIDTH) / 2;

  // Set Message Color
  Color.Red = Color.Green = Color.Blue = 0xFF;

  // Draw New Message
  if (MessageArg != -1) {
    PrintXY (XPos, YPos, &Color, NULL, Message, MessageArg);
  } else {
    PrintXY (XPos, YPos, &Color, NULL, Message);
  }
}

VOID
StartMassStorage (IN EFI_BLOCK_IO_PROTOCOL *BlockIo)
{
  EFI_STATUS Status;

  // Print Instruction Message
  PrintUI (L"[Arrow Key Up] Exit Mass Storage", -1, TRUE);

  // Clear Bottom Message
  PrintUI (NULL, -1, FALSE);

  // Assing Block IO Protocol
  Status = mUsbMsdProtocol->AssignBlkIoHandle (mUsbMsdProtocol, BlockIo, 0);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Assing Block IO Protocol! Status = %r\n", __FUNCTION__, Status));
    return;
  }

  // Start USB Device
  Status = mUsbMsdProtocol->StartDevice (mUsbMsdProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Start USB Device! Status = %r\n", __FUNCTION__, Status));
    return;
  }

  // Reset Input Protocol
  gST->ConIn->Reset (gST->ConIn, TRUE);

  // Handle USB Events & Input
  while (TRUE) {
    EFI_INPUT_KEY Key;

    // Execute Event Handler
    mUsbMsdProtocol->EventHandler (mUsbMsdProtocol);

    // Get current Key
    gST->ConIn->ReadKeyStroke (gST->ConIn, &Key);

    // Exit Loop
    if (Key.ScanCode == SCAN_UP) {
      break;
    }
  }

  // Stop USB Device
  Status = mUsbMsdProtocol->StopDevice (mUsbMsdProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Stop USB Device! Status = %r\n", __FUNCTION__, Status));
    return;
  }

  // Unassing Block IO Protocol
  Status = mUsbMsdProtocol->AssignBlkIoHandle (mUsbMsdProtocol, NULL, 0);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Unassing Block IO Protocol! Status = %r\n", __FUNCTION__, Status));
  }
}

VOID
SelectUfsLun ()
{
  CHAR16  *Option[2]    = { L"UFS LUN %u", L"Back" };
  BOOLEAN  BackSelected = FALSE;
  UINT8    SelectedLun  = 0;

Selection:
  // Print Instruction Message
  PrintUI (L"[Arrow Keys] Switch UFS LUN - [Enter] Select UFS LUN", -1, TRUE);

  // Clear Bottom Message
  PrintUI (Option[0], 0, FALSE);

  // Wait for Keypress
  gBS->WaitForEvent (1, &gST->ConIn->WaitForKey, NULL);

  // Reset Input Protocol
  gST->ConIn->Reset (gST->ConIn, TRUE);

  // Handle Input
  while (TRUE) {
    EFI_INPUT_KEY Key;

    // Wait for Keypress
    gBS->WaitForEvent (1, &gST->ConIn->WaitForKey, NULL);

    // Get Current Key Press
    gST->ConIn->ReadKeyStroke (gST->ConIn, &Key);

    // Execute Current Option
    if (Key.UnicodeChar == CHAR_CARRIAGE_RETURN) {
      break;
    }

    // Select Next Option
    if ((Key.ScanCode == SCAN_UP && SelectedLun < 7) && UfsLunData[SelectedLun + 1].BlockIoProtocol != NULL) {
      if (BackSelected) {
        BackSelected = FALSE;
      } else {
        SelectedLun++;
      }
    }

    // Select Previous Option
    if (Key.ScanCode == SCAN_DOWN) {
      if (SelectedLun > 0) {
        SelectedLun--;
      } else {
        BackSelected = TRUE;
      }
    }

    // Print Current Option
    if (BackSelected) {
      PrintUI (Option[1], -1, FALSE);
    } else {
      PrintUI (Option[0], SelectedLun, FALSE);
    }
  }

  // Exit LUN Selection
  if (BackSelected) {
    return;
  }

  // Start Mass Storage
  StartMassStorage (UfsLunData[SelectedLun].BlockIoProtocol);

  // Go to Selection
  goto Selection;
}

EFI_STATUS
MassStorageHandler ()
{
  CHAR16 *Option[3]      = { L"Start Mass Storage", L"Reboot", L"Power Off" };
  UINT8   SelectedOption = 0;

  // Draw Mass Storage Graphic
  DisplayBootGraphic (BG_MASS_STORAGE);

Menu:
  // Print Instruction Message
  PrintUI (L"[Arrow Keys] Switch Option - [Enter] Select Option", -1, TRUE);

  // Clear Bottom Message
  PrintUI (Option[0], -1, FALSE);

  // Wait for Keypress
  gBS->WaitForEvent (1, &gST->ConIn->WaitForKey, NULL);

  // Reset Input Protocol
  gST->ConIn->Reset (gST->ConIn, TRUE);

  // Handle Input
  while (TRUE) {
    EFI_INPUT_KEY Key;

    // Wait for Keypress
    gBS->WaitForEvent (1, &gST->ConIn->WaitForKey, NULL);

    // Get Current Key Press
    gST->ConIn->ReadKeyStroke (gST->ConIn, &Key);

    // Execute Current Option
    if (Key.UnicodeChar == CHAR_CARRIAGE_RETURN) {
      break;
    }

    // Select Next Option
    if (Key.ScanCode == SCAN_UP && SelectedOption < ARRAY_SIZE (Option) - 1) {
      SelectedOption++;
    }

    // Select Previous Option
    if (Key.ScanCode == SCAN_DOWN && SelectedOption > 0) {
      SelectedOption--;
    }

    // Print Current Option
    PrintUI (Option[SelectedOption], -1, FALSE);
  }

  // Execute Selected Option
  switch (SelectedOption) {
    case 0:
      // Enter UFS LUN Selection / Start Mass Storage
      if (EmmcBlockIoProtocol != NULL) {
        StartMassStorage (EmmcBlockIoProtocol);
      } else {
        SelectUfsLun ();
      }

      // Go back
      goto Menu;

    case 1:
      // Reboot
      gRT->ResetSystem (EfiResetCold, EFI_SUCCESS, 0, NULL);
      break;

    case 2:
      // Shutdown
      gRT->ResetSystem (EfiResetShutdown, EFI_SUCCESS, 0, NULL);
      break;

    default:
      return EFI_INVALID_PARAMETER;
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

  // Locate GOP Protocol
  Status = gBS->LocateProtocol (&gEfiGraphicsOutputProtocolGuid, NULL, (VOID *)&mGopProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Locate GOP Protocol! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  // Locate USB Mass Storage Protocol
  Status = gBS->LocateProtocol (&gEfiUsbMsdProtocolGuid, NULL, (VOID *)&mUsbMsdProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Locate USB Mass Storage Protocol! Status = %r\n", __FUNCTION__, Status));
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
