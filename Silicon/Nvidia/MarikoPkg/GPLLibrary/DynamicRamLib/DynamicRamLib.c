/**
  Copyright (C) 2010 - 2015, NVIDIA Corporation <www.nvidia.com>
  SPDX-License-Identifier: GPL-2.0+
**/

#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/DynamicRamLib.h>
#include <Library/MemoryMapHelperLib.h>

#include "MemoryController.h"

EFI_STATUS
GetMemorySize (OUT UINT64 *MemorySize)
{
  EFI_STATUS                      Status;
  EFI_MEMORY_REGION_DESCRIPTOR_EX MemoryControllerRegion;
  UINT64                          RamSize;
  UINT32                          EMemConfiguration;

  // Locate Memory Controller Region
  Status = LocateMemoryMapAreaByName ("Memory Controller", &MemoryControllerRegion);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate Memory Controller Region! Status = %r\n", Status));
    return Status;
  }

  // Populate Memory Controller Structure
  MemoryController *CONST Mc = (MemoryController *)MemoryControllerRegion.Address;

  // Read EMEM Configuration
  EMemConfiguration = MmioRead32 ((UINTN)&Mc->mc_emem_cfg);

  // Calculate Memory Size
  if (EMemConfiguration >= 4096) {
    RamSize = ((UINT32)~0U) & ~(0x1000 - 1);
  } else {
    RamSize = (UINT64)EMemConfiguration * 1024 * 1024; // "unsigned long" instead of "UINT64"?

#ifdef MDE_CPU_AARCH64
    if (EMemConfiguration <= (0 - FixedPcdGet64 (PcdSystemMemoryBase)) / (1024 * 1024)) {
      // TODO: Implement other RAM Function!
      DEBUG ((EFI_D_ERROR, "Unsupported Option!\n"));
      return EFI_UNSUPPORTED;
    }
#endif
  }

  *MemorySize = RamSize;

  return EFI_SUCCESS;
}
