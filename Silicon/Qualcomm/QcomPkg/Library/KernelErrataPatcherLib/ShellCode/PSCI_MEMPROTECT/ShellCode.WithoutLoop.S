/** @file

  Patches NTOSKRNL to not cause a SError when reading/writing ACTLR_EL1
  Patches NTOSKRNL to not cause a SError when reading/writing AMCNTENSET0_EL0
  Patches NTOSKRNL to not cause a bugcheck when attempting to use
  PSCI_MEMPROTECT Due to an issue in QHEE

  Shell Code to patch kernel mode components before NTOSKRNL

  Copyright (c) 2022-2023 DuoWoA authors

  SPDX-License-Identifier: MIT

**/

//VOID
//OslArm64TransferToKernel (
//  INT VOID *OsLoaderBlock, INT *KernelAddress
//  );
_Start:
	mov	x12, x0
	ldr	x8, [x12, 16]!
	cmp	x8, x12
	beq	_Payload
	mov	x6, 725
	movk	x6, 0x1800, lsl 16
	movk	x6, 0x3, lsl 32
	movk	x6, 0xd280, lsl 48
	mov	x7, 3
	movk	x7, 0xd280, lsl 16
	movk	x7, 0x2, lsl 32
	movk	x7, 0xd280, lsl 48
	mov	x10, 1
	movk	x10, 0xd280, lsl 16
	movk	x10, 0x240, lsl 32
	movk	x10, 0x1800, lsl 48
	mov	w11, 960
	movk	w11, 0xd65f, lsl 16
	mov	x9, 2
	movk	x9, 0xd280, lsl 16
	movk	x9, 0x1, lsl 32
	movk	x9, 0xd280, lsl 48
	b	.L9
.L14:
	ldr	x3, [x2, 8]
	cmp	x3, x9
	beq	.L13
.L7:
	add	x2, x2, 4
	cmp	x2, x5
	bcs	.L5
.L8:
	ldr	x3, [x2]
	cmp	x3, x6
	beq	.L14
	cmp	x3, x7
	bne	.L7
	ldr	x3, [x2, 8]
	and	x3, x3, -549755813889
	cmp	x3, x10
	bne	.L7
	str	w11, [x2, -28]
	b	.L7
.L13:
	str	w11, [x2, -32]
	b	.L7
.L5:
	ldr	x8, [x8]
	cmp	x8, x12
	b	_Payload
.L9:
	ldr	x2, [x8, 48]
	ldr	w5, [x8, 64]
	add	x5, x5, x2
	cmp	x2, x5
	bcc	.L8
	b	.L5

_Dead:
	/* We should never get here */
	b		_Dead

.text
.align 4

_Payload:
	/* Your code will get ran right after this binary */
