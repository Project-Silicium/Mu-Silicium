#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationHelperLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/HsI2cLib.h>
#include <Library/IoLib.h>

#include <Protocol/EFIUsi.h>
#include <Protocol/EFIGpio.h>
#include <Protocol/EFIHsI2c.h>

#include "HsI2c.h"

//
// Global Protocols
//
STATIC EFI_USI_PROTOCOL  *mUsiProtocol;
STATIC EFI_GPIO_PROTOCOL *mGpioProtocol;

//
// Global Variables
//
STATIC EFI_HSI2C_BUS_DATA *BusData;
STATIC UINT8               BusCount;

EFI_STATUS
SetupBusGpio (IN EFI_HSI2C_GPIO_DATA *Gpio)
{
  EFI_STATUS Status;

  // Configure SCL GPIO Pin
  Status = mGpioProtocol->SetFunction (Gpio->SclBankId, Gpio->SclBankNum, Gpio->SclPin, Gpio->Function);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Configure SDA GPIO Pin
  Status = mGpioProtocol->SetFunction (Gpio->SdaBankId, Gpio->SdaBankNum, Gpio->SdaPin, Gpio->Function);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}

VOID
ResetConfig (
  IN UINT8                 BusNumber,
  IN EFI_HSI2C_BUS        *Bus,
  IN EFI_HSI2C_SPEED_MODE  SpeedMode)
{
  UINT32 Config;

  // Get Current HSI2C Config
  Config = MmioRead32 ((UINTN)&Bus->conf);

  // Re-set Master & Trailing Count
  MmioWrite32 ((UINTN)&Bus->ctrl,       HSI2C_MASTER);
  MmioWrite32 ((UINTN)&Bus->trail_ctrl, HSI2C_TRAILING_COUNT);

  // Check Speed Mode
  if (SpeedMode == HIGH_SPD) {
    // Set Address
    MmioWrite32 ((UINTN)&Bus->address, HSI2C_MASTER_ID (MASTER_ID (BusNumber)));

    // Set High Speed Mode
    Config |= HSI2C_HS_MODE;
  }

  // Set Auto Mode
  Config |= HSI2C_AUTO_MODE;

  // Write new HSI2C Config
  MmioWrite32 ((UINTN)&Bus->conf, Config);
}

VOID
SetTiming (
  IN EFI_HSI2C_BUS        *Bus,
  IN EFI_HSI2C_CLOCK_DATA *Clock)
{
  BOOLEAN UseMask2         = FALSE;
  UINT32  OpClk            = 0;
  UINT32  Factor           = 0;
  UINT32  PostMinus        = 0;
  UINT32  Scalar           = 0;
  UINT32  DivFactor        = 0;
  UINTN   Reg1             = 0;
  UINTN   Reg2             = 0;
  UINTN   Reg3             = 0;

  // Set Config Values
  switch ((UINT8)Clock->SpeedMode) {
    case STAND_SPD:
      OpClk     = I2C_STAND_TX_CLOCK;
      Factor    = 16;
      Scalar    = 25;
      DivFactor = 10;
      UseMask2  = FALSE;
      Reg3      = (UINTN)&Bus->timing_fs3;
      Reg2      = (UINTN)&Bus->timing_fs2;
      Reg1      = (UINTN)&Bus->timing_fs1;
      break;

    case FAST_SPD:
      OpClk     = I2C_FS_TX_CLOCK;
      Factor    = 15;
      Scalar    = 9;
      DivFactor = 10;
      UseMask2  = TRUE;
      Reg3      = (UINTN)&Bus->timing_fs3;
      Reg2      = (UINTN)&Bus->timing_fs2;
      Reg1      = (UINTN)&Bus->timing_fs1;
      break;

    case FAST_PLUS_SPD:
      OpClk     = I2C_FAST_PLUS_TX_CLOCK;
      Factor    = 16;
      PostMinus = 1;
      Scalar    = 4;
      DivFactor = 10;
      UseMask2  = FALSE;
      Reg3      = (UINTN)&Bus->timing_fs3;
      Reg2      = (UINTN)&Bus->timing_fs2;
      Reg1      = (UINTN)&Bus->timing_fs1;
      break;

    case HIGH_SPD:
      OpClk     = I2C_HS_TX_CLOCK;
      Factor    = 15;
      Scalar    = 7;
      DivFactor = 100;
      UseMask2  = TRUE;
      Reg3      = (UINTN)&Bus->timing_hs3;
      Reg2      = (UINTN)&Bus->timing_hs2;
      Reg1      = (UINTN)&Bus->timing_hs1;
      break;

    default:
      return;
  }

  // Save Clock Freq
  UINT32 ClockFreq = Clock->Freq;

  // Calculate Divider & Base Timing
  UINT32 Div        = (ClockFreq / (OpClk * Factor) - PostMinus) & 0xFF;
  UINT32 BaseTiming = ((Scalar * ClockFreq) / 1000000) / ((Div + 1) * DivFactor);

  // Set Masked Timing
  UINT32 MaskedTiming       = UseMask2 ? TIMING_MASK_2 (BaseTiming)     : TIMING_MASK_1 (BaseTiming);
  UINT32 MaskedTimingMinus1 = UseMask2 ? TIMING_MASK_2 (BaseTiming - 1) : TIMING_MASK_1 (BaseTiming - 1);

  // Set new Timings
  MmioAndThenOr32 (Reg3, ~0x00FF0000, Div                << 16);
  MmioAndThenOr32 (Reg2, ~0x000000FF, MaskedTiming       <<  0);
  MmioAndThenOr32 (Reg1, ~0x00FF0000, MaskedTimingMinus1 << 16);
}

