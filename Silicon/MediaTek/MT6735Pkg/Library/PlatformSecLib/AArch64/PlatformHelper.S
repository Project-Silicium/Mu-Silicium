//
// Copyright (c) 2012-2013, ARM Limited. All rights reserved.
// SPDX-License-Identifier: BSD-2-Clause-Patent
//

#include <AsmMacroLib.h>

ASM_FUNC (PlatformAssemblyInitialize)
  EL1_OR_EL2 (x0)
1:ret
// Switch to EL1
2:mov x0, #(1 << 31)
  msr hcr_el2, x0

  msr elr_el2, x30

  mov x0, #0x3C5 // EL1h
  msr spsr_el2, x0
  isb
  eret
