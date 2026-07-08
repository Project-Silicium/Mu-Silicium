#ifndef _USI_LIB_H_
#define _USI_LIB_H_

//
// USI Data
//
typedef struct {
  EFI_PHYSICAL_ADDRESS UsiAddress;
  EFI_PHYSICAL_ADDRESS ControllerAddress;
  UINT8                I2cBus;
  UINT8                SpiBus;
  UINT8                UartBus;
} EFI_USI_DATA;

/**
  This Function Returns the Platform USI Data.

  @param[out] Data                         - The USI Data.
  @param[out] Count                        - The Number of USI.
**/
VOID
GetUsiData (
  OUT EFI_USI_DATA **Data,
  OUT UINT8         *Count
  );

#endif /* _USI_LIB_H_ */
