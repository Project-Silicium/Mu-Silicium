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

#include <Library/CacheMaintenanceLib.h>
#include <Library/TimerLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryMapHelperLib.h>

#include "MshcDxe.h"

//
// Global Variables
//
UINTN           gMshcBaseAddr;
CARD_INFO       gCardInfo;
BOOLEAN         gMediaChange    = TRUE;
BOOLEAN         gCardInit       = FALSE;

EFI_BLOCK_IO_MEDIA gSdMmcMedia = {
  SIGNATURE_32('e','m','m','c'),      // MediaId
  FALSE,                              // RemovableMedia
  TRUE,                               // MediaPresent
  FALSE,                              // LogicalPartition
  FALSE,                              // ReadOnly
  FALSE,                              // WriteCaching
  512,                                // BlockSize
  4,                                  // IoAlign
  0,                                  // Pad
  0                                   // LastBlock
};

MSHC_DEVICE_PATH gMshcDevicePath = {
  {
    HARDWARE_DEVICE_PATH,
    HW_VENDOR_DP,
    (UINT8)(sizeof (VENDOR_DEVICE_PATH)),
    (UINT8)((sizeof (VENDOR_DEVICE_PATH)) >> 8),
    0xb615f1f5, 0x5088, 0x43cd, 0x80, 0x9c, 0xa1, 0x6e, 0x52, 0x48, 0x7d, 0x00
  },
  {
    END_DEVICE_PATH_TYPE,
    END_ENTIRE_DEVICE_PATH_SUBTYPE,
    sizeof (EFI_DEVICE_PATH_PROTOCOL),
    0
  }
};

//
// Internal Functions
//

VOID
MshcParseCidData (
  UINT32 Response0,
  UINT32 Response1,
  UINT32 Response2,
  UINT32 Response3)
{
  gCardInfo.CIDData.MDT = (Response0 >> 8) & 0xFFF;
  gCardInfo.CIDData.PSN = ((Response0 >> 24) & 0xFF) | ((Response1 & 0xFFFFFF) << 8);
  gCardInfo.CIDData.PRV = (Response1 >> 24) & 0xFF;
  gCardInfo.CIDData.PNM[4] = (Response2) & 0xFF;
  gCardInfo.CIDData.PNM[3] = (Response2 >> 8) & 0xFF;
  gCardInfo.CIDData.PNM[2] = (Response2 >> 16) & 0xFF;
  gCardInfo.CIDData.PNM[1] = (Response2 >> 24) & 0xFF;
  gCardInfo.CIDData.PNM[0] = (Response3) & 0xFF;
  gCardInfo.CIDData.OID = (Response3 >> 8) & 0xFFFF;
  gCardInfo.CIDData.MID = (Response3 >> 24) & 0xFF;
}

EFI_STATUS
MshcGetCsd ()
{
  EFI_STATUS Status;
  UINTN      Resp[4];
  UINTN      CmdArgument = 0;

  // Send CMD9 to Retrieve CSD
  CmdArgument = gCardInfo.RCA << 16;
  Status = MshcSendCmd (CMD9, CMD9_INT_EN, CmdArgument);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Send CMD9! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  Resp[0] = MmioRead32 (gMshcBaseAddr + MSHCI_RESP0);
  Resp[1] = MmioRead32 (gMshcBaseAddr + MSHCI_RESP1);
  Resp[2] = MmioRead32 (gMshcBaseAddr + MSHCI_RESP2);
  Resp[3] = MmioRead32 (gMshcBaseAddr + MSHCI_RESP3);

  // Populate 128-bit CSD Register Data
  for (UINT32 i = 0; i < 4; i++) {
    ((UINT32 *)&(gCardInfo.CSDData))[i] = Resp[i];
  }

  return Status;
}

