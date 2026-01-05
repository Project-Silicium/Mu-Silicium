/**
  TegraI2cDxe.c - Tegra I2C Controller Driver

  Copyright (c) 2018 naehrwert
  Copyright (c) 2026 Mu-Silicium Project

  SPDX-License-Identifier: GPL-2.0
**/

#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/IoLib.h>
#include <Library/TimerLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/TegraPinmuxLib.h>
#include <Protocol/EfiTegraI2c.h>
#include <T210.h>
#include <TegraClockRegs.h>

//
// I2C Register Offsets
//
#define I2C_CNFG            0x00
#define I2C_CMD_ADDR0       0x04
#define I2C_CMD_DATA1       0x0C
#define I2C_STATUS          0x1C
#define I2C_CLK_DIVISOR     0x6C
#define I2C_CONFIG_LOAD     0x8C

//
// I2C Controller Base Addresses
//
STATIC UINT32 mI2cBases[] = {
  I2C1_BASE,
  I2C2_BASE,
  I2C3_BASE,
  I2C4_BASE,
  I2C5_BASE,
  I2C6_BASE
};

STATIC BOOLEAN mI2cInitialized[6] = { FALSE };

STATIC
EFI_STATUS
I2cWait (
  IN UINT32 Base
  )
{
  UINT32 Timeout;

  MmioWrite32 (Base + I2C_CONFIG_LOAD, 0x25);

  for (Timeout = 0; Timeout < 20; Timeout++) {
    MicroSecondDelay (1);
    if (!(MmioRead32 (Base + I2C_CONFIG_LOAD) & 1)) {
      return EFI_SUCCESS;
    }
  }

  return EFI_TIMEOUT;
}

