#include <Library/DebugLib.h>
#include <Library/BdsExtensionLib.h>
#include <Library/MsBootPolicyLib.h>
#include <Library/MsBootPolicy.h>
#include <Library/MsBootOptionsLib.h>
#include <Library/MsBootOptions.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/MsNVBootReasonLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DeviceBootManagerLib.h>
#include <Library/UefiLib.h>
#include <Library/AcpiPlatformUpdateLib.h>

VOID
BdsExtensionAfterConsole ()
{
  EFI_STATUS                    Status;
  EFI_GRAPHICS_OUTPUT_PROTOCOL *mConsoleOutHandle;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL Black;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL White;

  // Locate Console Out Handle
  Status = gBS->HandleProtocol (gST->ConsoleOutHandle, &gEfiGraphicsOutputProtocolGuid, (VOID *)&mConsoleOutHandle);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Locate Console Out Protocol! Status %r\n", __FUNCTION__, Status));
  } else {
    // Combo Message
#if AB_SLOT_SUPPORT == 1
    CHAR16 *ComboMessage = L"[Volume Up] UEFI Menu - [Volume Down] Slot Switch";
#else
    CHAR16 *ComboMessage = L"[Volume Up] UEFI Menu";
#endif

    // Set Pos for Combo Message
    UINTN XPos = (mConsoleOutHandle->Mode->Info->HorizontalResolution - StrLen(ComboMessage) * EFI_GLYPH_WIDTH) / 2;
    UINTN YPos = (mConsoleOutHandle->Mode->Info->VerticalResolution - EFI_GLYPH_HEIGHT) * 48 / 50;

    // Set Color for the Message
    Black.Blue = Black.Green = Black.Red = Black.Reserved = 0;
    White.Blue = White.Green = White.Red = White.Reserved = 0xFF;

    // Display Combo Message
    PrintXY (XPos, YPos, &White, &Black, ComboMessage);
  }
}

// Dummy Function Needed for Event Notification Callback
STATIC
VOID
DummyNotify (
  IN EFI_EVENT Event,
  IN VOID     *Context) 
{
  // Do Nothing
}

VOID
BdsExtensionPlatformConnectList ()
{
  EFI_STATUS Status;
  EFI_EVENT  UsbConfigEvt;
  EFI_EVENT  SDCardEvt;

  // Update the ACPI Tables with SoC Runtime Information
  PlatformUpdateAcpiTables ();

  if (FixedPcdGetBool(PcdStartUsbController)) {
    // Notify the USB Controller to Start Now
    Status = gBS->CreateEventEx (EVT_NOTIFY_SIGNAL, TPL_CALLBACK, DummyNotify, NULL, &gUsbControllerInitGuid, &UsbConfigEvt);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%a: Failed to Signal USB Controller Start Event! Status = %r\n", __FUNCTION__, Status));
    } else {
      gBS->SignalEvent (UsbConfigEvt);
      gBS->CloseEvent  (UsbConfigEvt);
    }
  }

  if (FixedPcdGetBool(PcdSDCardSlotPresent)) {
    // Notify the SDC Controller to Init SD Card Now
    Status = gBS->CreateEventEx (EVT_NOTIFY_SIGNAL, TPL_CALLBACK, DummyNotify, NULL, &gSDCardInitGuid, &SDCardEvt);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%a: Failed to Signal SD Card Init Event! Status = %r\n", __FUNCTION__, Status));
    } else {
      gBS->SignalEvent (SDCardEvt);
      gBS->CloseEvent  (SDCardEvt);
    }
  }
}

EFI_STATUS
EFIAPI
BdsExtensionPriorityBoot (EFI_BOOT_MANAGER_LOAD_OPTION *BootOption)
{
  EFI_STATUS  Status        = EFI_SUCCESS;
  BOOLEAN     FrontPageBoot = MsBootPolicyLibIsVolumeUp ();
  BOOLEAN     SlotSwitch    = MsBootPolicyLibIsVolumeDown ();

  MsBootPolicyLibClearBootRequests ();

  //
  // There are four cases:
  //   1.  Nothing pressed.           - Return EFI_NOT_FOUND.
  //   2.  FrontPageBoot              - Load FrontPage.
  //   3.  SlotSwitch                 - Load Slot Switch App.
  //

  if (SlotSwitch) {
#if AB_SLOT_SUPPORT == 1
    Status = BuildFwLoadOption (BootOption, PcdGetPtr (PcdSlotSwitchApp), "VOL-");
#else
    Status = EFI_NOT_FOUND;
#endif
  } else if (FrontPageBoot) {
    Status = MsBootOptionsLibGetBootManagerMenu (BootOption, "VOL+");
    SetRebootReason (OEM_REBOOT_TO_SETUP_KEY);
  } else {
    Status = EFI_NOT_FOUND;
  }

  return Status;
}
