#include <Library/PmicLib.h>

STATIC
EFI_PMIC_DATA
gPmicData = {
  {
    {
      .Id        = ID_S2MPS19,
      .BusNumber = 0
    }
  }
};

EFI_PMIC_DATA*
GetPmicData ()
{
  return &gPmicData;
}
