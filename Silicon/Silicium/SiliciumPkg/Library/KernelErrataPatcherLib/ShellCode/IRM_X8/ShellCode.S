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
	mov	x10, x0
	ldr	x9, [x10, 16]!
	cmp	x9, x10
	beq	_Payload
	mov	x3, 0
	mov	x4, 0
	mov	w8, 52136
	movk	w8, 0xd518, lsl 16
	mov	x7, 3
	movk	x7, 0x9, lsl 16
	movk	x7, 0x16, lsl 32
	movk	x7, 0x9, lsl 48
	b	.L9
.L5:
	ldr	x9, [x9]
	cmp	x9, x10
	beq	.L13
.L9:
	ldr	x2, [x9, 48]
	ldr	w6, [x9, 64]
	add	x6, x6, x2
	cmp	x2, x6
	bcs	.L5
.L8:
	ldr	w5, [x2]
	cmp	w5, w8
	csel	x4, x4, x2, ne
	ldr	x5, [x2]
	cmp	x5, x7
	csel	x3, x3, x2, ne
	cmp	x4, 0
	ccmp	x3, 0, 4, ne
	bne	.L5
	add	x2, x2, 4
	cmp	x2, x6
	bcc	.L8
	b	.L5
.L13:
	cmp	x4, 0
	ccmp	x3, 0, 4, ne
	beq	_Payload
	sub	x2, x3, #168
	sub	x2, x2, x4
	ubfx	w2, w2, 2, 26
	mov	w5, -1811939328
	orr	w2, w2, w5
	str	w2, [x4]
	mov	x2, 31741
	movk	x2, 0xa9bf, lsl 16
	movk	x2, 0x3fd, lsl 32
	movk	x2, 0x9100, lsl 48
	str	x2, [x3, -168]
	mov	x2, 584
	movk	x2, 0xb640, lsl 16
	movk	x2, 0xf908, lsl 32
	movk	x2, 0x9257, lsl 48
	str	x2, [x3, -160]
	mov	x2, 264
	movk	x2, 0xb240, lsl 16
	movk	x2, 0x3f6, lsl 32
	movk	x2, 0xaa08, lsl 48
	str	x2, [x3, -152]
	mov	x2, 169
	movk	x2, 0xd538, lsl 16
	movk	x2, 0xd29, lsl 32
	movk	x2, 0x9278, lsl 48
	str	x2, [x3, -144]
	mov	x2, 20
	movk	x2, 0xd280, lsl 16
	movk	x2, 0x115, lsl 32
	movk	x2, 0x5280, lsl 48
	str	x2, [x3, -136]
	mov	x2, 8863
	movk	x2, 0xeb49, lsl 16
	movk	x2, 0xa0, lsl 32
	movk	x2, 0x5400, lsl 48
	str	x2, [x3, -128]
	mov	x2, 48778
	movk	x2, 0xd370, lsl 16
	movk	x2, 0x2c8, lsl 32
	movk	x2, 0xaa0a, lsl 48
	str	x2, [x3, -120]
	mov	x2, 52136
	movk	x2, 0xd518, lsl 16
	movk	x2, 0x3f9f, lsl 32
	movk	x2, 0xd503, lsl 48
	str	x2, [x3, -112]
	mov	x2, 1684
	movk	x2, 0x9100, lsl 16
	movk	x2, 0x6b5, lsl 32
	movk	x2, 0x5100, lsl 48
	str	x2, [x3, -104]
	mov	x2, 65301
	movk	x2, 0x35ff, lsl 16
	movk	x2, 0x2, lsl 32
	movk	x2, 0x1400, lsl 48
	str	x2, [x3, -96]
	mov	x2, 52136
	movk	x2, 0xd518, lsl 16
	movk	x2, 0x7bfd, lsl 32
	movk	x2, 0xa8c1, lsl 48
	str	x2, [x3, -88]
	mov	w2, 960
	movk	w2, 0xd65f, lsl 16
	str	w2, [x3, -80]

.text
.align 4

_Payload:
	/* Your code will get ran right after this binary */
