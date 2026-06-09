#ifndef _SPEEDY_LIB_H_
#define _SPEEDY_LIB_H_

/**
  This Function Returns the Platform SPEEDY Buses.

  @param[out] Bus                          - The SPEEDY Buses.
  @param[out] Count                        - The Number of SPEEDY Buses.
**/
VOID
GetSpeedyBuses (
  OUT EFI_PHYSICAL_ADDRESS **Bus,
  OUT UINT8                 *Count
  );

#endif /* _SPEEDY_LIB_H_ */
