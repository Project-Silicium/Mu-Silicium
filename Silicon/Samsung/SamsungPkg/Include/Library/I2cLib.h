#ifndef _I2C_LIB_H_
#define _I2C_LIB_H_

#include <Device/Gpio.h>

//
// Legacy I2C GPIO Data
//
typedef struct {
  EFI_GPIO_FUNCTION  Function;
  EFI_GPIO_PULL_MODE Pull;
  EFI_GPIO_BANK_ID   SclBankId;
  EFI_GPIO_BANK_ID   SdaBankId;
  UINT8              SclBankNum;
  UINT8              SdaBankNum;
  UINT8              SclPin;
  UINT8              SdaPin;
} EFI_I2C_GPIO_DATA;

//
// Legacy I2C Clock Data
//
typedef struct {
  UINT32 SourceFrequency;
  UINT32 BusFrequency;
  UINT32 SdaDelay; // Nanoseconds
} EFI_I2C_CLOCK_DATA;

//
// Legacy I2C Bus Data
//
typedef struct {
  EFI_PHYSICAL_ADDRESS Address;
  EFI_I2C_GPIO_DATA     Gpio;
  EFI_I2C_CLOCK_DATA    Clock;
  BOOLEAN               Initialized;
} EFI_I2C_BUS_DATA;

/**
  This Function Returns the Platform Legacy I2C Buses.

  @param[out] Data                         - The Legacy I2C Data.
  @param[out] Count                        - The Number of Legacy I2C Buses.
**/
VOID
GetI2cBusData (
  OUT EFI_I2C_BUS_DATA **Data,
  OUT UINT8             *Count
  );

#endif /* _I2C_LIB_H_ */
