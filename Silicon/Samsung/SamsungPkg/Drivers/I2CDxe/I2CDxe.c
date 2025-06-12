#include <Library/I2CHostTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/TimerLib.h>
#include <Library/DebugLib.h>

#include <Protocol/EfiGpio.h>
#include <Protocol/EfiI2C.h>

//
// Global Variables
//
EFI_GPIO_PROTOCOL *mGpioProtocol;
I2CHost TargetHost;

STATIC
VOID
I2CLow (VOID)
{
  mGpioProtocol->SetPin(TargetHost.SclBank, TargetHost.SclBankOffset, TargetHost.SclPin, GPIO_LEVEL_LOW);
  mGpioProtocol->SetPin(TargetHost.SdaBank, TargetHost.SdaBankOffset, TargetHost.SdaPin, GPIO_LEVEL_LOW);
  MicroSecondDelay(TargetHost.Delay);

  mGpioProtocol->SetPin(TargetHost.SclBank, TargetHost.SclBankOffset, TargetHost.SclPin, GPIO_LEVEL_HIGH);
  mGpioProtocol->SetPin(TargetHost.SdaBank, TargetHost.SdaBankOffset, TargetHost.SdaPin, GPIO_LEVEL_LOW);
  MicroSecondDelay(TargetHost.Delay);

  mGpioProtocol->SetPin(TargetHost.SclBank, TargetHost.SclBankOffset, TargetHost.SclPin, GPIO_LEVEL_HIGH);
  mGpioProtocol->SetPin(TargetHost.SdaBank, TargetHost.SdaBankOffset, TargetHost.SdaPin, GPIO_LEVEL_LOW);
  MicroSecondDelay(TargetHost.Delay);

  mGpioProtocol->SetPin(TargetHost.SclBank, TargetHost.SclBankOffset, TargetHost.SclPin, GPIO_LEVEL_LOW);
  mGpioProtocol->SetPin(TargetHost.SdaBank, TargetHost.SdaBankOffset, TargetHost.SdaPin, GPIO_LEVEL_LOW);
  MicroSecondDelay(TargetHost.Delay);
}

STATIC
VOID
I2CHigh (VOID)
{
  mGpioProtocol->SetPin(TargetHost.SclBank, TargetHost.SclBankOffset, TargetHost.SclPin, GPIO_LEVEL_LOW);
  mGpioProtocol->SetPin(TargetHost.SdaBank, TargetHost.SdaBankOffset, TargetHost.SdaPin, GPIO_LEVEL_HIGH);
  MicroSecondDelay(TargetHost.Delay);

  mGpioProtocol->SetPin(TargetHost.SclBank, TargetHost.SclBankOffset, TargetHost.SclPin, GPIO_LEVEL_HIGH);
  mGpioProtocol->SetPin(TargetHost.SdaBank, TargetHost.SdaBankOffset, TargetHost.SdaPin, GPIO_LEVEL_HIGH);
  MicroSecondDelay(TargetHost.Delay);

  mGpioProtocol->SetPin(TargetHost.SclBank, TargetHost.SclBankOffset, TargetHost.SclPin, GPIO_LEVEL_HIGH);
  mGpioProtocol->SetPin(TargetHost.SdaBank, TargetHost.SdaBankOffset, TargetHost.SdaPin, GPIO_LEVEL_HIGH);
  MicroSecondDelay(TargetHost.Delay);

  mGpioProtocol->SetPin(TargetHost.SclBank, TargetHost.SclBankOffset, TargetHost.SclPin, GPIO_LEVEL_LOW);
  mGpioProtocol->SetPin(TargetHost.SdaBank, TargetHost.SdaBankOffset, TargetHost.SdaPin, GPIO_LEVEL_HIGH);
  MicroSecondDelay(TargetHost.Delay);
}

STATIC
VOID
I2CStart (VOID)
{
  mGpioProtocol->SetPin(TargetHost.SclBank, TargetHost.SclBankOffset, TargetHost.SclPin, GPIO_LEVEL_HIGH);
  mGpioProtocol->SetPin(TargetHost.SdaBank, TargetHost.SdaBankOffset, TargetHost.SdaPin, GPIO_LEVEL_HIGH);
  MicroSecondDelay(TargetHost.Delay);

  mGpioProtocol->SetPin(TargetHost.SclBank, TargetHost.SclBankOffset, TargetHost.SclPin, GPIO_LEVEL_HIGH);
  mGpioProtocol->SetPin(TargetHost.SdaBank, TargetHost.SdaBankOffset, TargetHost.SdaPin, GPIO_LEVEL_LOW);
  MicroSecondDelay(TargetHost.Delay);

  MicroSecondDelay(TargetHost.Delay);

  mGpioProtocol->SetPin(TargetHost.SclBank, TargetHost.SclBankOffset, TargetHost.SclPin, GPIO_LEVEL_LOW);
  mGpioProtocol->SetPin(TargetHost.SdaBank, TargetHost.SdaBankOffset, TargetHost.SdaPin, GPIO_LEVEL_LOW);
  MicroSecondDelay(TargetHost.Delay);
}

