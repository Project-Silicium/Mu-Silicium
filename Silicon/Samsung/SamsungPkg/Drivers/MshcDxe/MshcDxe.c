/**
  Copyright (c) 2012, Samsung Electronics Co. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
**/

#include <Uefi.h>

#include <Library/BaseLib.h>
#include <Library/TimerLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DevicePathLib.h>
#include <Library/MshcHostBridge.h>

#include "MshcDxe.h"

STATIC CONST EFI_GUID mMshcDevicePathGuid =
  { 0x1e2d5d8e, 0x7c4a, 0x4f19, { 0x9a, 0x6b, 0x30, 0x88, 0x21, 0x4d, 0xc1, 0x02 } };

STATIC
EFI_STATUS
MshcSendAppCmd (
  IN OUT MSHC_INSTANCE *Instance,
  IN     UINTN          Cmd,
  IN     UINTN          CmdArgument,
  OUT    UINT32        *Response OPTIONAL)
{
  EFI_STATUS Status = EFI_DEVICE_ERROR;
  UINT32     AppResponse;

  for (UINTN Attempt = 0; Attempt < 4; Attempt++) {
    Status = MshcSendCmd (&Instance->Controller, CMD55, (UINT32)Instance->CardInfo.RCA << 16, &AppResponse);
    if (EFI_ERROR (Status)) {
      continue;
    }

    if (!(AppResponse & R1_APP_CMD)) {
      DEBUG ((EFI_D_ERROR, "%a: The Card refused CMD55! Response = 0x%08X\n", __FUNCTION__, AppResponse));
      Status = EFI_DEVICE_ERROR;
      continue;
    }

    return MshcSendCmd (&Instance->Controller, Cmd, CmdArgument, Response);
  }

  return Status;
}

STATIC
EFI_STATUS
MshcReadScr (IN OUT MSHC_INSTANCE *Instance)
{
  EFI_STATUS Status;
  UINT32     Raw[SD_SCR_SIZE / sizeof (UINT32)];
  UINT32     Scr;

  SetMem (Raw, sizeof (Raw), 0);

  Status = MshcPrepareTransfer (&Instance->Controller, Raw, SD_SCR_SIZE, 1, READ);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = MshcSendAppCmd (Instance, ACMD51, 0, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Send ACMD51! Status = %r\n", __FUNCTION__, Status));
    MshcRecover (&Instance->Controller);
    return Status;
  }

  Status = MshcReadData (&Instance->Controller, Raw, SD_SCR_SIZE);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Read the Configuration Register! Status = %r\n", __FUNCTION__, Status));
    MshcRecover (&Instance->Controller);
    return Status;
  }

  Scr = SwapBytes32 (Raw[0]);

  if (SCR_STRUCTURE (Scr) != 0) {
    DEBUG ((EFI_D_ERROR, "%a: Unknown Configuration Register Layout %u!\n", __FUNCTION__, SCR_STRUCTURE (Scr)));
    return EFI_UNSUPPORTED;
  }

  Instance->CardInfo.SpecVersion  = (UINT8)SCR_SPEC_VERSION (Scr);
  Instance->CardInfo.Supports4Bit = !!(SCR_BUS_WIDTHS (Scr) & SCR_BUS_WIDTH_4);

  return EFI_SUCCESS;
}

STATIC
BOOLEAN
MshcTuningPhaseWorks (IN OUT MSHC_INSTANCE *Instance)
{
  STATIC CONST UINT8 Expected[] = SD_TUNING_PATTERN_4BIT;

  UINT32 Aligned[SD_TUNING_BLOCK_SIZE / sizeof (UINT32)];
  UINT8 *Block = (UINT8 *)Aligned;

  SetMem (Block, SD_TUNING_BLOCK_SIZE, 0);

  if (EFI_ERROR (MshcPrepareTransfer (&Instance->Controller, Block, SD_TUNING_BLOCK_SIZE, 1, READ))) {
    return FALSE;
  }

  if (EFI_ERROR (MshcSendCmd (&Instance->Controller, CMD19, 0, NULL))) {
    MshcRecover (&Instance->Controller);
    return FALSE;
  }

  if (EFI_ERROR (MshcReadData (&Instance->Controller, Block, SD_TUNING_BLOCK_SIZE))) {
    MshcRecover (&Instance->Controller);
    return FALSE;
  }

  return (CompareMem (Block, Expected, SD_TUNING_BLOCK_SIZE) == 0);
}

