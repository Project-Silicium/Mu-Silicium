#include <Library/DebugLib.h>
#include <Library/I2cLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryAllocationHelperLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Protocol/EFIGpio.h>
#include <Protocol/EFII2c.h>

#include "I2c.h"

STATIC EFI_GPIO_PROTOCOL *mGpioProtocol;

STATIC EFI_I2C_BUS_DATA *mBusData;
STATIC UINT8             mBusCount;

STATIC
EFI_STATUS
I2cSetupBusGpio (IN EFI_I2C_GPIO_DATA *Gpio)
{
  EFI_STATUS Status;

  Status = mGpioProtocol->SetPull (Gpio->SclBankId, Gpio->SclBankNum, Gpio->SclPin, Gpio->Pull);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = mGpioProtocol->SetPull (Gpio->SdaBankId, Gpio->SdaBankNum, Gpio->SdaPin, Gpio->Pull);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = mGpioProtocol->SetFunction (Gpio->SclBankId, Gpio->SclBankNum, Gpio->SclPin, Gpio->Function);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return mGpioProtocol->SetFunction (Gpio->SdaBankId, Gpio->SdaBankNum, Gpio->SdaPin, Gpio->Function);
}

STATIC
EFI_STATUS
I2cSetClock (
  IN EFI_I2C_BUS        *Bus,
  IN EFI_I2C_CLOCK_DATA *Clock)
{
  UINT32 Control;
  UINT32 Divider;
  UINT32 Prescaler;
  UINT32 SdaDelay;

  if ((Clock->SourceFrequency == 0) || (Clock->BusFrequency == 0)) {
    return EFI_INVALID_PARAMETER;
  }

  Prescaler = 16;
  Divider   = (Clock->SourceFrequency + (Prescaler * Clock->BusFrequency) - 1) /
              (Prescaler * Clock->BusFrequency);

  if (Divider > 16) {
    Prescaler = 512;
    Divider   = (Clock->SourceFrequency + (Prescaler * Clock->BusFrequency) - 1) /
                (Prescaler * Clock->BusFrequency);
  }

  if ((Divider == 0) || (Divider > 16)) {
    return EFI_UNSUPPORTED;
  }

  Control = I2C_CON_ACK_ENABLE |
            I2C_CON_INTERRUPT_ENABLE |
            ((Divider - 1) & I2C_CON_SCALE_MASK);

  if (Prescaler == 512) {
    Control |= I2C_CON_DIVIDE_BY_512;
  }

  MmioWrite32 ((UINTN)&Bus->Control, Control);

  SdaDelay = 0;
  if (Clock->SdaDelay != 0) {
    SdaDelay = (Clock->SourceFrequency / 1000) * Clock->SdaDelay;
    SdaDelay = (SdaDelay + 999999) / 1000000;
    SdaDelay = (SdaDelay + 4) / 5;
    if (SdaDelay > I2C_LINE_CONTROL_DELAY_MASK) {
      SdaDelay = I2C_LINE_CONTROL_DELAY_MASK;
    }

    SdaDelay |= I2C_LINE_CONTROL_FILTER;
  }

  MmioWrite32 ((UINTN)&Bus->LineControl, SdaDelay);

  return EFI_SUCCESS;
}

STATIC
VOID
I2cDisableBus (IN EFI_I2C_BUS *Bus)
{
  MmioAnd32 ((UINTN)&Bus->Status, ~I2C_STAT_TXRX_ENABLE);
  MmioAnd32 (
    (UINTN)&Bus->Control,
    ~(I2C_CON_INTERRUPT_ENABLE | I2C_CON_INTERRUPT_PENDING | I2C_CON_ACK_ENABLE)
    );
}

STATIC
EFI_STATUS
I2cWaitBusIdle (IN EFI_I2C_BUS *Bus)
{
  for (UINTN Timeout = 0; Timeout < I2C_BUS_IDLE_TIMEOUT_US; Timeout++) {
    if ((MmioRead32 ((UINTN)&Bus->Status) & I2C_STAT_BUS_BUSY) == 0) {
      return EFI_SUCCESS;
    }

    gBS->Stall (1);
  }

  DEBUG ((
    EFI_D_ERROR,
    "I2C: Bus idle timeout: CON=0x%08x STAT=0x%08x ADD=0x%08x DS=0x%08x LC=0x%08x\n",
    MmioRead32 ((UINTN)&Bus->Control),
    MmioRead32 ((UINTN)&Bus->Status),
    MmioRead32 ((UINTN)&Bus->Address),
    MmioRead32 ((UINTN)&Bus->Data),
    MmioRead32 ((UINTN)&Bus->LineControl)
    ));

  return EFI_TIMEOUT;
}

