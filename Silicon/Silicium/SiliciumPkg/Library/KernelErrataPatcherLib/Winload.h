/**
  Copyright (c) 2021 Samuel Tulach
  Copyright (c) 2022-2023 DuoWoA authors
  Copyright (c) 2026 Project Silicium

  SPDX-License-Identifier: MIT
**/

#ifndef _WIN_LOAD_H_
#define _WIN_LOAD_H_

//
// Windows Codebase Semesters
//
struct {
  CHAR8  *Name;
  UINT16  TransferToKernelOffset;
  UINT32  TargetInstruction;
} CodebaseSemester[] = {
  {
    .Name                   = "Legacy",
    .TransferToKernelOffset = 0x1000,
    .TargetInstruction      = 0xD2800002
  },
  {
    .Name                   = "Germanium",
    .TransferToKernelOffset = 0x1080,
    .TargetInstruction      = 0xD2800002
  },
  {
    .Name                   = "Vibranium",
    .TransferToKernelOffset = 0x1090,
    .TargetInstruction      = 0xD2800002
  },
  {
    .Name                   = "Selenium",
    .TransferToKernelOffset = 0x1450,
    .TargetInstruction      = 0xD2800002
  },
  {
    .Name                   = "Bromine",
    .TransferToKernelOffset = 0x14D0,
    .TargetInstruction      = 0x52800014
  },
  {
    .Name                   = "Krypton",
    .TransferToKernelOffset = 0x1860,
    .TargetInstruction      = 0x52800015
  },
  {
    .Name                   = "Rubidium",
    .TransferToKernelOffset = 0x19D0,
    .TargetInstruction      = 0x52800015
  }
};

#endif /* _WIN_LOAD_H_ */
