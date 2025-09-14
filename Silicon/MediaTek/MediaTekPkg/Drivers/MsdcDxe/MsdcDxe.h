/** @file

  Provides some data structure definitions used by MediaTek
  Memory Stick and SD card host controller driver.

  Copyright (c) 2025, Arseniy Velikanov <me@adomerle.pw>

  SPDX-License-Identifier: GPL-2.0-only
**/

#ifndef _MSDC_DXE_H_
#define _MSDC_DXE_H_

#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/MemoryMapHelperLib.h>
#include <Library/UefiLib.h>
#include <Library/IoLib.h>
#include <Library/TimerLib.h>
#include <Library/DebugLib.h>
#include <Library/PrintLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DevicePathLib.h>
#include <Library/BaseMemoryLib.h>

#include <Library/MsdcImplLib.h>

#include <Protocol/SdMmcPassThru.h>
#include <Protocol/ComponentName.h>
#include <Protocol/ComponentName2.h>
#include <Protocol/DriverBinding.h>
#include <Protocol/DevicePath.h>

#include <IndustryStandard/Sd.h>
#include <IndustryStandard/Emmc.h>

//
// Constants
//

#define MSDC_FIFO_SIZE    (128)

#define MSDC_BUS_WIDTH_1  (0)
#define MSDC_BUS_WIDTH_4  (1)
#define MSDC_BUS_WIDTH_8  (2)

#define MSDC_MCLK_DIV       (0)
#define MSDC_MCLK_NO_DIV    (1)
#define MSDC_MCLK_DDR       (2)
#define MSDC_MCLK_HS400     (3)

#define SCLK_CYCLES_SHIFT   (20)

#define BLOCK_SIZE 512

//
// Registers
//

/* MSDC */
#define MSDC_CFG        (0x000)
#define MSDC_IOCON      (0x004)
#define MSDC_PS         (0x008)
#define MSDC_INT        (0x00c)
#define MSDC_INTEN      (0x010)
#define MSDC_FIFOCS     (0x014)
#define MSDC_TXDATA     (0x018)
#define MSDC_RXDATA     (0x01c)
#define SDC_CFG         (0x030)
#define SDC_CMD         (0x034)
#define SDC_ARG         (0x038)
#define SDC_STS         (0x03c)
#define SDC_RESP0       (0x040)
#define SDC_RESP1       (0x044)
#define SDC_RESP2       (0x048)
#define SDC_RESP3       (0x04c)
#define SDC_BLK_NUM     (0x050)
#define SDC_VOL_CHG     (0x054)
#define SDC_CSTS        (0x058)
#define SDC_CSTS_EN     (0x05c)
#define SDC_DATCRC_STS  (0x060)
#define SDC_ADV_CFG0    (0x064)
#define EMMC_CFG0       (0x070)
#define EMMC_CFG1       (0x074)
#define EMMC_STS        (0x078)
#define EMMC_IOCON      (0x07c)
#define SD_ACMD_RESP    (0x080)
#define SD_ACMD19_TRG   (0x084)
#define SD_ACMD19_STS   (0x088)
#define DMA_SA_HIGH4BIT (0x08c)
#define DMA_SA          (0x090)
#define DMA_CA          (0x094)
#define DMA_CTRL        (0x098)
#define DMA_LENGTH      (0x0a8)
#define MSDC_PATCH_BIT0 (0x0b0)
#define MSDC_PATCH_BIT1 (0x0b4)
#define MSDC_PATCH_BIT2 (0x0b8)
#define EMMC51_CFG0     (0x204)
#define EMMC50_CFG0     (0x208)
#define EMMC50_CFG1     (0x20c)
#define EMMC50_CFG2     (0x21c)
#define EMMC50_CFG3     (0x220)
#define EMMC50_CFG4     (0x224)
#define SDC_FIFO_CFG    (0x228)

/* MSDC_TOP */
#define TOP_CTRL        (0x0)
#define TOP_CMD         (0x4)
#define TOP_PAD_DS_TUNE (0x8)

/* MSDC_CFG */
#define MSDC_CFG_MODE           (BIT0)
#define MSDC_CFG_CCKPD          (BIT1)
#define MSDC_CFG_RST            (BIT2)
#define MSDC_CFG_PIO            (BIT3)
#define MSDC_CFG_CCKSB          (BIT7)
#define MSDC_CFG_HS400CKMD      (BIT22)

