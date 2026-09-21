#ifndef _HYP_LOADER_H_
#define _HYP_LOADER_H_

#include <Library/PcdLib.h>

//
// SMC Function IDs
//
#define SMC_VMM_CALL_MAGIC_64 0xC2000400
#define SMC_VMM_CALL_MAGIC_32 0x82000400

//
// HYP Start Arguments
//
#define VMM_STACK_OFFSET FixedPcdGet16 (PcdVmmStackOffset)
#define VMM_MODE_AARCH32 0
#define VMM_MODE_AARCH64 1

#endif /* _HYP_LOADER_H_ */
