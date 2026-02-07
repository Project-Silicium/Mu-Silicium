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
#include <Protocol/EfiS2mps1922.h>

#include "S2mps1922.h"

//
// Global Variables
//
STATIC EFI_SPEEDY_PROTOCOL             *mSpeedyProtocol;
STATIC EFI_MEMORY_REGION_DESCRIPTOR_EX  Speedy1MemoryRegion;
STATIC EFI_MEMORY_REGION_DESCRIPTOR_EX  Speedy2MemoryRegion;

EFI_STATUS
DisableWtsr ()
{
  EFI_STATUS Status;
  UINT8      Value;

  // Read S2MPS19_RTC_WTSR_SMPL Register
  Status = mSpeedyProtocol->Read (Speedy1MemoryRegion.Address, S2MPS19_RTC_ADDR, S2MPS19_RTC_WTSR_SMPL, &Value);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Read S2MPS19_RTC_WTSR_SMPL Register! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  // Disable WTSR
  Value &= ~(1 << 6);

  // Write S2MPS19_RTC_WTSR_SMPL Register
  Status = mSpeedyProtocol->Write (Speedy1MemoryRegion.Address, S2MPS19_RTC_ADDR, S2MPS19_RTC_WTSR_SMPL, Value);
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

  // Read S2MPS19_RTC_WTSR_SMPL Register
  Status = mSpeedyProtocol->Read (Speedy1MemoryRegion.Address, S2MPS19_RTC_ADDR, S2MPS19_RTC_WTSR_SMPL, &Value);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Read S2MPS19_RTC_WTSR_SMPL Register! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  // Disable SMPL
  Value &= ~(1 << 7);
  Value &= ~(1 << 8);

  // Write S2MPS19_RTC_WTSR_SMPL Register
  Status = mSpeedyProtocol->Write (Speedy1MemoryRegion.Address, S2MPS19_RTC_ADDR, S2MPS19_RTC_WTSR_SMPL, Value);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Disable SMPL! Status = %r\n", __FUNCTION__, Status));
    ASSERT_EFI_ERROR (Status);
  }

  return EFI_SUCCESS;
}

EFI_STATUS
PowerDown ()
{
  EFI_STATUS Status;

  // Check Init State
  if (!FixedPcdGetBool (PcdInitS2mps22)) {
    DEBUG ((EFI_D_ERROR, "%a: This SoC does not Support this Function!\n", __FUNCTION__));
    return EFI_UNSUPPORTED;
  }

  // Write S2MPS22_PM_CTRL1 Register
  Status = mSpeedyProtocol->Write (Speedy2MemoryRegion.Address, S2MPS22_PM_ADDR, S2MPS22_PM_CTRL1, 0x80);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Power Down PMIC! Status = %r\n", __FUNCTION__, Status));
    ASSERT_EFI_ERROR (Status);
  }

  return EFI_SUCCESS;
}

STATIC EFI_S2MPS_19_22_PROTOCOL mS2mps1922 = {
  DisableWtsr,
  DisableSmpl,
  PowerDown
};

