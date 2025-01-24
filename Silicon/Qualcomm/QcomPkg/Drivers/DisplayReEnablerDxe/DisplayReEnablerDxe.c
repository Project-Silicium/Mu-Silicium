#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Protocol/EFIDisplayPwr.h>

EFI_STATUS
EFIAPI
ReEnableDisplay (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS                  Status;
  EFI_DISPLAY_POWER_PROTOCOL *mDisplayPowerProtocol;

  // Locate Display Power State Protocol
  Status = gBS->LocateProtocol (&gEfiDisplayPowerStateProtocolGuid, NULL, (VOID *)&mDisplayPowerProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate Display Power State Protocol! Status = %r\n", Status));
    return Status;
  }

  // Turn Off Display
  Status = mDisplayPowerProtocol->SetDisplayPowerState (mDisplayPowerProtocol, EfiDisplayPowerStateOff);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Turn Off Display! Status = %r\n", Status));
    return Status;
  }

  // Wait a bit
  gBS->Stall (1000);

  // Turn On Display
  Status = mDisplayPowerProtocol->SetDisplayPowerState (mDisplayPowerProtocol, EfiDisplayPowerStateOn);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Turn On Display! Status = %r\n", Status));
    return Status;
  }

  return EFI_SUCCESS;
}
