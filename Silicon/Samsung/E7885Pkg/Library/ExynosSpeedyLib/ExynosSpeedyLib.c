/**
  Copyright (C) Samsung Electronics Co. LTD

  This software is proprietary of Samsung Electronics.
  No part of this software, either material or conceptual may be copied or distributed, transmitted,
  transcribed, stored in a retrieval system or translated into any human or computer language in any form by any means,
  electronic, mechanical, manual or otherwise, or disclosed
  to third parties without the express written permission of Samsung Electronics.

  Alternatively, this program is free software in case of open source project
  you can redistribute it and/or modify
  it under the terms of the GNU General Public License version 2 as
  published by the Free Software Foundation.
**/

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
  MmioWrite32 (0x11CE0000 + SPEEDY_INT_STATUS, 0xFFFFFFFF);

  // Reset Speedy
  SWResetSpeedy (0x11CE0000);

  // Enable Speedy
  EnableSpeedy (0x11CE0000);
}
