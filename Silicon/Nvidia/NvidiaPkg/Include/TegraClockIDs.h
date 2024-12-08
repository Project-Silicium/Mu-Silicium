/**
  Copyright (C) 2013-2015, NVIDIA Corporation <www.nvidia.com>
  SPDX-License-Identifier: GPL-2.0+
**/

#ifndef _TEGRA_CLOCK_IDS_H_
#define _TEGRA_CLOCK_IDS_H_

//
// Define Clock IDs (Some are Missing?)
//      
#define CLOCK_ID_CGENERAL   FixedPcdGet8 (PcdCGeneralID)
#define CLOCK_ID_MEMORY     FixedPcdGet8 (PcdMemoryID)
#define CLOCK_ID_PERIPH     FixedPcdGet8 (PcdPeriphID)
#define CLOCK_ID_AUDIO      FixedPcdGet8 (PcdAudioID)
#define CLOCK_ID_USB        FixedPcdGet8 (PcdUsbID)
#define CLOCK_ID_DISPLAY    FixedPcdGet8 (PcdDisplayID)
#define CLOCK_ID_DISPLAY2   FixedPcdGet8 (PcdDisplay2ID)
#define CLOCK_ID_XCPU       FixedPcdGet8 (PcdXCpuID)
#define CLOCK_ID_EPCI       FixedPcdGet8 (PcdEpciID)
#define CLOCK_ID_SFROM32KHZ FixedPcdGet8 (PcdSfrom32KhzID)
#define CLOCK_ID_DP         FixedPcdGet8 (PcdDpID)
#define CLOCK_ID_32KHZ      FixedPcdGet8 (Pcd32KhzID)
#define CLOCK_ID_OSC        FixedPcdGet8 (PcdOscID)
#define CLOCK_ID_CLK_M      FixedPcdGet8 (PcdClkmID)
#define CLOCK_ID_COUNT      FixedPcdGet8 (PcdIDCount)

#define CLOCK_ID_FIRST        CLOCK_ID_CGENERAL
#define CLOCK_ID_FIRST_SIMPLE CLOCK_ID_XCPU

#endif /* _TEGRA_CLOCK_IDS_H_ */
