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

#ifndef _GPIO_BANK_ID_H_
#define _GPIO_BANK_ID_H_

//
// GPIO Bank IDs
//
typedef enum {
  BANK_ID_A = 1,
  BANK_ID_B,
  BANK_ID_C,
  BANK_ID_D,
  BANK_ID_E,
  BANK_ID_F,
  BANK_ID_G,
  BANK_ID_H,
  BANK_ID_I,
  BANK_ID_J,
  BANK_ID_K,
  BANK_ID_L,
  BANK_ID_M,
  BANK_ID_N,
  BANK_ID_O,
  BANK_ID_P,
  BANK_ID_Q,
  BANK_ID_R,
  BANK_ID_S,
  BANK_ID_T,
  BANK_ID_U,
  BANK_ID_V,
  BANK_ID_W,
  BANK_ID_X,
  BANK_ID_Y,
  BANK_ID_Z,

  BANK_ID_COUNT
} EFI_GPIO_BANK_ID;

//
// GPIO Functions
//
typedef enum {
  FUNCTION_INPUT,
  FUNCTION_OUTPUT,
  FUNCTION_2,
  FUNCTION_3,

  FUNCTION_NUM
} EFI_GPIO_FUNCTION;

//
// GPIO Pull Modes
//
typedef enum {
  PULL_NONE,
  PULL_DOWN,
  PULL_UP,

  PULL_NUM
} EFI_GPIO_PULL_MODE;

#endif /* _GPIO_BANK_ID_H_ */
