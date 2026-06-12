#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Protocol/EFISpeedy.h>

#include "S2mps19.h"

//
// Global Variables
//
STATIC EFI_SPEEDY_PROTOCOL *mSpeedyProtocol;
STATIC UINT8                mBusNumber;

EFI_STATUS
S2mps19SetWtsr (IN BOOLEAN Enable)
{
  EFI_STATUS Status;
  UINT8      Value;

  // Get current WTSR Config
  Status = mSpeedyProtocol->Read (mBusNumber, S2MPS19_RTC_ADDR, S2MPS19_RTC_WTSR_SMPL, &Value);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Enable/Disable WTSR
  if (Enable) {
    Value |=  S2MPS19_RTC_WTSR_SET;
  } else {
    Value &= ~S2MPS19_RTC_WTSR_SET;
  }

  // Write new WTSR Config
  Status = mSpeedyProtocol->Write (mBusNumber, S2MPS19_RTC_ADDR, S2MPS19_RTC_WTSR_SMPL, Value);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
S2mps19SetSmpl (IN BOOLEAN Enable)
{
  EFI_STATUS Status;
  UINT8      Value;

  // Get current SMPL Config
  Status = mSpeedyProtocol->Read (mBusNumber, S2MPS19_RTC_ADDR, S2MPS19_RTC_WTSR_SMPL, &Value);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Enable/Disable SMPL
  if (Enable) {
    Value |=  S2MPS19_RTC_SMPL_SET;
  } else {
    Value &= ~S2MPS19_RTC_SMPL_SET;
  }

  // Write new SMPL Config
  Status = mSpeedyProtocol->Write (mBusNumber, S2MPS19_RTC_ADDR, S2MPS19_RTC_WTSR_SMPL, Value);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
EnableUnknown ()
{
  EFI_STATUS Status;
  UINT8      Value;

  // ?
  Status = mSpeedyProtocol->Read (mBusNumber, S2MPS19_RTC_ADDR, 0x1B, &Value);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // ?
  Value |= BIT6;

  // ?
  Status = mSpeedyProtocol->Write (mBusNumber, S2MPS19_RTC_ADDR, 0x1B, Value);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
InitS2mps19 (
  IN EFI_SPEEDY_PROTOCOL *SpeedyProtocol,
  IN UINT8                BusNumber)
{
  EFI_STATUS Status;

  // Save SPEEDY Details
  mSpeedyProtocol = SpeedyProtocol;
  mBusNumber      = BusNumber;

  // Enable ?
  Status = EnableUnknown ();
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Enable Unknown!\n", __FUNCTION__));
    return Status;
  }

  // Enable WTSR
  Status = S2mps19SetWtsr (TRUE);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Enable WTSR!\n", __FUNCTION__));
    return Status;
  }

  // Enable SMPL
  Status = S2mps19SetSmpl (TRUE);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Enable SMPL!\n", __FUNCTION__));
    return Status;
  }

  DEBUG ((EFI_D_WARN, "%a: Successfull\n", __FUNCTION__));

  return EFI_SUCCESS;
}
