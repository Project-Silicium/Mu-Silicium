/** @file

  Patches NTOSKRNL to not cause a SError when reading/writing ACTLR_EL1
  Patches NTOSKRNL to not cause a SError when reading/writing AMCNTENSET0_EL0
  Patches NTOSKRNL to not cause a bugcheck when attempting to use
  PSCI_MEMPROTECT Due to an issue in QHEE

  Copyright (c) 2022-2023 DuoWoA authors

  SPDX-License-Identifier: MIT

**/
#include "KernelErrataPatcherLib.h"

GCC_ASM_IMPORT (KernelErrataPatcherExitBootServices)
GCC_ASM_EXPORT (ExitBootServicesWrapper)

// Wrapper around Exit boot service for hooking into winload.efi

//EFI_STATUS
//EFIAPI
//ExitBootServicesWrapper (
//  IN EFI_HANDLE ImageHandle, IN UINTN MapKey
//  );
ExitBootServicesWrapper:
        // the x30 register contains the last return addr
        mov x2, x30

        // Go for it
        b KernelErrataPatcherExitBootServices