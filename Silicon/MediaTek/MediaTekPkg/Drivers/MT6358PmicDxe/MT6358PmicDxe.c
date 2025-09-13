#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryMapHelperLib.h>
#include <Library/DebugLib.h>
#include <Library/TimerLib.h>
#include <Library/IoLib.h>

#include <Protocol/MtkPmicWrapper.h>
#include <Protocol/MtkPmic.h>

#define MT6358_TOPSTATUS 0x28
#define MT6358_PWRKEY_MASK BIT1
#define MT6358_HOMEKEY_MASK BIT3

STATIC MTK_PMIC_WRAPPER_PROTOCOL *mPmicWrapper = NULL;

VOID
PowerButtonPressed (
  OUT BOOLEAN *Pressed)
{
  UINT16 Value;
  mPmicWrapper->Read (MT6358_TOPSTATUS, &Value);
  Value &= MT6358_PWRKEY_MASK;

  *Pressed = (Value == 0) ? TRUE : FALSE;
}

VOID
HomeButtonPressed (
  OUT BOOLEAN *Pressed)
{
  UINT16 Value;
  mPmicWrapper->Read (MT6358_TOPSTATUS, &Value);
  Value &= MT6358_HOMEKEY_MASK;

  *Pressed = (Value == 0) ? TRUE : FALSE;
}

STATIC MTK_PMIC_PROTOCOL mPmic = {
  PowerButtonPressed,
  HomeButtonPressed,
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

  // Register MT6358 PMIC Protocol
  Status = gBS->InstallMultipleProtocolInterfaces (&ImageHandle, &gMediaTekPmicProtocolGuid, &mPmic, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Register MT6358 PMIC Protocol! Status = %r\n", Status));
    return Status;
  }

  return Status;
}