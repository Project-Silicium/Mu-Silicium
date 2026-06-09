#ifndef _EFI_USI_H_
#define _EFI_USI_H_

//
// USI Modes
//
typedef enum {
  MODE_UART,
  MODE_SPI,
  MODE_I2C
} EFI_USI_MODE;

//
// USI Bus Type
//
typedef enum {
  BUS_TYPE_UART,
  BUS_TYPE_SPI,
  BUS_TYPE_I2C
} EFI_USI_BUS_TYPE;

/**
  This Function Returns the Controller Address of the Specified Bus.

  @param[in]  BusNumber                    - The Bus Number.
  @param[in]  BusType                      - The Bus Type.
  @param[out] Address                      - The Controller Address.

  @return EFI_SUCCESS                      - Successfully Returned the Controller Address.
  @return EFI_INVALID_PARAMETER            - The "Address" Parameter is NULL.
  @return EFI_INVALID_PARAMETER            - The "BusType" Parameter is Invalid.
  @return EFI_NOT_FOUND                    - The Specified Bus was not Found.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_USI_GET_CONTROLLER_ADDRESS) (
  IN  UINT8                 BusNumber,
  IN  EFI_USI_BUS_TYPE      BusType,
  OUT EFI_PHYSICAL_ADDRESS *Address
  );

/**
  This Function sets the Mode of the Associated USI of the Specified Controller.

  @param[in] Address                       - The Controller Address.
  @param[in] Mode                          - The new USI Mode.

  @return EFI_SUCCESS                      - Successfully Changed the Mode of USI.
  @return EFI_INVALID_PARAMETER            - The "Mode" Parameter is Invalid.
  @return EFI_NOT_FOUND                    - The Specified Controller was not Found.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_USI_SET_MODE) (
  IN EFI_PHYSICAL_ADDRESS Address,
  IN EFI_USI_MODE         Mode
  );

//
// Define Protocol
//
typedef struct {
  EFI_USI_GET_CONTROLLER_ADDRESS GetControllerAddress;
  EFI_USI_SET_MODE               SetMode;
} EFI_USI_PROTOCOL;

#endif /* _EFI_USI_H_ */
