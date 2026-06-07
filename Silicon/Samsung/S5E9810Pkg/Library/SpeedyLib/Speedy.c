#include <Library/SpeedyLib.h>

STATIC
EFI_SPEEDY_BUS_DATA
gSpeedyBuses[] = {
  // Bus Number, Bus Address
  {0, 0x141C0000}
};

VOID
GetSpeedyBusData (
  OUT EFI_SPEEDY_BUS_DATA **Data,
  OUT UINT8                *Count)
{
  // Pass Data
  *Data  = gSpeedyBuses;
  *Count = ARRAY_SIZE (gSpeedyBuses);
}
