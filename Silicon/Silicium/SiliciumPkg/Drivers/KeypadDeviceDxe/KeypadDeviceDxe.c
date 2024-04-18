#include <Library/DebugLib.h>
#include <Library/KeypadDeviceLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Configuration/BootDevices.h>

STATIC KEYPAD_DEVICE_PROTOCOL mKeypadDevice = {
  KeypadDeviceReset,
  KeypadDeviceGetKeys
};

EFI_STATUS
EFIAPI
InitializeKeypadDevice (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status;

  // Install Keypad Device Protocol in a new Handle
  Status = gBS->InstallMultipleProtocolInterfaces(&ImageHandle, &gKeypadDeviceProtocolGuid, &mKeypadDevice, &gEfiDevicePathProtocolGuid, &KeypadDevicePath, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Register Keypad Device Protocol!\n"));
    ASSERT_EFI_ERROR(Status);
  }

  return Status;
}
