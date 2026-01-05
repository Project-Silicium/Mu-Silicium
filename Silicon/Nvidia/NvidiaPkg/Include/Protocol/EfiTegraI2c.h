/**
  EfiTegraI2c.h - Tegra I2C Protocol Definition

  Copyright (c) 2026 Mu-Silicium Project

  SPDX-License-Identifier: GPL-2.0
**/

#ifndef _EFI_TEGRA_I2C_H_
#define _EFI_TEGRA_I2C_H_

#include <Uefi.h>

#define EFI_TEGRA_I2C_PROTOCOL_GUID \
  { 0x8e1b5c3a, 0x6f2d, 0x4e8a, { 0x9b, 0x3c, 0x7d, 0x4e, 0x5f, 0x6a, 0x8b, 0x9c } }

extern EFI_GUID gEfiTegraI2cProtocolGuid;

typedef struct _EFI_TEGRA_I2C_PROTOCOL EFI_TEGRA_I2C_PROTOCOL;

typedef
EFI_STATUS
(EFIAPI *EFI_TEGRA_I2C_INIT)(
  IN UINT32 I2cIndex
  );

typedef
EFI_STATUS
(EFIAPI *EFI_TEGRA_I2C_SEND_BYTE)(
  IN UINT32 I2cIndex,
  IN UINT8  DevAddr,
  IN UINT8  RegAddr,
  IN UINT8  Data
  );

typedef
EFI_STATUS
(EFIAPI *EFI_TEGRA_I2C_RECV_BYTE)(
  IN  UINT32 I2cIndex,
  IN  UINT8  DevAddr,
  IN  UINT8  RegAddr,
  OUT UINT8  *Data
  );

typedef
EFI_STATUS
(EFIAPI *EFI_TEGRA_I2C_SEND_BUFFER)(
  IN UINT32 I2cIndex,
  IN UINT8  DevAddr,
  IN UINT8  RegAddr,
  IN UINT8  *Buffer,
  IN UINT32 Size
  );

typedef
EFI_STATUS
(EFIAPI *EFI_TEGRA_I2C_RECV_BUFFER)(
  IN  UINT32 I2cIndex,
  IN  UINT8  DevAddr,
  IN  UINT8  RegAddr,
  OUT UINT8  *Buffer,
  IN  UINT32 Size
  );

struct _EFI_TEGRA_I2C_PROTOCOL {
  EFI_TEGRA_I2C_INIT        Init;
  EFI_TEGRA_I2C_SEND_BYTE   SendByte;
  EFI_TEGRA_I2C_RECV_BYTE   RecvByte;
  EFI_TEGRA_I2C_SEND_BUFFER SendBuffer;
  EFI_TEGRA_I2C_RECV_BUFFER RecvBuffer;
};

#endif
