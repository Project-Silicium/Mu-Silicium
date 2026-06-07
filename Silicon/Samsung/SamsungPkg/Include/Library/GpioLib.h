/**
  Copyright (C) Samsung Electronics Co. LTD

  This software is proprietary of Samsung Electronics.
  No part of this software, either material or conceptual may be copied or distributed, transmitted,
  transcribed, stored in a retrieval system or translated into any human or computer language in any form by any means,
  electronic, mechanical, manual or otherwise, or disclosed
  to third parties without the express written permission of Samsung Electronics.
**/

#ifndef _GPIO_LIB_H_
#define _GPIO_LIB_H_

#include <GpioBankId.h>

//
// GPIO Limits
//
#define MAX_GPIO_BANK_COUNT 64
#define MAX_GPIO_PIN_COUNT  8

//
// GPIO Bank Data
//
typedef struct {
  EFI_GPIO_BANK_ID Id;
  UINT8            Number;
  UINT16           Offset;
} EFI_GPIO_BANK_DATA;

//
// GPIO Controller Data
//
typedef struct {
  EFI_PHYSICAL_ADDRESS Address;
  EFI_GPIO_BANK_DATA   BankData[MAX_GPIO_BANK_COUNT];
} EFI_GPIO_CONTROLLER_DATA;

/**
  This Function Returns the Platform GPIO Controller Data.

  @param[out] Data                         - The GPIO Controller Data.
  @param[out] Count                        - The GPIO Controller Count.
**/
VOID
GetGpioControllerData (
  OUT EFI_GPIO_CONTROLLER_DATA **Data,
  OUT UINT8                     *Count
  );

#endif /* _GPIO_LIB_H_ */
