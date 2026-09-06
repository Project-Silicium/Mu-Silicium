#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Protocol/EFIHsI2c.h>

#include "S2mpb03.h"

//
// Global Variables
//
STATIC EFI_HSI2C_PROTOCOL *mHsI2cProtocol = NULL;
STATIC UINT8               mBusNumber     = 0;
STATIC UINT8               mSlaveAddress  = 0;

EFI_STATUS
S2mpb03SetLdo (
  IN UINT8             LdoNumber,
  IN EFI_PMIC_LDO_MODE Mode,
  IN BOOLEAN           Enable)
{
  EFI_STATUS Status;
  UINT8      Value;

  // Verify HSI2C Protocol
  if (mHsI2cProtocol == NULL) {
    return EFI_NOT_READY;
  }

  // Verify LDO Number
  if (!LdoNumber || LdoNumber > MAX_S2MPB03_LDO_COUNT) {
    return EFI_NOT_FOUND;
  }

  // S2MBP03 only can enable normal mode.
  if (Enable && Mode != S2MPS_MODE_NORMAL) {
    return EFI_UNSUPPORTED;
  }

  // Get LDO Register
  UINT8 LdoRegister = S2MPB03_REG_LDO_CTRL (LdoNumber);

  // Get current LDO Config
  Status = mHsI2cProtocol->Read (mBusNumber, mSlaveAddress, LdoRegister, &Value);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Set Specified LDO State
  if (Enable) {
    Value |= S2MPB03_LDO_ENABLE;
  } else {
    Value &= ~S2MPB03_LDO_ENABLE;
  }

  // Write new LDO Config
  return mHsI2cProtocol->Write (mBusNumber, mSlaveAddress, LdoRegister, Value);
}

EFI_STATUS
InitS2mpb03 (
  IN EFI_HSI2C_PROTOCOL *HsI2cProtocol,
  IN UINT8               BusNumber,
  IN UINT32              Slave)
{
  EFI_STATUS Status;
  UINT8      PmicId;

  // Init HSI2C Bus
  Status = HsI2cProtocol->InitBus (BusNumber);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Init HSI2C Bus %u for S2MPB03! Status = %r\n", BusNumber, Status));
    return Status;
  }

  // Verify the PMIC is present.
  Status = HsI2cProtocol->Read (BusNumber, (UINT8)Slave, S2MPB03_REG_PMIC_ID, &PmicId);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Probe S2MPB03 on Bus %u! Status = %r\n", BusNumber, Status));
    return Status;
  }

  // Save HSI2C Details
  mHsI2cProtocol = HsI2cProtocol;
  mBusNumber     = BusNumber;
  mSlaveAddress  = (UINT8)Slave;

  DEBUG ((EFI_D_WARN, "S2MPB03 PMIC ID = 0x%02x\n", PmicId));

  return EFI_SUCCESS;
}
