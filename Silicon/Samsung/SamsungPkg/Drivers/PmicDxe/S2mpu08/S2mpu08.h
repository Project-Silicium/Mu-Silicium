/**
  Copyright@ Samsung Electronics Co. LTD

  This software is proprietary of Samsung Electronics.
  No part of this software, either material or conceptual may be copied or distributed, transmitted,
  transcribed, stored in a retrieval system or translated into any human or computer language in any form by any means,
  electronic, mechanical, manual or otherwise, or disclosed
  to third parties without the express written permission of Samsung Electronics.
**/

#ifndef _S2MPU_08_H_
#define _S2MPU_08_H_

#include <Device/Pmic.h>

//
// Slave Addresses
//
#define S2MPU08_TOP_ADDR            0x00
#define S2MPU08_PM_ADDR             0x01
#define S2MPU08_RTC_ADDR            0x02

//
// TOP Register Addresses
//
#define S2MPU08_TOP_PMICID          0x00

//
// PM Register Addresses
//
#define S2MPU08_PM_LDO_CTRL(x)      ((x) <= 2 ? (0x2B + (x)) : (0x2C + (x)))
#define S2MPU08_PM_L36CTRL          0x50
#define S2MPU08_PM_L35_WA_SEQ       0x75
#define S2MPU08_PM_L35_WA_EXT       0xFF

//
// Regulator Limits
//
#define MAX_S2MPU08_LDO_COUNT       37

//
// LDO Voltage Selection
//
// Each Regulator belongs to one of five Groups from drivers/regulator/s2mpu08.c.
//
#define S2MPU08_LDO_VSEL_MASK       0x3F
#define S2MPU08_LDO_GROUPS { \
  1, 3, 2, 4, 1, 1, 2, 5, 5, 5, 5, 1, 2, 3, 1, \
  2, 1, 3, 3, 2, 1, 1, 1, 2, 2, 3, 1, 1, 1, 2, \
  2, 3, 2, 3, 3, 2, 3                          \
}

#define S2MPU08_LDO_GROUP_MIN_UV    { 700000, 700000, 1800000, 500000, 300000 }
#define S2MPU08_LDO_GROUP_STEP_UV   {  12500,  25000,   25000,  12500,  25000 }

//
// LDO Regulator Modes
//
#define S2MPU08_OUTPUT_ON_TCXO      BIT6
#define S2MPU08_OUTPUT_ON_NORMAL   (BIT6 | BIT7)

#endif /* _S2MPU_08_H_ */
