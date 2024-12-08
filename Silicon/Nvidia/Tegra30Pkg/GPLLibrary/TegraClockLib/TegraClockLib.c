/**
  Copyright (C) 2010-2015, NVIDIA Corporation <www.nvidia.com>
  SPDX-License-Identifier: GPL-2.0+
**/

#include <TegraClockIDs.h>

#include <Library/IoLib.h>
#include <Library/TegraClockLib.h>

#define OSC_FREQ_SHIFT  28
#define OSC_FREQ_MASK  (0xF << OSC_FREQ_SHIFT)

PllClockInfo
PllInfoTable[CLOCK_ID_PLL_COUNT] = {
  { .m_shift = 0, .m_mask = 0xFF, .n_shift = 8, .n_mask = 0x3FF, .p_shift = 20, .p_mask = 0x0F, .lock_ena = 24, .lock_det = 27, .kcp_shift = 28, .kcp_mask = 3,   .kvco_shift = 27, .kvco_mask = 1 },   // PLLC
  { .m_shift = 0, .m_mask = 0xFF, .n_shift = 8, .n_mask = 0x3FF, .p_shift = 0,  .p_mask = 0,    .lock_ena = 0,  .lock_det = 27, .kcp_shift = 1,  .kcp_mask = 3,   .kvco_shift = 0,  .kvco_mask = 1 },   // PLLM
  { .m_shift = 0, .m_mask = 0x1F, .n_shift = 8, .n_mask = 0x3FF, .p_shift = 20, .p_mask = 0x07, .lock_ena = 18, .lock_det = 27, .kcp_shift = 8,  .kcp_mask = 0xF, .kvco_shift = 4,  .kvco_mask = 0xF }, // PLLP
  { .m_shift = 0, .m_mask = 0x1F, .n_shift = 8, .n_mask = 0x3FF, .p_shift = 20, .p_mask = 0x07, .lock_ena = 18, .lock_det = 27, .kcp_shift = 8,  .kcp_mask = 0xF, .kvco_shift = 4,  .kvco_mask = 0xF }, // PLLA
  { .m_shift = 0, .m_mask = 0x1F, .n_shift = 8, .n_mask = 0x3FF, .p_shift = 20, .p_mask = 0x01, .lock_ena = 22, .lock_det = 27, .kcp_shift = 8,  .kcp_mask = 0xF, .kvco_shift = 4,  .kvco_mask = 0xF }, // PLLU
  { .m_shift = 0, .m_mask = 0x1F, .n_shift = 8, .n_mask = 0x3FF, .p_shift = 20, .p_mask = 0x07, .lock_ena = 22, .lock_det = 27, .kcp_shift = 8,  .kcp_mask = 0xF, .kvco_shift = 4,  .kvco_mask = 0xF }, // PLLD
  { .m_shift = 0, .m_mask = 0xFF, .n_shift = 8, .n_mask = 0xFF,  .p_shift = 20, .p_mask = 0x0F, .lock_ena = 18, .lock_det = 27, .kcp_shift = 8,  .kcp_mask = 0xF, .kvco_shift = 0,  .kvco_mask = 0 },   // PLLX
  { .m_shift = 0, .m_mask = 0xFF, .n_shift = 8, .n_mask = 0xFF,  .p_shift = 0,  .p_mask = 0,    .lock_ena = 9,  .lock_det = 11, .kcp_shift = 6,  .kcp_mask = 3,   .kvco_shift = 0,  .kvco_mask = 1 },   // PLLE
  { .m_shift = 0, .m_mask = 0x0F, .n_shift = 8, .n_mask = 0x3FF, .p_shift = 20, .p_mask = 0x07, .lock_ena = 18, .lock_det = 27, .kcp_shift = 8,  .kcp_mask = 0xF, .kvco_shift = 4,  .kvco_mask = 0xF }, // PLLS (RESERVED)
  { .m_shift = 0, .m_mask = 0x1F, .n_shift = 8, .n_mask = 0x3FF, .p_shift = 20, .p_mask = 0x07, .lock_ena = 22, .lock_det = 27, .kcp_shift = 8,  .kcp_mask = 0xF, .kvco_shift = 4,  .kvco_mask = 0xF }  // PLLD2
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

    case CLOCK_ID_DISPLAY2:
      return &ClockController->plld2;

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
  return ParentRate;
}