STATIC
VOID
I2CRestart (VOID)
{
  mGpioProtocol->SetPin(TargetHost.SclBank, TargetHost.SclBankOffset, TargetHost.SclPin, GPIO_LEVEL_LOW);
  mGpioProtocol->SetPin(TargetHost.SdaBank, TargetHost.SdaBankOffset, TargetHost.SdaPin, GPIO_LEVEL_HIGH);
  MicroSecondDelay(TargetHost.Delay);

  mGpioProtocol->SetPin(TargetHost.SclBank, TargetHost.SclBankOffset, TargetHost.SclPin, GPIO_LEVEL_HIGH);
  mGpioProtocol->SetPin(TargetHost.SdaBank, TargetHost.SdaBankOffset, TargetHost.SdaPin, GPIO_LEVEL_HIGH);
  MicroSecondDelay(TargetHost.Delay);

  mGpioProtocol->SetPin(TargetHost.SclBank, TargetHost.SclBankOffset, TargetHost.SclPin, GPIO_LEVEL_HIGH);
  mGpioProtocol->SetPin(TargetHost.SdaBank, TargetHost.SdaBankOffset, TargetHost.SdaPin, GPIO_LEVEL_LOW);
  MicroSecondDelay(TargetHost.Delay);

  MicroSecondDelay(TargetHost.Delay);

  mGpioProtocol->SetPin(TargetHost.SclBank, TargetHost.SclBankOffset, TargetHost.SclPin, GPIO_LEVEL_LOW);
  mGpioProtocol->SetPin(TargetHost.SdaBank, TargetHost.SdaBankOffset, TargetHost.SdaPin, GPIO_LEVEL_LOW);
  MicroSecondDelay(TargetHost.Delay);
}

STATIC
VOID
I2CEnd (VOID)
{
  mGpioProtocol->SetPin(TargetHost.SclBank, TargetHost.SclBankOffset, TargetHost.SclPin, GPIO_LEVEL_LOW);
  mGpioProtocol->SetPin(TargetHost.SdaBank, TargetHost.SdaBankOffset, TargetHost.SdaPin, GPIO_LEVEL_LOW);
  MicroSecondDelay(TargetHost.Delay);

  mGpioProtocol->SetPin(TargetHost.SclBank, TargetHost.SclBankOffset, TargetHost.SclPin, GPIO_LEVEL_HIGH);
  mGpioProtocol->SetPin(TargetHost.SdaBank, TargetHost.SdaBankOffset, TargetHost.SdaPin, GPIO_LEVEL_LOW);
  MicroSecondDelay(TargetHost.Delay);

  MicroSecondDelay(TargetHost.Delay);

  mGpioProtocol->SetPin(TargetHost.SclBank, TargetHost.SclBankOffset, TargetHost.SclPin, GPIO_LEVEL_HIGH);
  mGpioProtocol->SetPin(TargetHost.SdaBank, TargetHost.SdaBankOffset, TargetHost.SdaPin, GPIO_LEVEL_HIGH);
  MicroSecondDelay(TargetHost.Delay);
}

STATIC
EFI_STATUS
I2CAcknowledgement (VOID)
{
  UINT32 Acknowledgement;
  UINT32 SdaValue;

  mGpioProtocol->SetPin(TargetHost.SclBank, TargetHost.SclBankOffset, TargetHost.SclPin, GPIO_LEVEL_LOW);
  MicroSecondDelay(TargetHost.Delay);

  mGpioProtocol->SetDirectionInput(TargetHost.SdaBank, TargetHost.SdaBankOffset, TargetHost.SdaPin);

  mGpioProtocol->SetPin(TargetHost.SclBank, TargetHost.SclBankOffset, TargetHost.SclPin, GPIO_LEVEL_HIGH);
  MicroSecondDelay(TargetHost.Delay);

  mGpioProtocol->GetPin(TargetHost.SdaBank, TargetHost.SdaBankOffset, TargetHost.SdaPin, &SdaValue);

  mGpioProtocol->SetPin(TargetHost.SclBank, TargetHost.SclBankOffset, TargetHost.SclPin, GPIO_LEVEL_HIGH);
  MicroSecondDelay(TargetHost.Delay);

  Acknowledgement = SdaValue;

  mGpioProtocol->SetPin(TargetHost.SdaBank, TargetHost.SdaBankOffset, TargetHost.SdaPin, GPIO_LEVEL_LOW);
  mGpioProtocol->SetDirectionOutput(TargetHost.SdaBank, TargetHost.SdaBankOffset, TargetHost.SdaPin, FALSE);

  mGpioProtocol->SetPin(TargetHost.SclBank, TargetHost.SclBankOffset, TargetHost.SclPin, GPIO_LEVEL_LOW);
  MicroSecondDelay(TargetHost.Delay);

  if (Acknowledgement)
  {
    DEBUG((EFI_D_WARN, "I2C Recieved no ACK\n"));
    return EFI_TIMEOUT;
  }
  return EFI_SUCCESS;
}

