/**
  Copyright@ Samsung Electronics Co. LTD

  This software is proprietary of Samsung Electronics.
  No part of this software, either material or conceptual may be copied or distributed, transmitted,
  transcribed, stored in a retrieval system or translated into any human or computer language in any form by any means,
  electronic, mechanical, manual or otherwise, or disclosed
  to third parties without the express written permission of Samsung Electronics.
**/

#ifndef _S2MPB_02_H_
#define _S2MPB_02_H_

#include <Device/Pmic.h>

//
// Register Addresses
//
#define S2MPB02_REG_ID              0x00
#define S2MPB02_REG_CTRL            0x04
#define S2MPB02_REG_B1CTRL1         0x0B
#define S2MPB02_REG_B2CTRL1         0x0D
#define S2MPB02_REG_BB1CTRL1        0x0F
#define S2MPB02_REG_BST_CTRL2       0x1D
#define S2MPB02_REG_L1CTRL          0x1E
#define S2MPB02_REG_LDO_CTRL(x)    (S2MPB02_REG_L1CTRL + (x - 1))

//
// Regulator Limits
//
#define MAX_S2MPB02_LDO_COUNT       18
#define MAX_S2MPB02_BUCK_COUNT      3

//
// The Buck Boost is exposed as the Last BUCK Number, since the Regulator
// Protocol has no separate Buck Boost Entry Point.
//
#define S2MPB02_BUCK_BOOST          3

//
// Regulator Enable Masks
//
#define S2MPB02_LDO_ENABLE          BIT7
#define S2MPB02_BUCK_ENABLE        (BIT6 | BIT7)

//
// Revision Marker, read from BST_CTRL2
//
#define S2MPB02_BST_CTRL2_REV1      0x90

#endif /* _S2MPB_02_H_ */
