/**
  Copyright@ Samsung Electronics Co. LTD

  This software is proprietary of Samsung Electronics.
  No part of this software, either material or conceptual may be copied or distributed, transmitted,
  transcribed, stored in a retrieval system or translated into any human or computer language in any form by any means,
  electronic, mechanical, manual or otherwise, or disclosed
  to third parties without the express written permission of Samsung Electronics.
**/

#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryMapHelperLib.h>

#include <Protocol/EfiSpeedy.h>
#include <Protocol/EfiS2mpu09.h>

#include "S2mpu09.h"

STATIC EFI_SPEEDY_PROTOCOL          *mSpeedyProtocol;
STATIC EFI_MEMORY_REGION_DESCRIPTOR  SpeedyMemoryRegion;

EFI_STATUS
DisableWtsr ()
{
  EFI_STATUS Status;
  UINT8      Value;

  // Read S2MPU09_RTC_WTSR_SMPL Register
  Status = mSpeedyProtocol->Read (SpeedyMemoryRegion.Address, S2MPU09_RTC_ADDR,
                                   S2MPU09_RTC_WTSR_SMPL, &Value);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Read RTC_WTSR_SMPL! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  // Disable WSTR
  Value &= ~WTSR_EN_MASK;

  // Write S2MPU09_RTC_WTSR_SMPL Register
  Status = mSpeedyProtocol->Write (SpeedyMemoryRegion.Address, S2MPU09_RTC_ADDR,
                                    S2MPU09_RTC_WTSR_SMPL, Value);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Disable WTSR! Status = %r\n", __FUNCTION__, Status));
    ASSERT_EFI_ERROR (Status);
  }

  return EFI_SUCCESS;
}

EFI_STATUS
DisableSmpl ()
{
  EFI_STATUS Status;
  UINT8      Value;

  // Read S2MPU09_RTC_WTSR_SMPL Register
  Status = mSpeedyProtocol->Read (SpeedyMemoryRegion.Address, S2MPU09_RTC_ADDR,
                                   S2MPU09_RTC_WTSR_SMPL, &Value);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Read RTC_WTSR_SMPL! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  // Disable SMPL
  Value &= ~SMPL_EN_MASK;

  // Write S2MPU09_RTC_WTSR_SMPL Register
  Status = mSpeedyProtocol->Write (SpeedyMemoryRegion.Address, S2MPU09_RTC_ADDR,
                                    S2MPU09_RTC_WTSR_SMPL, Value);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Disable SMPL! Status = %r\n", __FUNCTION__, Status));
    ASSERT_EFI_ERROR (Status);
  }

  return EFI_SUCCESS;
}

STATIC EFI_S2MPU09_PROTOCOL mS2mpu09 = {
  DisableWtsr,
  DisableSmpl
};

