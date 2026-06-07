#include <Library/GpioLib.h>

VOID
GetGpioControllerData (
  OUT EFI_GPIO_CONTROLLER_DATA **Data,
  OUT UINT8                     *Count)
{
  // Pass Dummy Data
  *Data  = NULL;
  *Count = 0;
}