STATIC
UINT32
MshcTuneSamplePhase (IN OUT MSHC_INSTANCE *Instance)
{
  UINT32 Original  = Instance->Controller.Host.ClkSelTiming & CLKSEL_SAMPLE_MASK;
  UINT32 Passed    = 0;
  UINT32 BestRun   = 0;
  UINT32 BestPhase = Original;

  for (UINT32 Phase = 0; Phase < MSHC_TUNING_PHASES; Phase++) {
    MshcSetSamplePhase (&Instance->Controller, Phase);

    if (MshcTuningPhaseWorks (Instance)) {
      Passed |= (1u << Phase);
    }
  }

  if (Passed == 0) {
    DEBUG ((EFI_D_ERROR, "%a: No Phase Worked!\n", __FUNCTION__));
    MshcSetSamplePhase (&Instance->Controller, Original);
    return 0;
  }

  if (Passed == ((1u << MSHC_TUNING_PHASES) - 1)) {
    MshcSetSamplePhase (&Instance->Controller, Original);
    return MSHC_TUNING_PHASES;
  }

  UINT32 Run = 0;

  for (UINT32 i = 0; i < MSHC_TUNING_PHASES * 2; i++) {
    if (Passed & (1u << (i % MSHC_TUNING_PHASES))) {
      Run++;

      if (Run > BestRun) {
        BestRun   = Run;
        BestPhase = ((i - (Run / 2)) % MSHC_TUNING_PHASES);
      }
    } else {
      Run = 0;
    }
  }

  MshcSetSamplePhase (&Instance->Controller, BestPhase);

  return BestRun;
}

EFI_STATUS
MshcDecodeCsd (IN OUT MSHC_INSTANCE *Instance)
{
  CARD_INFO *CardInfo  = &Instance->CardInfo;
  UINT32    *Csd       = CardInfo->CSD;
  UINT32     Structure = Csd[0] >> 30;

  if (Structure == 0) {
    UINT32 ReadBlLen  = (Csd[1] >> 16) & 0xF;
    UINT32 CSize      = ((Csd[1] & 0x3FF) << 2) | (Csd[2] >> 30);
    UINT32 CSizeMult  = (Csd[2] >> 15) & 0x7;

    if (ReadBlLen < 9 || ReadBlLen > 11) {
      DEBUG ((EFI_D_ERROR, "%a: The Card reported an Invalid Block Length of %u!\n", __FUNCTION__, ReadBlLen));
      return EFI_DEVICE_ERROR;
    }

    CardInfo->NumBlocks = ((UINTN)(CSize + 1) << (CSizeMult + 2)) << (ReadBlLen - 9);
  } else if (Structure == 1 || Structure == 2) {
    UINT32 Mask  = (Structure == 1) ? 0x3F : 0xFFF;
    UINT32 CSize = ((Csd[1] & Mask) << 16) | (Csd[2] >> 16);

    CardInfo->NumBlocks = (UINTN)(CSize + 1) * 1024;
  } else {
    DEBUG ((EFI_D_ERROR, "%a: Unknown CSD Structure Version %u!\n", __FUNCTION__, Structure));
    return EFI_UNSUPPORTED;
  }

  UINTN Reachable = CardInfo->HighCapacity ? 0x100000000ULL : (0x100000000ULL / 0x200);

  if (CardInfo->NumBlocks > Reachable) {
    DEBUG ((EFI_D_WARN,
            "%a: The Card holds %lu Blocks but only %lu are Addressable, Capping.\n",
            __FUNCTION__, (UINT64)CardInfo->NumBlocks, (UINT64)Reachable));

    CardInfo->NumBlocks = Reachable;
  }

  CardInfo->BlockSize = 0x200;

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
MshcSwitchTo1V8 (IN OUT MSHC_INSTANCE *Instance)
{
  EFI_STATUS Status;
  UINT32     Response = 0;

  Status = MshcSendCmd (&Instance->Controller, CMD11, 0, &Response);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: The Card refused CMD11! Status = %r\n", __FUNCTION__, Status));
    goto PowerCycle;
  }

  if (Response & R1_ERROR_MASK) {
    DEBUG ((EFI_D_ERROR, "%a: The Card reported an Error on CMD11! Status = 0x%08X\n", __FUNCTION__, Response));
    return EFI_DEVICE_ERROR;
  }

  gBS->Stall (1000);

  if (!MshcIsCardBusy (&Instance->Controller)) {
    DEBUG ((EFI_D_ERROR, "%a: The Card is still busy.\n", __FUNCTION__));
    Status = EFI_DEVICE_ERROR;
    goto PowerCycle;
  }

  Status = MshcHostSwitchTo1V8 (&Instance->Controller);
  if (EFI_ERROR (Status)) {
    goto PowerCycle;
  }

  gBS->Stall (1000);

  if (MshcIsCardBusy (&Instance->Controller)) {
    DEBUG ((EFI_D_ERROR, "%a: The Card is still busy.\n", __FUNCTION__));
    Status = EFI_DEVICE_ERROR;
    goto PowerCycle;
  }

  return EFI_SUCCESS;

PowerCycle:
  MshcBoardPowerCycle (&Instance->Controller.Host);

  return Status;
}

