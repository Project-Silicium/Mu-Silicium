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

#define MSHC_CTRL_EMMC             0
#define MSHC_CTRL_SD               1
#define MSHC_HOST_COUNT            2

#define S2MPU08_VQMMC_LDO          2
#define S2MPU08_VMMC_LDO           35
#define MSHC_SD_DEFAULT_UV         3000000
#define MSHC_SD_MIN_OCR_UV         2700000

typedef struct {
  UINTN MuxTop;
  UINTN DivTop;
  UINTN GateTop;
  UINTN UserMux;
  UINTN GateAclk;
  UINTN GateSdclkin;
  UINTN Qch;
} MSHC_CMU_REGS;

STATIC CONST MSHC_CMU_REGS mCmuRegs[MSHC_HOST_COUNT] = {
  // Controller 0: eMMC (MMC_EMBD)
  {
    .MuxTop      = CLK_CON_MUX_CLKCMU_FSYS_MMC_EMBD,
    .DivTop      = CLK_CON_DIV_CLKCMU_FSYS_MMC_EMBD,
    .GateTop     = CLK_CON_GAT_CLKCMU_FSYS_MMC_EMBD,
    .UserMux     = PLL_CON0_MUX_FSYS_MMC_EMBD_USER,
    .GateAclk    = CLK_CON_GAT_MMC_EMBD_I_ACLK,
    .GateSdclkin = CLK_CON_GAT_MMC_EMBD_SDCLKIN,
    .Qch         = QCH_CON_MMC_EMBD_QCH,
  },
  // Controller 1: microSD (MMC_CARD)
  {
    .MuxTop      = CLK_CON_MUX_CLKCMU_FSYS_MMC_CARD,
    .DivTop      = CLK_CON_DIV_CLKCMU_FSYS_MMC_CARD,
    .GateTop     = CLK_CON_GAT_CLKCMU_FSYS_MMC_CARD,
    .UserMux     = PLL_CON0_MUX_FSYS_MMC_CARD_USER,
    .GateAclk    = CLK_CON_GAT_MMC_CARD_I_ACLK,
    .GateSdclkin = CLK_CON_GAT_MMC_CARD_SDCLKIN,
    .Qch         = QCH_CON_MMC_CARD_QCH,
  },
};

//
// Global Variables
//
STATIC EFI_GPIO_PROTOCOL           *mGpioProtocol   = NULL;
STATIC EFI_PMIC_REGULATOR_PROTOCOL *mPmicProtocol   = NULL;
STATIC UINT32                       mVqmmcDefaultUv = 0;

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
  CONST MSHC_CMU_REGS *Cmu = &mCmuRegs[Host->ControllerId];

  MmioAnd32 (Cmu->DivTop, ~(UINT32)CMU_DIV_RATIO_MASK);
  MshcCmuWaitBusy (Cmu->DivTop, CMU_DIV_BUSY);

  MmioAndThenOr32 (Cmu->MuxTop, ~(UINT32)CMU_MUX_SEL_MASK, CMU_MUX_SEL_PLL_SHARED0_DIV2);
  MshcCmuWaitBusy (Cmu->MuxTop, CMU_MUX_BUSY);

  MmioOr32 (Cmu->GateTop, CMU_GATE_ON);

  MmioOr32 (Cmu->UserMux, CMU_USER_MUX_SEL);
  MshcCmuWaitBusy (Cmu->UserMux, CMU_USER_MUX_BUSY);

  MmioOr32 (Cmu->GateAclk, CMU_GATE_ON);
  MmioOr32 (Cmu->GateSdclkin, CMU_GATE_ON);

  MmioAndThenOr32 (Cmu->Qch, ~(UINT32)CMU_QCH_ENABLE, CMU_QCH_CLOCK_REQ);

  Host->CiuClockHz = MSHC_CIU_CLOCK_PLL_HZ;
}