STATIC
EFI_STATUS
I2cWaitTransfer (
  IN EFI_I2C_BUS *Bus,
  IN BOOLEAN      IgnoreNack)
{
  UINT32 Status;

  for (UINTN Timeout = 0; Timeout < I2C_TRANSFER_TIMEOUT_US; Timeout++) {
    if ((MmioRead32 ((UINTN)&Bus->Control) & I2C_CON_INTERRUPT_PENDING) != 0) {
      Status = MmioRead32 ((UINTN)&Bus->Status);

      if ((Status & I2C_STAT_ARBITRATION_LOST) != 0) {
        return EFI_ABORTED;
      }

      if (!IgnoreNack && ((Status & I2C_STAT_LAST_BIT) != 0)) {
        return EFI_NO_RESPONSE;
      }

      return EFI_SUCCESS;
    }

    gBS->Stall (1);
  }

  DEBUG ((
    EFI_D_ERROR,
    "I2C: Transfer timeout: CON=0x%08x STAT=0x%08x ADD=0x%08x DS=0x%08x LC=0x%08x\n",
    MmioRead32 ((UINTN)&Bus->Control),
    MmioRead32 ((UINTN)&Bus->Status),
    MmioRead32 ((UINTN)&Bus->Address),
    MmioRead32 ((UINTN)&Bus->Data),
    MmioRead32 ((UINTN)&Bus->LineControl)
    ));

  return EFI_TIMEOUT;
}

STATIC
VOID
I2cReleaseBus (IN EFI_I2C_BUS *Bus)
{
  MmioAnd32 ((UINTN)&Bus->Control, ~I2C_CON_INTERRUPT_PENDING);
}

STATIC
EFI_STATUS
I2cStart (
  IN EFI_I2C_BUS *Bus,
  IN UINT8        SlaveAddr,
  IN BOOLEAN      IsRead,
  IN BOOLEAN      RepeatedStart)
{
  EFI_STATUS TransferStatus;
  UINT32 Status;
  UINT8  Address;

  Status  = IsRead ? I2C_STAT_MASTER_RECEIVE : I2C_STAT_MASTER_TRANSMIT;
  Status |= I2C_STAT_TXRX_ENABLE;
  Address = (UINT8)((SlaveAddr & 0x7F) << 1);
  if (IsRead) {
    Address |= 1;
  }

  MmioOr32 (
    (UINTN)&Bus->Control,
    I2C_CON_ACK_ENABLE | I2C_CON_INTERRUPT_ENABLE
    );
  MmioWrite32 ((UINTN)&Bus->Status, Status);
  MmioWrite32 ((UINTN)&Bus->Data, Address);
  gBS->Stall (1);
  MmioWrite32 ((UINTN)&Bus->Status, Status | I2C_STAT_START);

  if (RepeatedStart) {
    I2cReleaseBus (Bus);
  }

  TransferStatus = I2cWaitTransfer (Bus, FALSE);
  if (EFI_ERROR (TransferStatus)) {
    DEBUG ((
      EFI_D_ERROR,
      "I2C: %a START to 0x%02x failed: %r\n",
      IsRead ? "read" : "write",
      SlaveAddr,
      TransferStatus
      ));
  }

  return TransferStatus;
}

STATIC
EFI_STATUS
I2cWriteByte (
  IN EFI_I2C_BUS *Bus,
  IN UINT8        Data)
{
  EFI_STATUS Status;

  MmioWrite32 ((UINTN)&Bus->Data, Data);
  gBS->Stall (1);
  I2cReleaseBus (Bus);

  Status = I2cWaitTransfer (Bus, FALSE);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "I2C: Write byte 0x%02x failed: %r\n", Data, Status));
  }

  return Status;
}

STATIC
EFI_STATUS
I2cReadBytes (
  IN  EFI_I2C_BUS *Bus,
  OUT UINT8       *Data,
  IN  UINTN        DataLen)
{
  EFI_STATUS Status;

  for (UINTN Index = 0; Index < DataLen; Index++) {
    if (Index == (DataLen - 1)) {
      MmioAnd32 ((UINTN)&Bus->Control, ~I2C_CON_ACK_ENABLE);
    }

    I2cReleaseBus (Bus);

    Status = I2cWaitTransfer (Bus, Index == (DataLen - 1));
    if (EFI_ERROR (Status)) {
      return Status;
    }

    Data[Index] = (UINT8)MmioRead32 ((UINTN)&Bus->Data);
  }

  return EFI_SUCCESS;
}

