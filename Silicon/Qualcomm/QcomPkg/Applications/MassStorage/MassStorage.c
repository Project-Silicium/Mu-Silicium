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
// Global Protocols
//
STATIC EFI_GRAPHICS_OUTPUT_PROTOCOL *mGopProtocol;
STATIC EFI_USB_MSD_PROTOCOL         *mUsbMsdProtocol;

VOID
PrintUI (IN CHAR8 *Message)
{
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL Color[2];
  UINTN                         YPos;
  UINTN                         XPos;

  // Verify Parameter
  if (Message == NULL) {
    return;
  }

  // Get Screen Resolution
  UINT32 ScreenWidth  = mGopProtocol->Mode->Info->HorizontalResolution;
  UINT32 ScreenHeight = mGopProtocol->Mode->Info->VerticalResolution;

  // Calculate Message Position
  XPos = (ScreenWidth - AsciiStrLen (Message) * EFI_GLYPH_WIDTH) / 2;
  YPos = (ScreenHeight - EFI_GLYPH_HEIGHT) * 48 / 50;

  // Set Draw Colors
  Color[0].Red = Color[0].Green = Color[0].Blue = 0;
  Color[1].Red = Color[1].Green = Color[1].Blue = 255;

  // Clear Message Row
  mGopProtocol->Blt (mGopProtocol, &Color[0], EfiBltVideoFill, 0, 0, 0, YPos, ScreenWidth, 20, 0);

  // Print Message
  AsciiPrintXY (XPos, YPos, &Color[1], NULL, Message);
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
  PrintUI ("[Volume Up] Exit Mass Storage");

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
    gEfiUfsLU0Guid,
    /*
    gEfiUfsLU0Guid,            gEfiUfsLU1Guid,
    gEfiUfsLU2Guid,            gEfiUfsLU3Guid,
    gEfiUfsLU4Guid,            gEfiUfsLU5Guid,
    gEfiUfsLU6Guid,            gEfiUfsLU7Guid,
    */
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

  // Locate GOP Protocol
  Status = gBS->HandleProtocol (gST->ConsoleOutHandle, &gEfiGraphicsOutputProtocolGuid, (VOID *)&mGopProtocol);
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

  // Start Mass Storage
  MassStorageStart ();

  return EFI_SUCCESS;
}
