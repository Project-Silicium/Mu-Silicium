/**
  Copyright (c) 2011-2014, ARM Limited. All rights reserved.
  Copyright (c) 2014, Linaro Limited. All rights reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/HobLib.h>
#include <Library/PcdLib.h>

STATIC
VOID
BuildMemHobForFv (IN UINT16 Type)
{
  EFI_PEI_HOB_POINTERS      HobPtr;
  EFI_HOB_FIRMWARE_VOLUME2 *Hob;

  HobPtr.Raw = GetHobList ();

  while ((HobPtr.Raw = GetNextHob (Type, HobPtr.Raw)) != NULL) {
    if (Type == EFI_HOB_TYPE_FV2) {
      Hob = HobPtr.FirmwareVolume2;

      // Build Memory Allocation HOB to mark it as BootServicesData
      BuildMemoryAllocationHob (Hob->BaseAddress, EFI_SIZE_TO_PAGES(Hob->Length) * EFI_PAGE_SIZE, EfiBootServicesData);
    }

    HobPtr.Raw = GET_NEXT_HOB(HobPtr);
  }
}

EFI_STATUS
EFIAPI
PlatformPeim ()
{
  BuildFvHob (PcdGet64(PcdFvBaseAddress), PcdGet32(PcdFvSize));

  BuildMemHobForFv (EFI_HOB_TYPE_FV2);

  return EFI_SUCCESS;
}
