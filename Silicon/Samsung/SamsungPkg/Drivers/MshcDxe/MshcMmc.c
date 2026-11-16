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
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MshcHostBridge.h>

#include "MshcDxe.h"
#include "MshcHost.h"
#include "MshcDxe_CMD.h"

/**
  Writes a single Byte of the Extended CSD.
**/
STATIC
EFI_STATUS
MshcMmcSwitch (
  IN OUT MSHC_INSTANCE *Instance,
  IN     UINT8          Index,
  IN     UINT8          Value)
{
  EFI_STATUS Status;
  UINT32     Response = 0;

  Status = MshcSendCmd (&Instance->Controller, MMC_CMD6, MMC_SWITCH_ARG (Index, Value), &Response);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: CMD6 for Byte %u was Refused! Status = %r\n", __FUNCTION__, Index, Status));
    return Status;
  }

  if (Response & R1_ERROR_MASK) {
    DEBUG ((EFI_D_ERROR, "%a: The eMMC reported an Error on CMD6! Response = 0x%08X\n", __FUNCTION__, Response));
    return EFI_DEVICE_ERROR;
  }

  Status = MshcSendCmd (&Instance->Controller, CMD13, (UINT32)Instance->CardInfo.RCA << 16, &Response);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Read the eMMC Status! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  if (Response & R1_SWITCH_ERROR) {
    DEBUG ((EFI_D_ERROR, "%a: The eMMC rejected the Switch of Byte %u.\n", __FUNCTION__, Index));
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

/**
  Reads the Extended CSD into the Caller's Buffer.
**/
STATIC
EFI_STATUS
MshcMmcReadExtCsd (
  IN OUT MSHC_INSTANCE *Instance,
  OUT    UINT8         *ExtCsd)
{
  EFI_STATUS Status;

  SetMem (ExtCsd, EXT_CSD_SIZE, 0);

  Status = MshcPrepareTransfer (&Instance->Controller, ExtCsd, EXT_CSD_SIZE, 1, READ);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = MshcSendCmd (&Instance->Controller, MMC_CMD8, 0, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Send CMD8! Status = %r\n", __FUNCTION__, Status));
    MshcRecover (&Instance->Controller);
    return Status;
  }

  Status = MshcReadData (&Instance->Controller, ExtCsd, EXT_CSD_SIZE);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Read the Extended CSD! Status = %r\n", __FUNCTION__, Status));
    MshcRecover (&Instance->Controller);
    return Status;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
MshcMmcIdentify (IN OUT MSHC_INSTANCE *Instance)
{
  EFI_STATUS Status;
  UINT32     Response[4];
  UINTN      Elapsed;

  for (Elapsed = 0; ; Elapsed++) {
    Status = MshcSendCmd (&Instance->Controller, MMC_CMD1, MMC_OCR_SECTOR_MODE | MMC_OCR_VOLTAGE_WINDOW, &Response[0]);
    if (EFI_ERROR (Status)) {
      if (Elapsed == 0) {
        DEBUG ((EFI_D_ERROR, "%a: CMD1 Failed, no eMMC.\n", __FUNCTION__));
        return EFI_UNSUPPORTED;
      }

      DEBUG ((EFI_D_ERROR, "%a: CMD1 Failed! Status = %r\n", __FUNCTION__, Status));
      return Status;
    }

    if (Response[0] & OCR_BUSY) {
      break;
    }

    if (Elapsed >= MMC_OP_COND_TIMEOUT_MS) {
      return EFI_TIMEOUT;
    }

    MicroSecondDelay (1000);
  }

  Instance->CardInfo.CardType     = MMC_CARD;
  Instance->CardInfo.OCR          = Response[0];
  Instance->CardInfo.HighCapacity = !!(Response[0] & MMC_OCR_SECTOR_MODE);

  Status = MshcSendCmd (&Instance->Controller, CMD2, 0, Instance->CardInfo.CID);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Send CMD2! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  Status = MshcSendCmd (&Instance->Controller, MMC_CMD3, (UINT32)MMC_RCA << 16, &Response[0]);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Send CMD3! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  Instance->CardInfo.RCA = MMC_RCA;

  return EFI_SUCCESS;
}

/**
  Takes an Identified eMMC up to its Working Width and Speed.
**/
EFI_STATUS
MshcMmcConfigure (IN OUT MSHC_INSTANCE *Instance)
{
  EFI_STATUS Status;
  UINT32     Aligned[EXT_CSD_SIZE / sizeof (UINT32)];
  UINT8     *ExtCsd = (UINT8 *)Aligned;
  UINT8      Width;
  UINT8      WidthCode;

  Status = MshcSendCmd (&Instance->Controller, CMD9, (UINT32)Instance->CardInfo.RCA << 16, Instance->CardInfo.CSD);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Send CMD9! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  Status = MshcSendCmd (&Instance->Controller, CMD7, (UINT32)Instance->CardInfo.RCA << 16, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Send CMD7! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  Status = MshcSendCmd (&Instance->Controller, CMD16, 0x200, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Send CMD16! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  Status = MshcMmcReadExtCsd (Instance, ExtCsd);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Instance->CardInfo.BlockSize = 0x200;

  if (ExtCsd[EXT_CSD_REV] >= EXT_CSD_REV_SEC_CNT_VALID) {
    Instance->CardInfo.NumBlocks = (UINTN)ExtCsd[EXT_CSD_SEC_CNT]
                                 | ((UINTN)ExtCsd[EXT_CSD_SEC_CNT + 1] << 8)
                                 | ((UINTN)ExtCsd[EXT_CSD_SEC_CNT + 2] << 16)
                                 | ((UINTN)ExtCsd[EXT_CSD_SEC_CNT + 3] << 24);
  } else {
    Instance->CardInfo.NumBlocks = 0;
  }

  if (Instance->CardInfo.NumBlocks == 0) {
    DEBUG ((EFI_D_ERROR, "%a: The Extended CSD holds no Sector Count, Falling back to the CSD.\n", __FUNCTION__));

    Status = MshcDecodeCsd (Instance);
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }

  DEBUG ((EFI_D_ERROR,
          "%a: Extended CSD Revision %u, %lu Blocks.\n",
          __FUNCTION__,
          ExtCsd[EXT_CSD_REV],
          (UINT64)Instance->CardInfo.NumBlocks));

  Width = (Instance->Controller.Host.BusWidth == 8) ? 8 : (Instance->Controller.Host.BusWidth == 4) ? 4 : 1;

  if (Width > 1) {
    WidthCode = (Width == 8) ? EXT_CSD_BUS_WIDTH_8 : EXT_CSD_BUS_WIDTH_4;

    Status = MshcMmcSwitch (Instance, EXT_CSD_BUS_WIDTH, WidthCode);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%a: Staying on one Line, the eMMC would not Widen.\n", __FUNCTION__));
      Width = 1;
    }
  }

  MshcSetBusWidth (&Instance->Controller, Width);

  Status = MshcMmcSwitch (Instance, EXT_CSD_HS_TIMING, EXT_CSD_HS_TIMING_HS);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: The eMMC declined High Speed, Staying Slow.\n", __FUNCTION__));

    MshcSetClockFrequency (&Instance->Controller, MSHC_CLK_25M);
  } else {
    MshcSetClockFrequency (&Instance->Controller, MSHC_CLK_50M);
  }

  DEBUG ((EFI_D_ERROR, "%a: The eMMC is Running on %u Lines.\n", __FUNCTION__, Width));

  return EFI_SUCCESS;
}
