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

#define CON_MASK(x)      (0xF << ((x) << 2))
#define CON_SFR(x, v)    ((v) << ((x) << 2))

#define DAT_MASK(x)      (0x1 << (x))
#define DAT_SET(x)       (0x1 << (x))

#define PULL_MASK(x)     (0xF << ((x) << 2))
#define PULL_MODE(x, v)  ((v) << ((x) << 2))

#define DRV_MASK(x)      (0xF << ((x) << 2))
#define DRV_SET(x, m)    ((m) << ((x) << 2))

#define RATE_MASK(x)     (0x1 << (x + 16))
#define RATE_SET(x)      (0x1 << (x + 16))

//
// GPIO Bank
//
typedef struct {
  UINT32 Con;
  UINT32 Dat;
  UINT32 Pull;
  UINT32 Drv;
  UINT32 PdnCon;
  UINT32 PdnPull;
  UINT8  Reserved[8];
} EFI_GPIO_BANK;

#endif /* _GPIO_H_ */
