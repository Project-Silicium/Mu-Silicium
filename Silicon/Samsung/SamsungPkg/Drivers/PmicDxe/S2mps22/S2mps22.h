/**
  Copyright@ Samsung Electronics Co. LTD

  This software is proprietary of Samsung Electronics.
  No part of this software, either material or conceptual may be copied or distributed, transmitted,
  transcribed, stored in a retrieval system or translated into any human or computer language in any form by any means,
  electronic, mechanical, manual or otherwise, or disclosed
  to third parties without the express written permission of Samsung Electronics.
**/

#ifndef _S2MPS_22_H_
#define _S2MPS_22_H_

#include <Device/Pmic.h>

//
// Slave Addresses
//
#define S2MPS22_PM_ADDR          0x01

//
// PM Register Addresses
//
#define S2MPS22_PM_CTRL1           0x13
#define S2MPS22_PM_LDOS_CTRL(x)   (0x29 + x)

//
// Regulator Limits
//
#define MAX_S2MPS22_LDO_COUNT      7

//
// LDO Regulator Modes
//
#define S2MPS22_OUTPUT_ON_TCXO     BIT6
#define S2MPS22_OUTPUT_ON_NORMAL  (BIT6 | BIT7)

#endif /* _S2MPS_22_H_ */
