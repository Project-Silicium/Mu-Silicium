//
// Copyright (c) 2012-2013, ARM Limited. All rights reserved.
// SPDX-License-Identifier: BSD-2-Clause-Patent
//

#include <AsmMacroLib.h>

.set CTRL_M_BIT,	 (1 << 0)
.set CTRL_C_BIT,	 (1 << 2)
.set CTRL_I_BIT,	 (1 << 12)

ASM_FUNC (PlatformAssemblyInitialize)
	// Enter critical section: disable interrupt
	msr	daifset, #3
	isb

	// CLean, invalidate and disable data-cache
	dsb	sy								      // ensure ordering with previous memory accesses
	mrs	x0, clidr_el1						// read clidr
	and	x3, x0, #0x7000000			// extract loc from clidr
	lsr	x3, x3, #23							// left align loc bit field
	cbz	x3, Finished						// if loc is 0, then no need to clean
	mov	x10, #0							    // start clean at cache level 0

Loop1:
	add	x2, x10, x10, lsr #1		// work out 3x current cache level
	lsr	x1, x0, x2							// extract cache type bits from clidr
	and	x1, x1, #7							// mask of the bits for current cache only
	cmp	x1, #2							    // see what cache we have at this level
	b.lt	Skip								  // skip if no cache, or just i-cache
	mrs	x9, daif							  // make CSSELR and CCSIDR access atomic
	msr	csselr_el1, x10					// select current cache level in csselr
	isb									        // isb to sync the new cssr&csidr
	mrs	x1, ccsidr_el1					// read the new ccsidr
	msr	daif, x9
	and	x2, x1, #7							// extract the length of the cache lines
	add	x2, x2, #4							// add 4 (line length offset)
	mov	x4, #0x3ff
	and	x4, x4, x1, lsr #3			// find maximum number on the way size
	clz	w5, w4							    // find bit position of way size increment
	mov	x7, #0x7fff
	and	x7, x7, x1, lsr #13     // extract max number of the index size

Loop2:
	mov	x9, x4                  // create working copy of max way size

Loop3:
	lsl	x6, x9, x5
	orr	x11, x10, x6						// factor way and cache number into x11
	lsl	x6, x7, x2
	orr	x11, x11, x6						// factor index number into x11
	dc	cisw, x11							  // clean & invalidate by set/way
	subs	x9, x9, #1						// decrement the way
	b.ge	Loop3
	subs	x7, x7, #1						// decrement the index
	b.ge	Loop2

Skip:
	add	x10, x10, #2						// increment cache number
	cmp	x3, x10
	b.gt	Loop1

Finished:
	dsb sy

	// Invalidate I-Cache
	ic	iallu								    // Invalidate entire instruction cache
	dsb	sy
	isb

	// Turn off MMU, I-Cache, D-Cache
	mrs	 x0, sctlr_el1					// Get control register EL1
	and	 x0, x0, #~CTRL_C_BIT   // Disable D Cache
	and	 x0, x0, #~CTRL_I_BIT   // Disable I Cache
	and	 x0, x0, #~CTRL_M_BIT   // Disable MMU
	msr	 sctlr_el1, x0					// Write control register
	dsb sy
	isb

	// Flush TLB
	tlbi  vmalle1
	dsb sy
	isb

	// Return
	ret