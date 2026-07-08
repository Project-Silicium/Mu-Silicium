#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryMapHelperLib.h>
#include <Library/IoLib.h>

#include <Protocol/EFISpeedy.h>

#include "S2mps18.h"

//
// Global Variables
//
STATIC EFI_SPEEDY_PROTOCOL *mSpeedyProtocol = NULL;
STATIC UINT8                mBusNumber      = 0;

EFI_STATUS
S2mps18SetLdo (
  IN UINT8             LdoNumber,
  IN EFI_PMIC_LDO_MODE Mode,
  IN BOOLEAN           Enable)
{
  EFI_STATUS Status;
  UINT8      Value;

  // Verify LDO Number
  if (!LdoNumber || LdoNumber > MAX_S2MPS18_LDO_COUNT) {
    return EFI_NOT_FOUND;
  }

  // Verify Mode Parameter
  if (Enable && (Mode != S2MPS_MODE_TCXO && Mode != S2MPS_MODE_NORMAL)) {
    return EFI_INVALID_PARAMETER;
  }

  // Get LDO Register
  UINT8 LdoRegister = S2MPS18_PM_LDO_CTRL (LdoNumber);

  // Get current LDO Config
  Status = mSpeedyProtocol->Read (mBusNumber, S2MPS18_PM_ADDR, LdoRegister, &Value);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Clear LDO Mode
  Value &= ~S2MPS18_OUTPUT_ON_NORMAL;

  // Set Specified Mode
  if (Enable) {
    if (Mode == S2MPS_MODE_TCXO) {
      Value |= S2MPS18_OUTPUT_ON_TCXO;
    } else {
      Value |= S2MPS18_OUTPUT_ON_NORMAL;
    }
  }

  // Write new LDO Config
  Status = mSpeedyProtocol->Write (mBusNumber, S2MPS18_PM_ADDR, LdoRegister, Value);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
S2mps18SetWtsr (IN BOOLEAN Enable)
{
  EFI_STATUS Status;
  UINT8      Value;

  // Verify SPEEDY Protocol
  if (mSpeedyProtocol == NULL) {
    return EFI_NOT_READY;
  }

  // Get current WTSR Config
  Status = mSpeedyProtocol->Read (mBusNumber, S2MPS18_RTC_ADDR, S2MPS18_RTC_WTSR_SMPL, &Value);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Enable/Disable WTSR
  if (Enable) {
    Value |=  S2MPS18_RTC_WTSR_SET;
  } else {
    Value &= ~S2MPS18_RTC_WTSR_SET;
  }

  // Write new WTSR Config
  Status = mSpeedyProtocol->Write (mBusNumber, S2MPS18_RTC_ADDR, S2MPS18_RTC_WTSR_SMPL, Value);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
S2mps18SetSmpl (IN BOOLEAN Enable)
{
  // TODO!
  return EFI_ABORTED;
}

EFI_STATUS
EnableUnknown1 ()
{
  EFI_STATUS Status;
  UINT8      Value;

  // ?
  Status = mSpeedyProtocol->Read (mBusNumber, S2MPS18_RTC_ADDR, 0x1B, &Value);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // ?
  Value |= BIT6;

  // ?
  Status = mSpeedyProtocol->Write (mBusNumber, S2MPS18_RTC_ADDR, 0x1B, Value);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
EnableUnknown2 ()
{
  EFI_STATUS Status;
  UINT8      Value;

  // Get current INT2M Config
  Status = mSpeedyProtocol->Read (mBusNumber, S2MPS18_PM_ADDR, S2MPS18_PM_INT2M, &Value);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // ?
  Value |= (BIT1 | BIT2);

  // Write new INT2M Config
  Status = mSpeedyProtocol->Write (mBusNumber, S2MPS18_PM_ADDR, S2MPS18_PM_INT2M, Value);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
InitS2mps18 (
  IN EFI_SPEEDY_PROTOCOL *SpeedyProtocol,
  IN UINT8                BusNumber)
{
  EFI_STATUS                   Status;
  EFI_MEMORY_REGION_DESCRIPTOR PmuRegion;
  UINT8                        PmicId;

  // Save SPEEDY Details
  mSpeedyProtocol = SpeedyProtocol;
  mBusNumber      = BusNumber;

  // Locate "PMU" Memory Region
  Status = LocateMemoryRegionByName ("PMU", &PmuRegion);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Locate \"PMU\" Memory Region!\n", __FUNCTION__));
    return Status;
  }

  // Get PMIC ID
  Status = SpeedyProtocol->Read (BusNumber, 0, 0, &PmicId);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to get PMIC ID!\n", __FUNCTION__));
    return Status;
  }

  // Get PMIC SW Version
  UINT8 SwVersion = PmicId >> 3 & 0xF;

  // Enable ?
  Status = EnableUnknown1 ();
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Enable Unknown1!\n", __FUNCTION__));
    return Status;
  }

  // Skip OCP & Slew Rate Patch (Offset is SoC Specific?)
  if (SwVersion > 1) {
    MmioWrite32 (PmuRegion.Address + 0x880, 0xBADABCD);
  }

  // Enable ?
  Status = EnableUnknown2 ();
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Enable Unknown2!\n", __FUNCTION__));
    return Status;
  }

  // Disable WTSR
  Status = S2mps18SetWtsr (FALSE);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Disable WTSR!\n", __FUNCTION__));
    return Status;
  }

  return EFI_SUCCESS;
}
