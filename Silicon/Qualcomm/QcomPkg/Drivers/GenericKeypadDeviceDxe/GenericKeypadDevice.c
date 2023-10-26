#include <PiDxe.h>

#include <Library/DebugLib.h>
#include <Library/KeypadDeviceImplLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Protocol/KeypadDevice.h>

#include <Configuration/BootDevices.h>

STATIC KEYPAD_DEVICE_PROTOCOL mInternalKeypadDevice = {
  KeypadDeviceImplReset,
  KeypadDeviceImplGetKeys,
};

EFI_STATUS
EFIAPI
KeypadDeviceDxeInitialize(
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status;

  Status = gBS->InstallMultipleProtocolInterfaces(&ImageHandle, &gQcomKeypadDeviceProtocolGuid, &mInternalKeypadDevice, &gEfiDevicePathProtocolGuid, &KeypadDevicePath, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Install Keypad Device Protocol!\n"));
    ASSERT_EFI_ERROR(Status);
  }

  return Status;
}