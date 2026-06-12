/**
  Copyright@ Samsung Electronics Co. LTD

  This software is proprietary of Samsung Electronics.
  No part of this software, either material or conceptual may be copied or distributed, transmitted,
  transcribed, stored in a retrieval system or translated into any human or computer language in any form by any means,
  electronic, mechanical, manual or otherwise, or disclosed
  to third parties without the express written permission of Samsung Electronics.
**/

#ifndef _S2MPS_19_H_
#define _S2MPS_19_H_

//
// Slave Addresses
//
#define S2MPS19_PM_ADDR          0x01
#define S2MPS19_RTC_ADDR         0x02

//
// PM Register Addresses
//
#define S2MPS19_PM_INT1          0x00
#define S2MPS19_PM_INT2          0x01
#define S2MPS19_PM_INT3          0x02
#define S2MPS19_PM_INT4          0x03
#define S2MPS19_PM_INT5          0x04
#define S2MPS19_PM_PWRONSRC      0x10
#define S2MPS19_PM_OFFSRC        0x11
#define S2MPS19_PM_RTC_BUF       0x13
#define S2MPS19_PM_CTRL1         0x14
#define S2MPS19_PM_CTRL3         0x16
#define S2MPS19_PM_ETC_OTP       0x18
#define S2MPS19_PM_LDO2M_CTRL    0x47
#define S2MPS19_PM_LDO3M_CTRL    0x48
#define S2MPS19_PM_LDO4M_CTRL    0x49
#define S2MPS19_PM_LDO9M_CTRL    0x4E
#define S2MPS19_PM_LDO15M_CTRL   0x54
#define S2MPS19_PM_LDO10M_CTRL   0x4F
#define S2MPS19_PM_LDO11M_CTRL   0x50
#define S2MPS19_PM_LDO12M_CTRL   0x51
#define S2MPS19_PM_LDO16M_CTRL   0x55
#define S2MPS19_PM_LDO17M_CTRL   0x56

//
// RTC Register Addresses
//
#define S2MPS19_RTC_WTSR_SMPL    0x01
#define S2MPS19_RTC_UPDATE       0x02
#define S2MPS19_RTC_CAP_SEL      0x03
#define S2MPS19_RTC_MSEC         0x04
#define S2MPS19_RTC_SEC          0x05
#define S2MPS19_RTC_MIN          0x06
#define S2MPS19_RTC_HOUR         0x07
#define S2MPS19_RTC_WEEK         0x08
#define S2MPS19_RTC_DAY          0x09
#define S2MPS19_RTC_MON          0x0A
#define S2MPS19_RTC_YEAR         0x0B

//
// WTSR & SMPL Register Bits
//
#define S2MPS19_RTC_WTSR_SET     BIT6
#define S2MPS19_RTC_SMPL_SET    (BIT7 | BIT8)

#endif /* _S2MPS_19_H_ */
