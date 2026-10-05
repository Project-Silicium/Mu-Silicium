#include <Library/GpioBankLib.h>

VOID
GetGpioBanks (
  OUT EFI_GPIO_BANK **Bank,
  OUT UINT8          *Count)
{
  // Pass Empty Data
  *Bank  = NULL;
  *Count = 0;
}
