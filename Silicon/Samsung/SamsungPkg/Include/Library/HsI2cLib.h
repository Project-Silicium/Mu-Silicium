#ifndef _HSI2C_LIB_H_
#define _HSI2C_LIB_H_

#include <GpioData.h>

//
// HSI2C Speed Modes
//
typedef enum {
  STAND_SPD,
  FAST_SPD,
  FAST_PLUS_SPD,
  HIGH_SPD
} EFI_HSI2C_SPEED_MODE;

//
// HSI2C GPIO Data
//
typedef struct {
  EFI_GPIO_FUNCTION Function;
  EFI_GPIO_BANK_ID  SclBankId;
  EFI_GPIO_BANK_ID  SdaBankId;
  UINT8             SclBankNum;
  UINT8             SdaBankNum;
  UINT8             SclPin;
  UINT8             SdaPin;
} EFI_HSI2C_GPIO_DATA;

//
// HSI2C Clock Data
//
typedef struct {
  UINT32               Freq;
  EFI_HSI2C_SPEED_MODE SpeedMode;
} EFI_HSI2C_CLOCK_DATA;

//
// HSI2C Bus Data
//
typedef struct {
  EFI_HSI2C_GPIO_DATA  Gpio;
  EFI_HSI2C_CLOCK_DATA Clock;
  BOOLEAN              Initialized;
} EFI_HSI2C_BUS_DATA;

/**
  This Function Returns the Platform HSI2C Buses.

  @param[out] Data                         - The HSI2C Data.
  @param[out] Count                        - The Number of HSI2C Buses.
**/
VOID
GetHsI2cBusData (
  OUT EFI_HSI2C_BUS_DATA **Data,
  OUT UINT8               *Count
  );

#endif /* _HSI2C_LIB_H_ */
