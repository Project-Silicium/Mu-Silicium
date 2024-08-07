_Head:
  adr   x1, _Payload    /* Set _Entry address */
  b     _Start          /* Jump to the real code */

_StackBase:
  .quad UEFI_BASE       /* Text Offset */

_StackSize:
  .quad UEFI_SIZE       /* Image Size */
  .quad 0               /* Flags */
  .quad 0               /* Reserved */
  .quad 0               /* Reserved */
  .quad 0               /* Reserved */
  .ascii "ARM\x64"      /* ARM64 Magic */
  .long 0               /* Reserved */

_Start:
  mov   x11, CREATE_FDT_POINTER
  cmp   x11, 1
  b.ne  skip_dtb
  mov   x10, FDT_POINTER_ADDRESS
  str   x0, [x10]

skip_dtb:
  mov   x4, x1
  ldr   x5, _StackBase
  cmp   x4, x5
  beq   _Entry
  ldr   x6,  _StackSize

_CopyLoop:
  ldp   x2, x3, [x4], #16
  stp   x2, x3, [x5], #16
  subs  x6, x6, #16
  b.ne  _CopyLoop
  ldr   x5, _StackBase

_Entry:
  br    x5

_Dead:
  b     _Dead           /* We should never get here */

.text
.align 4

_Payload:
  /* Your code will get ran right after this binary */