STATIC
EFI_STATUS
MshcConfigurePin (
  IN EFI_GPIO_BANK_ID        BankId,
  IN UINT8                   BankNumber,
  IN UINT8                   Pin,
  IN EFI_GPIO_FUNCTION       Function,
  IN EFI_GPIO_PULL_MODE      Pull,
  IN EFI_GPIO_DRIVE_STRENGTH Drive)
{
  EFI_STATUS Status;

  Status = mGpioProtocol->SetFunction (BankId, BankNumber, Pin, Function);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Configure Pin %u! Status = %r\n", __FUNCTION__, Pin, Status));
    return Status;
  }

  Status = mGpioProtocol->SetPull (BankId, BankNumber, Pin, Pull);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to set Pull Mode of Pin %u! Status = %r\n", __FUNCTION__, Pin, Status));
    return Status;
  }

  return mGpioProtocol->SetDrive (BankId, BankNumber, Pin, Drive);
}

STATIC
EFI_STATUS
MshcSetupEmmcPins (VOID)
{
  EFI_STATUS Status;

  // GPF0-0: sd0_clk
  Status = MshcConfigurePin (BANK_ID_F, 0, 0, FUNCTION_2, PULL_NONE, DRIVE_2_5X);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // GPF0-1: sd0_cmd
  Status = MshcConfigurePin (BANK_ID_F, 0, 1, FUNCTION_2, PULL_UP, DRIVE_2_5X);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // GPF0-2: sd0_rdqs
  Status = MshcConfigurePin (BANK_ID_F, 0, 2, FUNCTION_2, PULL_DOWN, DRIVE_2_5X);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // GPF2-0..7: sd0_bus1..8
  for (UINT8 Pin = 0; Pin <= 7; Pin++) {
    Status = MshcConfigurePin (BANK_ID_F, 2, Pin, FUNCTION_2, PULL_UP, DRIVE_2_5X);
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
MshcSetupSdPins (VOID)
{
  EFI_STATUS Status;

  // GPF4-0: sd2_clk, GPF4-1: sd2_cmd, GPF4-2..5: sd2_bus1..4
  for (UINT8 Pin = 0; Pin <= 5; Pin++) {
    EFI_GPIO_PULL_MODE Pull = (Pin == 0) ? PULL_NONE : PULL_UP;

    Status = MshcConfigurePin (BANK_ID_F, 4, Pin, FUNCTION_2, Pull, DRIVE_2X);
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }

  // GPA0-7: dwmmc2_cd
  Status = mGpioProtocol->SetFunction (BANK_ID_A, 0, 7, FUNCTION_INPUT);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Configure GPA0-7! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  Status = mGpioProtocol->SetPull (BANK_ID_A, 0, 7, PULL_NONE);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to set the Pull Mode of GPA0-7! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
MshcSetupSdPower (VOID)
{
  EFI_STATUS Status;
  UINT32     VmmcUv = 0;

  if (mPmicProtocol == NULL) {
    Status = gBS->LocateProtocol (&gEfiPmicRegulatorProtocolGuid, NULL, (VOID **)&mPmicProtocol);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_WARN, "%a: Failed to Locate the PMIC Regulator Protocol! Status = %r\n", __FUNCTION__, Status));
      mPmicProtocol = NULL;
      return EFI_SUCCESS;
    }
  }

  if (EFI_ERROR (mPmicProtocol->GetLdoVoltage (ID_S2MPU08, S2MPU08_VQMMC_LDO, &mVqmmcDefaultUv)) ||
      mVqmmcDefaultUv < MSHC_SD_MIN_OCR_UV) {
    mVqmmcDefaultUv = MSHC_SD_DEFAULT_UV;
    Status = mPmicProtocol->SetLdoVoltage (ID_S2MPU08, S2MPU08_VQMMC_LDO, mVqmmcDefaultUv);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%a: Failed to Set Default VQMMC Voltage! Status = %r\n", __FUNCTION__, Status));
      return Status;
    }
  }

  if (EFI_ERROR (mPmicProtocol->GetLdoVoltage (ID_S2MPU08, S2MPU08_VMMC_LDO, &VmmcUv)) ||
      VmmcUv < MSHC_SD_MIN_OCR_UV) {
    Status = mPmicProtocol->SetLdoVoltage (ID_S2MPU08, S2MPU08_VMMC_LDO, MSHC_SD_DEFAULT_UV);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%a: Failed to Set Default VMMC Voltage! Status = %r\n", __FUNCTION__, Status));
      return Status;
    }
  }

  Status = mPmicProtocol->SetLdo (ID_S2MPU08, S2MPU08_VMMC_LDO, S2MPS_MODE_NORMAL, TRUE);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Enable VMMC! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  Status = mPmicProtocol->SetLdo (ID_S2MPU08, S2MPU08_VQMMC_LDO, S2MPS_MODE_NORMAL, TRUE);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Enable VQMMC! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  gBS->Stall (15000);

  return EFI_SUCCESS;
}