STATIC
VOID
I2cStop (IN EFI_I2C_BUS *Bus)
{
  MmioAnd32 ((UINTN)&Bus->Status, ~I2C_STAT_START);
  I2cReleaseBus (Bus);
  I2cWaitBusIdle (Bus);
  I2cDisableBus (Bus);
}

STATIC
EFI_STATUS
I2cGetBus (
  IN  UINT8         BusNumber,
  OUT EFI_I2C_BUS **Bus)
{
  if (BusNumber >= mBusCount) {
    return EFI_NOT_FOUND;
  }

  if (!mBusData[BusNumber].Initialized) {
    return EFI_NOT_READY;
  }

  *Bus = (EFI_I2C_BUS *)mBusData[BusNumber].Address;
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
I2cInitBus (IN UINT8 BusNumber)
{
  EFI_I2C_BUS *Bus;
  EFI_STATUS   Status;

  if (BusNumber >= mBusCount) {
    return EFI_NOT_FOUND;
  }

  if (mBusData[BusNumber].Initialized) {
    return EFI_SUCCESS;
  }

  Status = MapMemoryRegion (
             mBusData[BusNumber].Address,
             I2C_MMIO_LENGTH,
             EfiMemoryMappedIO
             );
  if (EFI_ERROR (Status)) {
    return EFI_ALREADY_STARTED;
  }

  Status = I2cSetupBusGpio (&mBusData[BusNumber].Gpio);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Bus = (EFI_I2C_BUS *)mBusData[BusNumber].Address;

  MmioWrite32 ((UINTN)&Bus->Control, 0);
  MmioWrite32 ((UINTN)&Bus->Status, 0);
  MmioWrite32 ((UINTN)&Bus->Address, 0);

  Status = I2cSetClock (Bus, &mBusData[BusNumber].Clock);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  MmioWrite32 (
    (UINTN)&Bus->Status,
    I2C_STAT_MASTER_TRANSMIT | I2C_STAT_TXRX_ENABLE
    );
  I2cDisableBus (Bus);

  mBusData[BusNumber].Initialized = TRUE;
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
I2cReadReg (
  IN  UINT8   BusNumber,
  IN  UINT8   SlaveAddr,
  IN  UINT32  SlaveReg,
  OUT UINT8  *Data,
  IN  UINTN   DataLen)
{
  EFI_I2C_BUS *Bus;
  EFI_STATUS   Status;

  if ((Data == NULL) || (DataLen == 0) || (SlaveAddr > 0x7F)) {
    return EFI_INVALID_PARAMETER;
  }

  Status = I2cGetBus (BusNumber, &Bus);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  for (UINT8 Retry = 0; Retry < I2C_TRANSFER_RETRIES; Retry++) {
    Status = I2cWaitBusIdle (Bus);
    if (EFI_ERROR (Status)) {
      I2cDisableBus (Bus);
      continue;
    }

    Status = I2cStart (Bus, SlaveAddr, FALSE, FALSE);
    if (!EFI_ERROR (Status)) {
      Status = I2cWriteByte (Bus, (UINT8)(SlaveReg & 0xFF));
    }

    if (!EFI_ERROR (Status)) {
      Status = I2cStart (Bus, SlaveAddr, TRUE, TRUE);
    }

    if (!EFI_ERROR (Status)) {
      Status = I2cReadBytes (Bus, Data, DataLen);
    }

    I2cStop (Bus);

    if (!EFI_ERROR (Status)) {
      return EFI_SUCCESS;
    }

    if ((Status == EFI_NO_RESPONSE) && (Retry >= 1)) {
      break;
    }

    gBS->Stall (100);
  }

  DEBUG ((EFI_D_ERROR, "I2C%u: Read from 0x%02x register 0x%02x failed: %r\n",
          BusNumber, SlaveAddr, (UINT8)SlaveReg, Status));

  return Status;
}

EFI_STATUS
EFIAPI
I2cWriteReg (
  IN UINT8   BusNumber,
  IN UINT8   SlaveAddr,
  IN UINT32  SlaveReg,
  IN UINT8  *Data,
  IN UINTN   DataLen)
{
  EFI_I2C_BUS *Bus;
  EFI_STATUS   Status;

  if (((Data == NULL) && (DataLen != 0)) || (SlaveAddr > 0x7F)) {
    return EFI_INVALID_PARAMETER;
  }

  Status = I2cGetBus (BusNumber, &Bus);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  for (UINT8 Retry = 0; Retry < I2C_TRANSFER_RETRIES; Retry++) {
    Status = I2cWaitBusIdle (Bus);
    if (EFI_ERROR (Status)) {
      I2cDisableBus (Bus);
      continue;
    }

    Status = I2cStart (Bus, SlaveAddr, FALSE, FALSE);
    if (!EFI_ERROR (Status)) {
      Status = I2cWriteByte (Bus, (UINT8)(SlaveReg & 0xFF));
    }

    for (UINTN Index = 0; !EFI_ERROR (Status) && (Index < DataLen); Index++) {
      Status = I2cWriteByte (Bus, Data[Index]);
    }

    I2cStop (Bus);

    if (!EFI_ERROR (Status)) {
      return EFI_SUCCESS;
    }

    if ((Status == EFI_NO_RESPONSE) && (Retry >= 1)) {
      break;
    }

    gBS->Stall (100);
  }

  DEBUG ((EFI_D_ERROR, "I2C%u: Write to 0x%02x register 0x%02x failed: %r\n",
          BusNumber, SlaveAddr, (UINT8)SlaveReg, Status));

  return Status;
}

EFI_STATUS
EFIAPI
I2cRead32 (
  IN  UINT8   BusNumber,
  IN  UINT8   SlaveAddr,
  IN  UINT32  SlaveReg,
  OUT UINT32 *Data)
{
  EFI_STATUS Status;
  UINT8      Buffer[4];

  if (Data == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = I2cReadReg (BusNumber, SlaveAddr, SlaveReg, Buffer, sizeof (Buffer));
  if (EFI_ERROR (Status)) {
    return Status;
  }

  *Data = ((UINT32)Buffer[0] << 24) |
          ((UINT32)Buffer[1] << 16) |
          ((UINT32)Buffer[2] <<  8) |
          ((UINT32)Buffer[3] <<  0);

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
I2cWrite32 (
  IN UINT8  BusNumber,
  IN UINT8  SlaveAddr,
  IN UINT32 SlaveReg,
  IN UINT32 Data)
{
  UINT8 Buffer[4];

  Buffer[0] = (Data >> 24) & 0xFF;
  Buffer[1] = (Data >> 16) & 0xFF;
  Buffer[2] = (Data >>  8) & 0xFF;
  Buffer[3] = Data & 0xFF;

  return I2cWriteReg (BusNumber, SlaveAddr, SlaveReg, Buffer, sizeof (Buffer));
}

EFI_STATUS
EFIAPI
I2cRead (
  IN  UINT8   BusNumber,
  IN  UINT8   SlaveAddr,
  IN  UINT32  SlaveReg,
  OUT UINT8  *Data)
{
  return I2cReadReg (BusNumber, SlaveAddr, SlaveReg, Data, 1);
}

EFI_STATUS
EFIAPI
I2cWrite (
  IN UINT8  BusNumber,
  IN UINT8  SlaveAddr,
  IN UINT32 SlaveReg,
  IN UINT8  Data)
{
  return I2cWriteReg (BusNumber, SlaveAddr, SlaveReg, &Data, 1);
}

STATIC EFI_I2C_PROTOCOL pI2c = {
  I2cInitBus,
  I2cRead32,
  I2cWrite32,
  I2cRead,
  I2cWrite,
  I2cReadReg,
  I2cWriteReg
};

EFI_STATUS
EFIAPI
RegisterI2c (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS   Status;

  GetI2cBusData (&mBusData, &mBusCount);
  if (mBusCount == 0) {
    return EFI_UNSUPPORTED;
  }

  Status = gBS->LocateProtocol (&gEfiGpioProtocolGuid, NULL, (VOID **)&mGpioProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate GPIO Protocol!\n"));
    return Status;
  }

  Status = gBS->InstallProtocolInterface (&ImageHandle, &gEfiI2cProtocolGuid, EFI_NATIVE_INTERFACE, &pI2c);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Register I2C Protocol!\n"));
  }

  return Status;
}
