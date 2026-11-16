#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Protocol/EFISpeedy.h>

#include "S2mpu08.h"

//
// Global Variables
//
STATIC EFI_SPEEDY_PROTOCOL *mSpeedyProtocol = NULL;
STATIC UINT8                mBusNumber      = 0;
STATIC UINT8                mPmicRev        = 0xFF;

STATIC
EFI_STATUS
S2mpu08UpdateReg (
  IN UINT8 Reg,
  IN UINT8 Value,
  IN UINT8 Mask)
{
  EFI_STATUS Status;
  UINT8      Current;

  Status = mSpeedyProtocol->Read (mBusNumber, S2MPU08_PM_ADDR, Reg, &Current);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Current = (Current & ~Mask) | (Value & Mask);

  return mSpeedyProtocol->Write (mBusNumber, S2MPU08_PM_ADDR, Reg, Current);
}

STATIC
EFI_STATUS
S2mpu08SetLdo35Rev0 (IN BOOLEAN Enable)
{
  EFI_STATUS Status;
  UINT8      L36Val;

  Status = mSpeedyProtocol->Read (mBusNumber, S2MPU08_PM_ADDR, S2MPU08_PM_L36CTRL, &L36Val);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (Enable) {
    if ((L36Val & S2MPU08_OUTPUT_ON_NORMAL) != 0) {
      Status = S2mpu08UpdateReg (S2MPU08_PM_L35_WA_SEQ, 0x00, 0xF0);
      if (EFI_ERROR (Status)) {
        return Status;
      }

      return S2mpu08UpdateReg (S2MPU08_PM_L35_WA_EXT, 0x00, 0x08);
    }

    Status = S2mpu08UpdateReg (S2MPU08_PM_L35_WA_SEQ, 0xF0, 0xF0);
    if (EFI_ERROR (Status)) {
      return Status;
    }

    return S2mpu08UpdateReg (S2MPU08_PM_L35_WA_EXT, 0x08, 0x08);
  }

  if ((L36Val & S2MPU08_OUTPUT_ON_NORMAL) != 0) {
    Status = S2mpu08UpdateReg (S2MPU08_PM_L35_WA_SEQ, 0xF0, 0xF0);
    if (EFI_ERROR (Status)) {
      return Status;
    }

    return S2mpu08UpdateReg (S2MPU08_PM_L35_WA_EXT, 0x00, 0x08);
  }

  Status = S2mpu08UpdateReg (S2MPU08_PM_L35_WA_SEQ, 0x00, 0xF0);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return S2mpu08UpdateReg (S2MPU08_PM_L35_WA_EXT, 0x00, 0x08);
}

EFI_STATUS
S2mpu08SetLdo (
  IN UINT8             LdoNumber,
  IN EFI_PMIC_LDO_MODE Mode,
  IN BOOLEAN           Enable)
{
  EFI_STATUS Status;
  UINT8      Value;

  // Verify LDO Number
  if (!LdoNumber || LdoNumber > MAX_S2MPU08_LDO_COUNT) {
    return EFI_NOT_FOUND;
  }

  // Verify SPEEDY Protocol
  if (mSpeedyProtocol == NULL) {
    return EFI_NOT_READY;
  }

  // Verify Mode Parameter
  if (Enable && (Mode != S2MPS_MODE_TCXO && Mode != S2MPS_MODE_NORMAL)) {
    return EFI_INVALID_PARAMETER;
  }

  if (mPmicRev == 0x00 && LdoNumber == 35) {
    return S2mpu08SetLdo35Rev0 (Enable);
  }

  // Get LDO Register
  UINT8 LdoRegister = S2MPU08_PM_LDO_CTRL (LdoNumber);

  // Get current LDO Config
  Status = mSpeedyProtocol->Read (mBusNumber, S2MPU08_PM_ADDR, LdoRegister, &Value);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Clear LDO Mode
  Value &= ~S2MPU08_OUTPUT_ON_NORMAL;

  // Set Specified Mode
  if (Enable) {
    if (Mode == S2MPS_MODE_TCXO) {
      Value |= S2MPU08_OUTPUT_ON_TCXO;
    } else {
      Value |= S2MPU08_OUTPUT_ON_NORMAL;
    }
  }

  // Write new LDO Config
  return mSpeedyProtocol->Write (mBusNumber, S2MPU08_PM_ADDR, LdoRegister, Value);
}