VOID
DisplayPmicInfos ()
{
  EFI_STATUS Status;
  UINT8      Value;

  // Read S2MPS19_PM_INT1 Register
  Status = mSpeedyProtocol->Read (Speedy1MemoryRegion.Address, S2MPS19_PM_ADDR, S2MPS19_PM_INT1, &Value);
  if (!EFI_ERROR (Status)) {
    DEBUG ((EFI_D_WARN, "S2MPS19_PM_INT1       = 0x%x\n", Value));
  }

  // Read S2MPS19_PM_INT2 Register
  Status = mSpeedyProtocol->Read (Speedy1MemoryRegion.Address, S2MPS19_PM_ADDR, S2MPS19_PM_INT2, &Value);
  if (!EFI_ERROR (Status)) {
    DEBUG ((EFI_D_WARN, "S2MPS19_PM_INT2       = 0x%x\n", Value));
  }

  // Read S2MPS19_PM_PWRONSRC Register
  Status = mSpeedyProtocol->Read (Speedy1MemoryRegion.Address, S2MPS19_PM_ADDR, S2MPS19_PM_PWRONSRC, &Value);
  if (!EFI_ERROR (Status)) {
    DEBUG ((EFI_D_WARN, "S2MPS19_PM_PWRONSRC   = 0x%x\n", Value));
  }

  // Read S2MPS19_PM_OFFSRC Register
  Status = mSpeedyProtocol->Read (Speedy1MemoryRegion.Address, S2MPS19_PM_ADDR, S2MPS19_PM_OFFSRC, &Value);
  if (!EFI_ERROR (Status)) {
    DEBUG ((EFI_D_WARN, "S2MPS19_PM_OFFSRC     = 0x%x\n", Value));
  }

  // Read S2MPS19_RTC_WTSR_SMPL Register
  Status = mSpeedyProtocol->Read (Speedy1MemoryRegion.Address, S2MPS19_PM_ADDR, S2MPS19_RTC_WTSR_SMPL, &Value);
  if (!EFI_ERROR (Status)) {
    DEBUG ((EFI_D_WARN, "S2MPS19_RTC_WTSR_SMPL = 0x%x\n", Value));
  }

  // Read S2MPS19_PM_CTRL1 Register
  Status = mSpeedyProtocol->Read (Speedy1MemoryRegion.Address, S2MPS19_PM_ADDR, S2MPS19_PM_CTRL1, &Value);
  if (!EFI_ERROR (Status)) {
    DEBUG ((EFI_D_WARN, "S2MPS19_PM_CTRL1      = 0x%x\n", Value));
  }

  // Read S2MPS19_PM_CTRL3 Register
  Status = mSpeedyProtocol->Read (Speedy1MemoryRegion.Address, S2MPS19_PM_ADDR, S2MPS19_PM_CTRL3, &Value);
  if (!EFI_ERROR (Status)) {
    DEBUG ((EFI_D_WARN, "S2MPS19_PM_CTRL3      = 0x%x\n", Value));
  }

  // Read S2MPS19_PM_RTC_BUF Register
  Status = mSpeedyProtocol->Read (Speedy1MemoryRegion.Address, S2MPS19_PM_ADDR, S2MPS19_PM_RTC_BUF, &Value);
  if (!EFI_ERROR (Status)) {
    DEBUG ((EFI_D_WARN, "S2MPS19_PM_RTC_BUF    = 0x%x\n", Value));
  }

  // Read S2MPS22_PM_CTRL1 Register
  if (FixedPcdGetBool (PcdInitS2mps22)) {
    Status = mSpeedyProtocol->Read (Speedy2MemoryRegion.Address, S2MPS22_PM_ADDR, S2MPS22_PM_CTRL1, &Value);
    if (!EFI_ERROR (Status)) {
      DEBUG ((EFI_D_WARN, "S2MPS22_PM_CTRL1      = 0x%x\n", Value));
    }
  }

  // Read S2MPS19_PM_RTC_BUF Register
  Status = mSpeedyProtocol->Read (Speedy1MemoryRegion.Address, S2MPS19_PM_ADDR, S2MPS19_PM_RTC_BUF, &Value);
  if (!EFI_ERROR (Status)) {
    DEBUG ((EFI_D_WARN, "S2MPS19_PM_RTC_BUF    = 0x%x\n", Value));
  }

  // Read S2MPS19_PM_LDO3M_CTRL Register
  Status = mSpeedyProtocol->Read (Speedy1MemoryRegion.Address, S2MPS19_PM_ADDR, S2MPS19_PM_LDO3M_CTRL, &Value);
  if (!EFI_ERROR (Status)) {
    DEBUG ((EFI_D_WARN, "S2MPS19_PM_LDO3M_CTRL = 0x%x\n", Value));
  }

  // Read S2MPS19_PM_LDO4M_CTRL Register
  Status = mSpeedyProtocol->Read (Speedy1MemoryRegion.Address, S2MPS19_PM_ADDR, S2MPS19_PM_LDO4M_CTRL, &Value);
  if (!EFI_ERROR (Status)) {
    DEBUG ((EFI_D_WARN, "S2MPS19_PM_LDO4M_CTRL = 0x%x\n", Value));
  }

  // Read S2MPS19_PM_LDO9M_CTRL Register
  Status = mSpeedyProtocol->Read (Speedy1MemoryRegion.Address, S2MPS19_PM_ADDR, S2MPS19_PM_LDO9M_CTRL, &Value);
  if (!EFI_ERROR (Status)) {
    DEBUG ((EFI_D_WARN, "S2MPS19_PM_LDO9M_CTRL = 0x%x\n", Value));
  }

  // Read S2MPS22_PM_LDO4S_CTRL Register
  if (FixedPcdGetBool (PcdInitS2mps22)) {
    Status = mSpeedyProtocol->Read (Speedy2MemoryRegion.Address, S2MPS22_PM_ADDR, S2MPS22_PM_LDO4S_CTRL, &Value);
    if (!EFI_ERROR (Status)) {
      DEBUG ((EFI_D_WARN, "S2MPS22_PM_LDO4S_CTRL = 0x%x\n", Value));
    }
  }
}

