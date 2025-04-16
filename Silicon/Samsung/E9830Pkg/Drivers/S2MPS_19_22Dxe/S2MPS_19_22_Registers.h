/*
 * Copyright@ Samsung Electronics Co. LTD
 *
 * This software is proprietary of Samsung Electronics.
 * No part of this software, either material or conceptual may be copied or distributed, transmitted,
 * transcribed, stored in a retrieval system or translated into any human or computer language in any form by any means,
 * electronic, mechanical, manual or otherwise, or disclosed
 * to third parties without the express written permission of Samsung Electronics.
 */

#ifndef _S2MPS_19_22_REGISTERS_H_
#define _S2MPS_19_22_REGISTERS_H_

/* S2MPS19 slave address */
#define S2MPS19_PM_ADDR		0x1
#define S2MPS19_RTC_ADDR	0x2

/* S2MPS22 slave address */
#define S2MPS22_PM_ADDR		0x1

/* S2MPS19 Register Address */
#define S2MPS19_PM_INT1		0x000
#define S2MPS19_PM_INT2		0x001
#define S2MPS19_PM_INT3		0x002
#define S2MPS19_PM_INT4		0x003
#define S2MPS19_PM_INT5		0x004
#define S2MPS19_PM_PWRONSRC	0x010
#define S2MPS19_PM_OFFSRC	0x011
#define S2MPS19_PM_RTC_BUF	0x013
#define S2MPS19_PM_CTRL1	0x014
#define S2MPS19_PM_CTRL3	0x016
#define S2MPS19_PM_ETC_OTP	0x018
#define S2MPS19_PM_RTC_BUF	0x013

#define S2MPS19_PM_LDO2M_CTRL	0x047
#define S2MPS19_PM_LDO3M_CTRL	0x048
#define S2MPS19_PM_LDO4M_CTRL	0x049
#define S2MPS19_PM_LDO9M_CTRL	0x04E
#define S2MPS19_PM_LDO15M_CTRL	0x054
#define S2MPS19_PM_LDO10M_CTRL	0x04F
#define S2MPS19_PM_LDO11M_CTRL	0x050
#define S2MPS19_PM_LDO12M_CTRL	0x051
#define S2MPS19_PM_LDO16M_CTRL	0x055
#define S2MPS19_PM_LDO17M_CTRL	0x056

#define S2MPS19_RTC_WTSR_SMPL	0x001
#define S2MPU19_RTC_CAP_SEL	0x003

/* S2MPS22 Register Address */
#define S2MPS22_PM_CTRL1	0x013
#define S2MPS22_PM_LDO4S_CTRL	0x02D

/*
 * CTRL1
 */
#define MRSTB_EN		(0x1 << 4)

/*
 * CTRL3
 */
#define WRSTBIEN		(0x1 << 6)
#define MRSEL		(0x1 << 5)
#define WRSTEN			(0x1 << 4)

/*
 * LDOx_CTRL
 */
#define S2MPS_OUTPUT_ON_NORMAL	(0x3 << 6)
#define S2MPS_OUTPUT_ON_TCXO	(0x1 << 6)


#define S2MPS19_RTC_WTSR_SMPL	0x001
#define S2MPS19_RTC_UPDATE	0x002
#define S2MPS19_RTC_CAP_SEL	0x003
#define S2MPS19_RTC_MSEC	0x004
#define S2MPS19_RTC_SEC		0x005
#define S2MPS19_RTC_MIN		0x006
#define S2MPS19_RTC_HOUR	0x007
#define S2MPS19_RTC_WEEK	0x008
#define S2MPS19_RTC_DAY		0x009
#define S2MPS19_RTC_MON		0x00A
#define S2MPS19_RTC_YEAR	0x00B

/* RTC Counter Register offsets */
enum {
  PMIC_RTC_SEC = 0,
  PMIC_RTC_MIN,
  PMIC_RTC_HOUR,
  PMIC_RTC_WEEK,
  PMIC_RTC_DATE,
  PMIC_RTC_MONTH,
  PMIC_RTC_YEAR,
  NR_PMIC_RTC_CNT_REGS,
};

/*
 * RTC_BUF
 */
#define _32KHZAP_EN	(0x1 << 0)
#define _32KHZPERI_EN	(0x1 << 2)

#endif  /* _S2MPS_19_22_REGISTERS_H_ */
