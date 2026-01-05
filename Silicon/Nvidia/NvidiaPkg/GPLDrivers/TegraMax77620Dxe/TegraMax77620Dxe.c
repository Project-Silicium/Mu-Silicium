/**
  TegraMax77620Dxe.c - MAX77620 PMIC Driver

  Copyright (c) 2018 naehrwert

  SPDX-License-Identifier: GPL-2.0
**/

#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/TimerLib.h>
#include <Protocol/EfiTegraI2c.h>
#include <Protocol/EfiTegraMax77620.h>
#include <Max77620.h>
#include <T210.h>

STATIC EFI_TEGRA_I2C_PROTOCOL *mI2c = NULL;

//
// Regulator Configuration
//
typedef struct {
  UINT8   Type;
  UINT32  MvStep;
  UINT32  MvMin;
  UINT32  MvMax;
  UINT8   VoltAddr;
  UINT8   CfgAddr;
  UINT8   VoltMask;
  UINT8   EnableMask;
  UINT8   EnableShift;
} REGULATOR_CONFIG;

#define REG_TYPE_SD   0
#define REG_TYPE_LDO  1

STATIC CONST REGULATOR_CONFIG mRegulators[] = {
  // SD0-SD3
  { REG_TYPE_SD,  12500,  600000, 1400000, MAX77620_REG_SD0, MAX77620_REG_SD0_CFG, MAX77620_SD0_VOLT_MASK, 0x30, 4 },
  { REG_TYPE_SD,  12500,  600000, 1125000, MAX77620_REG_SD1, MAX77620_REG_SD1_CFG, MAX77620_SD1_VOLT_MASK, 0x30, 4 },
  { REG_TYPE_SD,  12500,  600000, 1350000, MAX77620_REG_SD2, MAX77620_REG_SD2_CFG, MAX77620_SDX_VOLT_MASK, 0x30, 4 },
  { REG_TYPE_SD,  12500,  600000, 1800000, MAX77620_REG_SD3, MAX77620_REG_SD3_CFG, MAX77620_SDX_VOLT_MASK, 0x30, 4 },
  // LDO0-LDO8
  { REG_TYPE_LDO, 25000,  800000, 1200000, MAX77620_REG_LDO0_CFG, MAX77620_REG_LDO0_CFG2, MAX77620_LDO_VOLT_MASK, 0xC0, 6 },
  { REG_TYPE_LDO, 25000,  800000, 1050000, MAX77620_REG_LDO1_CFG, MAX77620_REG_LDO1_CFG2, MAX77620_LDO_VOLT_MASK, 0xC0, 6 },
  { REG_TYPE_LDO, 50000,  800000, 3300000, MAX77620_REG_LDO2_CFG, MAX77620_REG_LDO2_CFG2, MAX77620_LDO_VOLT_MASK, 0xC0, 6 },
  { REG_TYPE_LDO, 50000,  800000, 3100000, MAX77620_REG_LDO3_CFG, MAX77620_REG_LDO3_CFG2, MAX77620_LDO_VOLT_MASK, 0xC0, 6 },
  { REG_TYPE_LDO, 12500,  800000,  850000, MAX77620_REG_LDO4_CFG, MAX77620_REG_LDO4_CFG2, MAX77620_LDO_VOLT_MASK, 0xC0, 6 },
  { REG_TYPE_LDO, 50000,  800000, 1800000, MAX77620_REG_LDO5_CFG, MAX77620_REG_LDO5_CFG2, MAX77620_LDO_VOLT_MASK, 0xC0, 6 },
  { REG_TYPE_LDO, 50000,  800000, 2900000, MAX77620_REG_LDO6_CFG, MAX77620_REG_LDO6_CFG2, MAX77620_LDO_VOLT_MASK, 0xC0, 6 },
  { REG_TYPE_LDO, 50000,  800000, 1050000, MAX77620_REG_LDO7_CFG, MAX77620_REG_LDO7_CFG2, MAX77620_LDO_VOLT_MASK, 0xC0, 6 },
  { REG_TYPE_LDO, 50000,  800000, 1050000, MAX77620_REG_LDO8_CFG, MAX77620_REG_LDO8_CFG2, MAX77620_LDO_VOLT_MASK, 0xC0, 6 },
};

#define REGULATOR_COUNT (sizeof(mRegulators) / sizeof(mRegulators[0]))

STATIC
EFI_STATUS
PmicSendByte (
  IN UINT8 RegAddr,
  IN UINT8 Data
  )
{
  return mI2c->SendByte (I2C_5, MAX77620_I2C_ADDR, RegAddr, Data);
}

