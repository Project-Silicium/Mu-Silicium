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
	mov	x14, x0
	ldr	x8, [x14, 16]!
	cmp	x8, x14
	beq	_Payload
	mov	w7, 4136
	movk	w7, 0xd538, lsl 16
	mov	w6, 53928
	movk	w6, 0xd53b, lsl 16
	mov	w10, 61400
	movk	w10, 0x2ae7, lsl 16
	mov	x11, 725
	movk	x11, 0x1800, lsl 16
	movk	x11, 0x3, lsl 32
	movk	x11, 0xd280, lsl 48
	mov	x13, 3
	movk	x13, 0xd280, lsl 16
	movk	x13, 0x2, lsl 32
	movk	x13, 0xd280, lsl 48
	mov	x16, 1
	movk	x16, 0xd280, lsl 16
	movk	x16, 0x240, lsl 32
	movk	x16, 0x1800, lsl 48
	mov	w17, 960
	movk	w17, 0xd65f, lsl 16
	mov	x15, 2
	movk	x15, 0xd280, lsl 16
	movk	x15, 0x1, lsl 32
	movk	x15, 0xd280, lsl 48
	mov	w12, 8223
	movk	w12, 0xd503, lsl 16
	mov	w9, 8
	movk	w9, 0xd280, lsl 16
	b	.L11
.L6:
	add	w3, w3, w10
	cmp	w3, 1
	bls	.L15
	ldr	x3, [x2]
	cmp	x3, x11
	beq	.L16
	cmp	x3, x13
	bne	.L7
	ldr	x3, [x2, 8]
	and	x3, x3, -549755813889
	cmp	x3, x16
	bne	.L7
	str	w17, [x2, -28]
	b	.L7
.L15:
	str	w12, [x2]
.L7:
	add	x2, x2, 4
	cmp	x2, x5
	bcs	.L5
.L10:
	ldr	w3, [x2]
	cmp	w3, w7
	ccmp	w3, w6, 4, ne
	bne	.L6
	str	w9, [x2]
	b	.L7
.L16:
	ldr	x3, [x2, 8]
	cmp	x3, x15
	bne	.L7
	str	w17, [x2, -32]
	b	.L7
.L5:
	ldr	x8, [x8]
	cmp	x8, x14
	b	_Payload
.L11:
	ldr	x2, [x8, 48]
	ldr	w5, [x8, 64]
	add	x5, x5, x2
	cmp	x2, x5
	bcc	.L10
	b	.L5

_Dead:
	/* We should never get here */
	b		_Dead

.text
.align 4

_Payload:
	/* Your code will get ran right after this binary */

