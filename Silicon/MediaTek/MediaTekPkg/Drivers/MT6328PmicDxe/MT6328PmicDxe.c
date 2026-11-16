#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Protocol/MtkPmicWrapper.h>
#include <Protocol/MtkPmic.h>

#define MT6328_TOPSTATUS     0x0220
#define MT6328_PWRKEY_MASK   BIT1
#define MT6328_HOMEKEY_MASK  BIT2

STATIC MTK_PMIC_WRAPPER_PROTOCOL *mPmicWrapper = NULL;

VOID
PowerButtonPressed (
  OUT BOOLEAN *Pressed)
{
  UINT16 Value;

  // Read status of the power button from TopStatus register
  mPmicWrapper->Read (MT6328_TOPSTATUS, &Value);
  Value &= MT6328_PWRKEY_MASK;

  *Pressed = (Value == 0) ? TRUE : FALSE;
}

VOID
HomeButtonPressed (
  OUT BOOLEAN *Pressed)
{
  UINT16 Value;

  // Read status of the home button from TopStatus register
  mPmicWrapper->Read (MT6328_TOPSTATUS, &Value);
  Value &= MT6328_HOMEKEY_MASK;

  *Pressed = (Value == 0) ? TRUE : FALSE;
}

STATIC
EFI_STATUS
EFIAPI
RegulatorSetEnable (
  IN CONST CHAR8 *Name,
  IN BOOLEAN      Enable)
{
  return EFI_UNSUPPORTED;
}

STATIC
EFI_STATUS
EFIAPI
RegulatorIsEnabled (
  IN  CONST CHAR8 *Name,
  OUT BOOLEAN     *Enabled)
{
  return EFI_UNSUPPORTED;
}

STATIC
EFI_STATUS
EFIAPI
RegulatorSetVoltage (
  IN CONST CHAR8 *Name,
  IN UINT32       Voltage)
{
  return EFI_UNSUPPORTED;
}

STATIC
EFI_STATUS
EFIAPI
RegulatorGetVoltage (
  IN  CONST CHAR8 *Name,
  OUT UINT32      *Voltage)
{
  return EFI_UNSUPPORTED;
}

STATIC MTK_PMIC_PROTOCOL mPmic = {
  PowerButtonPressed,
  HomeButtonPressed,
  RegulatorSetEnable,
  RegulatorIsEnabled,
  RegulatorSetVoltage,
  RegulatorGetVoltage
};

EFI_STATUS
EFIAPI
InitPmic (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status;

  // Locate PMIC Wrapper Protocol
  Status = gBS->LocateProtocol (&gMediaTekPmicWrapperProtocolGuid, NULL, (VOID **)&mPmicWrapper);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate PMIC Wrapper Protocol! Status = %r\n", Status));
    return Status;
  }

  // Register MT6328 PMIC Protocol
  Status = gBS->InstallMultipleProtocolInterfaces (&ImageHandle, &gMediaTekPmicProtocolGuid, &mPmic, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Register MT6328 PMIC Protocol! Status = %r\n", Status));
    return Status;
  }

  return Status;
}