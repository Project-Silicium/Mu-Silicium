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
    },
    {
      .Id        = ID_S2MPB02,
      .BusNumber = 27,
      .Slave     = 0x59
    },
    {
      .Id        = ID_S2MPB03,
      .BusNumber = 38,
      .Slave     = 0x56
    }
  }
};

EFI_PMIC_DATA*
GetPmicData ()
{
  return &gPmicData;
}
