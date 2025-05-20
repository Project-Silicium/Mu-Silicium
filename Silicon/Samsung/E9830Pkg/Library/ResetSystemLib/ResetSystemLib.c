#include <Uefi.h>

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/ResetSystemLib.h>
#include <Library/IoLib.h>

#include <Protocol/EfiS2mps1922.h>

#include "Pmu.h"

VOID
ReadPmu (
  IN  UINT32  Offset,
  OUT UINT32 *Value)
{
  // Read PMU
  *Value = MmioRead32 (EXYNOS_PMU_BASE + Offset);
}

VOID
WritePmu (
  IN UINT32 Offset,
  IN UINT32 Value)
{
  // Write PMU
  MmioWrite32 (EXYNOS_PMU_BASE + Offset, Value);
}

VOID
SecSetRebootMagic (
  IN INT32 Magic,
  IN INT32 Offset,
  IN INT32 Mask)
{
  UINT32 Value;

  // Read SEC_DEBUG_MAGIC_INFORM State
  ReadPmu (SEC_DEBUG_MAGIC_INFORM, &Value);

  // Combine Values
  Mask  <<= Offset;
  Value &=  (~Mask);
  Value |=  Magic << Offset;

  // Write new SEC_DEBUG_MAGIC_INFORM State
  WritePmu (SEC_DEBUG_MAGIC_INFORM, Value);
}

VOID
EFIAPI
ResetCold ()
{
  // Do a Warm Reset instead
  ResetWarm ();
}

VOID
EFIAPI
ResetWarm ()
{
  // Disable Interrupts
  DisableInterrupts ();

  // Prevent LowPowerMode
  SecSetRebootMagic (SEC_REBOOT_NORMAL, SEC_REBOOT_END_OFFSET, 0xFF);

  // Clear RST_STAT
  WritePmu (RST_STAT, 0);

  // Do S/W Reset
  WritePmu (SWRESET, SWRESET_TRIGGER);

  // Do CPU Dead Loop
  CpuDeadLoop ();
}

VOID
EFIAPI
ResetShutdown ()
{
  EFI_STATUS                Status;
  EFI_S2MPS_19_22_PROTOCOL *mS2mps1922Protocol;
  UINT32                    Value;

  // Locate S2mps1922 PMIC Protocol
  Status = gBS->LocateProtocol (&gEfiS2mps1922ProtocolGuid, NULL, (VOID *)&mS2mps1922Protocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Locate S2mps1922 PMIC Protocol! Status = %r\n", __FUNCTION__, Status));
    goto Dead;
  }

  // Disable WTSR
  Status = mS2mps1922Protocol->DisableWtsr ();
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Disable WTSR! Status = %r\n", __FUNCTION__, Status));
    goto Dead;
  }

  // Disable SMPL
  Status = mS2mps1922Protocol->DisableSmpl ();
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Disable SMPL! Status = %r\n", __FUNCTION__, Status));
    goto Dead;
  }

  // Power Off PMIC
  Status = mS2mps1922Protocol->PowerDown ();
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Turn Off PMIC! Status = %r\n", __FUNCTION__, Status));
    goto Dead;
  }

  // Read current PS_HOLD State
  ReadPmu (PS_HOLD, &Value);

  // Set PS_HOLD Low
  Value &= 0xFFFFFEFF;

  // Write new PS_HOLD State
  WritePmu (PS_HOLD, Value);

Dead:
  // Do CPU Dead Loop
  CpuDeadLoop ();
}

VOID
EFIAPI
ResetPlatformSpecific (
  IN UINTN  DataSize,
  IN VOID  *ResetData)
{
  UINT32 Reason;

  // Disable Interrupts
  DisableInterrupts ();

  // Check Recovery Reboot Reason
  if (!StrCmp (ResetData, L"RECOVERY")) {
    Reason = SEC_RESET_REASON_RECOVERY;
  }

  // Check Download Mode Reboot Reason
  if (!StrCmp (ResetData, L"DOWNLOAD")) {
    Reason = SEC_RESET_REASON_DOWNLOAD;
  }

  // Check Bootloader Reboot Reason
  if (!StrCmp (ResetData, L"BOOTLOADER")) {
    Reason = SEC_RESET_REASON_BOOTLOADER;
  }

  // Check FastbootD Reboot Reason
  if (!StrCmp (ResetData, L"FASTBOOTD")) {
    // Write EXYNOS_PMU_SYSIP_DAT0 Register
    WritePmu (EXYNOS_PMU_SYSIP_DAT0, REBOOT_MODE_FASTBOOT);

    Reason = SEC_RESET_REASON_RECOVERY;
  }

  // Write Reboot Reason
  WritePmu (SEC_DEBUG_PANIC_INFORM, Reason);

  // Enable Interrupts
  EnableInterrupts ();

  // Do a Warm Reset
  ResetWarm ();
}

VOID
EFIAPI
ResetSystem (
  IN EFI_RESET_TYPE  ResetType,
  IN EFI_STATUS      ResetStatus,
  IN UINTN           DataSize,
  IN VOID           *ResetData)
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
