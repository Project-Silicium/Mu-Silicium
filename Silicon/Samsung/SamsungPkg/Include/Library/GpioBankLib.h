/**
  Copyright (C) Samsung Electronics Co. LTD

  This software is proprietary of Samsung Electronics.
  No part of this software, either material or conceptual may be copied or distributed, transmitted,
  transcribed, stored in a retrieval system or translated into any human or computer language in any form by any means,
  electronic, mechanical, manual or otherwise, or disclosed
  to third parties without the express written permission of Samsung Electronics.
**/

#ifndef _GPIO_BANK_LIB_H_
#define _GPIO_BANK_LIB_H_

#include <Device/Gpio.h>

//
// GPIO Bank
//
typedef struct {
  UINT8            CtrlNum;
  EFI_GPIO_BANK_ID Id;
  UINT8            Number;
  UINT16           Offset;
} EFI_GPIO_BANK;

/**
  This Function Returns the Platform GPIO Banks.

  @param[out] Bank                         - The GPIO Banks.
  @param[out] Count                        - The GPIO Bank Count.
**/
VOID
GetGpioBanks (
  OUT EFI_GPIO_BANK **Bank,
  OUT UINT8          *Count
  );

#endif /* _GPIO_BANK_LIB_H_ */
