/**
  Copyright (c) 2010-2019, NVIDIA CORPORATION.  All rights reserved.
  SPDX-License-Identifier: GPL-2.0
**/

#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryMapHelperLib.h>
#include <Library/TegraClockLib.h>

#include <Protocol/EfiTegraClock.h>

//
// Global Variables
//
STATIC ARM_MEMORY_REGION_DESCRIPTOR_EX ClockControllerMemoryRegion;
STATIC UINT32                          PllRate[CLOCK_ID_COUNT];

STATIC UINT32 OscFreq[CLOCK_OSC_FREQ_COUNT] = {
	13000000,
	16800000,
	0,
	0,
	19200000,
	38400000,
	0,
	0,
	12000000,
	48000000,
	0,
	0,
	26000000,
};

STATIC
PllClock*
GetPll (IN UINT8 ClockID)
{
  // Fill Clock Controller Structure
  ClockResetController *ClockController = (ClockResetController *)ClockControllerMemoryRegion.Address;

  // NOTE: An Assert was here.

  // Check for Invalid PLL
  if (ClockID >= (UINT8)TEGRA_CLK_PLLS) {
    DEBUG ((EFI_D_WARN, "%a: Invalid PLL %u was Passed!\n", __FUNCTION__, ClockID));
    return NULL;
  }

  return &ClockController->crc_pll[ClockID];
}

EFI_STATUS
GetClockRate (
  IN  UINT8   ClockID,
  OUT UINT32 *ClockRate)
{
  UINT32 ParentRate;
  UINT32 PllBase;

  // Check for Invalid Clock ID
  if (ClockID == 0xFF) {
    DEBUG ((EFI_D_ERROR, "%a: This Clock ID does not Exist on this Device!\n", __FUNCTION__));
    return EFI_NOT_FOUND;
  }

  // Set Structures
  PllClock       *Pll       = NULL;
  SimplePllClock *SimplePll = NULL;
  PllClockInfo   *PllInfo   = GetPllInfoTable (ClockID);

  // Get Parent Clock Rate
  ParentRate = OscFreq[GetOscClockFreq (ClockControllerMemoryRegion.Address)];
  if (ClockID == CLOCK_ID_OSC) {
    *ClockRate = ParentRate;
    return EFI_SUCCESS;
  } else if (ClockID == CLOCK_ID_CLK_M) {
    *ClockRate = GetClkmRate (ParentRate, ClockControllerMemoryRegion.Address);
    return EFI_SUCCESS;
  }

  // Determine PLL Type
  if (ClockID < (UINT8)TEGRA_CLK_PLLS) {
    Pll = GetPll (ClockID);
  } else {
    SimplePll = GetSimplePll (ClockID, ClockControllerMemoryRegion.Address);
  }

  // Check for Errors
  if (!Pll && !SimplePll) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Get Pll/SimplePll for Clock %u!\n", __FUNCTION__, ClockID));
    return EFI_DEVICE_ERROR;
  }

  // Get PLL Base
  if (Pll) {
    PllBase = MmioRead32 ((UINTN)&Pll->pll_base);
  } else {
    PllBase = MmioRead32 ((UINTN)&SimplePll->pll_base);
  }

  // Calculate Clock Rate
  *ClockRate = ParentRate * ((PllBase >> PllInfo->n_shift) & PllInfo->n_mask);

  // NOTE: Here was some Diff Code
  //       Not Sure if its needed at all as it did Nothing to the End Value

  return EFI_SUCCESS;
}

STATIC EFI_TEGRA_CLOCK_PROTOCOL mTegraClock = {
  GetClockRate
};

EFIAPI
EFI_STATUS
InitClocks (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status;

  // Get Clock Controller Region
  Status = LocateMemoryMapAreaByName ("Clock Controller", &ClockControllerMemoryRegion);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to get Clock Controller Memory Region! Status = %r\n\n", Status));
    return Status;
  }

  // Get Clock Rates
  Status  = GetClockRate (CLOCK_ID_CGENERAL,   &PllRate[CLOCK_ID_CGENERAL]);
  Status |= GetClockRate (CLOCK_ID_MEMORY,     &PllRate[CLOCK_ID_MEMORY]);
  Status |= GetClockRate (CLOCK_ID_PERIPH,     &PllRate[CLOCK_ID_PERIPH]);
  Status |= GetClockRate (CLOCK_ID_USB,        &PllRate[CLOCK_ID_USB]);
  Status |= GetClockRate (CLOCK_ID_DISPLAY,    &PllRate[CLOCK_ID_DISPLAY]);
  Status |= GetClockRate (CLOCK_ID_XCPU,       &PllRate[CLOCK_ID_XCPU]);
  Status |= GetClockRate (CLOCK_ID_OSC,        &PllRate[CLOCK_ID_OSC]);
  Status |= GetClockRate (CLOCK_ID_CLK_M,      &PllRate[CLOCK_ID_CLK_M]);

  if (CLOCK_ID_DISPLAY2 != 0xFF) {
    Status |= GetClockRate (CLOCK_ID_DISPLAY2, &PllRate[CLOCK_ID_DISPLAY2]);
  }

  // Check for Errors
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Get Clock Rates! Status = %r\n", Status));
    return EFI_DEVICE_ERROR;
  }

  // Set Clock Rate
  PllRate[CLOCK_ID_SFROM32KHZ] = 32768;

  // Print Clock Rates
  DEBUG ((EFI_D_WARN, "PLLC = %u Hz\n", PllRate[CLOCK_ID_CGENERAL]));
  DEBUG ((EFI_D_WARN, "PLLM = %u Hz\n", PllRate[CLOCK_ID_MEMORY]));
  DEBUG ((EFI_D_WARN, "PLLP = %u Hz\n", PllRate[CLOCK_ID_PERIPH]));
  DEBUG ((EFI_D_WARN, "PLLU = %u Hz\n", PllRate[CLOCK_ID_USB]));
  DEBUG ((EFI_D_WARN, "PLLD = %u Hz\n", PllRate[CLOCK_ID_DISPLAY]));
  DEBUG ((EFI_D_WARN, "PLLX = %u Hz\n", PllRate[CLOCK_ID_XCPU]));
  DEBUG ((EFI_D_WARN, "Osc  = %u Hz\n", PllRate[CLOCK_ID_OSC]));
  DEBUG ((EFI_D_WARN, "CLKM = %u Hz\n", PllRate[CLOCK_ID_CLK_M]));

  // NOTE: Do we need to Init Clocks here?
  //       The Bootloader before should have already Done that.

  // Register Tegra Clock Protocol
  Status = gBS->InstallMultipleProtocolInterfaces (&ImageHandle, &gEfiTegraClockProtocolGuid, &mTegraClock, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Register Tegra Clock Protocol!\n"));
    ASSERT_EFI_ERROR (Status);
  }

  return EFI_SUCCESS;
}