EFI_STATUS
InitPmic ()
{
  EFI_STATUS Status;
  UINT8      Value;

  // Read S2MPS19_PM_CTRL1 Register
  Status = mSpeedyProtocol->Read (Speedy1MemoryRegion.Address, S2MPS19_PM_ADDR, S2MPS19_PM_CTRL1, &Value);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Read S2MPS19_PM_CTRL1 Register!\n"));
    return Status;
  }

  // Disable Manual Reset
  Value &= ~MRSTB_EN;

  // Write S2MPS19_PM_CTRL1 Register
  Status = mSpeedyProtocol->Write (Speedy1MemoryRegion.Address, S2MPS19_PM_ADDR, S2MPS19_PM_CTRL1, Value);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Disable Manual Reset!\n"));
    return Status;
  }

  // Read S2MPS19_PM_CTRL3 Register
  Status = mSpeedyProtocol->Read (Speedy1MemoryRegion.Address, S2MPS19_PM_ADDR, S2MPS19_PM_CTRL3, &Value);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Read S2MPS19_PM_CTRL3 Register!\n"));
    return Status;
  }

  // Enable Warm Reset
  Value |= WRSTEN;

  // Write S2MPS19_PM_CTRL3 Register
  Status = mSpeedyProtocol->Write (Speedy1MemoryRegion.Address, S2MPS19_PM_ADDR, S2MPS19_PM_CTRL3, Value);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Enable Warm Reset!\n"));
    return Status;
  }

  // Read S2MPS19_PM_LDO3M_CTRL Register
  Status = mSpeedyProtocol->Read (Speedy1MemoryRegion.Address, S2MPS19_PM_ADDR, S2MPS19_PM_LDO3M_CTRL, &Value);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Read S2MPS19_PM_LDO3M_CTRL Register!\n"));
    return Status;
  }

  // Enable LDO3M
  Value |= S2MPS_OUTPUT_ON_NORMAL;

  // Write S2MPS19_PM_LDO3M_CTRL Register
  Status = mSpeedyProtocol->Write (Speedy1MemoryRegion.Address, S2MPS19_PM_ADDR, S2MPS19_PM_LDO3M_CTRL, Value);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Enable LDO3M!\n"));
    return Status;
  }

  // Read S2MPS19_PM_LDO4M_CTRL Register
  Status = mSpeedyProtocol->Read (Speedy1MemoryRegion.Address, S2MPS19_PM_ADDR, S2MPS19_PM_LDO4M_CTRL, &Value);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Read S2MPS19_PM_LDO4M_CTRL Register!\n"));
    return Status;
  }

  // Enable LDO4M
  Value |= S2MPS_OUTPUT_ON_NORMAL;

  // Write S2MPS19_PM_LDO4M_CTRL Register
  Status = mSpeedyProtocol->Write (Speedy1MemoryRegion.Address, S2MPS19_PM_ADDR, S2MPS19_PM_LDO4M_CTRL, Value);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Enable LDO4M!\n"));
    return Status;
  }

  // Read S2MPS19_PM_LDO9M_CTRL Register
  Status = mSpeedyProtocol->Read (Speedy1MemoryRegion.Address, S2MPS19_PM_ADDR, S2MPS19_PM_LDO9M_CTRL, &Value);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Read S2MPS19_PM_LDO9M_CTRL Register!\n"));
    return Status;
  }

  // Enable LDO9M
  Value |= S2MPS_OUTPUT_ON_NORMAL;

  // Write S2MPS19_PM_LDO9M_CTRL Register
  Status = mSpeedyProtocol->Write (Speedy1MemoryRegion.Address, S2MPS19_PM_ADDR, S2MPS19_PM_LDO9M_CTRL, Value);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Enable LDO9M!\n"));
    return Status;
  }

  if (FixedPcdGetBool (PcdInitS2mps22)) {
    // Read S2MPS22_PM_LDO4S_CTRL Register
    Status = mSpeedyProtocol->Read (Speedy2MemoryRegion.Address, S2MPS22_PM_ADDR, S2MPS22_PM_LDO4S_CTRL, &Value);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "Failed to Read S2MPS22_PM_LDO4S_CTRL Register!\n"));
      return Status;
    }

    // Enable LDO4S
    Value |= S2MPS_OUTPUT_ON_NORMAL;

    // Write S2MPS22_PM_LDO4S_CTRL Register
    Status = mSpeedyProtocol->Write (Speedy2MemoryRegion.Address, S2MPS22_PM_ADDR, S2MPS22_PM_LDO4S_CTRL, Value);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "Failed to Enable LDO4S!\n"));
      return Status;
    }
  }

  // Read S2MPS19_PM_LDO16M_CTRL Register
  Status = mSpeedyProtocol->Read (Speedy1MemoryRegion.Address, S2MPS19_PM_ADDR, S2MPS19_PM_LDO16M_CTRL, &Value);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Read S2MPS19_PM_LDO16M_CTRL Register!\n"));
    return Status;
  }

  // Enable LDO16M
  Value |= S2MPS_OUTPUT_ON_NORMAL;

  // Write S2MPS19_PM_LDO16M_CTRL Register
  Status = mSpeedyProtocol->Write (Speedy1MemoryRegion.Address, S2MPS19_PM_ADDR, S2MPS19_PM_LDO16M_CTRL, Value);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Enable LDO16M!\n"));
    return Status;
  }

  // Read S2MPS19_PM_LDO10M_CTRL Register
  Status = mSpeedyProtocol->Read (Speedy1MemoryRegion.Address, S2MPS19_PM_ADDR, S2MPS19_PM_LDO10M_CTRL, &Value);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Read S2MPS19_PM_LDO10M_CTRL Register!\n"));
    return Status;
  }

  // Reconfigure LDO10M
  Value |= S2MPS_OUTPUT_ON_TCXO;

  // Write S2MPS19_PM_LDO10M_CTRL Register
  Status = mSpeedyProtocol->Write (Speedy1MemoryRegion.Address, S2MPS19_PM_ADDR, S2MPS19_PM_LDO10M_CTRL, Value);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Reconfigure LDO10M!\n"));
    return Status;
  }

  // Read S2MPS19_PM_LDO11M_CTRL Register
  Status = mSpeedyProtocol->Read (Speedy1MemoryRegion.Address, S2MPS19_PM_ADDR, S2MPS19_PM_LDO11M_CTRL, &Value);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Read S2MPS19_PM_LDO11M_CTRL Register!\n"));
    return Status;
  }

  // Reconfigure LDO11M
  Value |= S2MPS_OUTPUT_ON_TCXO;

  // Write S2MPS19_PM_LDO11M_CTRL Register
  Status = mSpeedyProtocol->Write (Speedy1MemoryRegion.Address, S2MPS19_PM_ADDR, S2MPS19_PM_LDO11M_CTRL, Value);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Reconfigure LDO11M!\n"));
    return Status;
  }

  // Read S2MPS19_PM_LDO12M_CTRL Register
  Status = mSpeedyProtocol->Read (Speedy1MemoryRegion.Address, S2MPS19_PM_ADDR, S2MPS19_PM_LDO12M_CTRL, &Value);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Read S2MPS19_PM_LDO12M_CTRL Register!\n"));
    return Status;
  }

  // Reconfigure LDO12M
  Value |= S2MPS_OUTPUT_ON_TCXO;

  // Write S2MPS19_PM_LDO12M_CTRL Register
  Status = mSpeedyProtocol->Write (Speedy1MemoryRegion.Address, S2MPS19_PM_ADDR, S2MPS19_PM_LDO12M_CTRL, Value);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Reconfigure LDO12M!\n"));
    return Status;
  }

  // Read S2MPS19_RTC_WTSR_SMPL Register
  Status = mSpeedyProtocol->Read (Speedy1MemoryRegion.Address, S2MPS19_RTC_ADDR, S2MPS19_RTC_WTSR_SMPL, &Value);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Read S2MPS19_RTC_WTSR_SMPL Register!\n"));
    return Status;
  }

  // Enable WTSR
  Value = (Value & 0xB8) | 0x43;

  // Write S2MPS19_RTC_WTSR_SMPL Register
  Status = mSpeedyProtocol->Write (Speedy1MemoryRegion.Address, S2MPS19_RTC_ADDR, S2MPS19_RTC_WTSR_SMPL, Value);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Enable WTSR!\n"));
    return Status;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