STATIC
VOID
I2CNoAcknowledgement (VOID)
{
  I2CHigh();
}

STATIC
VOID
I2CMasterAcknowledgement (VOID)
{
  mGpioProtocol->SetPin(TargetHost.SclBank, TargetHost.SclBankOffset, TargetHost.SclPin, GPIO_LEVEL_LOW);
  mGpioProtocol->SetPin(TargetHost.SdaBank, TargetHost.SdaBankOffset, TargetHost.SdaPin, GPIO_LEVEL_HIGH);
  MicroSecondDelay(TargetHost.Delay);

  mGpioProtocol->SetPin(TargetHost.SclBank, TargetHost.SclBankOffset, TargetHost.SclPin, GPIO_LEVEL_LOW);
  mGpioProtocol->SetPin(TargetHost.SdaBank, TargetHost.SdaBankOffset, TargetHost.SdaPin, GPIO_LEVEL_LOW);
  MicroSecondDelay(TargetHost.Delay);

  mGpioProtocol->SetPin(TargetHost.SclBank, TargetHost.SclBankOffset, TargetHost.SclPin, GPIO_LEVEL_HIGH);
  mGpioProtocol->SetPin(TargetHost.SdaBank, TargetHost.SdaBankOffset, TargetHost.SdaPin, GPIO_LEVEL_LOW);
  MicroSecondDelay(TargetHost.Delay);

  MicroSecondDelay(TargetHost.Delay);

  mGpioProtocol->SetPin(TargetHost.SclBank, TargetHost.SclBankOffset, TargetHost.SclPin, GPIO_LEVEL_LOW);
  mGpioProtocol->SetPin(TargetHost.SdaBank, TargetHost.SdaBankOffset, TargetHost.SdaPin, GPIO_LEVEL_LOW);
  MicroSecondDelay(TargetHost.Delay);

  mGpioProtocol->SetPin(TargetHost.SclBank, TargetHost.SclBankOffset, TargetHost.SclPin, GPIO_LEVEL_LOW);
  mGpioProtocol->SetPin(TargetHost.SdaBank, TargetHost.SdaBankOffset, TargetHost.SdaPin, GPIO_LEVEL_HIGH);
  MicroSecondDelay(TargetHost.Delay);
}

EFI_STATUS
I2CRead (
  I2CHost Host,
  UINT32 SlaveAddress,
  UINT32 WordAddress,
  UINT8 *DataPointer)
{
  EFI_STATUS Status;
  INTN i;
  UINT32 SdaValue;

  TargetHost = Host;

  *DataPointer = 0;

  I2CStart();

  for (i = 8; i > 1; i--)
  {
    if((SlaveAddress >> (i - 1)) & 1)
        I2CHigh();
    else
        I2CLow();
  }

  I2CLow();

  I2CAcknowledgement();

  for (i = 8; i > 0; i--)
  {
    if((WordAddress >> (i - 1)) & 1)
        I2CHigh();
    else
        I2CLow();
  }

  I2CAcknowledgement();

  I2CRestart();

  for (i = 8; i > 1; i--)
  {
    if((SlaveAddress >> (i - 1)) & 1)
        I2CHigh();
    else
        I2CLow();
  }

  I2CHigh();

  Status = I2CAcknowledgement();

  mGpioProtocol->SetDirectionInput(TargetHost.SdaBank, TargetHost.SdaBankOffset, TargetHost.SdaPin);

  for (i = 8; i > 0; i--)
  {
    mGpioProtocol->SetPin(TargetHost.SclBank, TargetHost.SclBankOffset, TargetHost.SclPin, GPIO_LEVEL_LOW);
    MicroSecondDelay(TargetHost.Delay);

    mGpioProtocol->SetPin(TargetHost.SclBank, TargetHost.SclBankOffset, TargetHost.SclPin, GPIO_LEVEL_HIGH);
    MicroSecondDelay(TargetHost.Delay);

    mGpioProtocol->GetPin(TargetHost.SdaBank, TargetHost.SdaBankOffset, TargetHost.SdaPin, &SdaValue);

    *DataPointer |= (!!(SdaValue) << (i - 1));

    mGpioProtocol->SetPin(TargetHost.SclBank, TargetHost.SclBankOffset, TargetHost.SclPin, GPIO_LEVEL_HIGH);
    MicroSecondDelay(TargetHost.Delay);

    mGpioProtocol->SetPin(TargetHost.SclBank, TargetHost.SclBankOffset, TargetHost.SclPin, GPIO_LEVEL_LOW);
    MicroSecondDelay(TargetHost.Delay);
  }

  mGpioProtocol->SetDirectionOutput(TargetHost.SdaBank, TargetHost.SdaBankOffset, TargetHost.SdaPin, FALSE);

  I2CNoAcknowledgement();
  I2CEnd();

  return Status;
}