STATIC
EFI_STATUS
MshcIdentifyCard (IN OUT MSHC_INSTANCE *Instance)
{
  CARD_INFO  *CardInfo            = &Instance->CardInfo;
  EFI_STATUS  Status;
  UINT32      Response[4];
  UINT32      HostCapacitySupport = 0;

  CardInfo->RCA         = 0;
  CardInfo->SupportsUhs = FALSE;
  CardInfo->BusAt1V8    = FALSE;

  Status = MshcSendCmd (&Instance->Controller, CMD0, 0, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Send CMD0! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  gBS->Stall (2000);

  Status = MshcSendCmd (&Instance->Controller, CMD8, CMD8_ARG, &Response[0]);
  if (!EFI_ERROR (Status)) {
    if ((Response[0] & CMD8_RESPONSE_MASK) != CMD8_ARG) {
      DEBUG ((EFI_D_ERROR, "%a: Unable to switch voltage! Response = 0x%08X\n", __FUNCTION__, Response[0]));
      return EFI_UNSUPPORTED;
    }

    CardInfo->CardType  = SD_CARD_2;
    HostCapacitySupport = OCR_HCS | OCR_S18R;

  } else if (Status == EFI_NO_RESPONSE) {
    DEBUG ((EFI_D_ERROR, "%a: Card did not Answer CMD8, SD 1.\n", __FUNCTION__));
    CardInfo->CardType = SD_CARD;
  } else {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Send CMD8! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  for (UINTN Retry = 0; ; Retry++) {
    Status = MshcSendAppCmd (Instance, ACMD41, HostCapacitySupport | OCR_VOLTAGE_WINDOW, &Response[0]);
    if (EFI_ERROR (Status)) {
      if (Status == EFI_NO_RESPONSE) {
        // Most likely eMMC.
        Status = MshcSendCmd (&Instance->Controller, CMD0, 0, NULL);
        if (EFI_ERROR (Status)) {
          DEBUG ((EFI_D_ERROR, "%a: Failed to Send CMD0! Status = %r\n", __FUNCTION__, Status));
          return Status;
        }

        gBS->Stall (2000);

        return MshcMmcIdentify (Instance);
      }

      DEBUG ((EFI_D_ERROR, "%a: Failed to Send ACMD41! Status = %r\n", __FUNCTION__, Status));
      return Status;
    }

    if (Response[0] & OCR_BUSY) {
      break;
    }

    if (Retry >= ACMD41_TIMEOUT_MS) {
      DEBUG ((EFI_D_ERROR, "%a: The Card never left the Power Up Phase!\n", __FUNCTION__));
      return EFI_TIMEOUT;
    }

    gBS->Stall (1000);
  }

  CardInfo->OCR = Response[0];

  if ((CardInfo->CardType == SD_CARD_2) && (Response[0] & OCR_CCS)) {
    CardInfo->CardType     = SD_CARD_2_HIGH;
    CardInfo->HighCapacity = TRUE;
  }

  // The card answers in the same bit it was asked in
  CardInfo->SupportsUhs = !!(Response[0] & OCR_S18A);

  if (CardInfo->SupportsUhs && !Instance->SkipUhs) {
    Status = MshcSwitchTo1V8 (Instance);

    if (EFI_ERROR (Status)) {
      Instance->SkipUhs = TRUE;
      return EFI_NOT_READY;
    }

    CardInfo->BusAt1V8 = TRUE;
  }

  DEBUG ((EFI_D_WARN,
          "%a: Found an %a Card. OCR = 0x%08X\n",
          __FUNCTION__,
          CardInfo->HighCapacity ? "SDHC/SDXC" : "SDSC",
          CardInfo->OCR));

  Status = MshcSendCmd (&Instance->Controller, CMD2, 0, CardInfo->CID);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Send CMD2! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  Status = MshcSendCmd (&Instance->Controller, CMD3, 0, &Response[0]);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Send CMD3! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  CardInfo->RCA = (UINT16)(Response[0] >> 16);

  DEBUG ((EFI_D_ERROR, "%a: The Card published RCA 0x%04X.\n", __FUNCTION__, CardInfo->RCA));

  return EFI_SUCCESS;
}

STATIC
BOOLEAN
MshcSwitchAccessMode (
  IN OUT MSHC_INSTANCE *Instance,
  IN     UINT8          Mode)
{
  EFI_STATUS Status;
  UINT32     Aligned[SD_SWITCH_STATUS_SIZE / sizeof (UINT32)];
  UINT8     *SwitchStatus = (UINT8 *)Aligned;
  UINT32     Group1;

  if (Instance->CardInfo.SpecVersion < SCR_SPEC_VER_110) {
    DEBUG ((EFI_D_ERROR, "%a: The Card predates the Switch Command.\n", __FUNCTION__));
    return FALSE;
  }

  if (!(((Instance->CardInfo.CSD[1] >> 20) & 0xFFF) & CCC_SWITCH)) {
    DEBUG ((EFI_D_ERROR, "%a: The Card lacks the Switch Command Class.\n", __FUNCTION__));
    return FALSE;
  }

  Status = MshcPrepareTransfer (&Instance->Controller, SwitchStatus, SD_SWITCH_STATUS_SIZE, 1, READ);
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  Status = MshcSendCmd (&Instance->Controller, CMD6, SD_SWITCH_CHECK (Mode), NULL);
  if (EFI_ERROR (Status)) {
    MshcRecover (&Instance->Controller);
    return FALSE;
  }

  Status = MshcReadData (&Instance->Controller, SwitchStatus, SD_SWITCH_STATUS_SIZE);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: The Access Mode Query Failed, Staying at Default Speed.\n", __FUNCTION__));
    MshcRecover (&Instance->Controller);
    return FALSE;
  }

  Group1 = ((UINT32)SwitchStatus[12] << 8) | SwitchStatus[13];

  if (!(Group1 & (1u << Mode))) {
    DEBUG ((EFI_D_ERROR, "%a: The Card does not Offer Access Mode %u.\n", __FUNCTION__, Mode));
    return FALSE;
  }

  Status = MshcPrepareTransfer (&Instance->Controller, SwitchStatus, SD_SWITCH_STATUS_SIZE, 1, READ);
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  Status = MshcSendCmd (&Instance->Controller, CMD6, SD_SWITCH_SET (Mode), NULL);
  if (EFI_ERROR (Status)) {
    MshcRecover (&Instance->Controller);
    return FALSE;
  }

  Status = MshcReadData (&Instance->Controller, SwitchStatus, SD_SWITCH_STATUS_SIZE);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: The Switch to Access Mode %u Failed, Staying put.\n", __FUNCTION__, Mode));
    MshcRecover (&Instance->Controller);
    return FALSE;
  }

  if ((SwitchStatus[16] & 0xF) != Mode) {
    DEBUG ((EFI_D_WARN, "%a: The Card declined Access Mode %u.\n", __FUNCTION__, Mode));
    return FALSE;
  }

  gBS->Stall (1000);

  return TRUE;
}

