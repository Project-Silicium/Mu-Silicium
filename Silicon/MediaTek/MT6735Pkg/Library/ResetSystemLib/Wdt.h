#ifndef _WDT_H_
#define _WDT_H_

//
// TOPRGU (WatchDog) Registers
//
#define WDT_MODE                0x00
#define WDT_RESTART             0x08
#define WDT_SWRST               0x14

//
// WDT_MODE Bits
//
#define WDT_MODE_KEY            0x22000000
#define WDT_MODE_ENABLE         0x00000001
#define WDT_MODE_EXTEN          0x00000004
#define WDT_MODE_IRQ            0x00000008

//
// MTK uses this bit as "Bypass Power Key" flag
// If set, the preloader powers the device back on after WatchDog reset
// Otherwise, device stays off
//
#define WDT_MODE_AUTO_RESTART   0x00000010
#define WDT_MODE_DUAL           0x00000040

//
// Keys
//
#define WDT_RESTART_KEY         0x1971
#define WDT_SWRST_KEY           0x1209

#endif /* _WDT_H_ */
