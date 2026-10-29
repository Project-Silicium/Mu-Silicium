#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BootGraphicsLib.h>
#include <Library/DevicePathLib.h>
#include <Library/BootGraphics.h>
#include <Library/UefiLib.h>
#include <Library/PrintLib.h>

#include <Protocol/EFIUsbMsd.h>

#include "MassStorage.h"

//
// Global Protocols
//
STATIC EFI_GRAPHICS_OUTPUT_PROTOCOL *mGopProtocol;
STATIC EFI_USB_MSD_PROTOCOL         *mUsbMsdProtocol;

VOID
PrintStatusLine (
  IN UINTN        Line,
  IN CONST CHAR8 *Format,
  ...
  )
{
  VA_LIST                       Marker;
  CHAR8                         Buffer[128];
  UINT32                        ScreenWidth;
  UINT32                        ScreenHeight;
  UINTN                         XPos;
  UINTN                         YPos;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL Color[2];

  if (mGopProtocol == NULL || Format == NULL) {
    return;
  }

  VA_START (Marker, Format);
  AsciiVSPrint (Buffer, sizeof (Buffer), Format, Marker);
  VA_END (Marker);

  ScreenWidth  = mGopProtocol->Mode->Info->HorizontalResolution;
  ScreenHeight = mGopProtocol->Mode->Info->VerticalResolution;

  XPos = (ScreenWidth - AsciiStrLen (Buffer) * EFI_GLYPH_WIDTH) / 2;
  YPos = (ScreenHeight - EFI_GLYPH_HEIGHT) * (44 + Line * 2) / 50;

  Color[0].Red = Color[0].Green = Color[0].Blue = 0;
  Color[1].Red = Color[1].Green = Color[1].Blue = 255;

  mGopProtocol->Blt (mGopProtocol, &Color[0], EfiBltVideoFill, 0, 0, 0, YPos, ScreenWidth, 20, 0);
  AsciiPrintXY (XPos, YPos, &Color[1], NULL, Buffer);
}

VOID
PrintUI (IN CHAR8 *Message)
{
  PrintStatusLine (0, "%a", Message);
}

EFI_INPUT_KEY
GetPressedKey (IN BOOLEAN Wait)
{
  EFI_STATUS    Status;
  EFI_INPUT_KEY Key;

  Key.ScanCode    = SCAN_NULL;
  Key.UnicodeChar = CHAR_NULL;

  // Wait for Keypress
  if (Wait) {
    gBS->WaitForEvent (1, &gST->ConIn->WaitForKey, NULL);
  }

  // Get Detected Keypress
  Status = gST->ConIn->ReadKeyStroke (gST->ConIn, &Key);
  if (EFI_ERROR (Status)) {
    Key.ScanCode    = SCAN_NULL;
    Key.UnicodeChar = CHAR_NULL;
  }

  return Key;
}

VOID
MassStorageStart ()
{
  EFI_STATUS Status;
  UINT32     LoopCounter = 0;

  // Show Cancel Instruction
  PrintStatusLine (0, "[Volume / Power] Exit Mass Storage");

  // Flush any leftover keypresses from menu
  gST->ConIn->Reset (gST->ConIn, FALSE);

  // Start USB Device
  Status = mUsbMsdProtocol->StartDevice (mUsbMsdProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Start USB Device! Status = %r\n", __FUNCTION__, Status));
    PrintStatusLine (2, "StartDevice Failed: %r", Status);
    GetPressedKey (TRUE);
    return;
  }

  PrintStatusLine (2, "USB Active. Waiting for PC...");

  while (TRUE) {
    // Handle USB Events
    mUsbMsdProtocol->EventHandler (mUsbMsdProtocol);

    // Only poll buttons every 5000 events to avoid starving USB controller
    if (++LoopCounter >= 5000) {
      LoopCounter = 0;

      // Get current Keypress (any button exits)
      EFI_INPUT_KEY Key = GetPressedKey (FALSE);
      if (Key.ScanCode == SCAN_UP || Key.ScanCode == SCAN_DOWN || Key.ScanCode == 0x0102) {
        break;
      }
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
  CHAR8                     LunStr[64];

  LunStr[0] = '\0';

  // Set Storage GUIDs: enable all UFS LUNs (0-7), eMMC, and SD
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
    AssignableMsdLuns = 8;
  }

  // Go thru each Storage
  for (UINT8 i = 0; i < ARRAY_SIZE (StorageGuid); i++) {
    // Compare Assigned and Assignable LUNs
    if (AssignedMsdLuns >= AssignableMsdLuns) {
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
      continue;
    }

    // Assign Storage BLKIO Protocol
    Status = mUsbMsdProtocol->AssignBlkIoHandle (mUsbMsdProtocol, BlkIoProtocol, AssignedMsdLuns);
    if (!EFI_ERROR (Status)) {
      AsciiSPrint (LunStr + AsciiStrLen (LunStr), sizeof (LunStr) - AsciiStrLen (LunStr), " LU%u", i);
      AssignedMsdLuns++;
    }
  }

  PrintStatusLine (1, "Assigned %u LUNs:%a", AssignedMsdLuns, LunStr);

  // Verify Assigned MSD LUNs
  if (!AssignedMsdLuns) {
    DEBUG ((EFI_D_ERROR, "%a: No BLKIO Protocol was Assigned!\n", __FUNCTION__));
    PrintStatusLine (1, "No Storage Protocol Assigned!");
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

  // Show Mass Storage Splash
  DisplayBootGraphic (BG_MASS_STORAGE);

  // Show initial instruction
  PrintStatusLine (0, "[Volume / Power] Exit Mass Storage");

  // Locate USB MSD Protocol
  Status = gBS->LocateProtocol (&gEfiUsbMsdProtocolGuid, NULL, (VOID *)&mUsbMsdProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Locate USB MSD Protocol! Status = %r\n", __FUNCTION__, Status));
    PrintStatusLine (1, "Locate USB MSD Failed: %r", Status);
    GetPressedKey (TRUE);
    return Status;
  }

  // Assign Storage Handles
  Status = AssignStorageHandles ();
  if (EFI_ERROR (Status)) {
    PrintStatusLine (2, "Storage Assign Failed: %r", Status);
    GetPressedKey (TRUE);
    return Status;
  }

  // Disable Watchdog Timer
  gBS->SetWatchdogTimer (0, 0, 0, (CHAR16 *)NULL);

  // Start Mass Storage
  MassStorageStart ();

  // Unassign BLKIO Protocols
  for (UINT8 i = 0; i < MAX_UINT8; i++) {
    Status = mUsbMsdProtocol->AssignBlkIoHandle (mUsbMsdProtocol, NULL, i);
    if (EFI_ERROR (Status)) {
      break;
    }
  }

  return EFI_SUCCESS;
}
