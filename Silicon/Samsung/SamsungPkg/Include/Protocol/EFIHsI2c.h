#ifndef _EFI_HSI2C_H_
#define _EFI_HSI2C_H_

/**
  This Function Inits the Specified HSI2C Bus.

  @param[in] BusNumber                     - The Bus Number.

  @return EFI_SUCCESS                      - The Bus was Init Successfully.
  @return EFI_NOT_FOUND                    - The Bus does not Exist.
  @return EFI_ALREADY_STARTED              - The Controller is already in Use by SPI/UART.
  ...
**/
typedef
EFI_STATUS
(EFIAPI *EFI_HSI2C_INIT_BUS) (
  IN UINT8 BusNumber
  );

/**
  This Function Reads Data from the Specified Slave of the Specified HSI2C Bus.

  @param[in]  BusNumber                    - The Bus Number.
  @param[in]  SlaveAddr                    - The Slave Address.
  @param[in]  SlaveReg                     - The Slave Register.
  @param[out] Data                         - The Data.

  @return EFI_SUCCESS                      - The Data was Read Successfully.
  @return EFI_NOT_FOUND                    - The Bus does not Exist.
  @return EFI_NOT_READY                    - The Bus isn't Init yet.
  ...
**/
typedef
EFI_STATUS
(EFIAPI *EFI_HSI2C_READ_32) (
  IN  UINT8   BusNumber,
  IN  UINT8   SlaveAddr,
  IN  UINT32  SlaveReg,
  OUT UINT32 *Data
  );

/**
  This Function Writes the Specified Data to the Specified Slave of the Specified HSI2C Bus.

  @param[in] BusNumber                     - The Bus Number.
  @param[in] SlaveAddr                     - The Slave Address.
  @param[in] SlaveReg                      - The Slave Register.
  @param[in] Data                          - The Data.

  @return EFI_SUCCESS                      - The Data was Read Successfully.
  @return EFI_NOT_FOUND                    - The Bus does not Exist.
  @return EFI_NOT_READY                    - The Bus isn't Init yet.
  ...
**/
typedef
EFI_STATUS
(EFIAPI *EFI_HSI2C_WRITE_32) (
  IN UINT8  BusNumber,
  IN UINT8  SlaveAddr,
  IN UINT32 SlaveReg,
  IN UINT32 Data
  );

/**
  This Function Reads Data from the Specified Slave of the Specified HSI2C Bus.

  @param[in]  BusNumber                    - The Bus Number.
  @param[in]  SlaveAddr                    - The Slave Address.
  @param[in]  SlaveReg                     - The Slave Register.
  @param[out] Data                         - The Data.

  @return EFI_SUCCESS                      - The Data was Read Successfully.
  @return EFI_NOT_FOUND                    - The Bus does not Exist.
  @return EFI_NOT_READY                    - The Bus isn't Init yet.
  ...
**/
typedef
EFI_STATUS
(EFIAPI *EFI_HSI2C_READ) (
  IN  UINT8   BusNumber,
  IN  UINT8   SlaveAddr,
  IN  UINT32  SlaveReg,
  OUT UINT8  *Data
  );

/**
  This Function Writes the Specified Data to the Specified Slave of the Specified HSI2C Bus.

  @param[in] BusNumber                     - The Bus Number.
  @param[in] SlaveAddr                     - The Slave Address.
  @param[in] SlaveReg                      - The Slave Register.
  @param[in] Data                          - The Data.

  @return EFI_SUCCESS                      - The Data was Read Successfully.
  @return EFI_NOT_FOUND                    - The Bus does not Exist.
  @return EFI_NOT_READY                    - The Bus isn't Init yet.
  ...
**/
typedef
EFI_STATUS
(EFIAPI *EFI_HSI2C_WRITE) (
  IN UINT8  BusNumber,
  IN UINT8  SlaveAddr,
  IN UINT32 SlaveReg,
  IN UINT8  Data
  );

//
// Define Protocol
//
typedef struct {
  EFI_HSI2C_INIT_BUS InitBus;
  EFI_HSI2C_READ_32  Read32;
  EFI_HSI2C_WRITE_32 Write32;
  EFI_HSI2C_READ     Read;
  EFI_HSI2C_WRITE    Write;
} EFI_HSI2C_PROTOCOL;

#endif /* _EFI_HSI2C_H_ */
