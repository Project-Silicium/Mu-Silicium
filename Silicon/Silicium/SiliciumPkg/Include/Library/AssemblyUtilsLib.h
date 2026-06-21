/**
  Copyright (c) 2021 Samuel Tulach
  Copyright (c) 2022-2023 DuoWoA authors

  SPDX-License-Identifier: MIT
**/

#ifndef _ASSEMBLY_UTILS_H_
#define _ASSEMBLY_UTILS_H_

//
// ARM64 Instruction Details
//
#define ARM64_INSTRUCTION_LENGTH                                        4
#define ARM64_TOTAL_INSTRUCTION_LENGTH(x)                              (ARM64_INSTRUCTION_LENGTH * (x))
#define ARM64_INSTRUCTION(x)                                           (*(UINT32 *)(x))

//
// ARM64 Instruction Helpers
//
#define ARM64_BRANCH_LOCATION_INSTRUCTION(CurrentOffset, TargetOffset) (0x94000000U | ((UINT32)(((TargetOffset) - (CurrentOffset)) / ARM64_INSTRUCTION_LENGTH) & 0x7FFFFFFU))

#endif /* _ASSEMBLY_UTILS_H_ */