EFI_STATUS
I2CMultiRead (
  I2CHost Host,
  UINT32 SlaveAddress,
  UINT32 WordAddress,
  UINT8 *DataPointer)
{
  EFI_STATUS Status;
  UINT32 i, j, Data;

  UINT32 SdaValue;

  TargetHost = Host;

  I2CStart();

  for (i = 8; i > 1; i--)
  {
    if((SlaveAddress >> (i - 1)) & 1)
        I2CHigh();
    else
        I2CLow();
  }

  I2CLow();
  I2CAcknowledgement();

  for (i = 8; i > 0; i--)
  {
    if((WordAddress >> (i - 1)) & 1)
        I2CHigh();
    else
        I2CLow();
  }

  I2CAcknowledgement();

  I2CRestart();

  for (i = 8; i > 1; i--)
  {
    if((SlaveAddress >> (i - 1)) & 1)
        I2CHigh();
    else
        I2CLow();
  }

  I2CHigh();

  Status = I2CAcknowledgement();

  for (i = 0; i < 4; i++)
  {
    mGpioProtocol->SetDirectionInput(TargetHost.SdaBank, TargetHost.SdaBankOffset, TargetHost.SdaPin);

    Data = 0;

    for (j = 8; j > 0; j--)
    {
      mGpioProtocol->SetPin(TargetHost.SclBank, TargetHost.SclBankOffset, TargetHost.SclPin, GPIO_LEVEL_LOW);
      MicroSecondDelay(TargetHost.Delay);

      mGpioProtocol->SetPin(TargetHost.SclBank, TargetHost.SclBankOffset, TargetHost.SclPin, GPIO_LEVEL_HIGH);
      MicroSecondDelay(TargetHost.Delay);

      mGpioProtocol->GetPin(TargetHost.SdaBank, TargetHost.SdaBankOffset, TargetHost.SdaPin, &SdaValue);

      *DataPointer |= (!!(SdaValue) << (j - 1));

      mGpioProtocol->SetPin(TargetHost.SclBank, TargetHost.SclBankOffset, TargetHost.SclPin, GPIO_LEVEL_HIGH);
      MicroSecondDelay(TargetHost.Delay);

      mGpioProtocol->SetPin(TargetHost.SclBank, TargetHost.SclBankOffset, TargetHost.SclPin, GPIO_LEVEL_LOW);
      MicroSecondDelay(TargetHost.Delay);
    }

    mGpioProtocol->SetDirectionOutput(TargetHost.SdaBank, TargetHost.SdaBankOffset, TargetHost.SdaPin, FALSE);

    if (i < 3)
      I2CMasterAcknowledgement();
    else
      I2CNoAcknowledgement();

    *DataPointer |= (Data << (8 * i));
  }

  I2CEnd();

  return Status;
}

EFI_STATUS
I2CBurstRead (
  I2CHost Host,
  UINT32 SlaveAddress,
  UINT32 WordAddress,
  UINT8 *DataPointer,
  UINT8 Length)
{
  EFI_STATUS Status;
  UINT32 i, j;
  UINT32 SdaValue;

  TargetHost = Host;

  for (i = 0; i < Length; i++)
    *(DataPointer + i) = 0;

  I2CStart();

  for (i = 8; i > 1; i--)
  {
    if((SlaveAddress >> (i - 1)) & 1)
        I2CHigh();
    else
        I2CLow();
  }

  I2CLow();

  I2CAcknowledgement();

  for (i = 8; i > 0; i--)
  {
    if((WordAddress >> (i - 1)) & 1)
        I2CHigh();
    else
        I2CLow();
  }

  I2CAcknowledgement();

  I2CRestart();

  for (i = 8; i > 1; i--)
  {
    if((SlaveAddress >> (i - 1)) & 1)
        I2CHigh();
    else
        I2CLow();
  }

  I2CHigh();

  Status = I2CAcknowledgement();

  for (i = 0; i < Length; i++) {
    mGpioProtocol->SetDirectionInput(TargetHost.SdaBank, TargetHost.SdaBankOffset, TargetHost.SdaPin);

    for (j = 8; j > 0; j--)
    {
      mGpioProtocol->SetPin(TargetHost.SclBank, TargetHost.SclBankOffset, TargetHost.SclPin, GPIO_LEVEL_LOW);
      MicroSecondDelay(TargetHost.Delay);

      mGpioProtocol->SetPin(TargetHost.SclBank, TargetHost.SclBankOffset, TargetHost.SclPin, GPIO_LEVEL_HIGH);
      MicroSecondDelay(TargetHost.Delay);

      mGpioProtocol->GetPin(TargetHost.SdaBank, TargetHost.SdaBankOffset, TargetHost.SdaPin, &SdaValue);

      *DataPointer |= (!!(SdaValue) << (j - 1));

      mGpioProtocol->SetPin(TargetHost.SclBank, TargetHost.SclBankOffset, TargetHost.SclPin, GPIO_LEVEL_HIGH);
      MicroSecondDelay(TargetHost.Delay);

      mGpioProtocol->SetPin(TargetHost.SclBank, TargetHost.SclBankOffset, TargetHost.SclPin, GPIO_LEVEL_LOW);
      MicroSecondDelay(TargetHost.Delay);
    }

    DataPointer++;

    mGpioProtocol->SetDirectionOutput(TargetHost.SdaBank, TargetHost.SdaBankOffset, TargetHost.SdaPin, FALSE);

    if (i + 1 < Length)
      I2CLow();
  }

  I2CNoAcknowledgement();
  I2CEnd();

  return Status;
}