EFI_STATUS
MshcSetCardInfo ()
{
  EFI_STATUS Status;
  UINT32     ExtCsd[128];
  UINTN      CmdArgument;

  // Send the HS Timing Command
  CmdArgument = (MMC_SWITCH_MODE_WRITE_BYTE << 24) | (EXT_CSD_HS_TIMING << 16) | (0 << 8);
  Status = MshcSendCmd (ACMD6, ACMD6_INT_EN, CmdArgument);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Send ACMD6! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  // Set the Bus Width for EXT CSD
  CmdArgument = (MMC_SWITCH_MODE_WRITE_BYTE << 24) | (EXT_CSD_BUS_WIDTH << 16) | (EXT_CSD_BUS_WIDTH_1 << 8);
  Status = MshcSendCmd (ACMD6, ACMD6_INT_EN, CmdArgument);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Send ACMD6! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  // Prepare a Transfer to Get EXT CSD
  Status = MshcPrepareTransfer (ExtCsd, 1, READ);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Prepare Transfer for EXT CSD! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  // Send CMD8 to Get EXT CSD
  CmdArgument = 0;
  Status = MshcSendCmd (CMD8, CMD8_INT_EN, CmdArgument);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Send CMD8! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }
  
  if (!EFI_ERROR (Status)) {
    // Read EXT CSD
    MshcReadDma (ExtCsd, 1);
    
    // Set the Card Info
    gCardInfo.CardType       = MMC_CARD;
    gCardInfo.BlockSize      = BLEN_512BYTES;
    gCardInfo.NumBlocks      = ExtCsd[EXT_CSD_SEC_CNT / 4];
    gCardInfo.TotalNumBlocks = gCardInfo.NumBlocks;

  } else {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Get EXT CSD!\n", __FUNCTION__));
    return Status;
  }

  MshcSetClockFrequency (MSHC_CLK_25M);
  
  return Status;
}

EFI_STATUS
MshcIdentifyCard ()
{
  EFI_STATUS Status;
  UINTN      Resp[4];
  UINTN      CmdArgument = 0;
  UINT32     Timeout     = MAX_RETRY_COUNT;
  
  // Send CMD0 to Reset the Card
  Status = MshcSendCmd (CMD0, CMD0_INT_EN, CmdArgument);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Send CMD1! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  // Send CMD1 to Get the OCR Register
  do {
    CmdArgument = OCR_HCS | MMC_VDD_32_33 | MMC_VDD_33_34;
    Status = MshcSendCmd (CMD1, CMD1_INT_EN, CmdArgument);

    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%a: Failed to Send CMD1! Status = %r\n", __FUNCTION__, Status));
      return Status;
    }
    // Repeat until the Command is Done
    MicroSecondDelay(100);
  } while (!(MmioRead32 (gMshcBaseAddr + MSHCI_RESP0) & OCR_BUSY) && Timeout--);

  if (Timeout <= 0) {
    DEBUG ((EFI_D_ERROR, "%a: CMD1 OCR busy Timeout!\n", __FUNCTION__));
    return EFI_DEVICE_ERROR;
  }

  // Set OCR Data
  ((UINT32 *)&(gCardInfo.OCRData))[0] = MmioRead32 (gMshcBaseAddr + MSHCI_RESP0);

  // Send CMD2 to Get the CID Register
  CmdArgument = 0;
  Status = MshcSendCmd (CMD2, CMD2_INT_EN, CmdArgument);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Send CMD2! Status = %r\n", __FUNCTION__,Status));
    return Status;
  }

  Resp[0] = MmioRead32 (gMshcBaseAddr + MSHCI_RESP0);
  Resp[1] = MmioRead32 (gMshcBaseAddr + MSHCI_RESP1);
  Resp[2] = MmioRead32 (gMshcBaseAddr + MSHCI_RESP2);
  Resp[3] = MmioRead32 (gMshcBaseAddr + MSHCI_RESP3);

  // Parse the CID Register Data
  MshcParseCidData(Resp[0] << 8, (Resp[1] << 8) | (Resp[0] >> 24), (Resp[2] << 8) | (Resp[1] >> 24), (Resp[3] << 8) | (Resp[2] >> 24));

  // Send CMD3 to Get the RCA Register  
  CmdArgument = 0;
  Status = MshcSendCmd (CMD3, CMD3_INT_EN, CmdArgument);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Send CMD3! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  // Set RCA Data
  gCardInfo.RCA = (MmioRead32 ((gMshcBaseAddr + MSHCI_RESP0)) >> 16);

  MicroSecondDelay(1000);
  return EFI_SUCCESS;
}

