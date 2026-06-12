#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/PmicLib.h>

#include <Protocol/EFISpeedy.h>
#include <Protocol/EFIHsI2c.h>
//#include <Protocol/EFIPmic.h>

#include "Pmic.h"

/*
STATIC EFI_PMIC_PROTOCOL mPmic = {
  // TODO!
};
*/

EFI_STATUS
ParseSpeedyPmic (
  IN EFI_SPEEDY_PROTOCOL  *SpeedyProtocol,
  IN EFI_SPEEDY_PMIC_DATA *Pmic)
{
  // Verify PMIC ID
  if (!Pmic->Id) {
    return EFI_SUCCESS;
  }

  // Go thru each Supported SPEEDY PMIC
  for (UINT8 i = 0; i < MAX_SPEEDY_PMIC_COUNT; i++) {
    // Compare PMIC IDs
    if (Pmic->Id != SpeedyPmic[i].Id) {
      continue;
    }

    // Init SPEEDY PMIC
    return SpeedyPmic[i].Init (SpeedyProtocol, Pmic->BusNumber);
  }

  return EFI_NOT_FOUND;
}

EFI_STATUS
ParseHsI2cPmic (
  IN EFI_HSI2C_PROTOCOL  *HsI2cProtocol,
  IN EFI_HSI2C_PMIC_DATA *Pmic)
{
  // Verify PMIC ID
  if (!Pmic->Id) {
    return EFI_SUCCESS;
  }

  // Go thru each Supported HSI2C PMIC
  for (UINT8 i = 0; i < MAX_HSI2C_PMIC_COUNT; i++) {
    // Compare PMIC IDs
    if (Pmic->Id != HsI2cPmic[i].Id) {
      continue;
    }

    // Init HSI2C PMIC
    return HsI2cPmic[i].Init (HsI2cProtocol, Pmic->BusNumber, Pmic->Slave);
  }

  return EFI_NOT_FOUND;
}

VOID
InitSpeedyPmics (IN EFI_SPEEDY_PMIC_DATA *Pmic)
{
  EFI_STATUS           Status;
  EFI_SPEEDY_PROTOCOL *SpeedyProtocol;

  // Check First PMIC Entry
  if (Pmic[0].Id == 0) {
    return;
  }

  // Locate SPEEDY Protocol
  Status = gBS->LocateProtocol (&gEfiSpeedyProtocolGuid, NULL, (VOID *)&SpeedyProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate SPEEDY Protocol! Status = %r\n", Status));
    return;
  }

  // Go thru each SPEEDY PMIC
  for (UINT8 i = 0; i < MAX_SPEEDY_PMIC_COUNT; i++) {
    // Parse SPEEDY PMIC
    Status = ParseSpeedyPmic (SpeedyProtocol, &Pmic[i]);
    ASSERT_EFI_ERROR (Status);
  }
}

VOID
InitHsI2cPmics (IN EFI_HSI2C_PMIC_DATA *Pmic)
{
  EFI_STATUS          Status;
  EFI_HSI2C_PROTOCOL *HsI2cProtocol;

  // Check First PMIC Entry
  if (Pmic[0].Id == 0) {
    return;
  }

  // Locate HSI2C Protocol
  Status = gBS->LocateProtocol (&gEfiHsI2cProtocolGuid, NULL, (VOID *)&HsI2cProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate HSI2C Protocol! Status = %r\n", Status));
    return;
  }

  // Go thru each HSI2C PMIC
  for (UINT8 i = 0; i < MAX_HSI2C_PMIC_COUNT; i++) {
    // Parse HSI2C PMIC
    Status = ParseHsI2cPmic (HsI2cProtocol, &Pmic[i]);
    ASSERT_EFI_ERROR (Status);
  }
}

EFI_STATUS
EFIAPI
InitPmics (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  // Get PMIC Data
  EFI_PMIC_DATA *PmicData = GetPmicData ();

  // Verify PMIC Data
  if (PmicData == NULL) {
    return EFI_UNSUPPORTED;
  }

  // Init PMICs
  InitSpeedyPmics (PmicData->Speedy);
  InitHsI2cPmics  (PmicData->HsI2c);

  /*
  // Register PMIC Protocol
  Status = gBS->InstallProtocolInterface (&ImageHandle, &gEfiPmicProtocolGuid, EFI_NATIVE_INTERFACE, &mPmic);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Register PMIC Protocol!\n"));
    return Status;
  }
  */

  // Loop >:D
  while (TRUE);

  return EFI_SUCCESS;
}
