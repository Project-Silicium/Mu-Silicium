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
	ldr	x3, [x0, 16]
	ldr	x2, [x3, 48]
	ldr	w5, [x3, 64]
	add	x5, x5, x2
	cmp	x2, x5
	bcs	_Payload
	mov	w7, 4136
	movk	w7, 0xd538, lsl 16
	mov	w6, 53928
	movk	w6, 0xd53b, lsl 16
	mov	w9, 61400
	movk	w9, 0x2ae7, lsl 16
	mov	x10, 725
	movk	x10, 0x1800, lsl 16
	movk	x10, 0x3, lsl 32
	movk	x10, 0xd280, lsl 48
	mov	x12, 3
	movk	x12, 0xd280, lsl 16
	movk	x12, 0x2, lsl 32
	movk	x12, 0xd280, lsl 48
	mov	x14, 1
	movk	x14, 0xd280, lsl 16
	movk	x14, 0x240, lsl 32
	movk	x14, 0x1800, lsl 48
	mov	w15, 960
	movk	w15, 0xd65f, lsl 16
	mov	x13, 2
	movk	x13, 0xd280, lsl 16
	movk	x13, 0x1, lsl 32
	movk	x13, 0xd280, lsl 48
	mov	w11, 8223
	movk	w11, 0xd503, lsl 16
	mov	w8, 8
	movk	w8, 0xd280, lsl 16
	b	.L9
.L5:
	add	w3, w3, w9
	cmp	w3, 1
	bls	.L12
	ldr	x3, [x2]
	cmp	x3, x10
	beq	.L13
	cmp	x3, x12
	bne	.L6
	ldr	x3, [x2, 8]
	and	x3, x3, -549755813889
	cmp	x3, x14
	bne	.L6
	str	w15, [x2, -28]
	b	.L6
.L12:
	str	w11, [x2]
.L6:
	add	x2, x2, 4
	cmp	x2, x5
	bcs	_Payload
.L9:
	ldr	w3, [x2]
	cmp	w3, w7
	ccmp	w3, w6, 4, ne
	bne	.L5
	str	w8, [x2]
	b	.L6
.L13:
	ldr	x3, [x2, 8]
	cmp	x3, x13
	bne	.L6
	str	w15, [x2, -32]
	b	.L6

_Dead:
	/* We should never get here */
	b		_Dead

.text
.align 4

_Payload:
	/* Your code will get ran right after this binary */

