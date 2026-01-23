#include <AsmMacroLib.h>

ASM_FUNC (PlatformAssemblyInit)
  // Set Stack Pointer to EL1 (Actually for NX Only)
  msr SPSel, #1

  ret
