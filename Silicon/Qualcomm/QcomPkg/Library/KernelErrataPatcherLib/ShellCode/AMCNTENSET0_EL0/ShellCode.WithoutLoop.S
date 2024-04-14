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
	mov	x9, x0
	ldr	x6, [x9, 16]!
	cmp	x6, x9
	beq	_Payload
	mov	w5, 53920
	movk	w5, 0xd53b, lsl 16
	mov	w8, 11615
	movk	w8, 0x2844, lsl 16
	mov	w7, -763363328
	b	.L8
.L6:
	add	x2, x2, 4
	cmp	x2, x4
	bcs	.L5
.L7:
	ldr	w3, [x2]
	bics	wzr, w5, w3
	bne	.L6
	and	w3, w3, w8
	orr	w3, w3, w7
	str	w3, [x2]
	b	.L6
.L5:
	ldr	x6, [x6]
	cmp	x6, x9
	b	_Payload
.L8:
	ldr	x2, [x6, 48]
	ldr	w4, [x6, 64]
	add	x4, x4, x2
	cmp	x2, x4
	bcc	.L7
	b	.L5

_Dead:
	/* We should never get here */
	b		_Dead

.text
.align 4

_Payload:
	/* Your code will get ran right after this binary */
