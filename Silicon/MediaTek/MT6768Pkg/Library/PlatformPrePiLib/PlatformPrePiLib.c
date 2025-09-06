#include <Library/PlatformPrePiLib.h>
#include <Library/MemoryMapHelperLib.h>
#include <Library/IoLib.h>

#define OVL_BASE 0x1400b000
#define OVL_PITCH_OFFSET(x) (0x44 + 0x20 * (x))
#define OVL_CONST_BLEND BIT28

#define WDT_BASE 0x10007000
#define WDT_MODE_KEY 0x22000000

VOID
PlatformInitialize () {
    // Disable watchdog
    MmioWrite32(WDT_BASE, WDT_MODE_KEY);

    // Enable constant blending
    UINT32 val = MmioRead32(OVL_BASE + OVL_PITCH_OFFSET(0));
    val |= OVL_CONST_BLEND;
    MmioWrite32(OVL_BASE + OVL_PITCH_OFFSET(0), val);
}