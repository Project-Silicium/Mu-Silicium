/**
  Copyright (C) Samsung Electronics Co. LTD

  This software is proprietary of Samsung Electronics.
  No part of this software, either material or conceptual may be copied or distributed, transmitted,
  transcribed, stored in a retrieval system or translated into any human or computer language in any form by any means,
  electronic, mechanical, manual or otherwise, or disclosed
  to third parties without the express written permission of Samsung Electronics.
**/

#ifndef _EFI_EXYNOS_SPEEDY_H_
#define _EFI_EXYNOS_SPEEDY_H_

#define ACCESS_BURST     0
#define ACCESS_RANDOM    1
#define DIRECTION_READ   0
#define DIRECTION_WRITE  1

//
// Global GUID for the Exynos Speedy Protocol
//
#define EFI_EXYNOS_SPEEDY_PROTOCOL_GUID { 0x8504E67D, 0xB696, 0x445C, { 0xBD, 0x86, 0x99, 0x74, 0x85, 0xB9, 0x4C, 0xD3 } }

//
// Declare forward Reference to the Exynos Speedy Protocol
//
typedef struct _EFI_EXYNOS_SPEEDY_PROTOCOL EFI_EXYNOS_SPEEDY_PROTOCOL;

/**
  This Function Resets Speedy Fifo.

  @param SpeedyBase         - The Speedy Base Address.
**/
typedef
VOID
(EFIAPI *EFI_RESET_SPEEDY_FIFO) (
  UINT32 SpeedyBase
  );

/**
  This Function Reads from Speedy.

  @param SpeedyBase         - The Speedy Base Address.
  @param Slave              - The Slave Address.
  @param Addr               - The Address.
  @param Data               - The Data of Speedy.
**/
typedef
VOID
(EFIAPI *EFI_READ_SPEEDY) (
  UINT32 SpeedyBase,
  UINT8  Slave,
  UINT8  Addr,
  UINT8 *Data
  );

/**
  This Function Burst Reads from Speedy.

  @param SpeedyBase         - The Speedy Base Address.
  @param Slave              - The Slave Address.
  @param Addr               - The Address.
  @param Data               - The Data of Speedy.
  @param Count              - The Count of Reads.
**/
typedef
VOID
(EFIAPI *EFI_BURST_READ_SPEEDY) (
  UINT32 SpeedyBase,
  UINT8  Slave,
  UINT8  Addr,
  UINT8 *Data,
  UINT8  Count
  );

/**
  This Function Writes to Speedy.

  @param SpeedyBase         - The Speedy Base Address.
  @param Slave              - The Slave Address.
  @param Addr               - The Address.
  @param Data               - The Data for Speedy.
**/
typedef
VOID
(EFIAPI *EFI_WRITE_SPEEDY) (
  UINT32 SpeedyBase,
  UINT8  Slave,
  UINT8  Addr,
  UINT8  Data
  );

/**
  This Function Burst Writes to Speedy.

  @param SpeedyBase         - The Speedy Base Address.
  @param Slave              - The Slave Address.
  @param Addr               - The Address.
  @param Data               - The Data for Speedy.
  @param Count              - The Count of Writes.
**/
typedef
VOID
(EFIAPI *EFI_BURST_WRITE_SPEEDY) (
  UINT32 SpeedyBase,
  UINT8  Slave,
  UINT8  Addr,
  UINT8 *Data,
  UINT8  Count
  );

//
// Define Protocol Functions
//
struct _EFI_EXYNOS_SPEEDY_PROTOCOL {
  EFI_RESET_SPEEDY_FIFO  ResetFifo;
  EFI_READ_SPEEDY        ReadSpeedy;
  EFI_BURST_READ_SPEEDY  BurstReadSpeedy;
  EFI_WRITE_SPEEDY       WriteSpeedy;
  EFI_BURST_WRITE_SPEEDY BurstWriteSpeedy;
};

extern EFI_GUID gEfiExynosSpeedyProtocolGuid;

#endif /* _EFI_EXYNOS_SPEEDY_H_ */