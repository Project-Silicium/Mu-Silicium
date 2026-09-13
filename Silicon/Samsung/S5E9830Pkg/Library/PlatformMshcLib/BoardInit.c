/**
  Copyright (c) 2012, Samsung Electronics Co. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
**/

#include <Uefi.h>

#include <Library/BaseLib.h>
#include <Library/TimerLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryMapHelperLib.h>
#include <Library/MshcHostBridge.h>

#include <Device/Gpio.h>
#include <Device/Pmic.h>


#include <Protocol/EFIGpio.h>
#include <Protocol/EFIPmicRegulator.h>

#include "Cmu.h"
#include "MshcExynos.h"

#define SYSREG_HSI1_BASE                   0x13020000
#define SYSREG_HSI1_MMC_IOCOHERENCY        (SYSREG_HSI1_BASE + 0x700)
#define MMC_IOCOHERENCY_EN                 (BIT16 | BIT17)

//
// Global Variables
//
STATIC EFI_GPIO_PROTOCOL           *mGpioProtocol = NULL;
STATIC EFI_PMIC_REGULATOR_PROTOCOL *mPmicProtocol = NULL;
STATIC UINT32 mVqmmcDefaultUv = 0;

STATIC
EFI_STATUS
MshcCmuWaitBusy (
  IN UINTN  Register,
  IN UINT32 BusyBit)
{
  for (UINTN Timeout = CMU_TIMEOUT_COUNT; Timeout > 0; Timeout--) {
    if (!(MmioRead32 (Register) & BusyBit)) {
      return EFI_SUCCESS;
    }

    gBS->Stall (1);
  }

  DEBUG ((EFI_D_ERROR, "%a: CMU Register 0x%08X is still Busy!\n", __FUNCTION__, Register));

  return EFI_TIMEOUT;
}

STATIC
VOID
MshcSetupClocks (IN OUT MSHC_HOST *Host)
{
  MmioOr32 (CLK_CON_GAT_MMC_CARD_I_ACLK,  CMU_GATE_ON);
  MmioOr32 (CLK_CON_GAT_MMC_CARD_SDCLKIN, CMU_GATE_ON);

  MmioAndThenOr32 (QCH_CON_MMC_CARD_QCH, ~(UINT32)CMU_QCH_ENABLE, CMU_QCH_CLOCK_REQ);

  MmioAnd32 (CLK_CON_DIV_CLKCMU_HSI1_MMC_CARD, ~(UINT32)CMU_DIV_RATIO_MASK);
  MshcCmuWaitBusy (CLK_CON_DIV_CLKCMU_HSI1_MMC_CARD, CMU_DIV_BUSY);

  MmioAndThenOr32 (CLK_CON_MUX_CLKCMU_HSI1_MMC_CARD, ~(UINT32)CMU_MUX_SEL_MASK, CMU_MUX_SEL_PLL_SHARED2);
  MshcCmuWaitBusy (CLK_CON_MUX_CLKCMU_HSI1_MMC_CARD, CMU_MUX_BUSY);

  MmioOr32 (CLK_CON_GAT_CLKCMU_HSI1_MMC_CARD, CMU_GATE_ON);

  MmioOr32 (PLL_CON0_MUX_HSI1_MMC_CARD_USER, CMU_USER_MUX_SEL);
  MshcCmuWaitBusy (PLL_CON0_MUX_HSI1_MMC_CARD_USER, CMU_USER_MUX_BUSY);

  Host->CiuClockHz = MSHC_CIU_CLOCK_PLL_HZ;
}