VOID
DisplayPmicInfos ()
{
  EFI_STATUS Status;
  UINT8      Value;

  // Read S2MPU09_PM_INT1 Register
  Status = mSpeedyProtocol->Read (SpeedyMemoryRegion.Address, S2MPU09_PM_ADDR,
                                   S2MPU09_PM_INT1, &Value);
  if (!EFI_ERROR (Status)) {
    DEBUG ((EFI_D_WARN, "S2MPU09_PM_INT1     = 0x%x\n", Value));
  }

  // Read S2MPU09_PM_INT2 Register
  Status = mSpeedyProtocol->Read (SpeedyMemoryRegion.Address, S2MPU09_PM_ADDR,
                                   S2MPU09_PM_INT2, &Value);
  if (!EFI_ERROR (Status)) {
    DEBUG ((EFI_D_WARN, "S2MPU09_PM_INT2     = 0x%x\n", Value));
  }

  // Read S2MPU09_PM_PWRONSRC Register
  Status = mSpeedyProtocol->Read (SpeedyMemoryRegion.Address, S2MPU09_PM_ADDR,
                                   S2MPU09_PM_PWRONSRC, &Value);
  if (!EFI_ERROR (Status)) {
    DEBUG ((EFI_D_WARN, "S2MPU09_PM_PWRONSRC = 0x%x\n", Value));
  }

  // Read S2MPU09_PM_OFFSRC Register
  Status = mSpeedyProtocol->Read (SpeedyMemoryRegion.Address, S2MPU09_PM_ADDR,
                                   S2MPU09_PM_OFFSRC, &Value);
  if (!EFI_ERROR (Status)) {
    DEBUG ((EFI_D_WARN, "S2MPU09_PM_OFFSRC   = 0x%x\n", Value));
  }

  // Read S2MPU09_PM_RTC_BUF Register
  Status = mSpeedyProtocol->Read (SpeedyMemoryRegion.Address, S2MPU09_PM_ADDR,
                                   S2MPU09_PM_RTC_BUF, &Value);
  if (!EFI_ERROR (Status)) {
    DEBUG ((EFI_D_WARN, "S2MPU09_PM_RTC_BUFF = 0x%x\n", Value));
  }

  // Read S2MPU09_PM_CTRL1 Register
  Status = mSpeedyProtocol->Read (SpeedyMemoryRegion.Address, S2MPU09_PM_ADDR,
                                   S2MPU09_PM_CTRL1, &Value);
  if (!EFI_ERROR (Status)) {
    DEBUG ((EFI_D_WARN, "S2MPU09_PM_CTRL1    = 0x%x\n", Value));
  }

  // Read S2MPU09_PM_CTRL3 Register
  Status = mSpeedyProtocol->Read (SpeedyMemoryRegion.Address, S2MPU09_PM_ADDR,
                                   S2MPU09_PM_CTRL3, &Value);
  if (!EFI_ERROR (Status)) {
    DEBUG ((EFI_D_WARN, "S2MPU09_PM_CTRL3    = 0x%x\n", Value));
  }

  // Read S2MPU09_PM_LDO38_CTRL Register
  Status = mSpeedyProtocol->Read (SpeedyMemoryRegion.Address, S2MPU09_PM_ADDR,
                                   S2MPU09_PM_LDO38_CTRL, &Value);
  if (!EFI_ERROR (Status)) {
    DEBUG ((EFI_D_WARN, "S2MPU09_LDO38_CTRL  = 0x%x\n", Value));
  }

  // Read S2MPU09_PM_LDO39_CTRL Register
  Status = mSpeedyProtocol->Read (SpeedyMemoryRegion.Address, S2MPU09_PM_ADDR,
                                   S2MPU09_PM_LDO39_CTRL, &Value);
  if (!EFI_ERROR (Status)) {
    DEBUG ((EFI_D_WARN, "S2MPU09_LDO39_CTRL  = 0x%x\n", Value));
  }

  // Read S2MPU_RTC_WTSR_SMPL Register
  Status = mSpeedyProtocol->Read (SpeedyMemoryRegion.Address, S2MPU09_RTC_ADDR,
                                   S2MPU09_RTC_WTSR_SMPL, &Value);
  if (!EFI_ERROR (Status)) {
    DEBUG ((EFI_D_WARN, "S2MPU09_WTSR_SMPL   = 0x%x\n", Value));
  }
}