STATIC
EFI_STATUS
MshcConfigureCard (IN OUT MSHC_INSTANCE *Instance)
{
  CARD_INFO  *CardInfo  = &Instance->CardInfo;
  MSHC_HOST  *Host      = &Instance->Controller.Host;
  EFI_STATUS  Status;
  UINTN       Frequency = MSHC_CLK_25M;

  if (CardInfo->CardType == MMC_CARD) {
    return MshcMmcConfigure (Instance);
  }

  Status = MshcSendCmd (&Instance->Controller, CMD9, (UINT32)CardInfo->RCA << 16, CardInfo->CSD);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Send CMD9! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  Status = MshcDecodeCsd (Instance);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = MshcSendCmd (&Instance->Controller, CMD7, (UINT32)CardInfo->RCA << 16, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Send CMD7! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  Status = MshcReadScr (Instance);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = MshcSendCmd (&Instance->Controller, CMD16, 0x200, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Send CMD16! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  if (CardInfo->Supports4Bit) {
    Status = MshcSendAppCmd (Instance, ACMD6, SD_BUS_WIDTH_4, NULL);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%a: Failed to Send ACMD6! Status = %r\n", __FUNCTION__, Status));
      return Status;
    }

    MshcSetBusWidth (&Instance->Controller, 4);
  } else {
    DEBUG ((EFI_D_ERROR, "%a: The Card keeps to a Single Line.\n", __FUNCTION__));
  }

  if (CardInfo->BusAt1V8) {
    if (MshcSwitchAccessMode (Instance, SD_ACCESS_MODE_SDR104)) {
      Status = MshcSetTiming (&Instance->Controller, Host->ClkSelTimingSdr104, MSHC_CLK_200M);

      if (!EFI_ERROR (Status)) {
        UINT32 Window = MshcTuneSamplePhase (Instance);

        if (Window >= MSHC_TUNING_MIN_RUN) {
          DEBUG ((EFI_D_ERROR, "%a: Running at SDR104.\n", __FUNCTION__));
          return Status;
        }

        MshcSetTiming (&Instance->Controller, Host->ClkSelTimingSdr50, MSHC_CLK_100M);
      }
    }

    if (MshcSwitchAccessMode (Instance, SD_ACCESS_MODE_SDR50)) {
      DEBUG ((EFI_D_ERROR, "%a: Running at SDR50.\n", __FUNCTION__));

      Status = MshcSetTiming (&Instance->Controller, Host->ClkSelTimingSdr50, MSHC_CLK_100M);

      if (!EFI_ERROR (Status)) {
        MshcTuneSamplePhase (Instance);
      }

      return Status;
    }
  }

  if (MshcSwitchAccessMode (Instance, SD_ACCESS_MODE_HS)) {
    Frequency = MSHC_CLK_50M;
  }

  Status = MshcSetClockFrequency (&Instance->Controller, Frequency);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to set the Bus Clock! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
MshcDetectCard (IN OUT MSHC_INSTANCE *Instance)
{
  EFI_STATUS Status;

  ZeroMem (&Instance->CardInfo, sizeof (CARD_INFO));

  Status = MshcInitialize (&Instance->Controller);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Initialize the Host Controller! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  Status = MshcSetClockFrequency (&Instance->Controller, MSHC_CLK_400K);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to set the Identification Clock! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  Status = MshcIdentifyCard (Instance);

  if ((Status == EFI_NOT_READY) && Instance->SkipUhs) {
    return MshcDetectCard (Instance);
  }

  if (EFI_ERROR (Status)) {
    return Status;
  }

  return MshcConfigureCard (Instance);
}

STATIC
EFI_STATUS
MshcRecoverCard (IN OUT MSHC_INSTANCE *Instance)
{
  EFI_STATUS Status;
  UINT32     PreviousCid[4];
  UINTN      PreviousBlocks = Instance->CardInfo.NumBlocks;

  CopyMem (PreviousCid, Instance->CardInfo.CID, sizeof (PreviousCid));

  Status = MshcBoardPowerCycle (&Instance->Controller.Host);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Power Cycle the Card! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  Status = MshcDetectCard (Instance);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: The Card did not come back! Status = %r\n", __FUNCTION__, Status));

    Instance->Media.MediaPresent = FALSE;
    Instance->Media.LastBlock    = 0;

    return Status;
  }

  Instance->Media.MediaPresent = TRUE;
  Instance->Media.ReadOnly     = MshcBoardIsReadOnly (&Instance->Controller.Host);
  Instance->Media.LastBlock    = Instance->CardInfo.NumBlocks - 1;

  if (CompareMem (PreviousCid, Instance->CardInfo.CID, sizeof (PreviousCid)) != 0) {
    DEBUG ((EFI_D_ERROR, "%a: A different Card answered after the Cycle.\n", __FUNCTION__));
    Instance->Media.MediaId++;
    return EFI_MEDIA_CHANGED;
  }

  if (Instance->CardInfo.NumBlocks != PreviousBlocks) {
    DEBUG ((EFI_D_ERROR,
            "%a: The Card came back a different Size, %lu Blocks against %lu.\n",
            __FUNCTION__, (UINT64)Instance->CardInfo.NumBlocks, (UINT64)PreviousBlocks));
    Instance->Media.MediaId++;
    return EFI_MEDIA_CHANGED;
  }

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
MshcTransferBlock (
  IN OUT MSHC_INSTANCE *Instance,
  IN     UINTN          Lba,
  IN OUT VOID          *Buffer,
  IN     UINTN          BlockCount,
  IN     OPERATION_TYPE OperationType)
{
  EFI_STATUS Status;
  UINTN      Cmd;
  UINTN      CmdArgument;
  UINT32     Response = 0;

  if (OperationType == READ) {
    Cmd = (BlockCount > 1) ? CMD18 : CMD17;
  } else {
    Cmd = (BlockCount > 1) ? CMD25 : CMD24;
  }

  CmdArgument = Instance->CardInfo.HighCapacity ? Lba : (Lba * 0x200);

  Status = MshcPrepareTransfer (&Instance->Controller, Buffer, 0x200, BlockCount, OperationType);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = MshcSendCmd (&Instance->Controller, Cmd, CmdArgument, &Response);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Send CMD%u! Status = %r\n", __FUNCTION__, INDX (Cmd), Status));
    return Status;
  }

  if (Response & R1_ERROR_MASK) {
    DEBUG ((EFI_D_ERROR,
            "%a: The Card rejected CMD%u at LBA %u! Status = 0x%08X\n",
            __FUNCTION__, INDX (Cmd), Lba, Response));

    MshcRecover (&Instance->Controller);
    return EFI_DEVICE_ERROR;
  }

  if (OperationType == READ) {
    Status = MshcReadData (&Instance->Controller, Buffer, BlockCount * 0x200);
  } else {
    Status = MshcWriteData (&Instance->Controller, Buffer, BlockCount * 0x200);
  }

  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (BlockCount > 1) {
    return MshcWaitAutoStop (&Instance->Controller);
  }

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
MshcReadWrite (
  IN     EFI_BLOCK_IO_PROTOCOL *This,
  IN     UINT32                 MediaId,
  IN     EFI_LBA                Lba,
  IN OUT VOID                  *Buffer,
  IN     UINTN                  BufferSize,
  IN     OPERATION_TYPE         OperationType)
{
  MSHC_INSTANCE *Instance = MSHC_INSTANCE_FROM_BLOCKIO (This);
  EFI_STATUS     Status   = EFI_SUCCESS;
  EFI_TPL        OldTpl;
  UINTN          Remaining;
  UINT8         *Position = Buffer;

  // Check Buffer Params
  if (Buffer == NULL || BufferSize == 0) {
    return EFI_INVALID_PARAMETER;
  }

  // Check card presence
  if (!This->Media->MediaPresent) {
    return EFI_NO_MEDIA;
  }

  // Check MediaId
  if (MediaId != This->Media->MediaId) {
    return EFI_MEDIA_CHANGED;
  }

  // Check WP
  if ((OperationType == WRITE) && This->Media->ReadOnly) {
    return EFI_WRITE_PROTECTED;
  }

  // Check if the LBA is Valid
  if (Lba > This->Media->LastBlock) {
    DEBUG ((EFI_D_ERROR, "%a: The LBA is Beyond the Card Capacity!\n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }

  // Check if the Buffer Size is Aligned
  if ((BufferSize % This->Media->BlockSize) != 0) {
    DEBUG ((EFI_D_ERROR, "%a: The Buffer Size is not Aligned!\n", __FUNCTION__));
    return EFI_BAD_BUFFER_SIZE;
  }

  for (Remaining = BufferSize; Remaining > 0; ) {
    UINTN ThisPass   = MIN (Remaining, MAX_MSHC_TRANSFER_SIZE);
    UINTN BlockCount = ThisPass / This->Media->BlockSize;

    BOOLEAN HoldsTheBus = !MshcIsDmaActive (&Instance->Controller);

    if (HoldsTheBus) {
      OldTpl = gBS->RaiseTPL (TPL_NOTIFY);
    }

    Status = MshcTransferBlock (Instance, (UINTN)Lba, Position, BlockCount, OperationType);

    if (HoldsTheBus) {
      gBS->RestoreTPL (OldTpl);
    }

    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%a: The Transfer at LBA %lu Failed! Status = %r\n", __FUNCTION__, Lba, Status));
      break;
    }

    Remaining -= ThisPass;
    Position  += ThisPass;
    Lba       += BlockCount;
  }

  return Status;
}

EFI_STATUS
EFIAPI
MshcReset (
  IN EFI_BLOCK_IO_PROTOCOL *This,
  IN BOOLEAN                ExtendedVerification)
{
  MSHC_INSTANCE *Instance = MSHC_INSTANCE_FROM_BLOCKIO (This);
  EFI_STATUS     Status;
  EFI_TPL        OldTpl;
  UINT32         PreviousCid[4];

  if (!ExtendedVerification) {
    return MshcResetAll (&Instance->Controller);
  }

  OldTpl = gBS->RaiseTPL (TPL_NOTIFY);

  CopyMem (PreviousCid, Instance->CardInfo.CID, sizeof (PreviousCid));

  Status = MshcDetectCard (Instance);

  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: The Card did not Answer, Power Cycling it.\n", __FUNCTION__));

    Status = MshcRecoverCard (Instance);

    gBS->RestoreTPL (OldTpl);

    return Status;
  }

  This->Media->MediaPresent = TRUE;
  This->Media->ReadOnly     = MshcBoardIsReadOnly (&Instance->Controller.Host);
  This->Media->LastBlock    = Instance->CardInfo.NumBlocks - 1;

  if (CompareMem (PreviousCid, Instance->CardInfo.CID, sizeof (PreviousCid)) != 0) {
    DEBUG ((EFI_D_ERROR, "%a: A different Card is now Seated.\n", __FUNCTION__));
    This->Media->MediaId++;
  }

  gBS->RestoreTPL (OldTpl);

  return Status;
}

EFI_STATUS
EFIAPI
MshcReadBlocks (
  IN  EFI_BLOCK_IO_PROTOCOL *This,
  IN  UINT32                 MediaId,
  IN  EFI_LBA                Lba,
  IN  UINTN                  BufferSize,
  OUT VOID                  *Buffer)
{
  return MshcReadWrite (This, MediaId, Lba, Buffer, BufferSize, READ);
}

EFI_STATUS
EFIAPI
MshcWriteBlocks (
  IN EFI_BLOCK_IO_PROTOCOL *This,
  IN UINT32                 MediaId,
  IN EFI_LBA                Lba,
  IN UINTN                  BufferSize,
  IN VOID                  *Buffer)
{
  return MshcReadWrite (This, MediaId, Lba, Buffer, BufferSize, WRITE);
}

EFI_STATUS
EFIAPI
MshcFlushBlocks (IN EFI_BLOCK_IO_PROTOCOL *This)
{
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
MshcEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status;
  UINTN      HostCount;
  UINTN      Installed = 0;
  EFI_STATUS LastError = EFI_NOT_FOUND;

  HostCount = MshcBoardGetHostCount ();
  if (HostCount == 0) {
    DEBUG ((EFI_D_ERROR, "%a: No MSHC Controllers on this Board.\n", __FUNCTION__));
    return EFI_UNSUPPORTED;
  }

  for (UINTN Index = 0; Index < HostCount; Index++) {
    MSHC_INSTANCE *Instance = AllocateZeroPool (sizeof (MSHC_INSTANCE));
    if (Instance == NULL) {
      LastError = EFI_OUT_OF_RESOURCES;
      continue;
    }

    Instance->Signature              = MSHC_INSTANCE_SIGNATURE;
    Instance->Media.MediaId          = SIGNATURE_32 ('m', 's', 'h', 'c') + (UINT32)Index;
    Instance->Media.RemovableMedia   = TRUE;
    Instance->Media.MediaPresent     = FALSE;
    Instance->Media.LogicalPartition = FALSE;
    Instance->Media.ReadOnly         = FALSE;
    Instance->Media.WriteCaching     = FALSE;
    Instance->Media.BlockSize        = 0x200;
    Instance->Media.IoAlign          = 4;
    Instance->Media.LastBlock        = 0;

    Instance->BlockIo.Revision       = EFI_BLOCK_IO_PROTOCOL_REVISION;
    Instance->BlockIo.Media          = &Instance->Media;
    Instance->BlockIo.Reset          = MshcReset;
    Instance->BlockIo.ReadBlocks     = MshcReadBlocks;
    Instance->BlockIo.WriteBlocks    = MshcWriteBlocks;
    Instance->BlockIo.FlushBlocks    = MshcFlushBlocks;

    Instance->DevicePath.Mmc.Header.Type    = HARDWARE_DEVICE_PATH;
    Instance->DevicePath.Mmc.Header.SubType = HW_VENDOR_DP;
    SetDevicePathNodeLength (&Instance->DevicePath.Mmc.Header, sizeof (VENDOR_DEVICE_PATH) + sizeof (UINT8));
    CopyGuid (&Instance->DevicePath.Mmc.Guid, &mMshcDevicePathGuid);
    Instance->DevicePath.Controller = (UINT8)Index;
    SetDevicePathEndNode (&Instance->DevicePath.End);

    Status = MshcBoardInit (Index, &Instance->Controller.Host);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%a: Failed to Initialize Board Specifics for Controller %u! Status = %r\n", __FUNCTION__, Index, Status));
      FreePool (Instance);
      LastError = Status;
      continue;
    }

    if (!MshcBoardIsCardPresent (&Instance->Controller.Host)) {
      DEBUG ((EFI_D_ERROR, "%a: Controller %u Card Slot is Empty.\n", __FUNCTION__, Index));
      FreePool (Instance);
      LastError = EFI_NOT_FOUND;
      continue;
    }

    Status = MshcDetectCard (Instance);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%a: Failed to Detect Card on Controller %u! Status = %r\n", __FUNCTION__, Index, Status));
      MshcDeinitDma (&Instance->Controller);
      FreePool (Instance);
      LastError = Status;
      continue;
    }

    Instance->Media.MediaPresent   = TRUE;
    Instance->Media.RemovableMedia = (Instance->CardInfo.CardType != MMC_CARD);
    Instance->Media.ReadOnly       = MshcBoardIsReadOnly (&Instance->Controller.Host);
    Instance->Media.LastBlock      = Instance->CardInfo.NumBlocks - 1;

    Status = gBS->InstallMultipleProtocolInterfaces (
                    &Instance->Handle,
                    &gEfiBlockIoProtocolGuid,
                    &Instance->BlockIo,
                    &gEfiDevicePathProtocolGuid,
                    &Instance->DevicePath,
                    NULL);

    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%a: Failed to Install the Block I/O Protocol for Controller %u! Status = %r\n", __FUNCTION__, Index, Status));
      MshcDeinitDma (&Instance->Controller);
      FreePool (Instance);
      LastError = Status;
      continue;
    }

    Installed++;
  }

  return (Installed > 0) ? EFI_SUCCESS : LastError;
}