UINTN
MshcBoardGetHostCount (VOID)
{
  return MSHC_HOST_COUNT;
}

EFI_STATUS
MshcBoardInit (
  IN     UINTN      Index,
  IN OUT MSHC_HOST *Host)
{
  EFI_MEMORY_REGION_DESCRIPTOR MemRegion;
  EFI_STATUS                   Status;
  CHAR8                       *RegionName;

  if (Index >= MSHC_HOST_COUNT) {
    return EFI_NOT_FOUND;
  }

  if (mGpioProtocol == NULL) {
    Status = gBS->LocateProtocol (&gEfiGpioProtocolGuid, NULL, (VOID **)&mGpioProtocol);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%a: Failed to Locate the GPIO Protocol! Status = %r\n", __FUNCTION__, Status));
      return Status;
    }
  }

  RegionName = (Index == MSHC_CTRL_EMMC) ? "MSHC" : "MSHC2";

  Status = LocateMemoryRegionByName (RegionName, &MemRegion);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Locate the %a Memory Region! Status = %r\n", __FUNCTION__, RegionName, Status));
    return Status;
  }

  Host->ControllerId  = Index;
  Host->BaseAddress   = MemRegion.Address;
  Host->Coherent      = FALSE;
  Host->ClkSelDivider = MSHC_CLKSEL_DIV;

  if (Index == MSHC_CTRL_EMMC) {
    Host->BusWidth           = 8;
    Host->ClkSelTiming       = MSHC_EMMC_TIMING_SDR;
    Host->ClkSelTimingSdr50  = MSHC_EMMC_TIMING_SDR;
    Host->ClkSelTimingSdr104 = MSHC_EMMC_TIMING_SDR;

    MshcSetupClocks (Host);

    return MshcSetupEmmcPins ();
  }

  Host->BusWidth           = 4;
  Host->ClkSelTiming       = MSHC_SD_TIMING_SDR;
  Host->ClkSelTimingSdr50  = MSHC_SD_TIMING_SDR50;
  Host->ClkSelTimingSdr104 = MSHC_SD_TIMING_SDR104;

  Status = MshcSetupSdPower ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  MshcSetupClocks (Host);

  return MshcSetupSdPins ();
}

EFI_STATUS
MshcBoardSetCiuClock (
  IN OUT MSHC_HOST *Host,
  IN     UINTN      Frequency)
{
  UINTN Wanted;
  UINTN Div;
  UINTN Ratio;

  if (Host->ControllerId >= MSHC_HOST_COUNT) {
    return EFI_INVALID_PARAMETER;
  }

  Wanted = (Frequency < MSHC_CCLKIN_MIN) ? MSHC_CCLKIN_MIN : Frequency;
  Div    = Wanted * (Host->ClkSelDivider + 1);
  Ratio  = (MSHC_CIU_CLOCK_PLL_HZ + Div - 1) / Div;

  if (Ratio == 0) {
    Ratio = 1;
  } else if (Ratio > CMU_DIV_RATIO_MASK + 1) {
    Ratio = CMU_DIV_RATIO_MASK + 1;
  }

  MmioAndThenOr32 (mCmuRegs[Host->ControllerId].DivTop, ~(UINT32)CMU_DIV_RATIO_MASK, (UINT32)(Ratio - 1));
  MshcCmuWaitBusy (mCmuRegs[Host->ControllerId].DivTop, CMU_DIV_BUSY);

  Host->CiuClockHz = MSHC_CIU_CLOCK_PLL_HZ / Ratio;

  return EFI_SUCCESS;
}

