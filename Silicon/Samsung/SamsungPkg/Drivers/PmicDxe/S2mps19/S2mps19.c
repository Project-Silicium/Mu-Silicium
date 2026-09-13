#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Protocol/EFISpeedy.h>

#include "S2mps19.h"

//
// Global Variables
//
STATIC EFI_SPEEDY_PROTOCOL *mSpeedyProtocol = NULL;
STATIC UINT8                mBusNumber      = 0;

EFI_STATUS
S2mps19SetLdo (
  IN UINT8             LdoNumber,
  IN EFI_PMIC_LDO_MODE Mode,
  IN BOOLEAN           Enable)
{
  EFI_STATUS Status;
  UINT8      Value;

  // Verify LDO Number
  if (!LdoNumber || LdoNumber > MAX_S2MPS19_LDO_COUNT) {
    return EFI_NOT_FOUND;
  }

  // Verify Mode Parameter
  if (Enable && (Mode != S2MPS_MODE_TCXO && Mode != S2MPS_MODE_NORMAL)) {
    return EFI_INVALID_PARAMETER;
  }

  // Get LDO Register
  UINT8 LdoRegister = S2MPS19_PM_LDOM_CTRL (LdoNumber);

  // Get current LDO Config
  Status = mSpeedyProtocol->Read (mBusNumber, S2MPS19_PM_ADDR, LdoRegister, &Value);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Clear LDO Mode
  Value &= ~S2MPS19_OUTPUT_ON_NORMAL;

  // Set Specified Mode
  if (Enable) {
    if (Mode == S2MPS_MODE_TCXO) {
      Value |= S2MPS19_OUTPUT_ON_TCXO;
    } else {
      Value |= S2MPS19_OUTPUT_ON_NORMAL;
    }
  }

  // Write new LDO Config
  Status = mSpeedyProtocol->Write (mBusNumber, S2MPS19_PM_ADDR, LdoRegister, Value);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
S2mps19SetLdoVoltage (
  IN UINT8  LdoNumber,
  IN UINT32 Microvolts)
{
  EFI_STATUS Status;
  UINT8      Value;
  UINT8      Groups[]  = S2MPS19_LDO_GROUPS;
  UINT32     MinUv[]   = S2MPS19_LDO_GROUP_MIN_UV;
  UINT32     StepUv[]  = S2MPS19_LDO_GROUP_STEP_UV;

  // Verify LDO Number
  if (!LdoNumber || LdoNumber > MAX_S2MPS19_LDO_COUNT) {
    return EFI_NOT_FOUND;
  }

  // Verify SPEEDY Protocol
  if (mSpeedyProtocol == NULL) {
    return EFI_NOT_READY;
  }

  UINT8  Group = Groups[LdoNumber - 1];
  UINT32 Min   = MinUv[Group - 1];
  UINT32 Step  = StepUv[Group - 1];

  //
  // Only whole steps inside the range the group can produce are accepted. A
  // rounded voltage on a rail feeding a card is not worth the convenience.
  //
  if (Microvolts < Min || Microvolts > Min + (S2MPS19_LDO_VSEL_MASK * Step)) {
    DEBUG ((EFI_D_ERROR, "%a: LDO%u cannot Produce %u uV!\n", __FUNCTION__, LdoNumber, Microvolts));
    return EFI_INVALID_PARAMETER;
  }

  if (((Microvolts - Min) % Step) != 0) {
    DEBUG ((EFI_D_ERROR, "%a: %u uV is not a Whole Step for LDO%u!\n", __FUNCTION__, Microvolts, LdoNumber));
    return EFI_INVALID_PARAMETER;
  }

  // Get LDO Register
  UINT8 LdoRegister = S2MPS19_PM_LDOM_CTRL (LdoNumber);

  // Get current LDO Config
  Status = mSpeedyProtocol->Read (mBusNumber, S2MPS19_PM_ADDR, LdoRegister, &Value);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Replace the Voltage Selection, leaving the Enable State alone
  Value &= ~S2MPS19_LDO_VSEL_MASK;
  Value |= (UINT8)((Microvolts - Min) / Step);

  return mSpeedyProtocol->Write (mBusNumber, S2MPS19_PM_ADDR, LdoRegister, Value);
}

EFI_STATUS
S2mps19GetLdoVoltage (
  IN  UINT8   LdoNumber,
  OUT UINT32 *Microvolts)
{
  EFI_STATUS Status;
  UINT8      Value;
  UINT8      Groups[] = S2MPS19_LDO_GROUPS;
  UINT32     MinUv[]  = S2MPS19_LDO_GROUP_MIN_UV;
  UINT32     StepUv[] = S2MPS19_LDO_GROUP_STEP_UV;

  if (Microvolts == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (!LdoNumber || LdoNumber > MAX_S2MPS19_LDO_COUNT) {
    return EFI_NOT_FOUND;
  }

  if (mSpeedyProtocol == NULL) {
    return EFI_NOT_READY;
  }

  Status = mSpeedyProtocol->Read (mBusNumber, S2MPS19_PM_ADDR, S2MPS19_PM_LDOM_CTRL (LdoNumber), &Value);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  UINT8 Group = Groups[LdoNumber - 1];

  DEBUG ((EFI_D_ERROR,
          "%a: LDO%u CTRL = 0x%02X (Enable %u, VSEL 0x%02X, Group %u)\n",
          __FUNCTION__, LdoNumber, Value,
          (Value >> 6) & 0x3, Value & S2MPS19_LDO_VSEL_MASK, Group));

  *Microvolts = MinUv[Group - 1] + ((Value & S2MPS19_LDO_VSEL_MASK) * StepUv[Group - 1]);

  return EFI_SUCCESS;
}

EFI_STATUS
S2mps19SetWtsr (IN BOOLEAN Enable)
{
  EFI_STATUS Status;
  UINT8      Value;

  // Verify SPEEDY Protocol
  if (mSpeedyProtocol == NULL) {
    return EFI_NOT_READY;
  }

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

  // Verify SPEEDY Protocol
  if (mSpeedyProtocol == NULL) {
    return EFI_NOT_READY;
  }

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

  return EFI_SUCCESS;
}