EFI_STATUS
I2CWordAddressRead (
  I2CHost Host,
  UINT32 SlaveAddress,
  UINT32 WordAddress,
  UINT32 *DataPointer)
{
  EFI_STATUS Status;
  UINT32 i, j, Data;
  UINT8 AddressData[4];
  UINT32 SdaValue;

  TargetHost = Host;

  AddressData[3] = WordAddress & 0xFF;
  AddressData[2] = (WordAddress >> 8) & 0xFF;
  AddressData[1] = (WordAddress >> 16) & 0xFF;
  AddressData[0] = WordAddress >> 24;

  *DataPointer = 0;

  I2CStart();

  for (i = 8; i > 1; i--)
  {
    if((SlaveAddress >> (i - 1)) & 1)
      I2CHigh();
    else
      I2CLow();
  }

  I2CLow();

  I2CAcknowledgement();

  for (i = 0; i < 4; i++)
  {
    for (j = 8; j > 0; j--)
    {
      if((AddressData[i] >> (j - 1)) & 1)
        I2CHigh();
      else
        I2CLow();
    }

    I2CAcknowledgement();
  }

  I2CRestart();

  for (i = 8; i > 1; i--)
  {
    if((SlaveAddress >> (i - 1)) & 1)
      I2CHigh();
    else
      I2CLow();
  }

  I2CHigh();

  Status = I2CAcknowledgement();

  mGpioProtocol->SetDirectionInput(TargetHost.SdaBank, TargetHost.SdaBankOffset, TargetHost.SdaPin);

  for (i = 0; i < 4; i++)
  {
    mGpioProtocol->SetDirectionInput(TargetHost.SdaBank, TargetHost.SdaBankOffset, TargetHost.SdaPin);

    Data = 0;

    for (j = 8; j < 0; j--)
    {
      mGpioProtocol->SetPin(TargetHost.SclBank, TargetHost.SclBankOffset, TargetHost.SclPin, GPIO_LEVEL_LOW);
      MicroSecondDelay(TargetHost.Delay);

      mGpioProtocol->SetPin(TargetHost.SclBank, TargetHost.SclBankOffset, TargetHost.SclPin, GPIO_LEVEL_HIGH);
      MicroSecondDelay(TargetHost.Delay);

      mGpioProtocol->GetPin(TargetHost.SdaBank, TargetHost.SdaBankOffset, TargetHost.SdaPin, &SdaValue);

      Data |= (!!(SdaValue) << (j - 1));

      mGpioProtocol->SetPin(TargetHost.SclBank, TargetHost.SclBankOffset, TargetHost.SclPin, GPIO_LEVEL_HIGH);
      MicroSecondDelay(TargetHost.Delay);

      mGpioProtocol->SetPin(TargetHost.SclBank, TargetHost.SclBankOffset, TargetHost.SclPin, GPIO_LEVEL_LOW);
      MicroSecondDelay(TargetHost.Delay);
    }

    mGpioProtocol->SetDirectionOutput(TargetHost.SdaBank, TargetHost.SdaBankOffset, TargetHost.SdaPin, FALSE);

    if (i < 3)
      I2CMasterAcknowledgement();
    else
      I2CNoAcknowledgement();

    *DataPointer |= (Data << (8 * i));
  }

  I2CEnd();

  return Status;
}

