	.arch	armv8-a
	.file	"PoC.c"
	.text
	.align	2
	.global	PreOslArm64TransferToKernel
	.type	PreOslArm64TransferToKernel, %function
PreOslArm64TransferToKernel:
	.cfi_startproc
				stp			x29, x30, [sp, -16]!
	.cfi_def_cfa_offset	16
	.cfi_offset	29, -16
	.cfi_offset	30, -8
				mov			x29, sp

				tbz			x10, #0x28, .no_irm				// Check if IRM bit is set (bit 40)
				and			x10, x10, #0xfffffeffffffffff	// Remove IRM bit (bit 40)
				orr			x10, x10, #1					// Add bit for target list = 1

				mov			x22, x10						// Move current SGI value into X22
				mrs			x9, mpidr_el1					// Current CPU MPIDR
				and			x9, x9, #0xF00					// AND with 0xF00 to only have Affinity 1

				mov			x20, #0							// Current CPU Index
				mov			w21, #8							// Max CPU Index
.loop:
				cmp			x20, x9,lsr#8					// Compare current CPU index with (Affinity 1) >> 8
				b.eq			.skip							// If equal, move on to the next one
				lsl			x8, x20, #0x10					// Shift current CPU index by 16
				orr			x10, x22, x8					// Add affinity to the SGI register value
				msr			icc_sgi1r_el1, x10				// Write SGI1R register
				dsb			sy								// Synchronization barrier
.skip:
				add			x20, x20, #1					// 
				sub			w21, w21, #1					// 
				cbnz			w21, .loop						// 

				b			.exit							// 

.no_irm:
				msr			icc_sgi1r_el1, x10				// 

.exit:
				ldp			x29, x30, [sp], 16
	.cfi_restore 30
	.cfi_restore 29
	.cfi_def_cfa_offset 0
				ret
	.cfi_endproc
.LFE0:
	.size	PreOslArm64TransferToKernel, .-PreOslArm64TransferToKernel
	.ident	"GCC:	(Ubuntu	11.4.0-1ubuntu1~22.04)	11.4.0"
	.section	.note.GNU-stack,"",@progbits
