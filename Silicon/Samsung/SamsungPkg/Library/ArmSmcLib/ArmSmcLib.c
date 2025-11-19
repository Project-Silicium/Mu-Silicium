/**
  Copyright (c) 2021, NUVIA Inc. All rights reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/ArmSmcLib.h>
#include <Library/BaseMemoryLib.h>

VOID
ArmCallSmc (IN OUT ARM_SMC_ARGS *Args)
{
  // Do Nothing
}

UINTN
ArmCallSmc3 (
  IN     UINTN  Function,
  IN OUT UINTN *Arg1 OPTIONAL,
  IN OUT UINTN *Arg2 OPTIONAL,
  IN OUT UINTN *Arg3 OPTIONAL)
{
  // Set SMC Parameter
  register UINT64 Reg0 __asm__ ("x0") = (UINT64)Function;
  register UINT64 Reg1 __asm__ ("x1") = (UINT64)Arg1;
  register UINT64 Reg2 __asm__ ("x2") = (UINT64)Arg2;
  register UINT64 Reg3 __asm__ ("x3") = (UINT64)Arg3;

  // Call SMC
  __asm__ volatile ("dsb\tsy\n" "smc\t0\n" : "+r"(Reg0), "+r"(Reg1), "+r"(Reg2), "+r"(Reg3));

  return Reg0;
}

UINTN
ArmCallSmc2 (
  IN     UINTN  Function,
  IN OUT UINTN *Arg1 OPTIONAL,
  IN OUT UINTN *Arg2 OPTIONAL,
  OUT    UINTN *Arg3 OPTIONAL)
{
  return ArmCallSmc3 (Function, Arg1, Arg2, 0);
}

UINTN
ArmCallSmc1 (
  IN     UINTN  Function,
  IN OUT UINTN *Arg1 OPTIONAL,
  OUT    UINTN *Arg2 OPTIONAL,
  OUT    UINTN *Arg3 OPTIONAL)
{
  return ArmCallSmc3 (Function, Arg1, 0, 0);
}

UINTN
ArmCallSmc0 (
  IN  UINTN  Function,
  OUT UINTN *Arg1 OPTIONAL,
  OUT UINTN *Arg2 OPTIONAL,
  OUT UINTN *Arg3 OPTIONAL)
{
  return ArmCallSmc3 (Function, 0, 0, 0);
}