/* MSDC_IOCON */
#define MSDC_IOCON_RSPL         (BIT1)
#define MSDC_IOCON_DDLSEL       (BIT3)

/* MSDC_INT | INTEN */
#define MSDC_INT_MMCIRQ         (BIT0)
#define MSDC_INT_MSDCCDSC       (BIT1)
#define MSDC_INT_SDACDCRDY      (BIT2)
#define MSDC_INT_ACMDRDY        (BIT3)
#define MSDC_INT_ACMDTMO        (BIT4)
#define MSDC_INT_ACMDCRCERR     (BIT5)
#define MSDC_INT_DMAQEPTY       (BIT6)
#define MSDC_INT_CMDRDY         (BIT8)
#define MSDC_INT_CMDTMO         (BIT9)
#define MSDC_INT_CMDCRCERR      (BIT10)
#define MSDC_INT_SDCSTS         (BIT11)
#define MSDC_INT_XFER_COMPL     (BIT12)
#define MSDC_INT_DMAXFDNE       (BIT13)
#define MSDC_INT_DATTMO         (BIT14)
#define MSDC_INT_DATCRCERR      (BIT15)
#define MSDC_INT_AC19DONE       (BIT16)
#define MSDC_INT_BDCSERR        (BIT17)
#define MSDC_INT_GPDCSERR       (BIT18)

/* MSDC_FIFOCS */
#define MSDC_FIFOCS_CLR         (BIT31)

/* SDC_CFG */
#define SDC_CFG_BUS_WIDTH_SHIFT (16)
#define SDC_CFG_SDIO            (BIT19)
#define SDC_CFG_SDIOIDE         (BIT20)

/* SDC_CMD */
#define SDC_CMD_RSP_TYPE_SHIFT  (7)
#define SDC_CMD_SINGLE_BLK      (BIT11)
#define SDC_CMD_MULTIPLE_BLK    (BIT12)
#define SDC_CMD_RW              (BIT13)
#define SDC_CMD_STOP_CMD        (BIT14)
#define SDC_CMD_AUTO12          (BIT28)
#define SDC_CMD_BLK_SIZE_SHIFT  (16)

/* SDC_STS */
#define SDC_STS_BUSY            (BIT0)
#define SDC_STS_CMDBUSY         (BIT1)

/* SDC_ADV_CFG0 */
#define SDC_RX_ENHANCE_EN       (BIT20)

/* EMMC50_CFG0 */
#define EMMC50_CFG0_CRCSTSSEL   (BIT4)

/* MSDC_PATCH_BIT1 */
#define MSDC_PB1_BUSYCHECKSEL   (BIT7)

/* MSDC_PATCH_BIT2 */
#define MSDC_PB2_RESPWAIT       (0xc)
#define MSDC_PB2_RESPWAIT_SHIFT (2)
#define MSDC_PB2_CFGRESP        (BIT15)
#define MSDC_PB2_RESPSTSENSEL   (0x70000)
#define MSDC_PB2_CFGCRCSTS      (BIT28)
#define MSDC_PB2_CRCSTSENSEL    (0xe0000000)

/* MSDC_PAD_TUNE */
#define MSDC_PAD_TUNE_RD_SEL    (BIT13)
#define MSDC_PAD_TUNE_CMD_SEL   (BIT21)

/* SDC_FIFO_CFG */
#define SDC_FIFO_CFG_WRVALIDSEL (BIT24)
#define SDC_FIFO_CFG_RDVALIDSEL (BIT25)

/* TOP_CTRL */
#define PAD_DAT_RD_RXDLY2_SEL   (BIT12)
#define PAD_DAT_RD_RXDLY_SEL    (BIT13)
#define DATA_K_VALUE_SEL        (BIT14)
#define SDC_RX_ENH_EN           (BIT15)

/* TOP_CMD */
#define PAD_CMD_RD_RXDLY2_SEL   (BIT10)
#define PAD_CMD_RD_RXDLY_SEL    (BIT11)

//
// Structures
//

#define MSDC_PRIVATE_SIGNATURE SIGNATURE_32 ('M', 'S', 'D', 'C')