EFI_STATUS
I2CMultiAddressRead (
  I2CHost Host,
  UINT32 SlaveAddress,
  UINT16 WordAddress,
  UINT16 *DataPointer)
{
  EFI_STATUS Status;
  UINT16 Data;
  UINT8 AddressData[2];
  INTN i, j;
  UINT32 SdaValue;

  TargetHost = Host;

  AddressData[0] = WordAddress & 0xFF;
  AddressData[1] = WordAddress >> 8;

  *DataPointer = 0;

  I2CStart();

  for (i = 8; i > 1; i--)
  {
    if((SlaveAddress >> (i - 1)) & 1)
      I2CHigh();
    else
      I2CLow();
  }

  I2CLow();

  I2CAcknowledgement();

  for (i = 8; i > 0; i--)
  {
    if((AddressData[0] >> (i - 1)) & 1)
      I2CHigh();
    else
      I2CLow();
  }

  I2CAcknowledgement();

  for (i = 8; i > 0; i--)
  { 
    if((AddressData[1] >> (i - 1)) & 1)
      I2CHigh();
    else
      I2CLow();
  }

  I2CAcknowledgement();

  I2CRestart();

  for (i = 8; i > 1; i--)
  {
    if((SlaveAddress >> (i - 1)) & 1)
      I2CHigh();
    else
      I2CLow();
  }

  I2CHigh();

  Status = I2CAcknowledgement();

  for (i = 0; i < 2; i++)
  {
    Data = 0;
    mGpioProtocol->SetDirectionInput(TargetHost.SdaBank, TargetHost.SdaBankOffset, TargetHost.SdaPin);

    for (j = 8; j > 0; j--)
    {
      mGpioProtocol->SetPin(TargetHost.SclBank, TargetHost.SclBankOffset, TargetHost.SclPin, GPIO_LEVEL_LOW);
      MicroSecondDelay(TargetHost.Delay);

      mGpioProtocol->SetPin(TargetHost.SclBank, TargetHost.SclBankOffset, TargetHost.SclPin, GPIO_LEVEL_HIGH);
      MicroSecondDelay(TargetHost.Delay);

      mGpioProtocol->GetPin(TargetHost.SdaBank, TargetHost.SdaBankOffset, TargetHost.SdaPin, &SdaValue);

      Data |= (!!(SdaValue) << (j - 1));

      mGpioProtocol->SetPin(TargetHost.SclBank, TargetHost.SclBankOffset, TargetHost.SclPin, GPIO_LEVEL_HIGH);
      MicroSecondDelay(TargetHost.Delay);

      mGpioProtocol->SetPin(TargetHost.SclBank, TargetHost.SclBankOffset, TargetHost.SclPin, GPIO_LEVEL_LOW);
      MicroSecondDelay(TargetHost.Delay);
    }

    mGpioProtocol->SetDirectionOutput(TargetHost.SdaBank, TargetHost.SdaBankOffset, TargetHost.SdaPin, FALSE);

    if(i < 1)
      I2CMasterAcknowledgement();
    else
      I2CNoAcknowledgement();

    *DataPointer |= (Data << (8 * i));
  }

  I2CEnd();

  return Status;
}

EFI_STATUS
I2CBurstReadCMD (
  I2CHost Host,
  UINT32 SlaveAddress,
  UINT8 *DataPointer,
  UINT8 Length)
{
  EFI_STATUS Status;
  INTN i;
  UINT32 SdaValue;

  TargetHost = Host;

  *DataPointer = 0;

  I2CStart();

  for (i = 8; i > 1; i--)
  {
    if((SlaveAddress >> (i - 1)) & 1)
      I2CHigh();
    else
      I2CLow();
  }

  I2CHigh();

  Status = I2CAcknowledgement();

  mGpioProtocol->SetDirectionInput(TargetHost.SdaBank, TargetHost.SdaBankOffset, TargetHost.SdaPin);

  for (i = 8; i > 0; i--)
  {
    mGpioProtocol->SetPin(TargetHost.SclBank, TargetHost.SclBankOffset, TargetHost.SclPin, GPIO_LEVEL_LOW);
    MicroSecondDelay(TargetHost.Delay);

    mGpioProtocol->SetPin(TargetHost.SclBank, TargetHost.SclBankOffset, TargetHost.SclPin, GPIO_LEVEL_HIGH);
    MicroSecondDelay(TargetHost.Delay);

    mGpioProtocol->GetPin(TargetHost.SdaBank, TargetHost.SdaBankOffset, TargetHost.SdaPin, &SdaValue);

    *DataPointer |= (!!(SdaValue) << (i - 1));

    mGpioProtocol->SetPin(TargetHost.SclBank, TargetHost.SclBankOffset, TargetHost.SclPin, GPIO_LEVEL_HIGH);
    MicroSecondDelay(TargetHost.Delay);

    mGpioProtocol->SetPin(TargetHost.SclBank, TargetHost.SclBankOffset, TargetHost.SclPin, GPIO_LEVEL_LOW);
    MicroSecondDelay(TargetHost.Delay);
  }

  mGpioProtocol->SetDirectionOutput(TargetHost.SdaBank, TargetHost.SdaBankOffset, TargetHost.SdaPin, FALSE);

  I2CNoAcknowledgement();

  I2CEnd();

  return Status;
}

