/** @file
  ResetSystemLib implementation using Pmic to Handle Resets and Shutdowns

  Copyright (c) 2017 - 2018, Linaro Ltd. All rights reserved.<BR>
  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
  Copyright (c) 2022, Arm Limited. All rights reserved.<BR>
  Copyright (c) 2015-2016, The Linux Foundation. All rights reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#include <PiDxe.h>

#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/ResetSystemLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/TimerLib.h>

#include <Protocol/QcomPmic.h>

// PS_HOLD REgisters
#define PS_HOLD            0x004AB000
#define PS_HOLD_RESET_CTL  0x85A
#define PS_HOLD_RESET_CTL2 0x85B

// PS_HOLD Reset Types
#define PS_HOLD_SHUTDOWN   0x4
#define PS_HOLD_COLD_RESET 0x7
#define PS_HOLD_WARM_RESET 0x1

EFI_STATUS
ConfigureResetType(UINT8 ResetType)
{
  EFI_STATUS          Status;
  QCOM_PMIC_PROTOCOL *mPm8x41Protocol;

  // Locate Pm8x41 Protocol
  Status = gBS->LocateProtocol(&gQcomPmicProtocolGuid, NULL, (VOID *)&mPm8x41Protocol);
  if (EFI_ERROR (Status)) {
    goto exit;
  }

  // Disable PS_HOLD_RESET
  Status = mPm8x41Protocol->WriteReg(PS_HOLD_RESET_CTL2, 0);
  if (EFI_ERROR (Status)) {
    goto exit;
  }

  // Wait a Bit before Continue to Shutdown
  MicroSecondDelay(300);

  // Configure Reset Type
  Status = mPm8x41Protocol->WriteReg(PS_HOLD_RESET_CTL, ResetType);
  if (EFI_ERROR (Status)) {
    goto exit;
  }

  // Enable PS_HOLD_RESET
  Status = mPm8x41Protocol->WriteReg(PS_HOLD_RESET_CTL2, (1 << 7));

exit:
  return Status;
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
ResetCold (VOID)
{
  EFI_STATUS Status;

  // Configure Reset Type
  Status = ConfigureResetType(PS_HOLD_COLD_RESET);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Configure Reset Type! Status = %r\n", Status));
    return;
  }

  // Drop PS_HOLD
  MmioWrite32(PS_HOLD, 0);
}

/**
  This function causes a system-wide initialization (warm reset), in which all processors
  are set to their initial state. Pending cycles are not corrupted.

  If this function returns, it means that the system does not support warm reset.
**/
VOID
EFIAPI
ResetWarm (VOID)
{
  EFI_STATUS Status;

  // Configure Reset Type
  Status = ConfigureResetType(PS_HOLD_WARM_RESET);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Configure Reset Type! Status = %r\n", Status));
    return;
  }

  // Drop PS_HOLD
  MmioWrite32(PS_HOLD, 0);
}

/**
  This function causes the system to enter a power state equivalent
  to the ACPI G2/S5 or G3 states.

  If this function returns, it means that the system does not support shutdown reset.
**/
VOID
EFIAPI
ResetShutdown (VOID)
{
  EFI_STATUS Status;
  
  // Configure Reset Type
  Status = ConfigureResetType(PS_HOLD_SHUTDOWN);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Configure Reset Type! Status = %r\n", Status));
    return;
  }

  // Drop PS_HOLD
  MmioWrite32(PS_HOLD, 0);
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
  IN VOID  *ResetData)
{
  // Map the platform specific reset as Cold reboot
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
  IN EFI_RESET_TYPE ResetType,
  IN EFI_STATUS     ResetStatus,
  IN UINTN          DataSize,
  IN VOID          *ResetData OPTIONAL)
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