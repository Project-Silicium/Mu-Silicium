#include <Library/SpeedyLib.h>

STATIC
EFI_PHYSICAL_ADDRESS
gSpeedyBuses[] = {
  0x15970000,
  0x15980000
};

VOID
GetSpeedyBuses (
  OUT EFI_PHYSICAL_ADDRESS **Bus,
  OUT UINT8                 *Count)
{
  // Pass Data
  *Bus   = gSpeedyBuses;
  *Count = ARRAY_SIZE (gSpeedyBuses);
}