STATIC
EFI_STATUS
I2cSendPacket (
  IN UINT32 Index,
  IN UINT8  DevAddr,
  IN UINT8  *Buffer,
  IN UINT32 Size
  )
{
  UINT32 Base;
  UINT32 Data = 0;
  UINT32 Timeout;

  if (Index >= 6 || Size > 4 || Buffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Base = mI2cBases[Index];
  CopyMem (&Data, Buffer, Size);

  MmioWrite32 (Base + I2C_CMD_ADDR0, DevAddr << 1);
  MmioWrite32 (Base + I2C_CMD_DATA1, Data);
  MmioWrite32 (Base + I2C_CNFG, ((Size * 2 - 2) << 1) | 0x2800);

  if (EFI_ERROR (I2cWait (Base))) {
    return EFI_TIMEOUT;
  }

  MmioWrite32 (Base + I2C_CNFG, (MmioRead32 (Base + I2C_CNFG) & ~0x200) | 0x200);

  for (Timeout = 0; Timeout < 1000; Timeout++) {
    MicroSecondDelay (10);
    if (!(MmioRead32 (Base + I2C_STATUS) & 0x100)) {
      break;
    }
  }

  if (MmioRead32 (Base + I2C_STATUS) & 0xF0) {
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
I2cRecvPacket (
  IN  UINT32 Index,
  IN  UINT8  DevAddr,
  OUT UINT8  *Buffer,
  IN  UINT32 Size
  )
{
  UINT32 Base;
  UINT32 Data;
  UINT32 Timeout;

  if (Index >= 6 || Size > 4 || Buffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Base = mI2cBases[Index];

  MmioWrite32 (Base + I2C_CMD_ADDR0, (DevAddr << 1) | 1);
  MmioWrite32 (Base + I2C_CNFG, ((Size * 2 - 2) << 1) | 0x2840);

  if (EFI_ERROR (I2cWait (Base))) {
    return EFI_TIMEOUT;
  }

  MmioWrite32 (Base + I2C_CNFG, (MmioRead32 (Base + I2C_CNFG) & ~0x200) | 0x200);

  for (Timeout = 0; Timeout < 1000; Timeout++) {
    MicroSecondDelay (10);
    if (!(MmioRead32 (Base + I2C_STATUS) & 0x100)) {
      break;
    }
  }

  if (MmioRead32 (Base + I2C_STATUS) & 0xF0) {
    return EFI_DEVICE_ERROR;
  }

  Data = MmioRead32 (Base + I2C_CMD_DATA1);
  CopyMem (Buffer, &Data, Size);

  return EFI_SUCCESS;
}

STATIC
VOID
I2cEnableClock (
  IN UINT32 Index
  )
{
  UINT32 SrcReg;
  UINT32 RstClr;
  UINT32 EnbSet;
  UINT32 ClkBit;

  switch (Index) {
  case 0:
    SrcReg = CLK_RST_CONTROLLER_CLK_SOURCE_I2C1;
    RstClr = CLK_RST_CONTROLLER_RST_DEV_L_CLR;
    EnbSet = CLK_RST_CONTROLLER_CLK_ENB_L_SET;
    ClkBit = CLK_L_I2C1;
    break;
  case 1:
    SrcReg = CLK_RST_CONTROLLER_CLK_SOURCE_I2C2;
    RstClr = CLK_RST_CONTROLLER_RST_DEV_H_CLR;
    EnbSet = CLK_RST_CONTROLLER_CLK_ENB_H_SET;
    ClkBit = CLK_H_I2C2;
    break;
  case 2:
    SrcReg = CLK_RST_CONTROLLER_CLK_SOURCE_I2C3;
    RstClr = CLK_RST_CONTROLLER_RST_DEV_U_CLR;
    EnbSet = CLK_RST_CONTROLLER_CLK_ENB_U_SET;
    ClkBit = CLK_U_I2C3;
    break;
  case 3:
    SrcReg = CLK_RST_CONTROLLER_CLK_SOURCE_I2C4;
    RstClr = CLK_RST_CONTROLLER_RST_DEV_V_CLR;
    EnbSet = CLK_RST_CONTROLLER_CLK_ENB_V_SET;
    ClkBit = CLK_V_I2C4;
    break;
  case 4:
    SrcReg = CLK_RST_CONTROLLER_CLK_SOURCE_I2C5;
    RstClr = CLK_RST_CONTROLLER_RST_DEV_H_CLR;
    EnbSet = CLK_RST_CONTROLLER_CLK_ENB_H_SET;
    ClkBit = CLK_H_I2C5;
    break;
  case 5:
    SrcReg = CLK_RST_CONTROLLER_CLK_SOURCE_I2C6;
    RstClr = CLK_RST_CONTROLLER_RST_DEV_X_CLR;
    EnbSet = CLK_RST_CONTROLLER_CLK_ENB_X_SET;
    ClkBit = CLK_X_I2C6;
    break;
  default:
    return;
  }

  // Set clock source to PLLP, divisor 19 (~20MHz)
  MmioWrite32 (CLOCK_BASE + SrcReg, 19);
  MmioWrite32 (CLOCK_BASE + EnbSet, ClkBit);
  MicroSecondDelay (10);
  MmioWrite32 (CLOCK_BASE + RstClr, ClkBit);
  MicroSecondDelay (10);
}

STATIC
EFI_STATUS
EFIAPI
TegraI2cInit (
  IN UINT32 I2cIndex
  )
{
  UINT32 Base;

  if (I2cIndex >= 6) {
    return EFI_INVALID_PARAMETER;
  }

  if (mI2cInitialized[I2cIndex]) {
    return EFI_SUCCESS;
  }

  Base = mI2cBases[I2cIndex];

  I2cEnableClock (I2cIndex);
  PinmuxConfigI2c (I2cIndex);

  MmioWrite32 (Base + I2C_CLK_DIVISOR, 0x50001);
  MmioWrite32 (Base + I2C_CNFG, 0x90003);

  I2cWait (Base);
  MicroSecondDelay (20000);

  mI2cInitialized[I2cIndex] = TRUE;
  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
EFIAPI
TegraI2cSendByte (
  IN UINT32 I2cIndex,
  IN UINT8  DevAddr,
  IN UINT8  RegAddr,
  IN UINT8  Data
  )
{
  UINT8 Buffer[2];

  Buffer[0] = RegAddr;
  Buffer[1] = Data;

  return I2cSendPacket (I2cIndex, DevAddr, Buffer, 2);
}

STATIC
EFI_STATUS
EFIAPI
TegraI2cRecvByte (
  IN  UINT32 I2cIndex,
  IN  UINT8  DevAddr,
  IN  UINT8  RegAddr,
  OUT UINT8  *Data
  )
{
  EFI_STATUS Status;

  Status = I2cSendPacket (I2cIndex, DevAddr, &RegAddr, 1);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return I2cRecvPacket (I2cIndex, DevAddr, Data, 1);
}

STATIC
EFI_STATUS
EFIAPI
TegraI2cSendBuffer (
  IN UINT32 I2cIndex,
  IN UINT8  DevAddr,
  IN UINT8  RegAddr,
  IN UINT8  *Buffer,
  IN UINT32 Size
  )
{
  UINT8 TxBuffer[4];

  if (Size > 3) {
    return EFI_INVALID_PARAMETER;
  }

  TxBuffer[0] = RegAddr;
  CopyMem (&TxBuffer[1], Buffer, Size);

  return I2cSendPacket (I2cIndex, DevAddr, TxBuffer, Size + 1);
}

STATIC
EFI_STATUS
EFIAPI
TegraI2cRecvBuffer (
  IN  UINT32 I2cIndex,
  IN  UINT8  DevAddr,
  IN  UINT8  RegAddr,
  OUT UINT8  *Buffer,
  IN  UINT32 Size
  )
{
  EFI_STATUS Status;

  Status = I2cSendPacket (I2cIndex, DevAddr, &RegAddr, 1);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return I2cRecvPacket (I2cIndex, DevAddr, Buffer, Size);
}

STATIC EFI_TEGRA_I2C_PROTOCOL mTegraI2cProtocol = {
  TegraI2cInit,
  TegraI2cSendByte,
  TegraI2cRecvByte,
  TegraI2cSendBuffer,
  TegraI2cRecvBuffer
};

EFI_STATUS
EFIAPI
TegraI2cDxeInitialize (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  EFI_STATUS Status;

  // Initialize I2C5 for PMIC
  Status = TegraI2cInit (I2C_5);

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gEfiTegraI2cProtocolGuid,
                  &mTegraI2cProtocol,
                  NULL
                  );

  return Status;
}
