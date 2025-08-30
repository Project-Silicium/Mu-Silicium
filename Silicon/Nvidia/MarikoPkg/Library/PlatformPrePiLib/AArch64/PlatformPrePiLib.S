#include <AsmMacroLib.h>

ASM_FUNC (PlaformAssemblyInit)
  // Set Stack Pointer to EL1 (Actually for NX Only)
  msr SPSel, #1

  ret
