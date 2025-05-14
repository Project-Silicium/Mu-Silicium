/**
  Copyright (c) 2011-2014, ARM Limited. All rights reserved.
  Copyright (c) 2014, Linaro Limited. All rights reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/PlatformHobLib.h>
#include <Library/PcdLib.h>

STATIC
VOID
BuildMemHobForFv (IN UINT16 Type)
{
  EFI_PEI_HOB_POINTERS      HobPtr;
  EFI_HOB_FIRMWARE_VOLUME2 *Hob;

  // Get HOB List
  HobPtr.Raw = GetHobList ();

  // Build each HOB
  while ((HobPtr.Raw = GetNextHob (Type, HobPtr.Raw)) != NULL) {
    if (Type == EFI_HOB_TYPE_FV2) {
      Hob = HobPtr.FirmwareVolume2;

      // Build Memory Allocation HOB
      BuildMemoryAllocationHob (Hob->BaseAddress, EFI_SIZE_TO_PAGES (Hob->Length) * EFI_PAGE_SIZE, EfiBootServicesData);
    }

    // Switch HOB
    HobPtr.Raw = GET_NEXT_HOB (HobPtr);
  }
}

EFI_STATUS
EFIAPI
PlatformPeim ()
{
  // Build FV HOB
  BuildFvHob (PcdGet64 (PcdFvBaseAddress), PcdGet32 (PcdFvSize));
  BuildMemHobForFv (EFI_HOB_TYPE_FV2);

  // Build Platform HOBs
  BuildPlatformHobs ();

  return EFI_SUCCESS;
}
