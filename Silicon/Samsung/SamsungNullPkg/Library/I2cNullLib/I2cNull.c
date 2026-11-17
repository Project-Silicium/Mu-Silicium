#include <Library/I2cLib.h>

VOID
GetI2cBusData (
  OUT EFI_I2C_BUS_DATA **Data,
  OUT UINT8             *Count)
{
  *Data  = NULL;
  *Count = 0;
}
