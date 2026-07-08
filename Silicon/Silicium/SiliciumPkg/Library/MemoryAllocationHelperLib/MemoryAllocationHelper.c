/**
  Copyright (c) 2015-2018, 2020, The Linux Foundation. All rights reserved.
  Copyright (c) 2022. Sunflower2333. All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

  - Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.

  - Redistributions in binary form must reproduce the above
    copyright notice, this list of conditions and the following
    disclaimer in the documentation and/or other materials provided
    with the distribution.

  - Neither the name of The Linux Foundation nor the names of its
    contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
  WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
  ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
  BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
  BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
  OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
  IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**/

#include <Library/DxeServicesTableLib.h>
#include <Library/MemoryAllocationHelperLib.h>
#include <Library/ArmMmuLib.h>

EFI_STATUS
MapMemoryRegion (
  IN EFI_PHYSICAL_ADDRESS Address,
  IN UINT64               Length,
  IN EFI_MEMORY_TYPE      Type)
{
  EFI_STATUS          Status;
  EFI_GCD_MEMORY_TYPE MemoryType;
  UINT64              MemoryAttribute[2];

  // Set Memory Region Details
  switch (Type) {
    case EfiConventionalMemory:
      MemoryType         = EfiGcdMemoryTypeSystemMemory;
      MemoryAttribute[0] = ARM_MEMORY_REGION_ATTRIBUTE_WRITE_BACK;
      MemoryAttribute[1] = EFI_MEMORY_WB;
      break;

    case EfiMemoryMappedIO:
      MemoryType         = EfiGcdMemoryTypeMemoryMappedIo;
      MemoryAttribute[0] = ARM_MEMORY_REGION_ATTRIBUTE_DEVICE;
      MemoryAttribute[1] = EFI_MEMORY_UC;
      break;

    default:
      return EFI_UNSUPPORTED;
  }

  // Add new Memory Space
  Status = gDS->AddMemorySpace (MemoryType, Address, Length, 0xF);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Set Memory Attributes
  Status = ArmSetMemoryAttributes (Address, Length, MemoryAttribute[0], 0);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Set Memory Space Attributes
  Status = gDS->SetMemorySpaceAttributes (Address, Length, MemoryAttribute[1]);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}
