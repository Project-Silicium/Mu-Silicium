#include <Library/PmicLib.h>

STATIC
EFI_PMIC_DATA
gPmicData = {
  {
    {
      .Id        = ID_S2MPS19,
      .BusNumber = 0
    },
    {
      .Id        = ID_S2MPS22,
      .BusNumber = 1
    }
  },
  {
    {
      .Id        = ID_S2DOS05,
      .BusNumber = 4,
      .Slave     = 0x60
    }
  }
};

EFI_PMIC_DATA*
GetPmicData ()
{
  return &gPmicData;
}