#define MSDC_PRIVATE_FROM_THIS(a) \
    CR(a, MSDC_PRIVATE_DATA, PassThru, MSDC_PRIVATE_SIGNATURE)

typedef struct {
  UINT32 BlkSize;
  UINT32 Sclk;
  UINT32 TimeoutNs;
  UINT32 TimeoutClks;
} MSDC_HOST_DATA;

typedef enum {
  UnknownCard,
  EmmcCard,
  SdCard
} SD_MCC_CARD_TYPE;

typedef struct {
  UINT32 Lba;
  SD_MCC_CARD_TYPE CardType;
} SD_INFO;

typedef struct {
  UINT32 Signature;

  EFI_HANDLE                    ControllerHandle;
  EFI_SD_MMC_PASS_THRU_PROTOCOL PassThru;

  UINT32 Index;

  EFI_PHYSICAL_ADDRESS MsdcMmioReg;
  EFI_PHYSICAL_ADDRESS TopMmioReg;

  MSDC_HOST_DATA  HostData;
  SD_INFO         SdInfo;
} MSDC_PRIVATE_DATA;

typedef
EFI_STATUS
(*CARD_DETECT_ROUTINE) (
  IN MSDC_PRIVATE_DATA *Private
  );

EFI_STATUS MsdcPassThru (
  EFI_SD_MMC_PASS_THRU_PROTOCOL *This,
  UINT8 Slot,
  EFI_SD_MMC_PASS_THRU_COMMAND_PACKET *Packet,
  EFI_EVENT Event
  );

EFI_STATUS MsdcGetNextSlot (
  EFI_SD_MMC_PASS_THRU_PROTOCOL *This,
  UINT8 *Slot
  );

EFI_STATUS MsdcBuildDevicePath (
  EFI_SD_MMC_PASS_THRU_PROTOCOL *This,
  UINT8 Slot,
  EFI_DEVICE_PATH_PROTOCOL **DevicePath
  );

EFI_STATUS MsdcGetSlotNumber (
  EFI_SD_MMC_PASS_THRU_PROTOCOL *This,
  EFI_DEVICE_PATH_PROTOCOL *DevicePath,
  UINT8 *Slot
  );

EFI_STATUS MsdcResetDevice (
  EFI_SD_MMC_PASS_THRU_PROTOCOL *This,
  UINT8 Slot
  );

EFI_STATUS
MsdcSendCmd (
  MSDC_PRIVATE_DATA* Private,
  EFI_SD_MMC_PASS_THRU_COMMAND_PACKET *Packet
  );

VOID MsdcWrite (
  IN MSDC_PRIVATE_DATA *Private,
  IN UINT32 Offset,
  IN UINT32 Value
  );

VOID MsdcRead (
  IN MSDC_PRIVATE_DATA *Private,
  IN  UINT32  Offset,
  OUT UINT32 *Value
  );

VOID MsdcTopWrite (
  IN MSDC_PRIVATE_DATA *Private,
  IN UINT32 Offset,
  IN UINT32 Value
  );

VOID MsdcTopRead (
  IN MSDC_PRIVATE_DATA *Private,
  IN  UINT32  Offset,
  OUT UINT32 *Value
  );

VOID MsdcSetBits (
  IN MSDC_PRIVATE_DATA *Private,
  IN UINT32 Offset,
  IN UINT32 BitMask
  );

VOID MsdcClrSetBits (
  IN MSDC_PRIVATE_DATA *Private,
  IN UINT32 Offset,
  IN UINT32 BitMask,
  IN UINT32 BitMaskSet
  );

VOID MsdcClrBits (
  IN MSDC_PRIVATE_DATA *Private,
  IN UINT32 Offset,
  IN UINT32 BitMask
  );

VOID MsdcTopSetBits (
  IN MSDC_PRIVATE_DATA *Private,
  IN UINT32 Offset,
  IN UINT32 BitMask
  );

VOID MsdcTopClrSetBits (
  IN MSDC_PRIVATE_DATA *Private,
  IN UINT32 Offset,
  IN UINT32 BitMask,
  IN UINT32 BitMaskSet
  );

VOID MsdcTopClrBits (
  IN MSDC_PRIVATE_DATA *Private,
  IN UINT32 Offset,
  IN UINT32 BitMask
  );

#endif // _MSDC_DXE_H_