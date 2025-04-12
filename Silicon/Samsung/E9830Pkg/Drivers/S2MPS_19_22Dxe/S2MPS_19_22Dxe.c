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
#include <Protocol/EfiPMIC.h>

#include "S2MPS_19_22_Registers.h"

ARM_MEMORY_REGION_DESCRIPTOR_EX Speedy1Region;
ARM_MEMORY_REGION_DESCRIPTOR_EX Speedy2Region;

// Cached copy of the Speedy protocol instance
EFI_SPEEDY_PROTOCOL *gSpeedy = NULL;

VOID PMICInit ()
{
  UINT8 Register;

  /* Disable Manual Reset */
  gSpeedy->Read(Speedy1Region.Address, S2MPS19_PM_ADDR, S2MPS19_PM_CTRL1, &Register);
  Register &= ~MRSTB_EN;
  gSpeedy->Write(Speedy1Region.Address, S2MPS19_PM_ADDR, S2MPS19_PM_CTRL1, Register);

  /* Enable Warm Reset */
  gSpeedy->Read(Speedy1Region.Address, S2MPS19_PM_ADDR, S2MPS19_PM_CTRL3, &Register);
  Register |= WRSTEN;
  gSpeedy->Write(Speedy1Region.Address, S2MPS19_PM_ADDR, S2MPS19_PM_CTRL3, Register);

  /* LCD power */
  gSpeedy->Read(Speedy1Region.Address, S2MPS19_PM_ADDR, S2MPS19_PM_LDO3M_CTRL, &Register);
  Register |= S2MPS_OUTPUT_ON_NORMAL;
  gSpeedy->Write(Speedy1Region.Address, S2MPS19_PM_ADDR, S2MPS19_PM_LDO3M_CTRL, Register);

  gSpeedy->Read(Speedy1Region.Address, S2MPS19_PM_ADDR, S2MPS19_PM_LDO4M_CTRL, &Register);
  Register |= S2MPS_OUTPUT_ON_NORMAL;
  gSpeedy->Write(Speedy1Region.Address, S2MPS19_PM_ADDR, S2MPS19_PM_LDO4M_CTRL, Register);

  gSpeedy->Read(Speedy1Region.Address, S2MPS19_PM_ADDR, S2MPS19_PM_LDO9M_CTRL, &Register);
  Register |= S2MPS_OUTPUT_ON_NORMAL;
  gSpeedy->Write(Speedy1Region.Address, S2MPS19_PM_ADDR, S2MPS19_PM_LDO9M_CTRL, Register);

  gSpeedy->Read(Speedy2Region.Address, S2MPS22_PM_ADDR, S2MPS22_PM_LDO4S_CTRL, &Register);
  Register |= S2MPS_OUTPUT_ON_NORMAL;
  gSpeedy->Write(Speedy2Region.Address, S2MPS22_PM_ADDR, S2MPS22_PM_LDO4S_CTRL, Register);

  /* ICEN enable for PB02 & PB03 */
  gSpeedy->Read(Speedy1Region.Address, S2MPS19_PM_ADDR, S2MPS19_PM_LDO16M_CTRL, &Register);
  Register |= S2MPS_OUTPUT_ON_NORMAL;
  gSpeedy->Write(Speedy1Region.Address, S2MPS19_PM_ADDR, S2MPS19_PM_LDO16M_CTRL, Register);

  /* LDO 10, 11, 12 On for USB */
  gSpeedy->Read(Speedy1Region.Address, S2MPS19_PM_ADDR, S2MPS19_PM_LDO10M_CTRL, &Register);
  Register |= S2MPS_OUTPUT_ON_TCXO;
  gSpeedy->Write(Speedy1Region.Address, S2MPS19_PM_ADDR, S2MPS19_PM_LDO10M_CTRL, Register);

  gSpeedy->Read(Speedy1Region.Address, S2MPS19_PM_ADDR, S2MPS19_PM_LDO11M_CTRL, &Register);
  Register |= S2MPS_OUTPUT_ON_TCXO;
  gSpeedy->Write(Speedy1Region.Address, S2MPS19_PM_ADDR, S2MPS19_PM_LDO11M_CTRL, Register);

  gSpeedy->Read(Speedy1Region.Address, S2MPS19_PM_ADDR, S2MPS19_PM_LDO12M_CTRL, &Register);
  Register |= S2MPS_OUTPUT_ON_TCXO;
  gSpeedy->Write(Speedy1Region.Address, S2MPS19_PM_ADDR, S2MPS19_PM_LDO12M_CTRL, Register);

  /* Enable WTSR */
  gSpeedy->Read(Speedy1Region.Address, S2MPS19_RTC_ADDR, S2MPS19_RTC_WTSR_SMPL, &Register);
  Register = (Register & 0xB8) | 0x43;
  gSpeedy->Write(Speedy1Region.Address, S2MPS19_RTC_ADDR, S2MPS19_RTC_WTSR_SMPL, Register);
}

VOID PMICDisableWTSR ()
{
  UINT8 Register;

  gSpeedy->Read(Speedy1Region.Address, S2MPS19_RTC_ADDR, S2MPS19_RTC_WTSR_SMPL, &Register);

  Register &= ~(1 << 6); // Clear bit 6 (WTSR)

  gSpeedy->Write(Speedy1Region.Address, S2MPS19_RTC_ADDR, S2MPS19_RTC_WTSR_SMPL, Register);
}

VOID PMICDisableSMPL ()
{
  UINT8 Register;

  gSpeedy->Read(Speedy1Region.Address, S2MPS19_RTC_ADDR, S2MPS19_RTC_WTSR_SMPL, &Register);

  Register &= ~(1 << 7); // Clear bit 7 (SMPL)
  Register &= ~(1 << 8); // Clear bit 8 (SUB_SMPL)

  gSpeedy->Write(Speedy1Region.Address, S2MPS19_RTC_ADDR, S2MPS19_RTC_WTSR_SMPL, Register);
}

VOID PMICShutdown()
{
  UINT8 Register = 0x80;

  gSpeedy->Write(Speedy2Region.Address, S2MPS22_PM_ADDR, S2MPS22_PM_CTRL1, Register);
}

EFI_PMIC_PROTOCOL mPMIC = {
  PMICDisableWTSR,
  PMICDisableSMPL,
  PMICShutdown
};

EFI_STATUS
EFIAPI
InitS2MPS_19_22Driver (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status;

  // Locate Speedy Memory Regions. ASSERT if not found.
  Status = LocateMemoryMapAreaByName ("Speedy-1", &Speedy1Region);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate 'Speedy-1' Memory Region! Status = %r\n", Status));
    ASSERT_EFI_ERROR (Status);
  }

  Status = LocateMemoryMapAreaByName ("Speedy-2", &Speedy2Region);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate 'Speedy-2' Memory Region! Status = %r\n", Status));
    ASSERT_EFI_ERROR (Status);
  }

  // Find the Speedy protocol. ASSERT if not found.
  Status = gBS->LocateProtocol (&gEfiSpeedyProtocolGuid, NULL, (VOID **)&gSpeedy);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate Speedy Protocol!\n"));
    ASSERT_EFI_ERROR (Status);
  }

  PMICInit ();

  // Register PMIC Protocol
  Status = gBS->InstallMultipleProtocolInterfaces (&ImageHandle, &gEfiPMICProtocolGuid, &mPMIC, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Register PMIC Protocol!\n"));
    ASSERT_EFI_ERROR (Status);
  }

  DEBUG((EFI_D_WARN, "S2MPS_19_22 PMIC Initialised!\n"));

  return EFI_SUCCESS;
}
