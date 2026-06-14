#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/PmicLib.h>

#include <Protocol/EFISpeedy.h>
#include <Protocol/EFIHsI2c.h>
#include <Protocol/EFIPmicExtra.h>
#include <Protocol/EFIPmicRegulator.h>

#include "Pmic.h"

EFI_STATUS
PmicSetBuck (
  IN EFI_PMIC_ID Id,
  IN UINT8       BuckNumber,
  IN BOOLEAN     Enable)
{
  // Go thru each Supported PMIC
  for (UINT8 i = 0; i < ARRAY_SIZE (SupportedPmicRegulator); i++) {
    // Compare PMIC IDs
    if (Id != SupportedPmicRegulator[i].Id) {
      continue;
    }

    // Set BUCK Function
    EFI_PMIC_REGULATOR_SET_BUCK SetBuck = SupportedPmicRegulator[i].SetBuck;

    // Verify BUCK Function
    if (SetBuck == NULL) {
      return EFI_UNSUPPORTED;
    }

    // Set BUCK Regulator
    return SetBuck (BuckNumber, Enable);
  }

  return EFI_NOT_FOUND;
}

EFI_STATUS
PmicSetLdo (
  IN EFI_PMIC_ID Id,
  IN UINT8       LdoNumber,
  IN UINT8       Mode,
  IN BOOLEAN     Enable)
{
  // Go thru each Supported PMIC
  for (UINT8 i = 0; i < ARRAY_SIZE (SupportedPmicRegulator); i++) {
    // Compare PMIC IDs
    if (Id != SupportedPmicRegulator[i].Id) {
      continue;
    }

    // Set LDO Function
    EFI_PMIC_REGULATOR_SET_LDO SetLdo = SupportedPmicRegulator[i].SetLdo;

    // Verify LDO Function
    if (SetLdo == NULL) {
      return EFI_UNSUPPORTED;
    }

    // Set LDO Regulator
    return SetLdo (LdoNumber, Mode, Enable);
  }

  return EFI_NOT_FOUND;
}

STATIC EFI_PMIC_REGULATOR_PROTOCOL mPmicRegulator = {
  PmicSetBuck,
  PmicSetLdo
};

EFI_STATUS
PmicSetWtsr (
  IN EFI_PMIC_ID Id,
  IN BOOLEAN     Enable)
{
  // Go thru each Supported PMIC
  for (UINT8 i = 0; i < ARRAY_SIZE (SupportedPmicExtra); i++) {
    // Compare PMIC IDs
    if (Id != SupportedPmicExtra[i].Id) {
      continue;
    }

    // Set WTSR Function
    EFI_PMIC_EXTRA_SET_WTSR SetWtsr = SupportedPmicExtra[i].SetWtsr;

    // Verify WTSR Function
    if (SetWtsr == NULL) {
      return EFI_UNSUPPORTED;
    }

    // Set WTSR
    return SetWtsr (Enable);
  }

  return EFI_NOT_FOUND;
}

EFI_STATUS
PmicSetSmpl (
  IN EFI_PMIC_ID Id,
  IN BOOLEAN     Enable)
{
  // Go thru each Supported PMIC
  for (UINT8 i = 0; i < ARRAY_SIZE (SupportedPmicExtra); i++) {
    // Compare PMIC IDs
    if (Id != SupportedPmicExtra[i].Id) {
      continue;
    }

    // Set SMPL Function
    EFI_PMIC_EXTRA_SET_SMPL SetSmpl = SupportedPmicExtra[i].SetSmpl;

    // Verify Smpl Function
    if (SetSmpl == NULL) {
      return EFI_UNSUPPORTED;
    }

    // Set SMPL
    return SetSmpl (Enable);
  }

  return EFI_NOT_FOUND;
}

EFI_STATUS
PmicPowerDown (IN EFI_PMIC_ID Id)
{
  // Go thru each Supported PMIC
  for (UINT8 i = 0; i < ARRAY_SIZE (SupportedPmicExtra); i++) {
    // Compare PMIC IDs
    if (Id != SupportedPmicExtra[i].Id) {
      continue;
    }

    // Set Power Down Function
    EFI_PMIC_EXTRA_POWER_DOWN PowerDown = SupportedPmicExtra[i].PowerDown;

    // Verify WTSR Function
    if (PowerDown == NULL) {
      return EFI_UNSUPPORTED;
    }

    // Power Down PMIC
    return PowerDown ();
  }

  return EFI_NOT_FOUND;
}

STATIC EFI_PMIC_EXTRA_PROTOCOL mPmicExtra = {
  PmicSetWtsr,
  PmicSetSmpl,
  PmicPowerDown
};

EFI_STATUS
ParseSpeedyPmic (
  IN EFI_SPEEDY_PROTOCOL  *SpeedyProtocol,
  IN EFI_SPEEDY_PMIC_DATA *Pmic)
{
  // Verify PMIC ID
  if (!Pmic->Id) {
    return EFI_SUCCESS;
  }

  // Go thru each Supported PMIC
  for (UINT8 i = 0; i < ARRAY_SIZE (SupportedPmic); i++) {
    // Compare PMIC IDs
    if (Pmic->Id != SupportedPmic[i].Id) {
      continue;
    }

    // Get SPEEDY PMIC Init Function
    EFI_SPEEDY_PMIC_INIT Init = SupportedPmic[i].SpeedyInit;

    // Verify SPEEDY PMIC Init Function
    if (Init == NULL) {
      return EFI_UNSUPPORTED;
    }

    // Init SPEEDY PMIC
    return Init (SpeedyProtocol, Pmic->BusNumber);
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

  // Go thru each Supported PMIC
  for (UINT8 i = 0; i < ARRAY_SIZE (SupportedPmic); i++) {
    // Compare PMIC IDs
    if (Pmic->Id != SupportedPmic[i].Id) {
      continue;
    }

    // Get HSI2C PMIC Init Function
    EFI_HSI2C_PMIC_INIT Init = SupportedPmic[i].HsI2cInit;

    // Verify HSI2C PMIC Init Function
    if (Init == NULL) {
      return EFI_UNSUPPORTED;
    }

    // Init HSI2C PMIC
    return Init (HsI2cProtocol, Pmic->BusNumber, Pmic->Slave);
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
  EFI_STATUS Status;

  // Get PMIC Data
  EFI_PMIC_DATA *PmicData = GetPmicData ();

  // Verify PMIC Data
  if (PmicData == NULL) {
    return EFI_UNSUPPORTED;
  }

  // Init PMICs
  InitSpeedyPmics (PmicData->Speedy);
  InitHsI2cPmics  (PmicData->HsI2c);

  // Register PMIC Protocols
  Status = gBS->InstallMultipleProtocolInterfaces (
                &ImageHandle,
                &gEfiPmicExtraProtocolGuid,
                &mPmicExtra,
                &gEfiPmicRegulatorProtocolGuid,
                &mPmicRegulator,
                NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Register PMIC Protocols!\n"));
    return Status;
  }

  return EFI_SUCCESS;
}