EFI_STATUS
MshcBoardSetVoltage (
  IN OUT MSHC_HOST *Host,
  IN     UINT32     Millivolts)
{
  EFI_STATUS Status;
  UINT32     Microvolts;

  if (Host->ControllerId != MSHC_CTRL_SD) {
    return EFI_UNSUPPORTED;
  }

  if (mPmicProtocol == NULL) {
    return EFI_NOT_READY;
  }

  switch (Millivolts) {
    case 3300:
      Microvolts = (mVqmmcDefaultUv != 0) ? mVqmmcDefaultUv : MSHC_SD_DEFAULT_UV;
      break;

    case 1800:
      Microvolts = 1800000;
      break;

    default:
      DEBUG ((EFI_D_ERROR, "%a: %u mV is not a Voltage this Slot Offers.\n", __FUNCTION__, Millivolts));
      return EFI_UNSUPPORTED;
  }

  Status = mPmicProtocol->SetLdoVoltage (ID_S2MPU08, S2MPU08_VQMMC_LDO, Microvolts);
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

  if (Host->ControllerId != MSHC_CTRL_SD) {
    return EFI_UNSUPPORTED;
  }

  if (mPmicProtocol == NULL) {
    return EFI_NOT_READY;
  }

  DEBUG ((EFI_D_ERROR, "%a: Power Cycling the Card.\n", __FUNCTION__));

  mPmicProtocol->SetLdo (ID_S2MPU08, S2MPU08_VQMMC_LDO, S2MPS_MODE_NORMAL, FALSE);
  mPmicProtocol->SetLdo (ID_S2MPU08, S2MPU08_VMMC_LDO, S2MPS_MODE_NORMAL, FALSE);

  gBS->Stall (20000);

  if (mVqmmcDefaultUv != 0) {
    mPmicProtocol->SetLdoVoltage (ID_S2MPU08, S2MPU08_VQMMC_LDO, mVqmmcDefaultUv);
  }

  Status = mPmicProtocol->SetLdo (ID_S2MPU08, S2MPU08_VMMC_LDO, S2MPS_MODE_NORMAL, TRUE);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Enable VMMC! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  Status = mPmicProtocol->SetLdo (ID_S2MPU08, S2MPU08_VQMMC_LDO, S2MPS_MODE_NORMAL, TRUE);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Enable VQMMC! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  gBS->Stall (15000);

  return EFI_SUCCESS;
}

VOID
MshcBoardPostReset (IN OUT MSHC_HOST *Host)
{
  MmioOr32 (Host->BaseAddress + MSHCI_FORCE_CLK_STOP, MMC_HWACG_CONTROL);
  MmioOr32 (Host->BaseAddress + MSHCI_BLOCK_DMA_FOR_CI, BLOCK_DMA_NO_EXT_TIMEOUT);
}

BOOLEAN
MshcBoardIsCardPresent (IN MSHC_HOST *Host)
{
  EFI_STATUS Status;
  BOOLEAN    State;

  if (Host->ControllerId == MSHC_CTRL_EMMC) {
    return TRUE;
  }

  if (mGpioProtocol == NULL) {
    return FALSE;
  }

  Status = mGpioProtocol->GetState (BANK_ID_A, 0, 7, &State);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Read the Card Detect Pin! Status = %r\n", __FUNCTION__, Status));
    return FALSE;
  }

  return !State;
}

BOOLEAN
MshcBoardIsReadOnly (IN MSHC_HOST *Host)
{
  (VOID)Host;
  return FALSE;
}