EFI_STATUS
I2CWrite (
  I2CHost Host,
  UINT32 SlaveAddress,
  UINT32 WordAddress,
  UINT8 Data)
{
  EFI_STATUS Status;
  INTN i;

  TargetHost = Host;

  I2CStart();

  for (i = 8; i > 1; i--)
  {
    if((SlaveAddress >> (i - 1)) & 1)
        I2CHigh();
    else
        I2CLow();
  }

  I2CLow();
  I2CAcknowledgement();

  for (i = 8; i > 0; i--)
  {
    if((WordAddress >> (i - 1)) & 1)
        I2CHigh();
    else
        I2CLow();
  }

  I2CAcknowledgement();

  for (i = 8; i > 0; i--)
  {
    if((Data >> (i - 1)) & 1)
        I2CHigh();
    else
        I2CLow();
  }

  Status = I2CAcknowledgement();

  I2CEnd();

  return Status;
}

EFI_STATUS
I2CBurstWrite (
  I2CHost Host,
  UINT32 SlaveAddress,
  UINT32 WordAddress,
  UINT8 *DataPointer,
  UINT8 Length)
{
  EFI_STATUS Status;
  INTN i, j;

  TargetHost = Host;

  I2CStart();

  for (i = 8; i > 1; i--)
  {
    if((SlaveAddress >> (i - 1)) & 1)
      I2CHigh();
    else
      I2CLow();
  }

  I2CLow();

  I2CAcknowledgement();

  for (i = 8; i > 0; i--)
  {
    if((WordAddress >> (i - 1)) & 1)
      I2CHigh();
    else
      I2CLow();
  }

  I2CAcknowledgement();

  for (i = 0; i < Length; i++)
  {
    for (j = 8; j > 0; j--)
    {
      if((*DataPointer >>  (j - 1)) & 1)
        I2CHigh();
      else
        I2CLow();
    }

    DataPointer++;

    Status = I2CAcknowledgement();
  }

  I2CEnd();

  return Status;
}

EFI_STATUS
I2CWordAddressWrite (
  I2CHost Host,
  UINT32 SlaveAddress,
  UINT32 WordAddress,
  UINT32 WordData)
{
  EFI_STATUS Status;
  INTN i, j;
  UINT8 AddressData[4], Data[4];

  TargetHost = Host;

  AddressData[3] = WordAddress & 0xFF;
  AddressData[2] = (WordAddress >> 8) & 0xFF;
  AddressData[1] = (WordAddress >> 16) & 0xFF;
  AddressData[0] = WordAddress >> 24;

  Data[3] = WordData & 0xFF;
  Data[2] = (WordData >> 8) & 0xFF;
  Data[1] = (WordData >> 16) & 0xFF;
  Data[0] = WordData >> 24;

  I2CStart();

  for (i = 8; i > 1; i--)
  {
    if((SlaveAddress >> (i - 1)) & 1)
      I2CHigh();
    else
      I2CLow();
  }

  I2CLow();
  I2CAcknowledgement();

  for (i = 0; i < 4; i++)
  {
    for (j = 8; j > 0; j--)
    {
      if((AddressData[i] >> (j - 1)) & 1)
        I2CHigh();
      else
        I2CLow();
    }

    I2CAcknowledgement();
  }

  for (i = 0; i < 4; i++)
  {
    for (j = 8; j > 0; j--)
    {
      if((Data[i] >> (j - 1)) & 1)
        I2CHigh();
      else
        I2CLow();
    }

    Status = I2CAcknowledgement();
  }

  I2CEnd();

  return Status;
}

EFI_STATUS
I2CWriteWord (
  I2CHost Host,
  UINT32 SlaveAddress,
  UINT32 WordAddress,
  UINT16 *DataPointer)
{
  EFI_STATUS Status;
  UINT8 Data;
  INTN i;

  TargetHost = Host;

  I2CStart();

  for (i = 8; i > 1; i--)
  {
    if((SlaveAddress >> (i - 1)) & 1)
        I2CHigh();
    else
        I2CLow();
  }

  I2CLow();
  I2CAcknowledgement();

  for (i = 8; i > 0; i--)
  {
    if((WordAddress >> (i - 1)) & 1)
        I2CHigh();
    else
        I2CLow();
  }

  I2CAcknowledgement();

  Data = (*DataPointer >> 8) & 0xFF;

  for (i = 8; i > 0; i--)
  {
    if((Data >> (i - 1)) & 1)
        I2CHigh();
    else
        I2CLow();
  }

  I2CAcknowledgement();

  Data = (*DataPointer & 0xFF) & 0xFF;
  for (i = 8; i > 0; i--)
  {
    if((Data >> (i - 1)) & 1)
        I2CHigh();
    else
        I2CLow();
  }

  Status = I2CAcknowledgement();

  I2CEnd();

  return Status;
}

