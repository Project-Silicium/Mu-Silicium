/**
  Copyright@ Samsung Electronics Co. LTD

  This software is proprietary of Samsung Electronics.
  No part of this software, either material or conceptual may be copied or distributed, transmitted,
  transcribed, stored in a retrieval system or translated into any human or computer language in any form by any means,
  electronic, mechanical, manual or otherwise, or disclosed
  to third parties without the express written permission of Samsung Electronics.
**/

#ifndef _S2DOS_05_H_
#define _S2DOS_05_H_

#include <Device/Pmic.h>

//
// Register Addresses
//
#define S2DOS05_REG_DEV_ID         0x00
#define S2DOS05_REG_TOPSYS_STAT    0x01
#define S2DOS05_REG_STAT           0x02
#define S2DOS05_REG_EN             0x03
#define S2DOS05_REG_LDO_CFG(x)    (0x03 + x)
#define S2DOS05_REG_BUCK_CFG       0x08
#define S2DOS05_REG_BUCK_VOUT      0x09
#define S2DOS05_REG_IRQ_MASK       0x0D
#define S2DOS05_REG_IRQ            0x11
#define S2DOS05_REG_DEVICE_ID_PGM  0x61

//
// Device ID Bits
//
#define S2DOS05_ID_PGM_SM3080      BIT7

//
// Regulator Limits
//
#define MAX_S2DOS05_LDO_COUNT      4
#define MAX_S2DOS05_BUCK_COUNT     1

//
// Regulator Enable Bits
//
#define S2DOS05_ENABLE_LDO(x)     (BIT0 << (x - 1))
#define S2DOS05_ENABLE_BUCK        BIT4

#endif /* _S2DOS_05_H_ */
