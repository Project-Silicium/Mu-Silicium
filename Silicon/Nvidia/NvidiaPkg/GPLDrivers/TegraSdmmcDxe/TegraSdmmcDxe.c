/**
  TegraSdmmcDxe.c - Tegra SDMMC Controller Driver

  Copyright (c) 2026 Mu-Silicium Project

  SPDX-License-Identifier: GPL-2.0
**/

#include <PiDxe.h>
#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/TimerLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/CacheMaintenanceLib.h>
#include <Library/ArmSmcLib.h>
#include <Library/TegraPinmuxLib.h>
#include <Protocol/BlockIo.h>
#include <Protocol/DevicePath.h>
#include <Protocol/EfiTegraMax77620.h>
#include <T210.h>

//
// Base Addresses
//
#define SDMMC1_REG_BASE       0x700B0000
#define APB_MISC              0x70000000
#define CAR_BASE              0x60006000
#define GPIO_E_BASE           0x6000D100
#define PMC_BASE_ADDR         0x7000E400

//
// SDHCI Register Offsets
//
#define SDHCI_SYSAD           0x00
#define SDHCI_BLKSIZE         0x04
#define SDHCI_BLKCNT          0x06
#define SDHCI_ARGUMENT        0x08
#define SDHCI_TRNMOD          0x0C
#define SDHCI_CMDREG          0x0E
#define SDHCI_RSPREG0         0x10
#define SDHCI_RSPREG1         0x14
#define SDHCI_RSPREG2         0x18
#define SDHCI_RSPREG3         0x1C
#define SDHCI_PRNSTS          0x24
#define SDHCI_HOSTCTL         0x28
#define SDHCI_PWRCON          0x29
#define SDHCI_CLKCON          0x2C
#define SDHCI_TIMEOUTCON      0x2E
#define SDHCI_SWRST           0x2F
#define SDHCI_NORINTSTS       0x30
#define SDHCI_NORINTSTSEN     0x34
#define SDHCI_NORINTSIGEN     0x38
#define SDHCI_HOSTCTL2        0x3E
#define SDHCI_HCVER           0xFE
#define SDHCI_VENCLKCTL       0x100
#define SDHCI_VENMISCCTL      0x120
#define SDHCI_SDMEMCMPPADCTL  0x1E0
#define SDHCI_AUTOCALCFG      0x1E4
#define SDHCI_AUTOCALSTS      0x1EC
#define SDHCI_IOSPARE         0x1F0
#define SDHCI_IOTRIM          0x1AC

//
// Register Bits
//
#define PRNSTS_CARD_INSERTED  (1 << 16)

//
// Device Path
//
typedef struct {
  VENDOR_DEVICE_PATH        Vendor;
  EFI_DEVICE_PATH_PROTOCOL  End;
} SDMMC_DEVICE_PATH;

STATIC SDMMC_DEVICE_PATH mDevicePath = {
  {
    { HARDWARE_DEVICE_PATH, HW_VENDOR_DP, { sizeof (VENDOR_DEVICE_PATH), 0 } },
    { 0xb615f1f5, 0x5088, 0x43cd, { 0x80, 0x9c, 0xa1, 0x6e, 0x52, 0x48, 0x7d, 0x00 } }
  },
  { END_DEVICE_PATH_TYPE, END_ENTIRE_DEVICE_PATH_SUBTYPE, { sizeof (EFI_DEVICE_PATH_PROTOCOL), 0 } }
};

//
// State
//
STATIC UINT32                mBase;
STATIC UINT32                mRca;
STATIC EFI_BLOCK_IO_PROTOCOL mBlockIo;
STATIC EFI_BLOCK_IO_MEDIA    mMedia;

//
// Command Structure
//
typedef struct {
  UINT32 Idx;
  UINT32 Arg;
  UINT32 RspType;
  UINT32 Rsp[4];
} MMC_CMD;