STATIC
EFI_STATUS
PmicRecvByte (
  IN  UINT8 RegAddr,
  OUT UINT8 *Data
  )
{
  return mI2c->RecvByte (I2C_5, MAX77620_I2C_ADDR, RegAddr, Data);
}

STATIC
EFI_STATUS
EFIAPI
Max77620SetVoltage (
  IN UINT32 Id,
  IN UINT32 Mv
  )
{
  CONST REGULATOR_CONFIG *Reg;
  UINT8  Val;
  UINT32 Mult;
  EFI_STATUS Status;

  if (Id >= REGULATOR_COUNT) {
    return EFI_INVALID_PARAMETER;
  }

  Reg = &mRegulators[Id];

  if (Mv < Reg->MvMin || Mv > Reg->MvMax) {
    return EFI_INVALID_PARAMETER;
  }

  Mult = (Mv - Reg->MvMin) / Reg->MvStep;

  Status = PmicRecvByte (Reg->VoltAddr, &Val);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Val = (Val & ~Reg->VoltMask) | (Mult & Reg->VoltMask);

  Status = PmicSendByte (Reg->VoltAddr, Val);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  MicroSecondDelay (1000);
  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
EFIAPI
Max77620Enable (
  IN UINT32  Id,
  IN BOOLEAN Enable
  )
{
  CONST REGULATOR_CONFIG *Reg;
  UINT8  Val;
  UINT8  Addr;
  EFI_STATUS Status;

  if (Id >= REGULATOR_COUNT) {
    return EFI_INVALID_PARAMETER;
  }

  Reg = &mRegulators[Id];
  Addr = (Reg->Type == REG_TYPE_SD) ? Reg->CfgAddr : Reg->VoltAddr;

  Status = PmicRecvByte (Addr, &Val);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (Enable) {
    Val = (Val & ~Reg->EnableMask) | ((3 << Reg->EnableShift) & Reg->EnableMask);
  }
  else {
    Val &= ~Reg->EnableMask;
  }

  Status = PmicSendByte (Addr, Val);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  MicroSecondDelay (1000);
  return EFI_SUCCESS;
}

STATIC
UINT32
EFIAPI
Max77620GetStatus (
  IN UINT32 Id
  )
{
  CONST REGULATOR_CONFIG *Reg;
  UINT8 Val;

  if (Id >= REGULATOR_COUNT) {
    return 0;
  }

  Reg = &mRegulators[Id];

  if (Reg->Type == REG_TYPE_SD) {
    if (EFI_ERROR (PmicRecvByte (MAX77620_REG_STATSD, &Val))) {
      return 0;
    }
    return (Val & (1 << (Id + 4))) ? 0 : 1;
  }
  else {
    if (EFI_ERROR (PmicRecvByte (Reg->CfgAddr, &Val))) {
      return 0;
    }
    return (Val & 0x08) ? 1 : 0;
  }
}

STATIC
EFI_STATUS
EFIAPI
Max77620EnableSdCardPower (
  VOID
  )
{
  EFI_STATUS Status;

  // LDO2 @ 3.3V for SD card
  Status = Max77620SetVoltage (MAX77620_REGULATOR_LDO2, 3300000);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = Max77620Enable (MAX77620_REGULATOR_LDO2, TRUE);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  MicroSecondDelay (10000);
  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
EFIAPI
Max77620EnableTouchPower (
  VOID
  )
{
  EFI_STATUS Status;

  // LDO6 @ 2.9V for touchscreen
  Status = Max77620SetVoltage (MAX77620_REGULATOR_LDO6, 2900000);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = Max77620Enable (MAX77620_REGULATOR_LDO6, TRUE);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  MicroSecondDelay (10000);
  return EFI_SUCCESS;
}

STATIC EFI_TEGRA_MAX77620_PROTOCOL mMax77620Protocol = {
  Max77620SetVoltage,
  Max77620Enable,
  Max77620GetStatus,
  Max77620EnableSdCardPower,
  Max77620EnableTouchPower
};

EFI_STATUS
EFIAPI
TegraMax77620DxeInitialize (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  EFI_STATUS Status;
  UINT8      Cid;

  Status = gBS->LocateProtocol (&gEfiTegraI2cProtocolGuid, NULL, (VOID **)&mI2c);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = mI2c->Init (I2C_5);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = PmicRecvByte (MAX77620_REG_CID4, &Cid);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Enable SD card power
  Status = Max77620EnableSdCardPower ();

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gEfiTegraMax77620ProtocolGuid,
                  &mMax77620Protocol,
                  NULL
                  );

  return Status;
}
