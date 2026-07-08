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

#ifndef _GPIO_H_
#define _GPIO_H_

//
// GPIO MMIO
//
#define GPIO_MMIO_LENGTH              0x1000

//
// GPIO CON
//
#define CON_MASK(x)                   (15  << ((x) << 2))
#define CON_SFR(x, y)                 ((y) << ((x) << 2))

//
// GPIO DAT
//
#define DAT_MASK(x)                   (1 << (x))
#define DAT_SET(x)                    (0 << (x))

//
// GPIO Pull
//
#define PULL_MASK(x)                  CON_MASK(x)
#define PULL_MODE(x, y)               CON_SFR(x, y)

//
// GPIO Rate
//
#define DRV_MASK(x)                   CON_MASK(x)
#define DRV_SET(x, y)                 CON_SFR(x, y)

//
// GPIO Bank
//
typedef struct {
  UINT32 con;
  UINT32 dat;
  UINT32 pull;
  UINT32 drv;
  UINT32 pdn_con;
  UINT32 pdn_pull;
} EFI_GPIO_BANK;

#endif /* _GPIO_H_ */
