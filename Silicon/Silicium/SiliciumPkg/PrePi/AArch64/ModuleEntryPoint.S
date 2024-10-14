//
//  Copyright (c) 2011 - 2020, Arm Limited. All rights reserved.
//
//  SPDX-License-Identifier: BSD-2-Clause-Patent
//

#include <AsmMacroIoLibV8.h>

.set CTRL_SPAN_BIT, (1 << 23)

.text
.align 3

ASM_FUNC (_ModuleEntryPoint)
  // Run SoC Specific Code
  bl   ASM_PFX (PlaformAssemblyInit)

  // Get MpId
  bl   ASM_PFX (ArmReadMpidr)

  // Save MpId
  mov  x10, x0

SetSVCMode:
  // Clear x0 and x1
  mov  x0, #0
  mov  x1, #0

  // Disable all Interrupts
  bl   ASM_PFX (ArmDisableInterrupts)

  // Disable MMU and CPU Caches
  bl   ASM_PFX (ArmDisableCachesAndMmu)

  // Invalidate I-Cache
  bl   ASM_PFX (ArmInvalidateInstructionCache)

  // Invalidate TLB
  bl   ASM_PFX (ArmInvalidateTlb)

  // Rewrite SCTLR_EL1
  mrs  x0, sctlr_el1
  orr  x0, x0, #CTRL_SPAN_BIT
  msr  sctlr_el1, x0

SetupExceptionVector:
  // Get Exception Vector Address
  ldr  x0, =FixedPcdGet64(PcdCpuVectorBaseAddress)

  // Write Vector Table Address
  bl   ArmWriteVBar

  // Setup Exception Loop
  ldr  x1, NeverReturn
  add  x2, x0, #0x800
  mov  x3, x0

FillVectors:
  // Fill every 16 Bytes
  stp  x1, x1, [x3], #16
  cmp  x3, x2
  b.lt FillVectors

UntrapVFP:
  // Rewrite CPACR_EL1
  mrs  x0, cpacr_el1
  orr  x0, x0, #0x300000
  msr  cpacr_el1, x0

  // Fetch Subsequent Instructions
  isb  sy

SetupCoreStack:
  // Get Stack Base & Size
  ldr  x0, =FixedPcdGet32(PcdPrePiStackBase)
  ldr  x1, =FixedPcdGet32(PcdPrePiStackSize)

  // Preapre Stack Clear
  add  x2, x0, x1
  mov  x3, x0

  // Set Core Stack
  mov  v4.d[0], xzr
  mov  v4.d[1], xzr
  mov  v5.2d, v4.2d
  mov  v6.2d, v4.2d
  mov  v7.2d, v4.2d

ClearStack:
  // Clear 64 Bytes
  st4  { v4.2d, v5.2d, v6.2d, v7.2d }, [x3], #64
  cmp  x3, x2
  b.lt ClearStack

  // Init SP
  add  sp, x2, xzr

EnableCaches:
  // Invalidate D-Cache
  bl   ArmInvalidateDataCache

  // Enable I-Cache & D-Cache
  bl   ASM_PFX(ArmEnableInstructionCache)
  bl   ASM_PFX(ArmEnableDataCache)

JumpToC:
  // Restore Values
  mov  x0, x10
  mov  x1, x11
  mov  x2, x12

  // Set Jump Location
  ldr  x4, =ASM_PFX (CEntryPoint)

  // Clear Frame Pointer
  mov  x29, xzr

  // Jump to C Code
  blr  x4

.align 3

NeverReturn:
  b  NeverReturn
