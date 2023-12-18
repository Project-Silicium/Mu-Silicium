#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Protocol/EFIUsbPwrCtrl.h>

EFI_STATUS
EFIAPI
UsbPwr (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS          Status;
  EFI_USB_PWR_CTRL_PROTOCOL *mUsbPwrProtocol;

  Status = gBS->LocateProtocol(&gEfiUsbPwrCtrlProtocolGuid, NULL, (VOID *)&mUsbPwrProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate Usb power Protocol! Status = %r\n", Status));
    goto exit;
  }

  Status = mUsbPwrProtocol->SetVbusSourceEn(0, TRUE);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Set vbus en! Status = %r\n", Status));
    goto exit;
  }

exit:
  return Status;
}