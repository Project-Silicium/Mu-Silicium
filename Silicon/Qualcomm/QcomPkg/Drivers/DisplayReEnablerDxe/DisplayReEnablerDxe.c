#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Protocol/EFIDisplayPwr.h>

EFI_STATUS
EFIAPI
DisplayReEnablerDxeInitialization (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS          Status;
  EFI_DISPLAY_POWER_PROTOCOL *mDisplayPowerProtocol;

  Status = gBS->LocateProtocol (&gEfiDisplayPowerStateProtocolGuid, NULL, (VOID *)&mDisplayPowerProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate Display Power Protocol!\n"));
    return Status;
  }

  Status = mDisplayPowerProtocol->SetDisplayPowerState(mDisplayPowerProtocol, EfiDisplayPowerStateOff);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Disable Display!\n"));
    return Status;
  }

  gBS->Stall(1000);

  Status = mDisplayPowerProtocol->SetDisplayPowerState(mDisplayPowerProtocol, EfiDisplayPowerStateOn);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Enable Display!\n"));
    return Status;
  }

  return EFI_SUCCESS;
}
