_Head:
  adr   x1, _Payload    /* Set _Entry address */
  b     _Start          /* Jump to the real code */

#if REQUIRES_KERNEL_HEADER == 1
_StackBase:
  .quad FD_BASE         /* Text Offset */

_StackSize:
  .quad FD_SIZE         /* Image Size */
  .quad 0               /* Flags */
  .quad 0               /* Reserved */
  .quad 0               /* Reserved */
  .quad 0               /* Reserved */
  .ascii "ARM\x64"      /* ARM64 Magic */
  .long 0               /* Reserved */
#endif

_Start:
  mov   x4, x1
#if REQUIRES_KERNEL_HEADER == 1
  ldr   x5, _StackBase
#else
  ldr   x5, =FD_BASE
#endif
  cmp   x4, x5
  beq   _Entry
#if REQUIRES_KERNEL_HEADER == 1
  ldr   x6,  _StackSize
#else
  ldr   x6,  =FD_SIZE
#endif

_CopyLoop:
  ldp   x2, x3, [x4], #16
  stp   x2, x3, [x5], #16
  subs  x6, x6, #16
  b.ne  _CopyLoop
#if REQUIRES_KERNEL_HEADER == 1
  ldr   x5, _StackBase
#else
  ldr   x5, =FD_BASE
#endif

_Entry:
  br    x5

_Dead:
  b     _Dead

.text
.align 4

_Payload:
