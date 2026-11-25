#ifndef _USI_LIB_H_
#define _USI_LIB_H_

//
// USI Data
//
typedef enum {
  UsiVersion2 = 0,
  UsiVersion1,
  UsiVersionNone
} EFI_USI_VERSION;

typedef enum {
  UsiChannel0 = 0,
  UsiChannel1
} EFI_USI_CHANNEL;

typedef struct {
  EFI_PHYSICAL_ADDRESS UsiAddress;
  EFI_PHYSICAL_ADDRESS ControllerAddress;
  UINT8                I2cBus;
  UINT8                SpiBus;
  UINT8                UartBus;
  EFI_USI_VERSION      Version;
  EFI_USI_CHANNEL      Channel;
  UINT32               ActiveMode;
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
