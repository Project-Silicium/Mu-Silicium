/** @file
  Copyright (c) 2011, Samsung Electronics Co. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
**/

#ifndef _MSHCDXE_H_
#define _MSHCDXE_H_

#include <Protocol/BlockIo.h>
#include <Protocol/DevicePath.h>

#include <Library/MshcHostBridge.h>

#include "MshcHost.h"
#include "MshcDxe_CMD.h"

#define ACMD41_TIMEOUT_MS              (1000)

typedef enum {
  UNKNOWN_CARD,
  SD_CARD,
  SD_CARD_2,
  SD_CARD_2_HIGH,
  MMC_CARD
} CARD_TYPE;

//
// Extended CSD
//
#define EXT_CSD_SIZE                   (512)
#define EXT_CSD_BUS_WIDTH              (183)
#define EXT_CSD_HS_TIMING              (185)
#define EXT_CSD_REV                    (192)
#define EXT_CSD_SEC_CNT                (212)

#define EXT_CSD_BUS_WIDTH_1            (0)
#define EXT_CSD_BUS_WIDTH_4            (1)
#define EXT_CSD_BUS_WIDTH_8            (2)

#define EXT_CSD_HS_TIMING_LEGACY       (0)
#define EXT_CSD_HS_TIMING_HS           (1)

#define MMC_SWITCH_WRITE_BYTE          (3)
#define MMC_SWITCH_ARG(Index, Value) \
  (((UINT32)MMC_SWITCH_WRITE_BYTE << 24) | ((UINT32)(Index) << 16) | ((UINT32)(Value) << 8))


#define EXT_CSD_REV_SEC_CNT_VALID      (2)

#define MMC_OCR_SECTOR_MODE            (0x40000000)
#define MMC_OCR_VOLTAGE_WINDOW         (0x00FF8000)
#define MMC_OP_COND_TIMEOUT_MS         (1000)

typedef struct {
  UINT16    RCA;
  CARD_TYPE CardType;
  BOOLEAN   HighCapacity;
  BOOLEAN   SupportsUhs;
  BOOLEAN   BusAt1V8;

  //
  // Taken from the card's configuration register.
  //
  UINT8     SpecVersion;
  BOOLEAN   Supports4Bit;
  UINTN     BlockSize;
  UINTN     NumBlocks;
  UINT32    OCR;
  UINT32    CID[4];
  UINT32    CSD[4];
} CARD_INFO;

typedef struct {
  VENDOR_DEVICE_PATH  Mmc;
  EFI_DEVICE_PATH     End;
} MSHC_DEVICE_PATH;

//
// Global Variables
//
extern CARD_INFO gCardInfo;

/**
  Works out how many Blocks the Card holds from its CSD.
**/
EFI_STATUS
MshcDecodeCsd (VOID);

/**
  Brings an eMMC from Idle to Standby.

  @retval EFI_UNSUPPORTED       Nothing Answered, there is no eMMC here.
**/
EFI_STATUS
MshcMmcIdentify (VOID);

/**
  Takes an Identified eMMC up to its Working Width and Speed.
**/
EFI_STATUS
MshcMmcConfigure (VOID);

#endif /* _MSHCDXE_H_ */
