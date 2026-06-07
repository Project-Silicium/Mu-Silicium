#include <Library/DebugLib.h>
#include <Library/ArmSmcLib.h>
#include <Library/ArmHvcLib.h>

#include "RamManager.h"
#include "MemoryHole.h"

//
// Global Variables
//
STATIC UINTN SmcFunction[2]   = {SMC_CMD_GET_SOC_INFO,            SMC_DRM_GET_SOC_INFO};
STATIC UINTN HvcFunction[2]   = {HVC_GET_HARX_INFO,               HVC_GET_DRM_PLUGIN_INFO};
STATIC UINTN CmdSocInfoArg[2] = {SOC_INFO_TYPE_SEC_DRAM_BASE,     SOC_INFO_TYPE_SEC_DRAM_LENGTH};
STATIC UINTN DrmSocInfoArg[2] = {SOC_INFO_SEC_PGTBL_BASE,         SOC_INFO_SEC_PGTBL_LENGTH};
STATIC UINTN HarxInfoArg[2]   = {HARX_INFO_HARX_BASE,             HARX_INFO_HARX_LENGTH};
STATIC UINTN DrmPluginArg[2]  = {DRM_PLUGIN_INFO_DRM_PLUGIN_BASE, DRM_PLUGIN_INFO_DRM_PLUGIN_LENGTH};

EFI_STATUS
GetSecureDramRange (
  OUT EFI_PHYSICAL_ADDRESS *Address,
  OUT UINT64               *Length)
{
  UINTN HoleAddress;
  UINTN HoleLength;

  // Verify SoC Info Version
  if (ArmCallSmc0 (SmcFunction[0], NULL, NULL, NULL) != 0x66001000) {
    DEBUG ((EFI_D_ERROR, "This SoC Info Version is not Supported!\n"));
    return EFI_UNSUPPORTED;
  }

  // Get Secure DRAM Base
  HoleAddress = ArmCallSmc1 (SmcFunction[0], &CmdSocInfoArg[0], NULL, NULL);
  if (HoleAddress == MAX_UINTN) {
    DEBUG ((EFI_D_ERROR, "Failed to get Secure DRAM Base Address!\n"));
    return EFI_ABORTED;
  }

  // Get Secure DRAM Length
  HoleLength = ArmCallSmc1 (SmcFunction[0], &CmdSocInfoArg[1], NULL, NULL);
  if (HoleLength == MAX_UINTN) {
    DEBUG ((EFI_D_ERROR, "Failed to get Secure DRAM Length!\n"));
    return EFI_ABORTED;
  }

  // Pass Data
  *Address = HoleAddress;
  *Length  = HoleLength;

  return EFI_SUCCESS;
}

EFI_STATUS
VerifyDrmSocInfoOutput (IN UINTN Output)
{
  // Show Error
  switch (Output) {
    case 0:
      DEBUG ((EFI_D_ERROR, "This Type is Unsupported!\n"));
      return EFI_INVALID_PARAMETER;

    case 3:
      return EFI_UNSUPPORTED;

    case MAX_UINTN:
      DEBUG ((EFI_D_ERROR, "The DRM LDFW is not Initialized yet!\n"));
      return EFI_NOT_READY;
  }

  // Verify Alignment
  if ((Output & 0xFFFFF) != 0) {
    DEBUG ((EFI_D_ERROR, "The Output is not 1 MB Aligned!\n"));
    return EFI_ABORTED;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
GetSecurePgtblRange (
  OUT EFI_PHYSICAL_ADDRESS *Address,
  OUT UINT64               *Length)
{
  EFI_STATUS Status;
  UINTN      HoleAddress;
  UINTN      HoleLength;

  // Check Secure PGTBL Presense
  if (!FixedPcdGetBool (PcdHasSecurePgtbl)) {
    return EFI_NOT_FOUND;
  }

  // Get Secure PGTBL Base
  HoleAddress = ArmCallSmc1 (SmcFunction[1], &DrmSocInfoArg[0], NULL, NULL);

  // Verify Secure PGTBL Base
  Status = VerifyDrmSocInfoOutput (HoleAddress);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Get Secure PGTBL Length
  HoleLength = ArmCallSmc1 (SmcFunction[1], &DrmSocInfoArg[1], NULL, NULL);

  // Verify Secure PGTBL Length
  Status = VerifyDrmSocInfoOutput (HoleLength);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Pass Data
  *Address = HoleAddress;
  *Length  = HoleLength;

  return EFI_SUCCESS;
}

UINTN
SendHypervisorCall (
  IN UINTN Function,
  IN UINTN Arg)
{
  ARM_HVC_ARGS Args;

  // Set Hypervisor Call Arguments
  Args.Arg0 = Function;
  Args.Arg1 = Arg;
  Args.Arg2 = 0;
  Args.Arg3 = 0;
  Args.Arg4 = 0;

  // Send Hypervisor Call
  ArmCallHvc (&Args);

  // Return Hypervisor Output
  return Args.Arg0;
}

EFI_STATUS
GetHarxRange (
  OUT EFI_PHYSICAL_ADDRESS *Address,
  OUT UINT64               *Length)
{
  UINTN HoleAddress;
  UINTN HoleLength;

  // Check H-Arx Presense
  if (!FixedPcdGetBool (PcdHasHarx)) {
    return EFI_NOT_FOUND;
  }

  // Get H-Arx Info Version
  if (SendHypervisorCall (HvcFunction[0], 0) != 0xE1200100) {
    DEBUG ((EFI_D_ERROR, "This H-Arx Info Version is not Supported!\n"));
    return EFI_UNSUPPORTED;
  }

  // Get H-Arx Base Address
  HoleAddress = SendHypervisorCall (HvcFunction[0], HarxInfoArg[0]);
  if (HoleAddress == MAX_UINTN) {
    DEBUG ((EFI_D_ERROR, "Failed to get H-Arx Base Address!\n"));
    return EFI_ABORTED;
  }

  // Get H-Arx Length
  HoleLength = SendHypervisorCall (HvcFunction[0], HarxInfoArg[1]);
  if (HoleLength == MAX_UINTN) {
    DEBUG ((EFI_D_ERROR, "Failed to get H-Arx Length!\n"));
    return EFI_ABORTED;
  }

  // Pass Data
  *Address = HoleAddress;
  *Length  = HoleLength;

  return EFI_SUCCESS;
}

EFI_STATUS
GetDrmPluginRange (
  OUT EFI_PHYSICAL_ADDRESS *Address,
  OUT UINT64               *Length)
{
  UINTN HoleAddress;
  UINTN HoleLength;

  // Check DRM Plugin Presense
  if (!FixedPcdGetBool (PcdHasDrmPlugin)) {
    return EFI_NOT_FOUND;
  }

  // Get DRM Plugin Base Address
  HoleAddress = SendHypervisorCall (HvcFunction[1], DrmPluginArg[0]);
  if (HoleAddress == 0 || HoleAddress == 0xB113 || HoleAddress == 0xC) {
    DEBUG ((EFI_D_ERROR, "Failed to get DRM Plugin Base Address!\n"));
    return EFI_ABORTED;
  }

  // Get DRM Plugin Length
  HoleLength = SendHypervisorCall (HvcFunction[1], DrmPluginArg[1]);
  if (HoleLength == 0 || HoleLength == 0xB113 || HoleLength == 0xC) {
    DEBUG ((EFI_D_ERROR, "Failed to get DRM Plugin Length!\n"));
    return EFI_ABORTED;
  }

  // Pass Data
  *Address = HoleAddress;
  *Length  = HoleLength;

  return EFI_SUCCESS;
}
