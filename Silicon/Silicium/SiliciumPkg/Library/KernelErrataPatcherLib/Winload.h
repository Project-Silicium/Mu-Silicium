/**
  Copyright (c) 2021 Samuel Tulach
  Copyright (c) 2022-2023 DuoWoA authors

  SPDX-License-Identifier: MIT
**/

#ifndef _WIN_LOAD_H_
#define _WIN_LOAD_H_

//
// FWP Scan Limit
//
#define MAX_FWP_SCAN_LENGTH 0x300000

//
// Windows Semester Data
//
struct {
  CHAR8  *SemesterName;
  UINT16  TransferToKernelOffset;
  UINT32  TargetInstruction;
} WinSemesterData[] = {
  {
    .SemesterName           = "Legacy",
    .TransferToKernelOffset = 0x400,
    .TargetInstruction      = 0xD2800002
  },
  {
    .SemesterName           = "Germanium",
    .TransferToKernelOffset = 0x480,
    .TargetInstruction      = 0xD2800002
  },
  {
    .SemesterName           = "Selenium",
    .TransferToKernelOffset = 0x850,
    .TargetInstruction      = 0xD2800002
  },
  {
    .SemesterName           = "Bromine",
    .TransferToKernelOffset = 0x8D0,
    .TargetInstruction      = 0x52800014
  },
  {
    .SemesterName           = "Krypton",
    .TransferToKernelOffset = 0xC60,
    .TargetInstruction      = 0x52800015
  }
};

#endif /* _WIN_LOAD_H_ */
