.code 32
.text

.globl _start

_start:
  adr   r0, _Payload    /* Load the payload adress into r0 */
  ldr   r1, UefiBase    /* Load the expected payload base into r1 */
  cmp   r0, r1          /* Compare the adresses */
  beq   _Entry          /* Jump to entry if the adress is the same */
  ldr   r2, UefiSize

_CopyLoop:
  ldr   r3, [r0], #4
  str   r3, [r1], #4
  subs  r2, r2, #4      /* substract the size that got copied from fd size */
  bgt   _CopyLoop       /* branch if greater than zero */

_Entry:
  isb
  dsb
  ldr   r5, UefiBase
  bx    r5

UefiBase:
  .word UEFI_BASE

UefiSize:
  .word UEFI_SIZE

.text
.align 4

_Payload:
  /* code will get ran right after this binary */