#define MMC_RSP_PRESENT  (1 << 0)
#define MMC_RSP_136      (1 << 1)
#define MMC_RSP_CRC      (1 << 2)
#define MMC_RSP_BUSY     (1 << 3)
#define MMC_RSP_OPCODE   (1 << 4)
#define MMC_RSP_NONE     0
#define MMC_RSP_R1       (MMC_RSP_PRESENT | MMC_RSP_CRC | MMC_RSP_OPCODE)
#define MMC_RSP_R1b      (MMC_RSP_PRESENT | MMC_RSP_CRC | MMC_RSP_OPCODE | MMC_RSP_BUSY)
#define MMC_RSP_R2       (MMC_RSP_PRESENT | MMC_RSP_136 | MMC_RSP_CRC)
#define MMC_RSP_R3       MMC_RSP_PRESENT
#define MMC_RSP_R6       (MMC_RSP_PRESENT | MMC_RSP_CRC | MMC_RSP_OPCODE)
#define MMC_RSP_R7       (MMC_RSP_PRESENT | MMC_RSP_CRC | MMC_RSP_OPCODE)

//
// Commands
//
#define CMD_GO_IDLE       0
#define CMD_ALL_SEND_CID  2
#define CMD_SEND_RCA      3
#define CMD_SELECT_CARD   7
#define CMD_SEND_IF_COND  8
#define CMD_SEND_CSD      9
#define CMD_STOP          12
#define CMD_SET_BLOCKLEN  16
#define CMD_READ_SINGLE   17
#define CMD_READ_MULTI    18
#define CMD_WRITE_SINGLE  24
#define CMD_WRITE_MULTI   25
#define CMD_APP_CMD       55
#define ACMD_SD_SEND_OP   41

