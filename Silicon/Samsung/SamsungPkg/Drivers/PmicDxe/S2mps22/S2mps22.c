#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Protocol/EFISpeedy.h>

#include "S2mps22.h"

//
// Global Variables
//
STATIC EFI_SPEEDY_PROTOCOL *mSpeedyProtocol;
STATIC UINT8                mBusNumber;

EFI_STATUS
InitS2mps22 (
  IN EFI_SPEEDY_PROTOCOL *SpeedyProtocol,
  IN UINT8                BusNumber)
{
  // Save SPEEDY Details
  mSpeedyProtocol = SpeedyProtocol;
  mBusNumber      = BusNumber;

  DEBUG ((EFI_D_WARN, "%a: Successfull\n", __FUNCTION__));

  return EFI_SUCCESS;
}
