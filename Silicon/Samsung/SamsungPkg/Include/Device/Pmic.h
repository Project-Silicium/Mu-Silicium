/**
  Copyright@ Samsung Electronics Co. LTD

  This software is proprietary of Samsung Electronics.
  No part of this software, either material or conceptual may be copied or distributed, transmitted,
  transcribed, stored in a retrieval system or translated into any human or computer language in any form by any means,
  electronic, mechanical, manual or otherwise, or disclosed
  to third parties without the express written permission of Samsung Electronics.
**/

#ifndef _PMIC_DEVICE_H_
#define _PMIC_DEVICE_H_

//
// PMIC IDs
//
typedef enum {
  ID_S2MPS18 = 1,
  ID_S2MPS19,
  ID_S2MPS20,
  ID_S2MPS22
} EFI_PMIC_ID;

//
// PMIC LDO Modes
//
typedef enum {
  S2MPS_MODE_TCXO   = 1,
  S2MPS_MODE_NORMAL,

  MODE_COUNT
} EFI_PMIC_LDO_MODE;

#endif /* _PMIC_DEVICE_H_ */
