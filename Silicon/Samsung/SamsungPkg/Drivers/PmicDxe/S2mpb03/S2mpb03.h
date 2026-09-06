/**
  Copyright@ Samsung Electronics Co. LTD

  This software is proprietary of Samsung Electronics.
  No part of this software, either material or conceptual may be copied or distributed, transmitted,
  transcribed, stored in a retrieval system or translated into any human or computer language in any form by any means,
  electronic, mechanical, manual or otherwise, or disclosed
  to third parties without the express written permission of Samsung Electronics.
**/

#ifndef _S2MPB_03_H_
#define _S2MPB_03_H_

#include <Device/Pmic.h>

//
// Register Addresses
//
#define S2MPB03_REG_PMIC_ID         0x00
#define S2MPB03_REG_STATUS          0x01
#define S2MPB03_REG_CTRL            0x02
#define S2MPB03_REG_LDO1_CTRL       0x03
#define S2MPB03_REG_LDO_CTRL(x)    (S2MPB03_REG_LDO1_CTRL + (x - 1))

//
// Regulator Limits
//
#define MAX_S2MPB03_LDO_COUNT       7

//
// Regulator Enable Mask
//
#define S2MPB03_LDO_ENABLE          BIT7

#endif /* _S2MPB_03_H_ */
