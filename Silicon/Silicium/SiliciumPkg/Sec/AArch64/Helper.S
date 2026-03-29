//
// Copyright (c) 2011-2017, ARM Limited. All rights reserved.
// SPDX-License-Identifier: BSD-2-Clause-Patent
//

#include <AsmMacroLib.h>

#include <AArch64/AArch64.h>

//
// HCR_EL2 Bits
//
.set HCR_FIQ_BIT,    (1 << 3)
.set HCR_IRQ_BIT,    (1 << 4)
.set HCR_SERROR_BIT, (1 << 5)

ASM_FUNC (SetupExceptionLevel1)
  // Re-Configure Access Control
  mov   x0, #CPACR_DEFAULT
  b     ASM_PFX (ArmWriteCpacr)

ASM_FUNC (SetupExceptionLevel2)
  // Clear SCTRL_EL2
  msr   sctlr_el2, xzr

  // Get current Hypervisor Configuration
  mrs   x0, hcr_el2

  // Redirect Interrupts to EL2
  orr   x0, x0, #HCR_FIQ_BIT
  orr   x0, x0, #HCR_IRQ_BIT
  orr   x0, x0, #HCR_SERROR_BIT

  // Write new Hypervisor Configuration
  msr   hcr_el2, x0

  // Setup EL1 Exceptions
  tst   x0, #ARM_HCR_E2H
  b.ne  SetupExceptionLevel1

  // Enable Architectural Features
  mov   x0, #AARCH64_CPTR_DEFAULT
  msr   cptr_el2, x0

  ret

ASM_FUNCTION_REMOVE_IF_UNREFERENCED