EFI_STATUS
MshcConfigureCard ()
{
  EFI_STATUS Status;
  UINTN      CmdArgument = 0;

  // Send CMD7
  CmdArgument = gCardInfo.RCA << 16;
  Status = MshcSendCmd (CMD7, CMD7_INT_EN, CmdArgument);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Send CMD7! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  // Set the Card Info from the EXT CSD Register
  Status = MshcSetCardInfo ();
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Set the Card Info! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  // Add a small Delay for CMD16 to Succeed later
  MicroSecondDelay (10);

  // Send CMD16 to Set the Block Length
  CmdArgument = BLEN_512BYTES;
  Status = MshcSendCmd (CMD16, CMD16_INT_EN, CmdArgument);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Send CMD16! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }
  
  // Send CMD6 to Set the Device to 8-bit Mode
  CmdArgument = (MMC_SWITCH_MODE_WRITE_BYTE << 24) | (EXT_CSD_BUS_WIDTH << 16) | (EXT_CSD_BUS_WIDTH_8_DDR << 8);
  Status = MshcSendCmd (ACMD6, ACMD6_INT_EN, CmdArgument);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Send ACMD6! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }
  
  // Set the host controler to 8-bit mode
  MmioOr32 (gMshcBaseAddr + MSHCI_CTYPE, CARD_WIDTH8);
    
  return EFI_SUCCESS;
}

EFI_STATUS
MshcDetectCard ()
{
  EFI_STATUS Status;

  // Initialize the Host Controller
  Status = MshcInitialize ();
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Initialize the Host Controller! Status: %r\n", __FUNCTION__, Status));
    return Status;
  }

  // Set the Clock Frequency to 25 MHz for Card Configuration
  Status = MshcSetClockFrequency (MSHC_CLK_25M);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Set the Clock Frequency! Status: %r\n", __FUNCTION__, Status));
    return Status;
  }

  // Identify the Card
  Status = MshcIdentifyCard ();
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Identify the Card! Status: %r\n", __FUNCTION__, Status));
    return Status;
  }

  // Get Card Specific Data
  Status = MshcGetCsd ();
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Get CSD! Status: %r\n", __FUNCTION__, Status));
    return Status;
  }

  // Configure the Card
  Status = MshcConfigureCard ();
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Configure the Card! Status: %r\n", __FUNCTION__, Status));
    return Status;
  }

  // Patch the Media Structure
  gSdMmcMedia.LastBlock    = (gCardInfo.NumBlocks - 1);
  gSdMmcMedia.BlockSize    = 512;
  gSdMmcMedia.ReadOnly     = 0;
  gSdMmcMedia.MediaPresent = TRUE;
  gSdMmcMedia.MediaId++;

  // Set the Clock Frequency to 50 MHz for Normal Operation
  Status = MshcSetClockFrequency (MSHC_CLK_50M);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Set the Clock Frequency! Status: %r\n", __FUNCTION__, Status));
    return Status;
  }

  return Status;
}

EFI_STATUS
MshcTransferBlock (
  IN     EFI_BLOCK_IO_PROTOCOL *This,
  IN     UINTN                  Lba,
  IN OUT VOID                  *Buffer,
  IN     UINTN                  BlockCount,  
  IN     OPERATION_TYPE         OperationType)
{
  EFI_STATUS Status;
  UINTN      Cmd                = 0;
  UINTN      CmdInterruptEnable = 0;
  UINTN      CmdArgument        = 0;

  // Set the Command for the Operation Type
  if (OperationType == READ) {
    if (BlockCount > 1) {
      Cmd = CMD18; // Multi-block Read
      CmdInterruptEnable = CMD18_INT_EN;
    } else {
      Cmd = CMD17; // Single-block Read
      CmdInterruptEnable = CMD17_INT_EN;
    }
  } else if (OperationType == WRITE) {
    if (BlockCount > 1) {
      Cmd = CMD25; // Multi-block Write
      CmdInterruptEnable = CMD25_INT_EN;
    } else {
      Cmd = CMD24; // Single-block Write
      CmdInterruptEnable = CMD24_INT_EN;
    }
  }

  // Prepare the Transfer
  Status = MshcPrepareTransfer (Buffer, BlockCount, OperationType);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Prepare the Transfer! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  // Set the Command Argument for the Operation Type
  if (gCardInfo.OCRData.AccessMode & BIT1) {
    CmdArgument = Lba;
  } else {
    CmdArgument = Lba * This->Media->BlockSize;
  }

  // Send the Command
  Status = MshcSendCmd (Cmd, CmdInterruptEnable, CmdArgument);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Send CMD%d! Status = %r\n", __FUNCTION__, (Cmd & 0x3F), Status));
    return Status;
  }

  // Read or Write Data
  if (OperationType == READ) {
    Status = MshcReadDma (Buffer, BlockCount);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%a: Failed to Read Data! Status = %r\n", __FUNCTION__, Status));
      return Status;
    }
  } else if (OperationType == WRITE) {
    Status = MshcWriteDma (Buffer, BlockCount);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%a: Failed to Write Data! Status = %r\n", __FUNCTION__, Status));
      return Status;
    }
  }

  return EFI_SUCCESS;
}