EFI_STATUS
MshcBoardSetCiuClock (
  IN OUT MSHC_HOST *Host,
  IN     UINTN      Frequency)
{
  UINTN Wanted;
  UINTN Ratio;

  Wanted = (Frequency < MSHC_CCLKIN_MIN) ? MSHC_CCLKIN_MIN : Frequency;
  Ratio  = MSHC_CIU_CLOCK_PLL_HZ / (Wanted * (Host->ClkSelDivider + 1));

  if (Ratio == 0) {
    Ratio = 1;
  } else if (Ratio > CMU_DIV_RATIO_MASK + 1) {
    Ratio = CMU_DIV_RATIO_MASK + 1;
  }

  MmioAndThenOr32 (CLK_CON_DIV_CLKCMU_HSI1_MMC_CARD, ~(UINT32)CMU_DIV_RATIO_MASK, (UINT32)(Ratio - 1));
  MshcCmuWaitBusy (CLK_CON_DIV_CLKCMU_HSI1_MMC_CARD, CMU_DIV_BUSY);

  Host->CiuClockHz = MSHC_CIU_CLOCK_PLL_HZ / Ratio;

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
MshcSetupPins (VOID)
{
  EFI_STATUS Status;

  for (UINT8 Pin = 0; Pin <= 5; Pin++) {
    EFI_GPIO_PULL_MODE      Pull  = (Pin == 0) ? PULL_NONE : PULL_UP;
    EFI_GPIO_DRIVE_STRENGTH Drive = (Pin == 0) ? DRIVE_3X  : DRIVE_2_5X;

    Status = mGpioProtocol->SetFunction (BANK_ID_F, 1, Pin, FUNCTION_2);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%a: Failed to Configure GPF1-%u! Status = %r\n", __FUNCTION__, Pin, Status));
      return Status;
    }

    Status = mGpioProtocol->SetPull (BANK_ID_F, 1, Pin, Pull);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%a: Failed to set the Pull Mode of GPF1-%u! Status = %r\n", __FUNCTION__, Pin, Status));
      return Status;
    }

    Status = mGpioProtocol->SetDrive (BANK_ID_F, 1, Pin, Drive);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%a: Failed to set the Drive Strength of GPF1-%u! Status = %r\n", __FUNCTION__, Pin, Status));
      return Status;
    }
  }

  Status = mGpioProtocol->SetFunction (BANK_ID_A, 1, 5, FUNCTION_INPUT);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Configure GPA1-5! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  Status = mGpioProtocol->SetPull (BANK_ID_A, 1, 5, PULL_UP);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to set the Pull Mode of GPA1-5! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
MshcBoardSetVoltage (
  IN OUT MSHC_HOST *Host,
  IN     UINT32     Millivolts)
{
  EFI_STATUS Status;
  UINT32     Microvolts;

  if (mPmicProtocol == NULL) {
    return EFI_NOT_READY;
  }

  switch (Millivolts) {
    case 3300:
      if (mVqmmcDefaultUv == 0) {
        DEBUG ((EFI_D_ERROR, "%a: The Default Bus Voltage was never Read.\n", __FUNCTION__));
        return EFI_NOT_READY;
      }

      Microvolts = mVqmmcDefaultUv;
      break;

    case 1800:
      Microvolts = 1800000;
      break;

    default:
      DEBUG ((EFI_D_ERROR, "%a: %u mV is not a Voltage this Slot Offers.\n", __FUNCTION__, Millivolts));
      return EFI_UNSUPPORTED;
  }

  Status = mPmicProtocol->SetLdoVoltage (ID_S2MPS19, 2, Microvolts);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to move VQMMC to %u mV! Status = %r\n", __FUNCTION__, Millivolts, Status));
    return Status;
  }

  gBS->Stall (15000);

  DEBUG ((EFI_D_WARN, "%a: The Bus Supply is now at %u mV.\n", __FUNCTION__, Millivolts));

  return EFI_SUCCESS;
}