EFI_STATUS
I2CMultiAddressWrite (
  I2CHost Host,
  UINT32 SlaveAddress,
  UINT16 WordAddress,
  UINT16 Data)
{
  EFI_STATUS Status;

  INTN i;

  UINT8 AddressData[2];
  UINT8 RegisterData[2];

  TargetHost = Host;

  AddressData[0] = WordAddress & 0xFF;
  AddressData[1] = WordAddress >> 8;

  RegisterData[0] = Data & 0xFF;
  RegisterData[1] = Data >> 8;

  I2CStart();

  for (i = 8; i > 1; i--)
  {
    if((SlaveAddress >> (i - 1)) & 1)
      I2CHigh();
    else
      I2CLow();
  }

  I2CLow();
  I2CAcknowledgement();

  for (i = 8; i > 0; i--)
  {
    if((AddressData[0] >> (i - 1)) & 1)
      I2CHigh();
    else
      I2CLow();
  }

  I2CAcknowledgement();

  for (i = 8; i > 0; i--)
  {
    if((AddressData[1] >> (i - 1)) & 1)
      I2CHigh();
    else
      I2CLow();
  }

  I2CAcknowledgement();

  for (i = 8; i > 0; i--)
  {
    if((RegisterData[0] >> (i - 1)) & 1)
      I2CHigh();
    else
      I2CLow();
  }

  I2CAcknowledgement();

  for (i = 8; i > 0; i--)
  {
    if((RegisterData[1] >> (i - 1)) & 1)
      I2CHigh();
    else
      I2CLow();
  }

  Status = I2CAcknowledgement();

  I2CEnd();

  return Status;
}

EFI_STATUS
I2CBurstWriteCMD (
  I2CHost Host,
  UINT32 SlaveAddress,
  UINT16 WordAddress,
  UINT8 *DataPointer,
  UINT8 Length)
{
  EFI_STATUS Status;
  INTN i, j;

  TargetHost = Host;

  I2CEnd();

  for (i = 8; i > 1; i--)
  {
    if((SlaveAddress >> (i - 1)) & 1)
      I2CHigh();
    else
      I2CLow();
  }

  I2CLow();

  I2CAcknowledgement();

  for (i = 0; i < Length; i++)
  {
    for (j = 8; j > 0; j--)
    {
      if ((*DataPointer >> (j - 1)) & 1)
        I2CHigh();
      else
        I2CLow();
    }

    DataPointer++;

    Status = I2CAcknowledgement();
  }

  I2CEnd();

  return Status;
}

EFI_I2C_PROTOCOL mI2C = {
  I2CRead,
  I2CMultiRead,
  I2CBurstRead,
  I2CWordAddressRead,
  I2CMultiAddressRead,
  I2CBurstReadCMD,
  I2CWrite,
  I2CBurstWrite,
  I2CWordAddressWrite,
  I2CWriteWord,
  I2CMultiAddressWrite,
  I2CBurstWriteCMD
};

VOID
ConfigureI2CHosts (VOID)
{
  I2CHost *Hosts = GetI2CHosts();

  while (Hosts->Id != -1)
  {
    DEBUG((EFI_D_WARN, "Configuring I2C host with ID: %d\n", Hosts->Id));

    mGpioProtocol->SetDirectionOutput(Hosts->SclBank, Hosts->SclBankOffset, Hosts->SclPin, FALSE);
    mGpioProtocol->SetDirectionOutput(Hosts->SdaBank, Hosts->SdaBankOffset, Hosts->SdaPin, FALSE);

    mGpioProtocol->SetPull(Hosts->SclBank, Hosts->SclBankOffset, Hosts->SclPin, GPIO_PULL_NONE);
    mGpioProtocol->SetPull(Hosts->SdaBank, Hosts->SdaBankOffset, Hosts->SdaPin, GPIO_PULL_NONE);

    Hosts++;
  }
}

EFI_STATUS
EFIAPI
InitI2CDriver (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status;

  // Locate Gpio Protocol
  Status = gBS->LocateProtocol (&gEfiGpioProtocolGuid, NULL, (VOID *)&mGpioProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG((EFI_D_ERROR, "Unable to locate GPIO protocol!\n"));
    ASSERT(FALSE);
  }

  ConfigureI2CHosts();
  DEBUG((EFI_D_WARN, "I2C Hosts initialized.\n"));

  // Register I2C Protocol
  Status = gBS->InstallMultipleProtocolInterfaces (&ImageHandle, &gEfiI2CProtocolGuid, &mI2C, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG((EFI_D_ERROR, "Unable to install I2C Protocol!\n"));
    ASSERT(FALSE);
  }
  return EFI_SUCCESS;
}