EFI_STATUS
S2mpu08SetLdoVoltage (
  IN UINT8  LdoNumber,
  IN UINT32 Microvolts)
{
  EFI_STATUS Status;
  UINT8      Value;
  UINT8      Groups[] = S2MPU08_LDO_GROUPS;
  UINT32     MinUv[]  = S2MPU08_LDO_GROUP_MIN_UV;
  UINT32     StepUv[] = S2MPU08_LDO_GROUP_STEP_UV;

  if (!LdoNumber || LdoNumber > MAX_S2MPU08_LDO_COUNT) {
    return EFI_NOT_FOUND;
  }

  if (mSpeedyProtocol == NULL) {
    return EFI_NOT_READY;
  }

  UINT8  Group = Groups[LdoNumber - 1];
  UINT32 Min   = MinUv[Group - 1];
  UINT32 Step  = StepUv[Group - 1];

  if (Microvolts < Min || Microvolts > Min + (S2MPU08_LDO_VSEL_MASK * Step)) {
    DEBUG ((EFI_D_ERROR, "%a: LDO%u cannot Produce %u uV!\n", __FUNCTION__, LdoNumber, Microvolts));
    return EFI_INVALID_PARAMETER;
  }

  if (((Microvolts - Min) % Step) != 0) {
    DEBUG ((EFI_D_ERROR, "%a: %u uV is not a Whole Step for LDO%u!\n", __FUNCTION__, Microvolts, LdoNumber));
    return EFI_INVALID_PARAMETER;
  }

  UINT8 LdoRegister = S2MPU08_PM_LDO_CTRL (LdoNumber);

  Status = mSpeedyProtocol->Read (mBusNumber, S2MPU08_PM_ADDR, LdoRegister, &Value);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Value &= ~S2MPU08_LDO_VSEL_MASK;
  Value |= (UINT8)((Microvolts - Min) / Step);

  return mSpeedyProtocol->Write (mBusNumber, S2MPU08_PM_ADDR, LdoRegister, Value);
}

EFI_STATUS
S2mpu08GetLdoVoltage (
  IN  UINT8   LdoNumber,
  OUT UINT32 *Microvolts)
{
  EFI_STATUS Status;
  UINT8      Value;
  UINT8      Groups[] = S2MPU08_LDO_GROUPS;
  UINT32     MinUv[]  = S2MPU08_LDO_GROUP_MIN_UV;
  UINT32     StepUv[] = S2MPU08_LDO_GROUP_STEP_UV;

  if (Microvolts == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (!LdoNumber || LdoNumber > MAX_S2MPU08_LDO_COUNT) {
    return EFI_NOT_FOUND;
  }

  if (mSpeedyProtocol == NULL) {
    return EFI_NOT_READY;
  }

  Status = mSpeedyProtocol->Read (mBusNumber, S2MPU08_PM_ADDR, S2MPU08_PM_LDO_CTRL (LdoNumber), &Value);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  UINT8 Group = Groups[LdoNumber - 1];

  *Microvolts = MinUv[Group - 1] + ((Value & S2MPU08_LDO_VSEL_MASK) * StepUv[Group - 1]);

  return EFI_SUCCESS;
}

EFI_STATUS
InitS2mpu08 (
  IN EFI_SPEEDY_PROTOCOL *SpeedyProtocol,
  IN UINT8                BusNumber)
{
  EFI_STATUS Status;

  // Save SPEEDY Details
  mSpeedyProtocol = SpeedyProtocol;
  mBusNumber      = BusNumber;

  Status = SpeedyProtocol->Read (BusNumber, S2MPU08_TOP_ADDR, S2MPU08_TOP_PMICID, &mPmicRev);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_WARN, "%a: Failed to Read S2MPU08 PMIC ID! Status = %r\n", __FUNCTION__, Status));
    mPmicRev = 0xFF;
  }

  return EFI_SUCCESS;
}