EFI_STATUS
MshcReadWrite (
  IN  EFI_BLOCK_IO_PROTOCOL *This,
  IN  UINTN                  Lba,
  OUT VOID                  *Buffer,
  IN  UINTN                  BufferSize,
  IN  OPERATION_TYPE         OperationType)
{
  EFI_STATUS Status;
  UINTN      BlockCount;
  EFI_TPL    OldTpl;
  UINTN      RetryCount                   = 0;
  UINTN      BytesToBeTransferedThisPass  = 0;
  UINTN      BytesRemainingToBeTransfered = 0;

  // If the Card hasn't been Detected, Detect it
  if (gMediaChange == TRUE) {
    Status = MshcDetectCard ();
    
    gMediaChange = FALSE;
    
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%a: Failed to Detect the Card! Status = %r\n", __FUNCTION__, Status));
      gCardInit = FALSE;
      return Status;
    }
    gCardInit = TRUE;
  }

  // Check if the Buffer is Empty
  if (Buffer == NULL) {
    DEBUG ((EFI_D_ERROR, "%a: The Buffer is Empty!\n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
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
  
  // Get Current MSHC Status
  while ((RetryCount++ < MAX_RETRY_COUNT) && (MmioRead32 ((gMshcBaseAddr + MSHCI_STATUS)) & DATA_BUSY));
  
  if (RetryCount == MAX_RETRY_COUNT) {
    DEBUG ((EFI_D_ERROR, "%a: The Controller didn't Release in Time!\n", __FUNCTION__));
    return EFI_TIMEOUT;
  }

  OldTpl = gBS->RaiseTPL (TPL_NOTIFY);

  BytesRemainingToBeTransfered = BufferSize;

  while (BytesRemainingToBeTransfered > 0) {
    BytesToBeTransferedThisPass = (BytesRemainingToBeTransfered >= MAX_MSHC_TRANSFER_SIZE) ? MAX_MSHC_TRANSFER_SIZE : BytesRemainingToBeTransfered;
  
    BlockCount = BytesToBeTransferedThisPass / This->Media->BlockSize;
    Status = MshcTransferBlock (This, Lba, Buffer, BlockCount, OperationType);
  
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%a: The Block Transfer failed! Status = %r\n", __FUNCTION__, Status));
      goto ExitRestoreTPL;
    }
  
    BytesRemainingToBeTransfered -= BytesToBeTransferedThisPass;
    Lba += BlockCount;
    Buffer = (UINT8 *)Buffer + (This->Media->BlockSize * BlockCount);
  }
  
ExitRestoreTPL:
  gBS->RestoreTPL (OldTpl);

  return Status;
}


/**

  Reset the Block Device.



  @param  This         Indicates a pointer to the calling context.

  @param  ExtendedVerification Driver may perform diagnostics on reset.



  @retval EFI_SUCCESS      The device was reset.

  @retval EFI_DEVICE_ERROR   The device is not functioning properly and could

                 not be reset.



**/

EFI_STATUS
EFIAPI
MshcReset (
  IN EFI_BLOCK_IO_PROTOCOL *This,
  IN BOOLEAN               ExtendedVerification)
{
  return MshcResetAll ();
}


/**

  Read BufferSize bytes from Lba into Buffer.



  @param  This     Indicates a pointer to the calling context.

  @param  MediaId  Id of the media, changes every time the media is replaced.

  @param  Lba    The starting Logical Block Address to read from

  @param  BufferSize Size of Buffer, must be a multiple of device block size.

  @param  Buffer   A pointer to the destination buffer for the data. The caller is

           responsible for either having implicit or explicit ownership of the buffer.



  @retval EFI_SUCCESS       The data was read correctly from the device.

  @retval EFI_DEVICE_ERROR    The device reported an error while performing the read.

  @retval EFI_NO_MEDIA      There is no media in the device.

  @retval EFI_MEDIA_CHANGED   The MediaId does not matched the current device.

  @retval EFI_BAD_BUFFER_SIZE   The Buffer was not a multiple of the block size of the device.

  @retval EFI_INVALID_PARAMETER The read request contains LBAs that are not valid,

                or the buffer is not on proper alignment.

EFI_STATUS

**/

EFI_STATUS
EFIAPI
MshcReadBlocks (
  IN EFI_BLOCK_IO_PROTOCOL *This,
  IN UINT32                 MediaId,
  IN EFI_LBA                Lba,
  IN UINTN                  BufferSize,
  OUT VOID                 *Buffer)
{
  return MshcReadWrite (This, (UINTN)Lba, Buffer, BufferSize, READ);
}


/**

  Write BufferSize bytes from Lba into Buffer.



  @param  This     Indicates a pointer to the calling context.

  @param  MediaId  The media ID that the write request is for.

  @param  Lba    The starting logical block address to be written. The caller is

           responsible for writing to only legitimate locations.

  @param  BufferSize Size of Buffer, must be a multiple of device block size.

  @param  Buffer   A pointer to the source buffer for the data.



  @retval EFI_SUCCESS       The data was written correctly to the device.

  @retval EFI_WRITE_PROTECTED   The device can not be written to.

  @retval EFI_DEVICE_ERROR    The device reported an error while performing the write.

  @retval EFI_NO_MEDIA      There is no media in the device.

  @retval EFI_MEDIA_CHNAGED   The MediaId does not matched the current device.

  @retval EFI_BAD_BUFFER_SIZE   The Buffer was not a multiple of the block size of the device.

  @retval EFI_INVALID_PARAMETER The write request contains LBAs that are not valid,

                or the buffer is not on proper alignment.



**/

EFI_STATUS
EFIAPI
MshcWriteBlocks (
  IN EFI_BLOCK_IO_PROTOCOL *This,
  IN UINT32                 MediaId,
  IN EFI_LBA                Lba,
  IN UINTN                  BufferSize,
  IN VOID                  *Buffer)
{
  return MshcReadWrite (This, (UINTN)Lba, Buffer, BufferSize, WRITE);
}


/**

  Flush the Block Device.



  @param  This        Indicates a pointer to the calling context.



  @retval EFI_SUCCESS     All outstanding data was written to the device

  @retval EFI_DEVICE_ERROR  The device reported an error while writting back the data

  @retval EFI_NO_MEDIA    There is no media in the device.



**/

EFI_STATUS
EFIAPI
MshcFlushBlocks (IN EFI_BLOCK_IO_PROTOCOL *This)
{
  // Avoid Erasing the eMMC
  return EFI_SUCCESS;
}


EFI_BLOCK_IO_PROTOCOL gBlockIo = {
  EFI_BLOCK_IO_INTERFACE_REVISION,   // Revision
  &gSdMmcMedia,                      // *Media
  MshcReset,                         // Reset
  MshcReadBlocks,                    // ReadBlocks
  MshcWriteBlocks,                   // WriteBlocks
  MshcFlushBlocks                    // FlushBlocks
};


EFI_STATUS
EFIAPI
MshcEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable)
{
  EFI_MEMORY_REGION_DESCRIPTOR_EX MemRegion;
  EFI_STATUS                      Status;

  Status = LocateMemoryMapAreaByName ("MSHC", &MemRegion);
  if (EFI_ERROR (Status) && !MemRegion.Address) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate the MSHC Memory Region!\n"));
    return Status;
  }

  gMshcBaseAddr = MemRegion.Address;
  
  ZeroMem (&gCardInfo, sizeof (CARD_INFO));

  // Publish BlockIO
  Status = gBS->InstallMultipleProtocolInterfaces (
        &ImageHandle,
        &gEfiBlockIoProtocolGuid,
        &gBlockIo,
        &gEfiDevicePathProtocolGuid,
        &gMshcDevicePath,
        NULL);

  return Status;
}
