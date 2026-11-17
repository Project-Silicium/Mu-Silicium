#ifndef _PLATFORM_REGISTERS_H_
#define _PLATFORM_REGISTERS_H_

//
// Display OVL Registers
//
#define OVL_CON_OFFSET(x)             (0x30 + 0x20 * (x))

//
// Display OVL Bits
//
#define OVL_ALPHA_ENABLE              BIT8
#define OVL_CONST_ALPHA_MASK          0xFF
#define OVL_CONST_ALPHA_OPAQUE        0xFF

//
// WatchDog Timer Values
//
#define WDT_MODE_KEY                  0x22000000

//
// CPUXGPT Registers
//
#define CPUXGPT_CON_OFFSET            0x670
#define CPUXGPT_IDX_OFFSET            0x674
#define CPUXGPT_IDX_GLOBAL_CTRL       0x00

//
// CPUXGPT Bits
//
#define CPUXGPT_ENABLE                BIT0
#define CPUXGPT_CLK_DIV_MASK          (BIT8 | BIT9 | BIT10)
#define CPUXGPT_CLK_DIV2              BIT9

#endif /* _PLATFORM_REGISTERS_H_ */