VOID
ResetController (IN EFI_HSI2C_BUS *Bus)
{
  // Reset Controller
  MmioOr32  ((UINTN)&Bus->ctrl,  HSI2C_SW_RST);
  MmioAnd32 ((UINTN)&Bus->ctrl, ~HSI2C_SW_RST);
}

EFI_STATUS
HsI2cInitBus (IN UINT8 BusNumber)
{
  EFI_STATUS           Status;
  EFI_PHYSICAL_ADDRESS BusAddress;

  // Verify Bus Number
  if (BusNumber >= BusCount) {
    return EFI_NOT_FOUND;
  }

  // Check Init Flag
  if (BusData[BusNumber].Initialized) {
    return EFI_SUCCESS;
  }

  // Get HSI2C Controller Address
  Status = mUsiProtocol->GetControllerAddr (BusNumber, BUS_I2C, &BusAddress);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Map HSI2C Bus Memory
  Status = MapMemoryRegion (BusAddress, HSI2C_MMIO_LENGTH, EfiMemoryMappedIO);
  if (EFI_ERROR (Status)) {
    return EFI_ALREADY_STARTED;
  }

  // Setup HSI2C Bus GPIO
  Status = SetupBusGpio (&BusData[BusNumber].Gpio);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Set USI Mode
  Status = mUsiProtocol->SetMode (BusAddress, MODE_I2C);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Get HSI2C Bus
  EFI_HSI2C_BUS *Bus = (EFI_HSI2C_BUS *)BusAddress;

  // Reset USI CON
  MmioWrite32 ((UINTN)&Bus->usi_con, 0x0);

  // Reset HSI2C Controller
  ResetController (Bus);

  // Set HSI2C Timing
  SetTiming (Bus, &BusData[BusNumber].Clock);

  // Reset HSI2C Config
  ResetConfig (BusNumber, Bus, BusData[BusNumber].Clock.SpeedMode);

  // Toggle Init Flag
  BusData[BusNumber].Initialized = TRUE;

  return EFI_SUCCESS;
}

/**
  Check if transfer has failed.
**/
STATIC
EFI_STATUS
CheckTransferError (IN EFI_HSI2C_BUS *Bus)
{
  UINT32 IntStatus = MmioRead32 ((UINTN)&Bus->int_status);

  if ((IntStatus & HSI2C_INT_CHK_TRANS_STATE) == 0) {
    return EFI_SUCCESS;
  }

  // Write One to Clear, so the next Attempt starts clean
  MmioWrite32 ((UINTN)&Bus->int_status, IntStatus);

  if (IntStatus & HSI2C_INT_NO_DEV) {
    DEBUG ((EFI_D_ERROR, "HSI2C: NACK, INT_STATUS 0x%08x\n", IntStatus));
    return EFI_NO_RESPONSE;
  }

  DEBUG ((EFI_D_ERROR, "HSI2C: Transfer Error, INT_STATUS 0x%08x TRANS_STATUS 0x%08x\n",
          IntStatus, MmioRead32 ((UINTN)&Bus->trans_status)));

  return EFI_DEVICE_ERROR;
}

EFI_STATUS
HsI2cXferMsg (
  IN UINT8          BusNumber,
  IN EFI_HSI2C_BUS *Bus,
  IN UINT8          SlaveAddr,
  IN UINT8         *Buffer,
  IN UINTN          BufferLen,
  IN BOOLEAN        IsRead,
  IN BOOLEAN        Stop)
{
  UINT32 AutoConf   = 0;
  UINT32 Addr       = 0;
  UINT32 FifoCtl    = 0;
  UINT32 IntStatus  = 0;
  UINT32 FifoStatus = 0;
  UINT32 TrigLevel  = 0;
  UINT64     Timeout = 1000000;
  UINTN      MsgPtr  = 0;
  EFI_STATUS Status;

  // Disable Timeout
  MmioAnd32 ((UINTN)&Bus->timeout, ~HSI2C_TIMEOUT_EN);

  // Echo current Interrupt Status
  MmioWrite32 ((UINTN)&Bus->int_status, MmioRead32 ((UINTN)&Bus->int_status));

  // Set FIFO Trigger Level
  TrigLevel = (BufferLen >= FIFO_TRIG_CRITERIA) ? FIFO_TRIG_CRITERIA : BufferLen;
  FifoCtl = HSI2C_RXFIFO_EN                        |
            HSI2C_TXFIFO_EN                        |
            HSI2C_RXFIFO_TRIGGER_LEVEL (TrigLevel) |
            HSI2C_TXFIFO_TRIGGER_LEVEL (TrigLevel);

  // Write new FIFO Trigger Level
  MmioWrite32 ((UINTN)&Bus->fifo_ctrl, FifoCtl);

  // Set HSI2C Direction
  MmioAndThenOr32 (
    (UINTN)&Bus->ctrl,
    IsRead ? ~HSI2C_TXCHON : ~HSI2C_RXCHON,
    IsRead ?  HSI2C_RXCHON :  HSI2C_TXCHON
    );

  // Get current Target Address
  Addr = MmioRead32 ((UINTN)&Bus->address);

  // Set new Target Address
  Addr &= ~((0x3FF << 10) | (0x3FF << 0) | (0xFF << 24));
  Addr |=  (SlaveAddr & 0x7F) << 10;

  // Write new Target Address
  MmioWrite32 ((UINTN)&Bus->address, Addr);

  // Get current Auto Configuration
  AutoConf = MmioRead32 ((UINTN)&Bus->auto_conf);

  // Set new Auto Configuration
  AutoConf &= ~(HSI2C_READ_WRITE | HSI2C_STOP_AFTER_TRANS | 0xFFFF);
  AutoConf |= IsRead ? HSI2C_READ_WRITE : 0;
  AutoConf |= Stop ? HSI2C_STOP_AFTER_TRANS : 0;
  AutoConf |= (BufferLen & 0xFFFF);

  // Write new Auto Configuration
  MmioWrite32 ((UINTN)&Bus->auto_conf, AutoConf);

  // Set Inital Interrupt Status
  MmioWrite32 ((UINTN)&Bus->int_enable, HSI2C_INT_TRANSFER_DONE);

  // Set HSI2C Master Run
  MmioOr32 ((UINTN)&Bus->auto_conf, HSI2C_MASTER_RUN);

  if (IsRead) {
    while (Timeout--) {
      // Get current FIFO Status
      BOOLEAN FifoEmpty = (BOOLEAN)((MmioRead32 ((UINTN)&Bus->fifo_status) & HSI2C_RX_FIFO_EMPTY) != 0);

      // Read HSI2C Data
      if (!FifoEmpty && MsgPtr < BufferLen) {
        Buffer[MsgPtr++] = (UINT8)MmioRead32 ((UINTN)&Bus->rx_data);
      }

      if (MsgPtr >= BufferLen) {
        // Get current Interrupt Status
        IntStatus = MmioRead32 ((UINTN)&Bus->int_status);

        // Check Interrupt Status
        if (IntStatus & HSI2C_INT_TRANSFER_DONE) {
          // Clear Interrupt Status
          MmioWrite32 ((UINTN)&Bus->int_status, IntStatus);

          return EFI_SUCCESS;
        }
      }

      // Give up early on a Slave that is not answering
      Status = CheckTransferError (Bus);
      if (EFI_ERROR (Status)) {
        goto Failed;
      }
    }
  } else {
    while (Timeout && MsgPtr < BufferLen) {
      // Get current FIFO Level Mask
      UINT32 FifoLvlMask = MmioRead32 ((UINTN)&Bus->fifo_status) & HSI2C_TX_FIFO_LVL_MASK;

      // Write HSI2C Data
      if (FifoLvlMask < EXYNOS_FIFO_SIZE) {
        MmioWrite32 ((UINTN)&Bus->tx_data, Buffer[MsgPtr++]);
      }

      // Decrease Timeout
      Timeout--;
    }

    while (Timeout--) {
      // Get current Interrupt & FIFO Status
      IntStatus  = MmioRead32 ((UINTN)&Bus->int_status);
      FifoStatus = MmioRead32 ((UINTN)&Bus->fifo_status);

      // Check Statuses
      if ((IntStatus & HSI2C_INT_TRANSFER_DONE) && (FifoStatus & HSI2C_TX_FIFO_EMPTY)) {
        // Clear Interrupt Status
        MmioWrite32 ((UINTN)&Bus->int_status, IntStatus);

        return EFI_SUCCESS;
      }

      // Give up early on a Slave that is not answering
      Status = CheckTransferError (Bus);
      if (EFI_ERROR (Status)) {
        goto Failed;
      }

      // Wait 1us
      gBS->Stall (1);
    }
  }

  Status = EFI_TIMEOUT;

Failed:
  ResetController (Bus);
  ResetConfig (BusNumber, Bus, BusData[BusNumber].Clock.SpeedMode);

  return Status;
}