EFI_STATUS
InitPmic ()
{
  EFI_STATUS Status;
  UINT8      Value;

  // Read S2MPU09_PM_CTRL1 Register
  Status = mSpeedyProtocol->Read (SpeedyMemoryRegion.Address, S2MPU09_PM_ADDR,
                                   S2MPU09_PM_CTRL1, &Value);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Read CTRL1! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  // Disable Manual Reset
  Value &= ~MRSTB_EN;

  // Write S2MPU09_PN_CTRL1 Register
  Status = mSpeedyProtocol->Write (SpeedyMemoryRegion.Address, S2MPU09_PM_ADDR,
                                    S2MPU09_PM_CTRL1, Value);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Disable Manual Reset! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  // Read S2MPU09_PM_CTRL3 Register
  Status = mSpeedyProtocol->Read (SpeedyMemoryRegion.Address, S2MPU09_PM_ADDR,
                                   S2MPU09_PM_CTRL3, &Value);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Read CTRL3! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  // Enable Warm Reset
  Value |= WRSTEN;

  // Write S2MPU09_LM_CTRL3 Register
  Status = mSpeedyProtocol->Write (SpeedyMemoryRegion.Address, S2MPU09_PM_ADDR,
                                    S2MPU09_PM_CTRL3, Value);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Enable Warm Reset! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  // Read S2MPU09_PM_CTRL3 Register
  Status = mSpeedyProtocol->Read (SpeedyMemoryRegion.Address, S2MPU09_PM_ADDR,
                                   S2MPU09_PM_CTRL3, &Value);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Read CTRL3! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  // Enable AP warm reset detection
  Value |= WRSTBIEN;

  // Write S2MPU09_PM_CTRL3 Register
  Status = mSpeedyProtocol->Write (SpeedyMemoryRegion.Address, S2MPU09_PM_ADDR,
                                    S2MPU09_PM_CTRL3, Value);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Enable WRSTBIEN! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  // Read S2MPU09_PM_RTC_BUF Register
  Status = mSpeedyProtocol->Read (SpeedyMemoryRegion.Address, S2MPU09_PM_ADDR,
                                   S2MPU09_PM_RTC_BUF, &Value);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Read RTC_BUF! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  // PERI 32kHz on, AP 32kHz on
  Value |= (_32KHZPERI_EN | _32KHZAP_EN);

  // Write S2MPU09_PM_RTC_BUF Register
  Status = mSpeedyProtocol->Write (SpeedyMemoryRegion.Address, S2MPU09_PM_ADDR,
                                    S2MPU09_PM_RTC_BUF, Value);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Enable 32KHz! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  // Read S2MPU09_PM_LDO38_CTRL Register
  Status = mSpeedyProtocol->Read (SpeedyMemoryRegion.Address, S2MPU09_PM_ADDR,
                                   S2MPU09_PM_LDO38_CTRL, &Value);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Read LDO38! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  // Enable LDO38
  Value |= S2MPU09_OUTPUT_ON_NORMAL;

  // Write S2MPU09_PM_LDO38_CTRL Register
  Status = mSpeedyProtocol->Write (SpeedyMemoryRegion.Address, S2MPU09_PM_ADDR,
                                    S2MPU09_PM_LDO38_CTRL, Value);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Enable LDO38 (LCD)! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  // Read S2MPU09_PM_LDO39_CTRL Register
  Status = mSpeedyProtocol->Read (SpeedyMemoryRegion.Address, S2MPU09_PM_ADDR,
                                   S2MPU09_PM_LDO39_CTRL, &Value);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Read LDO39! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  // Enable LDO39
  Value |= S2MPU09_OUTPUT_ON_NORMAL;

  // Write S2MPU09_PM_LDO39_CTRL Register
  Status = mSpeedyProtocol->Write (SpeedyMemoryRegion.Address, S2MPU09_PM_ADDR,
                                    S2MPU09_PM_LDO39_CTRL, Value);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Enable LDO39 (LCD)! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  // Read S2MPU09_RTC_WTSR_SMPL Register
  Status = mSpeedyProtocol->Read (SpeedyMemoryRegion.Address, S2MPU09_RTC_ADDR,
                                   S2MPU09_RTC_WTSR_SMPL, &Value);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Read RTC_WTSR_SMPL! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  // Enable WTSR
  Value = (Value & 0xB8) | 0x43;

  // Write S2MPU09_RTC_WTSR_SMPL Register
  Status = mSpeedyProtocol->Write (SpeedyMemoryRegion.Address, S2MPU09_RTC_ADDR,
                                    S2MPU09_RTC_WTSR_SMPL, Value);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Enable WTSR! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
InitS2mpu09 (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status;

  // Locate Speedy Protocol
  Status = gBS->LocateProtocol (&gEfiSpeedyProtocolGuid, NULL, (VOID *)&mSpeedyProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Locate Speedy Protocol! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  // Locate "Speedy" Memory Region
  Status = LocateMemoryRegionByName ("Speedy", &SpeedyMemoryRegion);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to get 'Speedy' Memory Region! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  // Init PMIC
  Status = InitPmic ();
  ASSERT_EFI_ERROR (Status);

  // Display PMIC Infos
  DisplayPmicInfos ();

  // Register S2mpu09 PMIC Control
  Status = gBS->InstallMultipleProtocolInterfaces (&ImageHandle, &gEfiS2mpu09ProtocolGuid, &mS2mpu09, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Register S2MPU09 Protocol! Status = %r\n", __FUNCTION__, Status));
    ASSERT_EFI_ERROR (Status);
  }

  return EFI_SUCCESS;
}
