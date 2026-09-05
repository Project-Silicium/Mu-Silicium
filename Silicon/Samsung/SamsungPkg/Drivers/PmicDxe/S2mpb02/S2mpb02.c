#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Protocol/EFIHsI2c.h>

#include "S2mpb02.h"

//
// Global Variables
//
STATIC EFI_HSI2C_PROTOCOL *mHsI2cProtocol = NULL;
STATIC UINT8               mBusNumber     = 0;
STATIC UINT8               mSlaveAddress  = 0;

/**
  Sets or Clears the Passed Enable Bits in the Passed Control Register.
**/
STATIC
EFI_STATUS
S2mpb02UpdateEnable (
  IN UINT8   Register,
  IN UINT8   Mask,
  IN BOOLEAN Enable)
{
  EFI_STATUS Status;
  UINT8      Value;

  // Verify HSI2C Protocol
  if (mHsI2cProtocol == NULL) {
    return EFI_NOT_READY;
  }

  // Get current Control Config
  Status = mHsI2cProtocol->Read (mBusNumber, mSlaveAddress, Register, &Value);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Set Specified Regulator State
  if (Enable) {
    Value |= Mask;
  } else {
    Value &= ~Mask;
  }

  // Write new Control Config
  return mHsI2cProtocol->Write (mBusNumber, mSlaveAddress, Register, Value);
}

EFI_STATUS
S2mpb02SetLdo (
  IN UINT8             LdoNumber,
  IN EFI_PMIC_LDO_MODE Mode,
  IN BOOLEAN           Enable)
{
  // Verify LDO Number
  if (!LdoNumber || LdoNumber > MAX_S2MPB02_LDO_COUNT) {
    return EFI_NOT_FOUND;
  }

  // S2MBP02 only can enable normal mode.
  if (Enable && Mode != S2MPS_MODE_NORMAL) {
    return EFI_UNSUPPORTED;
  }

  // Set LDO Regulator
  return S2mpb02UpdateEnable (S2MPB02_REG_LDO_CTRL (LdoNumber), S2MPB02_LDO_ENABLE, Enable);
}

EFI_STATUS
S2mpb02SetBuck (
  IN UINT8   BuckNumber,
  IN BOOLEAN Enable)
{
  UINT8 Register;

  // Get BUCK Control Register
  switch (BuckNumber) {
    case 1:
      Register = S2MPB02_REG_B1CTRL1;
      break;

    case 2:
      Register = S2MPB02_REG_B2CTRL1;
      break;

    case S2MPB02_BUCK_BOOST:
      Register = S2MPB02_REG_BB1CTRL1;
      break;

    default:
      return EFI_NOT_FOUND;
  }

  // Set BUCK Regulator
  return S2mpb02UpdateEnable (Register, S2MPB02_BUCK_ENABLE, Enable);
}

EFI_STATUS
InitS2mpb02 (
  IN EFI_HSI2C_PROTOCOL *HsI2cProtocol,
  IN UINT8               BusNumber,
  IN UINT32              Slave)
{
  EFI_STATUS Status;
  UINT8      Revision;

  // Init HSI2C Bus
  Status = HsI2cProtocol->InitBus (BusNumber);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Init HSI2C Bus %u for S2MPB02! Status = %r\n", BusNumber, Status));
    return Status;
  }

  // Verify the PMIC is present.
  Status = HsI2cProtocol->Read (BusNumber, (UINT8)Slave, S2MPB02_REG_BST_CTRL2, &Revision);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Probe S2MPB02 on Bus %u! Status = %r\n", BusNumber, Status));
    return Status;
  }

  // Save HSI2C Details
  mHsI2cProtocol = HsI2cProtocol;
  mBusNumber     = BusNumber;
  mSlaveAddress  = (UINT8)Slave;

  return EFI_SUCCESS;
}