/**
  Resolves a Bus Number to its Controller.
**/
STATIC
EFI_STATUS
HsI2cGetBus (
  IN  UINT8           BusNumber,
  OUT EFI_HSI2C_BUS **Bus)
{
  EFI_STATUS           Status;
  EFI_PHYSICAL_ADDRESS BusAddress;

  // Verify Bus Number
  if (BusNumber >= BusCount) {
    return EFI_NOT_FOUND;
  }

  // Verify Init Flag
  if (!BusData[BusNumber].Initialized) {
    return EFI_NOT_READY;
  }

  // Get HSI2C Controller Address
  Status = mUsiProtocol->GetControllerAddr (BusNumber, BUS_I2C, &BusAddress);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  *Bus = (EFI_HSI2C_BUS *)BusAddress;

  return EFI_SUCCESS;
}

/**
  Reads from a Slave Register.
**/
EFI_STATUS
HsI2cReadReg (
  IN  UINT8   BusNumber,
  IN  UINT8   SlaveAddr,
  IN  UINT32  SlaveReg,
  OUT UINT8  *Data,
  IN  UINTN   DataLen)
{
  EFI_STATUS     Status;
  EFI_HSI2C_BUS *Bus;
  UINT8          RegAddr;

  Status = HsI2cGetBus (BusNumber, &Bus);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  for (UINT8 Retry = 0; Retry < HSI2C_XFER_RETRIES; Retry++) {
    RegAddr = (UINT8)(SlaveReg & 0xFF);

    Status = HsI2cXferMsg (BusNumber, Bus, SlaveAddr, &RegAddr, 1, FALSE, FALSE);

    if (!EFI_ERROR (Status)) {
      Status = HsI2cXferMsg (BusNumber, Bus, SlaveAddr, Data, DataLen, TRUE, TRUE);
      if (!EFI_ERROR (Status)) {
        return EFI_SUCCESS;
      }
    }

    if (Status == EFI_NO_RESPONSE && Retry >= 1) {
      break;
    }

    // Wait 100us
    gBS->Stall (100);
  }

  return Status;
}