EFI_STATUS
MshcBoardPowerCycle (IN OUT MSHC_HOST *Host)
{
  EFI_STATUS Status;

  if (mPmicProtocol == NULL) {
    return EFI_NOT_READY;
  }

  DEBUG ((EFI_D_ERROR, "%a: Power Cycling the Card.\n", __FUNCTION__));

  mPmicProtocol->SetLdo (ID_S2MPS19, 2, S2MPS_MODE_NORMAL, FALSE);
  mPmicProtocol->SetLdo (ID_S2MPS19, 15,  S2MPS_MODE_NORMAL, FALSE);

  gBS->Stall (20000);

  if (mVqmmcDefaultUv != 0) {
    mPmicProtocol->SetLdoVoltage (ID_S2MPS19, 2, mVqmmcDefaultUv);
  }

  Status = mPmicProtocol->SetLdo (ID_S2MPS19, 15, S2MPS_MODE_NORMAL, TRUE);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Enable VMMC! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  Status = mPmicProtocol->SetLdo (ID_S2MPS19, 2, S2MPS_MODE_NORMAL, TRUE);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Enable VQMMC! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  gBS->Stall (15000);

  return EFI_SUCCESS;
}

EFI_STATUS
MshcBoardInit (IN OUT MSHC_HOST *Host)
{
  EFI_MEMORY_REGION_DESCRIPTOR MemRegion;
  EFI_STATUS                   Status;

  Status = LocateMemoryRegionByName ("MSHC", &MemRegion);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Locate the MSHC Memory Region! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  Host->BaseAddress        = MemRegion.Address;
  Host->Coherent           = TRUE;
  Host->ClkSelTiming       = MSHC_TIMING_SDR;
  Host->ClkSelTimingSdr50  = MSHC_TIMING_SDR50;
  Host->ClkSelTimingSdr104 = MSHC_TIMING_SDR104;
  Host->ClkSelDivider      = MSHC_CLKSEL_DIV;

  Status = gBS->LocateProtocol (&gEfiGpioProtocolGuid, NULL, (VOID **)&mGpioProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Locate the GPIO Protocol! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  Status = gBS->LocateProtocol (&gEfiPmicRegulatorProtocolGuid, NULL, (VOID **)&mPmicProtocol);
  if (EFI_ERROR (Status))
  {
    DEBUG ((EFI_D_WARN, "%a: Failed to Locate the PMIC Regulator Protocol! Status = %r\n", __FUNCTION__, Status));
  }
  else
  {
    if (EFI_ERROR (mPmicProtocol->GetLdoVoltage (ID_S2MPS19, 2, &mVqmmcDefaultUv)))
    {
      mVqmmcDefaultUv = 0;
    }

    Status = mPmicProtocol->SetLdo (ID_S2MPS19, 15, S2MPS_MODE_NORMAL, TRUE);
    if (EFI_ERROR (Status))
    {
      DEBUG ((EFI_D_ERROR, "%a: Failed to Enable VMMC! Status = %r\n", __FUNCTION__, Status));
      return Status;
    }

    Status = mPmicProtocol->SetLdo (ID_S2MPS19, 2, S2MPS_MODE_NORMAL, TRUE);
    if (EFI_ERROR (Status))
    {
      DEBUG ((EFI_D_ERROR, "%a: Failed to Enable VWMMC! Status = %r\n", __FUNCTION__, Status));
      return Status;
    }

    gBS->Stall (15000);
  }

  MshcSetupClocks (Host);

  Status = MshcSetupPins ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  MmioOr32 (SYSREG_HSI1_MMC_IOCOHERENCY, MMC_IOCOHERENCY_EN);

  return EFI_SUCCESS;
}

BOOLEAN
MshcBoardIsCardPresent (VOID)
{
  EFI_STATUS Status;
  BOOLEAN    State;

  if (mGpioProtocol == NULL) {
    return FALSE;
  }

  Status = mGpioProtocol->GetState (BANK_ID_A, 1, 5, &State);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Read the Card Detect Pin! Status = %r\n", __FUNCTION__, Status));
    return FALSE;
  }

  return !State;
}

BOOLEAN
MshcBoardIsReadOnly (VOID)
{
  return FALSE;
}

VOID
MshcBoardPostReset (IN OUT MSHC_HOST *Host)
{
  MmioOr32 (Host->BaseAddress + MSHCI_FORCE_CLK_STOP, MMC_HWACG_CONTROL);
  MmioOr32 (Host->BaseAddress + MSHCI_BLOCK_DMA_FOR_CI, BLOCK_DMA_NO_EXT_TIMEOUT);
}