InitS2mps1922 (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status;

  // Locate Speedy Protocol
  Status = gBS->LocateProtocol (&gEfiSpeedyProtocolGuid, NULL, (VOID *)&mSpeedyProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate Speedy Protocol! Status = %r\n", Status));
    return Status;
  }

  // Locate "Speedy-1" Memory Region
  Status = LocateMemoryMapAreaByName ("Speedy-1", &Speedy1MemoryRegion);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to get 'Speedy-1' Memory Region! Status = %r\n", Status));
    return Status;
  }

  // Locate "Speedy-2" Memory Region
  Status = LocateMemoryMapAreaByName ("Speedy-2", &Speedy2MemoryRegion);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to get 'Speedy-2' Memory Region! Status = %r\n", Status));
    return Status;
  }

  // Init PMIC
  ASSERT_EFI_ERROR (InitPmic ());

  // Display PMIC Infos
  DisplayPmicInfos ();

  // Register S2mps1922 PMIC Protocol
  Status = gBS->InstallMultipleProtocolInterfaces (&ImageHandle, &gEfiS2mps1922ProtocolGuid, &mS2mps1922, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Register S2mps1922 PMIC Protocol!\n"));
    ASSERT_EFI_ERROR (Status);
  }

  return EFI_SUCCESS;
}
