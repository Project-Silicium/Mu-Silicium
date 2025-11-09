/**
  Copyright (c) 2014 - 2015 Samsung Electronics Co., Ltd.

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License version 2 as
  published by the Free Software Foundation.
**/

#include <Uefi.h>

#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryMapHelperLib.h>
#include <Library/ResetSystemLib.h>
#include <Library/IoLib.h>

#include <Protocol/EfiS2mps1922.h>

#include "Pmu.h"

//
// Global Variables
//
STATIC EFI_MEMORY_REGION_DESCRIPTOR_EX PmuRegion;

VOID
SetRebootMagic (IN UINT8 Magic)
{
  UINT32 Value;
  INT32  Mask;

  // Get current Magic
  Value = MmioRead32 (PmuRegion.Address + MAGIC_INFORM);

  // Set Mask
  Mask   = 0xFF;
  Mask <<= 16;

  // Reconfigure Magic
  Value &= ~Mask;
  Value |= Magic << 16;

  // Write new Magic
  MmioWrite32 (PmuRegion.Address + MAGIC_INFORM, Value);
}

VOID
EFIAPI
ResetCold ()
{
  EFI_STATUS Status;

  // Locate "PMU" Memory Region
  Status = LocateMemoryMapAreaByName ("PMU", &PmuRegion);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Locate 'PMU' Memory Region! Status = %r\n", __FUNCTION__, Status));
    goto Dead;
  }

  // Set new Reboot Magic
  SetRebootMagic (SEC_REBOOT_NORMAL);

  // Set Reboot Reason
  MmioWrite32 (PmuRegion.Address + PANIC_INFORM, RESET_REASON_UNKNOWN);

  // Set Reboot Location
  MmioWrite32 (PmuRegion.Address + SYSIP_DAT0, 0xFF);

  // Clear RST_STAT
  MmioWrite32 (PmuRegion.Address + RST_STAT, 0);

  // Do S/W Reset
  MmioWrite32 (PmuRegion.Address + SWRESET, SWRESET_TRIGGER);

Dead:
  DEBUG ((EFI_D_ERROR, "%a: Reboot Failed! Halting System now.\n"));

  // Do CPU Dead Loop
  CpuDeadLoop ();
}

VOID
EFIAPI
ResetWarm ()
{
  // Do Cold Reset instead
  ResetCold ();
}

VOID
EFIAPI
ResetShutdown ()
{
  EFI_STATUS                Status;
  EFI_S2MPS_19_22_PROTOCOL *mS2mps19Protocol;
  UINT32                    Value;

  // Locate "PMU" Memory Region
  Status = LocateMemoryMapAreaByName ("PMU", &PmuRegion);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Locate 'PMU' Memory Region! Status = %r\n", __FUNCTION__, Status));
    goto Dead;
  }

  // Locate S2mps19 PMIC Protocol
  Status = gBS->LocateProtocol (&gEfiS2mps1922ProtocolGuid, NULL, (VOID *)&mS2mps19Protocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Locate S2mps19 PMIC Protocol! Status = %r\n", __FUNCTION__, Status));
    goto Dead;
  }

  // Disable WTSR
  Status = mS2mps19Protocol->DisableWtsr ();
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Disable WTSR! Status = %r\n", __FUNCTION__, Status));
    goto Dead;
  }

  // Set new Reboot Magic
  SetRebootMagic (SEC_REBOOT_LPM);

  // Get Current PS_HOLD Value
  Value = MmioRead32 (PmuRegion.Address + PS_HOLD_CONTROL);

  // Set PS_HOLD Low
  Value &= 0xFFFFFEFF;

  // Write new PS_HOLD Value
  MmioWrite32 (PmuRegion.Address + PS_HOLD_CONTROL, Value);

Dead:
  DEBUG ((EFI_D_ERROR, "%a: Power Off Failed! Halting System now.\n"));

  // Do CPU Dead Loop
  CpuDeadLoop ();
}

VOID
EFIAPI
ResetPlatformSpecific (
  IN UINTN  DataSize,
  IN VOID  *ResetData)
{
  // Do Cold Reset instead
  ResetCold ();
}

VOID
EFIAPI
ResetSystem (
  IN EFI_RESET_TYPE  ResetType,
  IN EFI_STATUS      ResetStatus,
  IN UINTN           DataSize,
  IN VOID           *ResetData)
{
  // Select Reset Type
  switch (ResetType) {
    case EfiResetWarm:
      ResetWarm ();
      break;

    case EfiResetCold:
      ResetCold ();
      break;

    case EfiResetShutdown:
      ResetShutdown ();
      break;

    case EfiResetPlatformSpecific:
      ResetPlatformSpecific (DataSize, ResetData);
      break;
  }
}
