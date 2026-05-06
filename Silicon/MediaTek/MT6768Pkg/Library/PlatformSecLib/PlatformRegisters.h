#ifndef _PLATFORM_REGISTERS_H_
#define _PLATFORM_REGISTERS_H_

//
// Display OVL Registers
//
#define OVL_PITCH_OFFSET(x) (0x44 + 0x20 * (x))

//
// Display OVL Bits
//
#define OVL_CONST_BLEND BIT28

//
// WatchDog Timer Values
//
#define WDT_MODE_KEY 0x22000000

#endif /* _PLATFORM_REGISTERS_H_ */