STATIC
EFI_STATUS
SendCommand (
  IN OUT MMC_CMD *Cmd,
  IN     VOID    *Data,
  IN     UINT32  Blocks,
  IN     BOOLEAN IsRead
  )
{
  UINT32 Flags;
  UINT32 Mode = 0;
  UINT32 Mask;
  UINT32 Timeout;

  // Wait CMD inhibit
  Timeout = 10000;
  while ((MmioRead32 (mBase + SDHCI_PRNSTS) & 1) && Timeout--) {
    MicroSecondDelay (1);
  }
  if (Timeout == 0) {
    DEBUG ((DEBUG_ERROR, "SDMMC: CMD inhibit timeout\n"));
    return EFI_TIMEOUT;
  }

  if (Data != NULL) {
    if (IsRead) {
      InvalidateDataCacheRange (Data, Blocks * 512);
    } else {
      WriteBackDataCacheRange (Data, Blocks * 512);
    }

    MmioWrite32 (mBase + SDHCI_SYSAD, (UINT32)(UINTN)Data);
    MmioWrite16 (mBase + SDHCI_BLKSIZE, (7 << 12) | 512);
    MmioWrite16 (mBase + SDHCI_BLKCNT, (UINT16)Blocks);

    Mode = 0x01 | 0x02;  // DMA | BLKCNT
    if (Blocks > 1) Mode |= 0x20;
    if (IsRead) Mode |= 0x10;
  }

  MmioWrite32 (mBase + SDHCI_ARGUMENT, Cmd->Arg);
  if (Data != NULL) {
    MmioWrite16 (mBase + SDHCI_TRNMOD, (UINT16)Mode);
  }

  // Build flags
  if (!(Cmd->RspType & MMC_RSP_PRESENT)) {
    Flags = 0x00;
  } else if (Cmd->RspType & MMC_RSP_136) {
    Flags = 0x01;
  } else if (Cmd->RspType & MMC_RSP_BUSY) {
    Flags = 0x03;
  } else {
    Flags = 0x02;
  }
  if (Cmd->RspType & MMC_RSP_CRC) Flags |= 0x08;
  if (Cmd->RspType & MMC_RSP_OPCODE) Flags |= 0x10;
  if (Data != NULL) Flags |= 0x20;

  MmioWrite16 (mBase + SDHCI_CMDREG, (UINT16)((Cmd->Idx << 8) | Flags));

  // Wait CMD complete
  Timeout = 100000;
  while (Timeout--) {
    Mask = MmioRead32 (mBase + SDHCI_NORINTSTS);
    if (Mask & 0x01) {
      if (Data == NULL) MmioWrite32 (mBase + SDHCI_NORINTSTS, Mask);
      break;
    }
    MicroSecondDelay (1);
  }

  if (Timeout == 0) {
    DEBUG ((DEBUG_ERROR, "SDMMC: CMD%d timeout\n", Cmd->Idx));
    MmioWrite32 (mBase + SDHCI_NORINTSTS, 0xFFFFFFFF);
    return EFI_TIMEOUT;
  }

  if (Mask & 0x8000) {
    DEBUG ((DEBUG_ERROR, "SDMMC: CMD%d error, NORINTSTS=0x%08x\n", Cmd->Idx, Mask));
    MmioWrite32 (mBase + SDHCI_NORINTSTS, 0xFFFFFFFF);
    return EFI_DEVICE_ERROR;
  }

  // Read response
  if (Cmd->RspType & MMC_RSP_PRESENT) {
    if (Cmd->RspType & MMC_RSP_136) {
      Cmd->Rsp[0] = MmioRead32 (mBase + SDHCI_RSPREG3) << 8;
      Cmd->Rsp[0] |= MmioRead8 (mBase + SDHCI_RSPREG2 + 3);
      Cmd->Rsp[1] = MmioRead32 (mBase + SDHCI_RSPREG2) << 8;
      Cmd->Rsp[1] |= MmioRead8 (mBase + SDHCI_RSPREG1 + 3);
      Cmd->Rsp[2] = MmioRead32 (mBase + SDHCI_RSPREG1) << 8;
      Cmd->Rsp[2] |= MmioRead8 (mBase + SDHCI_RSPREG0 + 3);
      Cmd->Rsp[3] = MmioRead32 (mBase + SDHCI_RSPREG0) << 8;
    } else {
      Cmd->Rsp[0] = MmioRead32 (mBase + SDHCI_RSPREG0);
    }
  }

  // Wait data transfer
  if (Data != NULL) {
    while (TRUE) {
      Mask = MmioRead32 (mBase + SDHCI_NORINTSTS);
      if (Mask & 0x8000) {
        MmioWrite32 (mBase + SDHCI_NORINTSTS, 0xFFFFFFFF);
        return EFI_DEVICE_ERROR;
      }
      if (Mask & 0x08) {
        UINT32 Addr = MmioRead32 (mBase + SDHCI_SYSAD);
        MmioWrite32 (mBase + SDHCI_NORINTSTS, 0x08);
        MmioWrite32 (mBase + SDHCI_SYSAD, Addr);
      }
      if (Mask & 0x02) {
        MmioWrite32 (mBase + SDHCI_NORINTSTS, Mask);
        break;
      }
      MicroSecondDelay (10);
    }
  }

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
EFIAPI
BlockIoReset (
  IN EFI_BLOCK_IO_PROTOCOL *This,
  IN BOOLEAN               ExtendedVerification
  )
{
  MmioWrite8 (mBase + SDHCI_SWRST, 0x01);
  MicroSecondDelay (10000);
  return EFI_SUCCESS;
}

#define MAX_BLOCKS 65535

STATIC
EFI_STATUS
EFIAPI
BlockIoRead (
  IN  EFI_BLOCK_IO_PROTOCOL *This,
  IN  UINT32                MediaId,
  IN  EFI_LBA               Lba,
  IN  UINTN                 BufferSize,
  OUT VOID                  *Buffer
  )
{
  MMC_CMD Cmd;
  EFI_STATUS Status;
  UINT32 TotalBlocks;
  UINT32 ChunkBlocks;
  UINT8 *Ptr;

  if (BufferSize == 0) return EFI_SUCCESS;
  if (Buffer == NULL || BufferSize % 512) return EFI_INVALID_PARAMETER;
  if ((UINTN)Buffer >= 0x100000000ULL) return EFI_UNSUPPORTED;

  TotalBlocks = (UINT32)(BufferSize / 512);
  Ptr = (UINT8 *)Buffer;

  while (TotalBlocks > 0) {
    ChunkBlocks = (TotalBlocks > MAX_BLOCKS) ? MAX_BLOCKS : TotalBlocks;

    ZeroMem (&Cmd, sizeof (Cmd));
    Cmd.Idx = (ChunkBlocks > 1) ? CMD_READ_MULTI : CMD_READ_SINGLE;
    Cmd.Arg = (UINT32)Lba;
    Cmd.RspType = MMC_RSP_R1;

    Status = SendCommand (&Cmd, Ptr, ChunkBlocks, TRUE);
    if (EFI_ERROR (Status)) return Status;

    if (ChunkBlocks > 1) {
      ZeroMem (&Cmd, sizeof (Cmd));
      Cmd.Idx = CMD_STOP;
      Cmd.RspType = MMC_RSP_R1b;
      SendCommand (&Cmd, NULL, 0, FALSE);
    }

    Lba += ChunkBlocks;
    Ptr += (ChunkBlocks * 512);
    TotalBlocks -= ChunkBlocks;
  }

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
EFIAPI
BlockIoWrite (
  IN EFI_BLOCK_IO_PROTOCOL *This,
  IN UINT32                MediaId,
  IN EFI_LBA               Lba,
  IN UINTN                 BufferSize,
  IN VOID                  *Buffer
  )
{
  MMC_CMD Cmd;
  EFI_STATUS Status;
  UINT32 TotalBlocks;
  UINT32 ChunkBlocks;
  UINT8 *Ptr;

  if (BufferSize == 0) return EFI_SUCCESS;
  if (Buffer == NULL || BufferSize % 512) return EFI_INVALID_PARAMETER;
  if ((UINTN)Buffer >= 0x100000000ULL) return EFI_UNSUPPORTED;

  TotalBlocks = (UINT32)(BufferSize / 512);
  Ptr = (UINT8 *)Buffer;

  while (TotalBlocks > 0) {
    ChunkBlocks = (TotalBlocks > MAX_BLOCKS) ? MAX_BLOCKS : TotalBlocks;

    ZeroMem (&Cmd, sizeof (Cmd));
    Cmd.Idx = (ChunkBlocks > 1) ? CMD_WRITE_MULTI : CMD_WRITE_SINGLE;
    Cmd.Arg = (UINT32)Lba;
    Cmd.RspType = MMC_RSP_R1;

    Status = SendCommand (&Cmd, Ptr, ChunkBlocks, FALSE);
    if (EFI_ERROR (Status)) return Status;

    if (ChunkBlocks > 1) {
      ZeroMem (&Cmd, sizeof (Cmd));
      Cmd.Idx = CMD_STOP;
      Cmd.RspType = MMC_RSP_R1b;
      SendCommand (&Cmd, NULL, 0, FALSE);
    }

    Lba += ChunkBlocks;
    Ptr += (ChunkBlocks * 512);
    TotalBlocks -= ChunkBlocks;
  }

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
EFIAPI
BlockIoFlush (
  IN EFI_BLOCK_IO_PROTOCOL *This
  )
{
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
TegraSdmmcDxeInitialize (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  EFI_STATUS Status;
  EFI_TEGRA_MAX77620_PROTOCOL *Pmic;
  MMC_CMD Cmd;
  UINT32 Val;
  INT32 Timeout;
  UINT32 Ocr;
  UINT64 Capacity;
  EFI_HANDLE Handle;

  mBase = SDMMC1_REG_BASE;

  DEBUG ((DEBUG_ERROR, "SDMMC: === DRIVER ENTRY ===\n"));

  // 1. PMC NO_IOPOWER fix via SMC
  {
    ARM_SMC_ARGS Args;
    Args.Arg0 = 0x82000005;
    Args.Arg1 = 0x7000E444;
    Args.Arg2 = 0;
    ArmCallSmc (&Args);
  }

  // Backup direct clear
  MmioAnd32 (0x7000E444, ~0x1000);

  // 2. PMIC SD power
  Status = gBS->LocateProtocol (&gEfiTegraMax77620ProtocolGuid, NULL, (VOID **)&Pmic);
  if (!EFI_ERROR (Status)) {
    Pmic->EnableSdCardPower ();
  }

  // 3. Pinmux
  PinmuxConfigSdmmc (0);

  // 4. CLK Loopback (0x700008D4)
  MmioWrite32 (0x700008D4, 1);

  // 5. PMC Power Detect
  MmioOr32 (0x7000E4E4, 0x1000);

  // 6. Pad Slew
  Val = MmioRead32 (0x70000A98);
  Val = (Val & 0x0FFFFFFF) | 0x50000000;
  MmioWrite32 (0x70000A98, Val);

  // 7. CAR Clock Enable
  Val = MmioRead32 (0x60006010);
  if (!(Val & (1 << 14))) {
    MmioWrite32 (0x60006320, (1 << 14));
    MicroSecondDelay (10);
  }
  MmioWrite32 (0x60006150, (0 << 29) | 18);
  MicroSecondDelay (10);

  Val = MmioRead32 (0x60006004);
  if (Val & (1 << 14)) {
    MmioWrite32 (0x60006304, (1 << 14));
    MicroSecondDelay (100);
  }

  // 8. Check controller
  UINT16 Ver = MmioRead16 (mBase + SDHCI_HCVER);
  if (Ver == 0x0000 || Ver == 0xFFFF) {
    return EFI_DEVICE_ERROR;
  }

  // 9. GPIO Power Cycle (E4)
  MmioOr32 (0x6000D100, 0x10);
  MmioOr32 (0x6000D110, 0x10);
  MmioAnd32 (0x6000D120, ~0x10);
  MicroSecondDelay (100000);
  MmioOr32 (0x6000D120, 0x10);
  MicroSecondDelay (20000);

  // 10. Check card
  Val = MmioRead32 (mBase + SDHCI_PRNSTS);
  if (!(Val & PRNSTS_CARD_INSERTED)) {
    return EFI_NOT_FOUND;
  }

  // 11. Software Reset
  MmioWrite8 (mBase + SDHCI_SWRST, 0x01);
  Timeout = 1000;
  while ((MmioRead8 (mBase + SDHCI_SWRST) & 0x01) && Timeout--) {
    MicroSecondDelay (100);
  }

  // 12. Bus Power
  MmioWrite8 (mBase + SDHCI_PWRCON, 0x0F);
  MicroSecondDelay (1000);

  // 13. Slow Clock
  MmioWrite16 (mBase + SDHCI_CLKCON, 0);
  MmioWrite16 (mBase + SDHCI_CLKCON, (27 << 8) | 1);
  Timeout = 1000;
  while (!(MmioRead16 (mBase + SDHCI_CLKCON) & 2) && Timeout--) {
    MicroSecondDelay (10);
  }
  MmioOr16 (mBase + SDHCI_CLKCON, 4);

  // 14. Vendor Clock (Tap=4, Trim=2)
  Val = MmioRead32 (mBase + SDHCI_VENCLKCTL);
  Val |= 0x01;
  Val &= ~0x00FF0000;
  Val |= (4 << 16);
  Val &= ~0x1F000000;
  Val |= (2 << 24);
  MmioWrite32 (mBase + SDHCI_VENCLKCTL, Val);

  // 15. Vendor Misc
  Val = MmioRead32 (mBase + SDHCI_VENMISCCTL);
  Val &= ~0xF0;
  MmioWrite32 (mBase + SDHCI_VENMISCCTL, Val);

  // 16. IO Trim
  MmioOr32 (mBase + SDHCI_IOSPARE, 0x00080000);
  Val = MmioRead32 (mBase + SDHCI_SDMEMCMPPADCTL);
  Val = (Val & ~0xF) | 7;
  MmioWrite32 (mBase + SDHCI_SDMEMCMPPADCTL, Val);
  Val = MmioRead32 (mBase + SDHCI_IOTRIM);
  Val &= ~(1 << 2);
  MmioWrite32 (mBase + SDHCI_IOTRIM, Val);

  // 17. CMD/DATA Reset
  MmioWrite8 (mBase + SDHCI_SWRST, 0x06);
  Timeout = 1000;
  while ((MmioRead8 (mBase + SDHCI_SWRST) & 0x06) && Timeout--) {
    MicroSecondDelay (100);
  }

  // 18. Host Control
  MmioAnd8 (mBase + SDHCI_HOSTCTL, ~0x02);
  MmioWrite16 (mBase + SDHCI_HOSTCTL2, 0);

  // 19. Auto Calibration
  UINT16 ClkSave = MmioRead16 (mBase + SDHCI_CLKCON);
  if (ClkSave & 4) {
    MmioWrite16 (mBase + SDHCI_CLKCON, ClkSave & ~4);
  }
  MmioOr32 (mBase + SDHCI_SDMEMCMPPADCTL, 0x80000000);
  MicroSecondDelay (1);
  MmioWrite32 (mBase + SDHCI_AUTOCALCFG, 0xA0000000);
  MicroSecondDelay (10);
  Timeout = 100;
  while ((MmioRead32 (mBase + SDHCI_AUTOCALSTS) & 0x80000000) && Timeout--) {
    MicroSecondDelay (100);
  }
  MmioAnd32 (mBase + SDHCI_SDMEMCMPPADCTL, ~0x80000000);
  if (ClkSave & 4) {
    MmioOr16 (mBase + SDHCI_CLKCON, 4);
  }

  // 20. Timeout & Interrupts
  MmioWrite8 (mBase + SDHCI_TIMEOUTCON, 0x0E);
  MmioWrite32 (mBase + SDHCI_NORINTSTSEN, 0xFFFFFFFF);
  MmioWrite32 (mBase + SDHCI_NORINTSIGEN, 0xFFFFFFFF);
  MmioWrite32 (mBase + SDHCI_NORINTSTS, 0xFFFFFFFF);

  // 21. CMD0
  ZeroMem (&Cmd, sizeof (Cmd));
  Cmd.Idx = CMD_GO_IDLE;
  Cmd.RspType = MMC_RSP_NONE;
  SendCommand (&Cmd, NULL, 0, FALSE);

  // 22. CMD8
  ZeroMem (&Cmd, sizeof (Cmd));
  Cmd.Idx = CMD_SEND_IF_COND;
  Cmd.Arg = 0x1AA;
  Cmd.RspType = MMC_RSP_R7;
  SendCommand (&Cmd, NULL, 0, FALSE);

  // 23. ACMD41 loop
  Ocr = 0;
  for (INT32 i = 200; i > 0; i--) {
    ZeroMem (&Cmd, sizeof (Cmd));
    Cmd.Idx = CMD_APP_CMD;
    Cmd.RspType = MMC_RSP_R1;
    SendCommand (&Cmd, NULL, 0, FALSE);

    ZeroMem (&Cmd, sizeof (Cmd));
    Cmd.Idx = ACMD_SD_SEND_OP;
    Cmd.Arg = 0x40300000;
    Cmd.RspType = MMC_RSP_R3;
    SendCommand (&Cmd, NULL, 0, FALSE);

    if (Cmd.Rsp[0] & 0x80000000) {
      Ocr = Cmd.Rsp[0];
      break;
    }
    MicroSecondDelay (10000);
  }

  if (Ocr == 0) {
    return EFI_DEVICE_ERROR;
  }

  // 24. CMD2
  ZeroMem (&Cmd, sizeof (Cmd));
  Cmd.Idx = CMD_ALL_SEND_CID;
  Cmd.RspType = MMC_RSP_R2;
  SendCommand (&Cmd, NULL, 0, FALSE);

  // 25. CMD3
  ZeroMem (&Cmd, sizeof (Cmd));
  Cmd.Idx = CMD_SEND_RCA;
  Cmd.RspType = MMC_RSP_R6;
  SendCommand (&Cmd, NULL, 0, FALSE);
  mRca = Cmd.Rsp[0] >> 16;

  // 26. CMD9
  ZeroMem (&Cmd, sizeof (Cmd));
  Cmd.Idx = CMD_SEND_CSD;
  Cmd.Arg = mRca << 16;
  Cmd.RspType = MMC_RSP_R2;
  SendCommand (&Cmd, NULL, 0, FALSE);

  // Calculate capacity
  if (Ocr & (1 << 30)) {
    UINT32 CSize = ((Cmd.Rsp[1] & 0x3F) << 16) | ((Cmd.Rsp[2] >> 16) & 0xFFFF);
    Capacity = ((UINT64)(CSize + 1)) << 19;
  } else {
    UINT32 CSize = ((Cmd.Rsp[1] & 0x3FF) << 2) | ((Cmd.Rsp[2] >> 30) & 0x3);
    UINT32 CMult = (Cmd.Rsp[2] >> 15) & 0x7;
    UINT32 BlkLen = 1 << ((Cmd.Rsp[1] >> 16) & 0xF);
    Capacity = ((UINT64)(CSize + 1)) << (CMult + 2);
    Capacity *= BlkLen;
  }

  // 27. CMD7
  ZeroMem (&Cmd, sizeof (Cmd));
  Cmd.Idx = CMD_SELECT_CARD;
  Cmd.Arg = mRca << 16;
  Cmd.RspType = MMC_RSP_R1b;
  SendCommand (&Cmd, NULL, 0, FALSE);

  // 28. CMD16
  ZeroMem (&Cmd, sizeof (Cmd));
  Cmd.Idx = CMD_SET_BLOCKLEN;
  Cmd.Arg = 512;
  Cmd.RspType = MMC_RSP_R1;
  SendCommand (&Cmd, NULL, 0, FALSE);

  // 29. High-speed clock
  MmioWrite16 (mBase + SDHCI_CLKCON, 0);
  MmioWrite32 (0x60006150, (0 << 29) | 7);
  MicroSecondDelay (100);
  MmioWrite16 (mBase + SDHCI_CLKCON, (0 << 8) | 1);
  Timeout = 1000;
  while (!(MmioRead16 (mBase + SDHCI_CLKCON) & 2) && Timeout--) {
    MicroSecondDelay (10);
  }
  MmioOr16 (mBase + SDHCI_CLKCON, 4);

  // Setup media
  mMedia.MediaId = 0x55AA55AA;
  mMedia.RemovableMedia = TRUE;
  mMedia.MediaPresent = TRUE;
  mMedia.LogicalPartition = FALSE;
  mMedia.ReadOnly = FALSE;
  mMedia.WriteCaching = FALSE;
  mMedia.BlockSize = 512;
  mMedia.IoAlign = 0;
  mMedia.LastBlock = (Capacity / 512) - 1;

  mBlockIo.Revision = EFI_BLOCK_IO_INTERFACE_REVISION;
  mBlockIo.Media = &mMedia;
  mBlockIo.Reset = BlockIoReset;
  mBlockIo.ReadBlocks = BlockIoRead;
  mBlockIo.WriteBlocks = BlockIoWrite;
  mBlockIo.FlushBlocks = BlockIoFlush;

  // Install
  Handle = NULL;
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &Handle,
                  &gEfiBlockIoProtocolGuid, &mBlockIo,
                  &gEfiDevicePathProtocolGuid, &mDevicePath,
                  NULL
                  );

  if (!EFI_ERROR (Status)) {
    gBS->ConnectController (Handle, NULL, NULL, TRUE);
  }

  return EFI_SUCCESS;
}
