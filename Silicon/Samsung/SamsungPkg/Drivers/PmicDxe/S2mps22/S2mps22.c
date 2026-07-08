#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Protocol/EFISpeedy.h>

#include "S2mps22.h"

//
// Global Variables
//
STATIC EFI_SPEEDY_PROTOCOL *mSpeedyProtocol = NULL;
STATIC UINT8                mBusNumber      = 0;

EFI_STATUS
S2mps22SetLdo (
  IN UINT8   LdoNumber,
  IN UINT8   Mode,
  IN BOOLEAN Enable)
{
  EFI_STATUS Status;
  UINT8      Value;

  // Verify LDO Number
  if (!LdoNumber || LdoNumber > MAX_S2MPS22_LDO_COUNT) {
    return EFI_NOT_FOUND;
  }

  // Verify Mode Parameter
  if (Enable && (Mode != S2MPS_MODE_TCXO && Mode != S2MPS_MODE_NORMAL)) {
    return EFI_INVALID_PARAMETER;
  }

  // Get LDO Register
  UINT8 LdoRegister = S2MPS22_PM_LDOS_CTRL (LdoNumber);

  // Get current LDO Config
  Status = mSpeedyProtocol->Read (mBusNumber, S2MPS22_PM_ADDR, LdoRegister, &Value);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Clear LDO Mode
  Value &= ~S2MPS22_OUTPUT_ON_NORMAL;

  // Set Specified Mode
  if (Enable) {
    if (Mode == S2MPS_MODE_TCXO) {
      Value |= S2MPS22_OUTPUT_ON_TCXO;
    } else {
      Value |= S2MPS22_OUTPUT_ON_NORMAL;
    }
  }

  // Write new LDO Config
  Status = mSpeedyProtocol->Write (mBusNumber, S2MPS22_PM_ADDR, LdoRegister, Value);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
S2mps22PowerDown ()
{
  EFI_STATUS Status;
  UINT8      Value;

  // Verify SPEEDY Protocol
  if (mSpeedyProtocol == NULL) {
    return EFI_NOT_READY;
  }

  // Get current CTRL1 Config
  Status = mSpeedyProtocol->Read (mBusNumber, S2MPS22_PM_ADDR, S2MPS22_PM_CTRL1, &Value);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // ?
  Value |= BIT7;

  // Write new CTRL1 Config
  Status = mSpeedyProtocol->Write (mBusNumber, S2MPS22_PM_ADDR, S2MPS22_PM_CTRL1, Value);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
InitS2mps22 (
  IN EFI_SPEEDY_PROTOCOL *SpeedyProtocol,
  IN UINT8                BusNumber)
{
  // Save SPEEDY Details
  mSpeedyProtocol = SpeedyProtocol;
  mBusNumber      = BusNumber;

  return EFI_SUCCESS;
}
