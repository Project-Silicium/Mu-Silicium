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
#include <Protocol/FirmwareVolumeBlock.h>

#include "MshcDxe_7870.h"
#include "MshcDxe_CMD.h"

#define BLEN_512BYTES                  (0x200)

#define MAX_RETRY_COUNT                (100000)

#define MSHC_CLK_400                   (400)
#define MSHC_CLK_25M                   (25000)
#define MSHC_CLK_50M                   (50000)

#define OCR_BUSY                       (0x80000000)
#define OCR_HCS                        (0x40000000)

#define MMC_VDD_165_195                (0x00000080)
#define MMC_VDD_20_21                  (0x00000100)
#define MMC_VDD_21_22                  (0x00000200)
#define MMC_VDD_22_23                  (0x00000400)
#define MMC_VDD_23_24                  (0x00000800)
#define MMC_VDD_24_25                  (0x00001000)
#define MMC_VDD_25_26                  (0x00002000)
#define MMC_VDD_26_27                  (0x00004000)
#define MMC_VDD_27_28                  (0x00008000)
#define MMC_VDD_28_29                  (0x00010000)
#define MMC_VDD_29_30                  (0x00020000)
#define MMC_VDD_30_31                  (0x00040000)
#define MMC_VDD_31_32                  (0x00080000)
#define MMC_VDD_32_33                  (0x00100000)
#define MMC_VDD_33_34                  (0x00200000)
#define MMC_VDD_34_35                  (0x00400000)
#define MMC_VDD_35_36                  (0x00800000)

#define MMC_SWITCH_MODE_CMD_SET        (0x0)
#define MMC_SWITCH_MODE_SET_BITS       (0x1)
#define MMC_SWITCH_MODE_CLEAR_BITS     (0x2)
#define MMC_SWITCH_MODE_WRITE_BYTE     (0x3)

// 
// EXT_CSD Fields
//
#define EXT_CSD_BUS_WIDTH              (183)
#define EXT_CSD_HS_TIMING              (185)
#define EXT_CSD_CARD_TYPE              (196)
#define EXT_CSD_REV                    (192)
#define EXT_CSD_SEC_CNT                (212)
#define BOOT_SIZE_MULTI                (226)
#define PARTITIONING_SUPPORT           (160)

// 
// EXT_CSD Definitions
//
#define EXT_CSD_CMD_SET_NORMAL         (0x1)
#define EXT_CSD_CMD_SET_SECURE         (0x1 << 1)
#define EXT_CSD_CMD_SET_CPSECURE       (0x1 << 2)

#define EXT_CSD_CARD_TYPE_26           (0x1)         // Card can Run at 26MHz
#define EXT_CSD_CARD_TYPE_52           (0x1 << 1)    // Card can Run at 52MHz
#define EXT_CSD_CARD_TYPE_52_DDR_18_30 (0x1 << 2)    // Card can Run at 52MHz DDR 1.8V or 3V
#define EXT_CSD_CARD_TYPE_52_DDR_12    (0x1 << 3)    // Card can Run at 52MHz DDR 1.2V

#define EXT_CSD_BUS_WIDTH_1            (0x0)         // Card is in 1 Bit Mode
#define EXT_CSD_BUS_WIDTH_4            (0x1)         // Card is in 4 Bit Mode
#define EXT_CSD_BUS_WIDTH_8            (0x2)         // Card is in 8 Bit Mode
#define EXT_CSD_BUS_WIDTH_4_DDR        (0x5)         // Card is in 4 Bit DDR Mode
#define EXT_CSD_BUS_WIDTH_8_DDR        (0x6)         // Card is in 8 Bit DDR Mode


typedef struct {
  UINT32 HsMaxDtr;
  UINT32 Sectors;
  UINT32 BootSizeMulti;
  UINT32 PartitioningSupport;
} MmcExtCsd;

typedef struct {
  UINT32  Reserved0          : 7;  // 0
  UINT32  V170_V195          : 1;  // 1.70V - 1.95V
  UINT32  V200_V260          : 7;  // 2.00V - 2.60V
  UINT32  V270_V360          : 9;  // 2.70V - 3.60V
  UINT32  RESERVED_1         : 5;  // Reserved
  UINT32  AccessMode         : 2;  // 00b (Byte Mode), 10b (Sector Mode)
  UINT32  Busy               : 1;  // This Bit is Set to LOW if the Card has not Finished the Power Up Routine
} OCR;

typedef struct {
  UINT32  NOT_USED;                // 1 [0:0]
  UINT32  CRC;                     // CRC7 Checksum [7:1]
  UINT32  MDT;                     // Manufacturing Date [19:8]
  UINT32  RESERVED_1;              // Reserved [23:20]
  UINT32  PSN;                     // Product Serial Bumber [55:24]
  UINT8   PRV;                     // Product Revision [63:56]
  UINT8   PNM[5];                  // Product Name [64:103]
  UINT16  OID;                     // OEM/Application ID [119:104]
  UINT8   MID;                     // Manufacturer ID [127:120]
} CID;

