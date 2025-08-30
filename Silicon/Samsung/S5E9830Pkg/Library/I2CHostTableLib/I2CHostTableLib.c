#include "PlatformI2CHostTableLib.h"

#include <Protocol/EfiI2C.h>

I2CHost *
GetI2CHosts (VOID)
{
  return S5E9830_I2CHost_Table;
}

EFI_STATUS
GetI2CHostById (
  INTN TargetId,
  OUT I2CHost Host)
{
    INTN NumHosts = sizeof(S5E9830_I2CHost_Table) / sizeof(S5E9830_I2CHost_Table[0]);

    if (TargetId == -1)
      return EFI_NOT_FOUND;

    for (INTN i = 0; i < NumHosts; i++)
    {
      if (S5E9830_I2CHost_Table[i].Id == TargetId)
        Host = S5E9830_I2CHost_Table[i];
    }

    return EFI_NOT_FOUND;
}
