#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/ResetSystemLib.h>
#include <Library/TimerLib.h>

#include <Protocol/EfiGpio.h>
#include <Protocol/EfiPMIC.h>

#include "SamsungPMURegisters.h"

// Cached copy of the GPIO protocol instance
EFI_GPIO_PROTOCOL *mGpioProtocol;

// Cached copy of the PMIC protocol instance
EFI_PMIC_PROTOCOL *gPMIC = NULL;

/**
  Samsung PMU Functions
  These are needed for updating registers in the PMU to set the reboot reason
  and to write the correct reboot magic.
**/
VOID
ExynosPMURead (
  UINT32 Offset,
  UINT32 *Value
  )
{
  *Value = MmioRead32(EXYNOS_PMU_BASE + Offset);
}

VOID
ExynosPMUWrite (
  UINT32 Offset,
  UINT32 Value
  )
{
  MmioWrite32(EXYNOS_PMU_BASE + Offset, Value);
}

VOID
SecSetRebootMagic (
  INT32 Magic,
  INT32 Offset,
  INT32 Mask
  )
{
  UINT32 Temp = 0;

  ExynosPMURead(SEC_DEBUG_MAGIC_INFORM, &Temp);

  Mask <<= Offset;
  Temp &= (~Mask);
  Temp |= Magic << Offset;

  ExynosPMUWrite(SEC_DEBUG_MAGIC_INFORM, Temp);
}

/**
  This function causes a system-wide reset (cold reset), in which
  all circuitry within the system returns to its initial state. This type of reset
  is asynchronous to system operation and operates without regard to
  cycle boundaries.

  If this function returns, it means that the system does not support cold reset.
**/
VOID
EFIAPI
ResetCold (
  VOID
  )
{
  // Cold reset isn't implemented
  ResetWarm ();
}

/**
  This function causes a system-wide initialization (warm reset), in which all processors
  are set to their initial state. Pending cycles are not corrupted.

  If this function returns, it means that the system does not support warm reset.
**/
VOID
EFIAPI
ResetWarm (
  VOID
  )
{
  DisableInterrupts ();

  /* Prevent LowPowerMode */
  SecSetRebootMagic(SEC_REBOOT_NORMAL, SEC_REBOOT_END_OFFSET, 0xFF);

  /* Clear RST_STAT */
  ExynosPMUWrite(RST_STAT, 0);

  /* Do S/W Reset */
  ExynosPMUWrite(SWRESET, SWRESET_TRIGGER);
}

/**
  This function causes the system to enter a power state equivalent
  to the ACPI G2/S5 or G3 states.

  If this function returns, it means that the system does not support shutdown reset.
**/
VOID
EFIAPI
ResetShutdown (
  VOID
  )
{
  UINT32 PMU_PS_HOLD = 0;
  EFI_STATUS Status = EFI_SUCCESS;
  UINT32 IsPressed = 0;

  // Find the Gpio protocol. ASSERT if not found.
  Status = gBS->LocateProtocol (&gEfiGpioProtocolGuid, NULL, (VOID *)&mGpioProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate GPIO Protocol!\n"));
    ASSERT_EFI_ERROR (Status);
  }

  // Find the PMIC protocol. ASSERT if not found.
  Status = gBS->LocateProtocol (&gEfiPMICProtocolGuid, NULL, (VOID **)&gPMIC);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate PMIC Protocol!\n"));
    ASSERT_EFI_ERROR (Status);
  }

  while (TRUE)
  {
    mGpioProtocol->GetPin ((GpioBank *)0x15850000, 0x40, 0x4, &IsPressed);

    if(!IsPressed)
    {
      MicroSecondDelay(1000000);
      continue;
    }

    // Disable PMIC
    gPMIC->DisableWTSR();
    gPMIC->DisableSMPL();
    gPMIC->ShutdownPMIC();

    // Set PS_HOLD low to power off
    ExynosPMURead(PS_HOLD, &PMU_PS_HOLD);
    PMU_PS_HOLD &= 0xFFFFFEFF;
    ExynosPMUWrite(PS_HOLD, PMU_PS_HOLD);
  }
}

/**
  This function causes a systemwide reset. The exact type of the reset is
  defined by the EFI_GUID that follows the Null-terminated Unicode string passed
  into ResetData. If the platform does not recognize the EFI_GUID in ResetData
  the platform must pick a supported reset type to perform.The platform may
  optionally log the parameters from any non-normal reset that occurs.

  @param[in]  DataSize   The size, in bytes, of ResetData.
  @param[in]  ResetData  The data buffer starts with a Null-terminated string,
                         followed by the EFI_GUID.
**/
VOID
EFIAPI
ResetPlatformSpecific (
  IN UINTN  DataSize,
  IN VOID   *ResetData
  )
{
  DisableInterrupts();

  UINT32 Reason = SEC_RESET_REASON_UNKNOWN;

  if (!StrCmp(ResetData, L"RECOVERY")) {
    Reason = SEC_RESET_REASON_RECOVERY;
  }
  else if(!StrCmp(ResetData, L"DOWNLOAD")) {
    Reason = SEC_RESET_REASON_DOWNLOAD;
  }
  else if(!StrCmp(ResetData, L"BOOTLOADER")) {
    Reason = SEC_RESET_REASON_BOOTLOADER;
  }
  else if(!StrCmp(ResetData, L"FASTBOOT")) {
    /* FastbootD requires a little more setup */
    Reason = SEC_RESET_REASON_RECOVERY;
    ExynosPMUWrite(EXYNOS_PMU_SYSIP_DAT0, REBOOT_MODE_FASTBOOT);
  }
  /* Set Reboot Reason */
  ExynosPMUWrite(SEC_DEBUG_PANIC_INFORM, Reason);

  EnableInterrupts();

  // Map the platform specific reset as reboot
  ResetCold ();
}

/**
  The ResetSystem function resets the entire platform.

  @param[in] ResetType      The type of reset to perform.
  @param[in] ResetStatus    The status code for the reset.
  @param[in] DataSize       The size, in bytes, of ResetData.
  @param[in] ResetData      For a ResetType of EfiResetCold, EfiResetWarm, or EfiResetShutdown
                            the data buffer starts with a Null-terminated string, optionally
                            followed by additional binary data. The string is a description
                            that the caller may use to further indicate the reason for the
                            system reset.
**/
VOID
EFIAPI
ResetSystem (
  IN EFI_RESET_TYPE  ResetType,
  IN EFI_STATUS      ResetStatus,
  IN UINTN           DataSize,
  IN VOID            *ResetData OPTIONAL
  )
{
  switch (ResetType) {
    case EfiResetWarm:
      ResetWarm ();
      break;

    case EfiResetCold:
      ResetCold ();
      break;

    case EfiResetShutdown:
      ResetShutdown ();
      return;

    case EfiResetPlatformSpecific:
      ResetPlatformSpecific (DataSize, ResetData);
      return;

    default:
      return;
  }
}
