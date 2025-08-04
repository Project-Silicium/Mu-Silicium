/**
  Copyright (C) 2013-2020, NVIDIA Corporation <www.nvidia.com>
  SPDX-License-Identifier: GPL-2.0+
**/

#include <TegraClockIDs.h>

#include <Library/IoLib.h>
#include <Library/TegraClockLib.h>

#define OSC_FREQ_SHIFT  28
#define OSC_FREQ_MASK  (0xF << OSC_FREQ_SHIFT)

PllClockInfo
PllInfoTable[CLOCK_ID_PLL_COUNT] = {
  { .m_shift = 0, .m_mask = 0xFF, .n_shift = 10, .n_mask = 0xFF, .p_shift = 20, .p_mask = 0x1F, .lock_ena = 32, .lock_det = 27, .kcp_shift = 0,  .kcp_mask = 0, .kvco_shift = 0,  .kvco_mask = 0 }, // PLLC
  { .m_shift = 0, .m_mask = 0xFF, .n_shift = 8,  .n_mask = 0xFF, .p_shift = 20, .p_mask = 0x1F, .lock_ena = 4,  .lock_det = 27, .kcp_shift = 1,  .kcp_mask = 3, .kvco_shift = 0,  .kvco_mask = 1 }, // PLLM
  { .m_shift = 0, .m_mask = 0xFF, .n_shift = 10, .n_mask = 0xFF, .p_shift = 20, .p_mask = 0x1F, .lock_ena = 18, .lock_det = 27, .kcp_shift = 0,  .kcp_mask = 3, .kvco_shift = 2,  .kvco_mask = 1 }, // PLLP
  { .m_shift = 0, .m_mask = 0xFF, .n_shift = 8,  .n_mask = 0xFF, .p_shift = 20, .p_mask = 0x1F, .lock_ena = 28, .lock_det = 27, .kcp_shift = 25, .kcp_mask = 3, .kvco_shift = 24, .kvco_mask = 1 }, // PLLA
  { .m_shift = 0, .m_mask = 0xFF, .n_shift = 8,  .n_mask = 0xFF, .p_shift = 16, .p_mask = 0x1F, .lock_ena = 29, .lock_det = 27, .kcp_shift = 25, .kcp_mask = 3, .kvco_shift = 24, .kvco_mask = 1 }, // PLLU
  { .m_shift = 0, .m_mask = 0xFF, .n_shift = 11, .n_mask = 0xFF, .p_shift = 20, .p_mask = 0x07, .lock_ena = 18, .lock_det = 27, .kcp_shift = 23, .kcp_mask = 3, .kvco_shift = 22, .kvco_mask = 1 }, // PLLD
  { .m_shift = 0, .m_mask = 0xFF, .n_shift = 8,  .n_mask = 0xFF, .p_shift = 20, .p_mask = 0x1F, .lock_ena = 18, .lock_det = 27, .kcp_shift = 1,  .kcp_mask = 3, .kvco_shift = 0,  .kvco_mask = 1 }, // PLLX
  { .m_shift = 0, .m_mask = 0xFF, .n_shift = 8,  .n_mask = 0xFF, .p_shift = 0,  .p_mask = 0,    .lock_ena = 9,  .lock_det = 11, .kcp_shift = 6,  .kcp_mask = 3, .kvco_shift = 0,  .kvco_mask = 1 }, // PLLE
  { .m_shift = 0, .m_mask = 0,    .n_shift = 0,  .n_mask = 0,    .p_shift = 0,  .p_mask = 0,    .lock_ena = 0,  .lock_det = 0,  .kcp_shift = 0,  .kcp_mask = 0, .kvco_shift = 0,  .kvco_mask = 0 }, // PLLS (Doesn't Exist)
  { .m_shift = 0, .m_mask = 0xFF, .n_shift = 8,  .n_mask = 0xFF, .p_shift = 19, .p_mask = 0x1F, .lock_ena = 30, .lock_det = 27, .kcp_shift = 25, .kcp_mask = 3, .kvco_shift = 24, .kvco_mask = 1 }  // PLLDP
};

PllClockInfo*
GetPllInfoTable (IN UINT8 ClockID)
{
  return &PllInfoTable[ClockID];
}

SimplePllClock*
GetSimplePll (
  IN UINT8                ClockID,
  IN EFI_PHYSICAL_ADDRESS ClockControllerBase)
{
  // Fill Clock Reset Controller Structure
  ClockResetController *ClockController = (ClockResetController *)ClockControllerBase;

  switch (ClockID) {
    case CLOCK_ID_XCPU:
    case CLOCK_ID_EPCI:
    case CLOCK_ID_SFROM32KHZ:
      return &ClockController->crc_pll_simple[ClockID - CLOCK_ID_FIRST_SIMPLE];

    default:
      return NULL;
  }
}

OscClockFreqs
GetOscClockFreq (IN EFI_PHYSICAL_ADDRESS ClockControllerBase)
{
  UINT32 Value;

  // Fill Clock Reset Controller Structure
  ClockResetController *ClockController = (ClockResetController *)ClockControllerBase;

  // Get OSC Clock Freq
  Value = MmioRead32 ((UINTN)&ClockController->crc_osc_ctrl);

  // Return OSC Freq
  return (Value & OSC_FREQ_MASK) >> OSC_FREQ_SHIFT;
}

UINT32
GetClkmRate (
  IN UINT32               ParentRate,
  IN EFI_PHYSICAL_ADDRESS ClockControllerBase)
{
  UINT32 Value;
  UINT32 Divider;

  // Fill Clock Reset Controller Structure
  ClockResetController *ClockController = (ClockResetController *)ClockControllerBase;

  // Get CLKM Spacre Reg 0
  Value = MmioRead32 ((UINTN)&ClockController->crc_spare_reg0);
  Divider = ((Value >> 2) & 3) +1;

  // Return CLKM Freq
  return ParentRate / Divider;
}
