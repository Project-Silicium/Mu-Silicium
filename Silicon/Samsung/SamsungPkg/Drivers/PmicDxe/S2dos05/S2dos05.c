#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Protocol/EFIHsI2c.h>

#include "S2dos05.h"

//
// Global Variables
//
STATIC EFI_HSI2C_PROTOCOL *mHsI2cProtocol = NULL;
STATIC UINT8               mBusNumber     = 0;
STATIC UINT8               mSlaveAddress  = 0;

/**
  Sets or Clears the Passed Regulator Bits in the Enable Register.
**/
STATIC
EFI_STATUS
S2dos05UpdateEnable (
  IN UINT8   Mask,
  IN BOOLEAN Enable)
{
  EFI_STATUS Status;
  UINT8      Value;

  // Verify HSI2C Protocol
  if (mHsI2cProtocol == NULL) {
    return EFI_NOT_READY;
  }

  // Get current Enable Config
  Status = mHsI2cProtocol->Read (mBusNumber, mSlaveAddress, S2DOS05_REG_EN, &Value);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Set Specified Regulator State
  if (Enable) {
    Value |= Mask;
  } else {
    Value &= ~Mask;
  }

  // Write new Enable Config
  return mHsI2cProtocol->Write (mBusNumber, mSlaveAddress, S2DOS05_REG_EN, Value);
}

EFI_STATUS
S2dos05SetLdo (
  IN UINT8             LdoNumber,
  IN EFI_PMIC_LDO_MODE Mode,
  IN BOOLEAN           Enable)
{
  // Verify LDO Number
  if (!LdoNumber || LdoNumber > MAX_S2DOS05_LDO_COUNT) {
    return EFI_NOT_FOUND;
  }

  // S2DOS05 only can enable normal mode.
  if (Enable && Mode != S2MPS_MODE_NORMAL) {
    return EFI_UNSUPPORTED;
  }

  // Set LDO Regulator
  return S2dos05UpdateEnable (S2DOS05_ENABLE_LDO (LdoNumber), Enable);
}

EFI_STATUS
S2dos05SetBuck (
  IN UINT8   BuckNumber,
  IN BOOLEAN Enable)
{
  // Verify BUCK Number
  if (!BuckNumber || BuckNumber > MAX_S2DOS05_BUCK_COUNT) {
    return EFI_NOT_FOUND;
  }

  // Set BUCK Regulator
  return S2dos05UpdateEnable (S2DOS05_ENABLE_BUCK, Enable);
}

EFI_STATUS
InitS2dos05 (
  IN EFI_HSI2C_PROTOCOL *HsI2cProtocol,
  IN UINT8               BusNumber,
  IN UINT32              Slave)
{
  EFI_STATUS Status;
  UINT8      DeviceId;

  // Init HSI2C Bus
  Status = HsI2cProtocol->InitBus (BusNumber);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Init HSI2C Bus %u for S2DOS05! Status = %r\n", BusNumber, Status));
    return Status;
  }

  // Verify the PMIC is present.
  Status = HsI2cProtocol->Read (BusNumber, (UINT8)Slave, S2DOS05_REG_DEVICE_ID_PGM, &DeviceId);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Probe S2DOS05 on Bus %u! Status = %r\n", BusNumber, Status));
    return Status;
  }

  // Save HSI2C Details
  mHsI2cProtocol = HsI2cProtocol;
  mBusNumber     = BusNumber;
  mSlaveAddress  = (UINT8)Slave;

  // The SM3080 is Register Compatible.
  if (DeviceId & S2DOS05_ID_PGM_SM3080) {
    DEBUG ((EFI_D_WARN, "This is an SM3080. ID = 0x%02x\n", DeviceId));
  }

  return EFI_SUCCESS;
}
