/**
  Copyright (c) 2022-2023 DuoWoA authors
  SPDX-License-Identifier: MIT
**/

#include "KernelErrataPatcherLib.h"

GCC_ASM_IMPORT (KernelErrataPatcherExitBootServices)
GCC_ASM_EXPORT (ExitBootServicesWrapper)

//EFI_STATUS
//EFIAPI
//ExitBootServicesWrapper (
//  IN EFI_HANDLE ImageHandle,
//  IN UINTN      MapKey
//  );
ExitBootServicesWrapper:
  // Save Return Address
  mov x2, x30

  // Jump to EBS Hook
  b KernelErrataPatcherExitBootServices