typedef struct {
  UINT8   NOT_USED           : 1;  // Not Used, always 1 [0:0]
  UINT8   CRC                : 7;  // CRC [7:1]

  UINT8   RESERVED_1         : 2;  // Reserved [9:8]
  UINT8   FILE_FORMAT        : 2;  // File Format [11:10]
  UINT8   TMP_WRITE_PROTECT  : 1;  // Temporary Write Protection [12:12]
  UINT8   PERM_WRITE_PROTECT : 1;  // Permanent Write Protection [13:13]
  UINT8   COPY               : 1;  // Copy Flag (OTP) [14:14]
  UINT8   FILE_FORMAT_GRP    : 1;  // File Format Group [15:15]

  UINT16  RESERVED_2         : 5;  // Reserved [20:16]
  UINT16  WRITE_BL_PARTIAL   : 1;  // Partial Blocks for Write Allowed [21:21]
  UINT16  WRITE_BL_LEN       : 4;  // Max Write Data Block Length [25:22]
  UINT16  R2W_FACTOR         : 3;  // Write Speed Factor [28:26]
  UINT16  RESERVED_3         : 2;  // Reserved [30:29]
  UINT16  WP_GRP_ENABLE      : 1;  // Write Protect Group Enable [31:31]
                                   
  UINT32  WP_GRP_SIZE        : 7;  // Write Protect Group Size [38:32]
  UINT32  SECTOR_SIZE        : 7;  // Erase Sector Size [45:39]
  UINT32  ERASE_BLK_EN       : 1;  // Erase Single Block Enable [46:46]
  UINT32  C_SIZE_MULT        : 3;  // Device Size Multiplier [49:47]
  UINT32  VDD_W_CURR_MAX     : 3;  // Max Write Current [52:50]
  UINT32  VDD_W_CURR_MIN     : 3;  // Max write current [55:53]
  UINT32  VDD_R_CURR_MAX     : 3;  // Max Read Current [58:56]
  UINT32  VDD_R_CURR_MIN     : 3;  // Max Read Current [61:59]
  UINT32  C_SIZELow2         : 2;  // Device Size Low Bits [63:62]

  UINT32  C_SIZEHigh10       : 10; // Device Size High Bits [73:64]
  UINT32  RESERVED_4         : 2;  // Reserved [75:74]
  UINT32  DSR_IMP            : 1;  // DSR Implemented [76:76]
  UINT32  READ_BLK_MISALIGN  : 1;  // Read Block Misalignment [77:77]
  UINT32  WRITE_BLK_MISALIGN : 1;  // Write Block Misalignment [78:78]
  UINT32  READ_BL_PARTIAL    : 1;  // Partial Blocks for Read Allowed [79:79]
  UINT32  READ_BL_LEN        : 4;  // Max Read Data Block Length [83:80]
  UINT32  CCC                : 12; // Card Command Classes [95:84]

  UINT8   TRAN_SPEED;              // Max Bus Clock Frequency [103:96]
  UINT8   NSAC;                    // Data Read Access - Time 2 in CLK Cycles (NSAC * 100) [111:104]
  UINT8   TAAC;                    // Data Read Access - Time 1 [119:112]

  UINT8   RESERVED_5         : 6;  // Reserved [125:120]
  UINT8   CSD_STRUCTURE      : 2;  // CSD Structure [127:126]
} CSD;

typedef enum {
  UNKNOWN_CARD,
  MMC_CARD,                        // MMC Card
  SD_CARD,                         // SD 1.1 Card
  SD_CARD_2,                       // SD 2.0 or Above Standard Card
  SD_CARD_2_HIGH,                  // SD 2.0 or Above High Capacity Card
  SD_CARD_MAX
} CARD_TYPE;

typedef struct {
  UINT16    RCA;
  UINTN     BlockSize;
  UINTN     NumBlocks;
  UINTN     TotalNumBlocks;
  UINTN     ClockFrequencySelect;
  CARD_TYPE CardType;
  OCR       OCRData;
  CID       CIDData;
  CSD       CSDData;
  MmcExtCsd ExtCsd;
} CARD_INFO;

typedef struct {
  VENDOR_DEVICE_PATH  Mmc;
  EFI_DEVICE_PATH     End;
} MSHC_DEVICE_PATH;

//
// Global Variables
//

extern EFI_BLOCK_IO_PROTOCOL gBlockIo;
extern EFI_BLOCK_IO_MEDIA    gSdMmcMedia;
extern CARD_INFO             gCardInfo;
extern BOOLEAN               gCardInit;

#endif

