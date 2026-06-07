#ifndef _SPEEDY_LIB_H_
#define _SPEEDY_LIB_H_

//
// SPEEDY Bus Data
//
typedef struct {
  IN UINT8                Number;
  IN EFI_PHYSICAL_ADDRESS Address;
} EFI_SPEEDY_BUS_DATA;

/**
  This Function Returns the Platform SPEEDY Bus Data.

  @param[out] Data                         - The SPEEDY Bus Data.
  @param[out] Count                        - The Count of SPEEDY Buses.
**/
VOID
GetSpeedyBusData (
  OUT EFI_SPEEDY_BUS_DATA **Data,
  OUT UINT8                *Count
  );

#endif /* _SPEEDY_LIB_H_ */