EFI_STATUS
HsI2cWriteReg (
  IN UINT8   BusNumber,
  IN UINT8   SlaveAddr,
  IN UINT32  SlaveReg,
  IN UINT8  *Data,
  IN UINTN   DataLen)
{
  EFI_STATUS     Status;
  EFI_HSI2C_BUS *Bus;
  UINT8          Buffer[1 + HSI2C_MAX_WRITE_LEN];

  if (DataLen > HSI2C_MAX_WRITE_LEN) {
    return EFI_INVALID_PARAMETER;
  }

  Status = HsI2cGetBus (BusNumber, &Bus);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  for (UINT8 Retry = 0; Retry < HSI2C_XFER_RETRIES; Retry++) {
    Buffer[0] = (UINT8)(SlaveReg & 0xFF);
    CopyMem (Buffer + 1, Data, DataLen);

    Status = HsI2cXferMsg (BusNumber, Bus, SlaveAddr, Buffer, DataLen + 1, FALSE, TRUE);
    if (!EFI_ERROR (Status)) {
      return EFI_SUCCESS;
    }

    if (Status == EFI_NO_RESPONSE && Retry >= 1) {
      break;
    }

    // Wait 100us
    gBS->Stall (100);
  }

  return Status;
}

EFI_STATUS
HsI2cRead32 (
  IN  UINT8   BusNumber,
  IN  UINT8   SlaveAddr,
  IN  UINT32  SlaveReg,
  OUT UINT32 *Data)
{
  EFI_STATUS Status;
  UINT8      Buf[4];

  if (Data == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = HsI2cReadReg (BusNumber, SlaveAddr, SlaveReg, Buf, sizeof (Buf));
  if (EFI_ERROR (Status)) {
    return Status;
  }

  *Data = ((UINT32)Buf[0] << 24) |
          ((UINT32)Buf[1] << 16) |
          ((UINT32)Buf[2] <<  8) |
          ((UINT32)Buf[3] <<  0);

  return EFI_SUCCESS;
}

EFI_STATUS
HsI2cWrite32 (
  IN UINT8  BusNumber,
  IN UINT8  SlaveAddr,
  IN UINT32 SlaveReg,
  IN UINT32 Data)
{
  UINT8 Buf[4];

  Buf[0] = (Data >> 24) & 0xFF;
  Buf[1] = (Data >> 16) & 0xFF;
  Buf[2] = (Data >>  8) & 0xFF;
  Buf[3] =  Data        & 0xFF;

  return HsI2cWriteReg (BusNumber, SlaveAddr, SlaveReg, Buf, sizeof (Buf));
}

EFI_STATUS
HsI2cRead (
  IN  UINT8   BusNumber,
  IN  UINT8   SlaveAddr,
  IN  UINT32  SlaveReg,
  OUT UINT8  *Data)
{
  if (Data == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  return HsI2cReadReg (BusNumber, SlaveAddr, SlaveReg, Data, 1);
}

EFI_STATUS
HsI2cWrite (
  IN UINT8  BusNumber,
  IN UINT8  SlaveAddr,
  IN UINT32 SlaveReg,
  IN UINT8  Data)
{
  return HsI2cWriteReg (BusNumber, SlaveAddr, SlaveReg, &Data, 1);
}

STATIC EFI_HSI2C_PROTOCOL mHsI2c = {
  HsI2cInitBus,
  HsI2cRead32,
  HsI2cWrite32,
  HsI2cRead,
  HsI2cWrite,
  HsI2cReadReg,
  HsI2cWriteReg
};

EFI_STATUS
EFIAPI
RegisterHsI2c (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status;

  // Get HSI2C Bus Data
  GetHsI2cBusData (&BusData, &BusCount);

  // Verify Bus Count
  if (!BusCount) {
    return EFI_UNSUPPORTED;
  }

  // Locate USI Protocol
  Status = gBS->LocateProtocol (&gEfiUsiProtocolGuid, NULL, (VOID *)&mUsiProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate USI Protocol!\n"));
    return Status;
  }

  // Locate GPIO Protocol
  Status = gBS->LocateProtocol (&gEfiGpioProtocolGuid, NULL, (VOID *)&mGpioProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate GPIO Protocol!\n"));
    return Status;
  }

  // Register HSI2C Protocol
  Status = gBS->InstallProtocolInterface (&ImageHandle, &gEfiHsI2cProtocolGuid, EFI_NATIVE_INTERFACE, &mHsI2c);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Register HSI2C Protocol!\n"));
    return Status;
  }

  return EFI_SUCCESS;
}
