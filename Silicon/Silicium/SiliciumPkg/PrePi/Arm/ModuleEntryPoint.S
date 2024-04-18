//
//  Copyright (c) 2011-2015, ARM Limited. All rights reserved.
//  Copyright (c) 2019, RUIKAI LIU and MR TUNNEL. All rights reserved.
//
//  This program and the accompanying materials
//  are licensed and made available under the terms and conditions of the BSD License
//  which accompanies this distribution.  The full text of the license may be found at
//  http://opensource.org/licenses/bsd-license.php
//
//  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
//  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
//
//

#include <AsmMacroIoLib.h>
#include <Library/PcdLib.h>
#include <Chipset/ArmV7.h>

#define LoadConstantToReg(Data, Reg) \
    ldr Reg, =Data

.global _StackBase
.global _StackSize

_StackBase:
  .quad FixedPcdGet32(PcdPrePiStackBase)

_StackSize:
  .quad FixedPcdGet32(PcdPrePiStackSize)

ASM_FUNC(_ModuleEntryPoint)
  mov r0, #0
  bl ASM_PFX(ArmReadMpidr)
  mov r8, r0

  /* First ensure all interrupts are disabled */
  bl ASM_PFX(ArmDisableInterrupts)

  /* Ensure that the MMU and caches are off */
  bl ASM_PFX(ArmDisableCachesAndMmu)

  /* Invalide I-Cache */
  bl ASM_PFX(ArmInvalidateInstructionCache)

  /* Invalidate TLB */
  bl ASM_PFX(ArmInvalidateTlb)

_SetupExceptionVector:
  LoadConstantToReg (FixedPcdGet32(PcdCpuVectorBaseAddress), r0)
  ldr r1, _NeverReturn
  mov r2, #0

_FillVectors:
  str     r1, [r0, r2]
  adds    r2, r2, #8      /* Fill every 8 byte */
  cmp     r2, #0x800      /* Vector Table Size */
  bne     _FillVectors

_SetupPrimaryCoreStack:
  ldr r0, _StackBase     /* Stack base arg0 */
  ldr r1, _StackSize     /* Stack size arg1 */

  add r4, r0, r1
  add sp, r4, #0

_EnableCache:
  bl ArmInvalidateDataCache
  bl ASM_PFX(ArmEnableInstructionCache)
  bl ASM_PFX(ArmEnableDataCache)

_PrepareArguments:
  mov   r0, r8
  ldr   r1, _StackBase
  ldr   r2, _StackSize
  mov   r3, sp

  // Move sec startup address into a data register
  // Ensure we're jumping to FV version of the code (not boot remapped alias)
  ldr   r4, =ASM_PFX(CEntryPoint)

  // Jump to PrePiCore C code
  //    r0 = MpId
  //    r1 = UefiMemoryBase
  //    r2 = StacksBase
  blx   r4

_NeverReturn:
  b _NeverReturn

.align 3