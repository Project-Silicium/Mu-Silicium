#ifndef _PMU_H_
#define _PMU_H_

//
// PMU Registers
//
#define PS_HOLD_CONTROL             0x030C
#define RST_STAT	                0x0404
#define SEC_DEBUG_MAGIC_INFORM      0x0808
#define SEC_DEBUG_PANIC_INFORM      0x080C
#define EXYNOS_PMU_SYSIP_DAT0       0x0810
#define SWRESET                     0x3A00

//
// PS_HOLD Register Bits
//
#define PS_HOLD_LOW                 BIT8

//
// SEC Reboot Specifics
//
#define SEC_REBOOT_END_OFFSET       (16)

#define SEC_REBOOT_NORMAL           0x4E

#define SEC_RESET_REASON_PREFIX     0x12345600

#define SEC_RESET_REASON_UNKNOWN    (SEC_RESET_REASON_PREFIX | 0x00)
#define SEC_RESET_REASON_DOWNLOAD   (SEC_RESET_REASON_PREFIX | 0x01)
#define SEC_RESET_REASON_RECOVERY   (SEC_RESET_REASON_PREFIX | 0x04)
#define SEC_RESET_REASON_BOOTLOADER (SEC_RESET_REASON_PREFIX | 0x0D)

//
// Reboot Modes
//
#define REBOOT_MODE_FASTBOOT	    0xFA

#endif /* _PMU_H_ */
