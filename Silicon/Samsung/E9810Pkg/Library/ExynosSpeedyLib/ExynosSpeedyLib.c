#include <Library/IoLib.h>
#include <Library/TimerLib.h>
#include <Library/ExynosSpeedyLib.h>

STATIC
VOID
SWResetSpeedy (UINT32 SpeedyBase)
{
  UINT32 SpeedyCtrl;

  // Set SpeedyCtrl Config
  SpeedyCtrl  = MmioRead32 (SpeedyBase + SPEEDY_CTRL);
  SpeedyCtrl |= SPEEDY_SW_RST;

  // Write Speedy Config
  MmioWrite32 (SpeedyBase + SPEEDY_CTRL, SpeedyCtrl);

  // Wait a Bit
  MicroSecondDelay (10);
}

STATIC
VOID
EnableSpeedy (UINT32 SpeedyBase)
{
  UINT32 SpeedyCtrl;

  // Set SpeedyCtrl Config
  SpeedyCtrl  = MmioRead32 (SpeedyBase + SPEEDY_CTRL);
  SpeedyCtrl |= SPEEDY_ENABLE;

  // Write SpeedyCtrl Config
  MmioWrite32 (SpeedyBase + SPEEDY_CTRL, SpeedyCtrl);
}

VOID
InitSpeedy ()
{
  // Clear Speedy Interrupt States
  MmioWrite32 (0x141C0000 + SPEEDY_INT_STATUS, 0xFFFFFFFF);

  // Reset Speedy
  SWResetSpeedy (0x141C0000);

  // Enable Speedy
  EnableSpeedy (0x141C0000);
}