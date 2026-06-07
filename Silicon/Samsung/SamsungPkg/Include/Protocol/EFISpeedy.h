#ifndef _EFI_SPEEDY_H_
#define _EFI_SPEEDY_H_

/**
  This Function Reads Data from the Specified Slave.

  @param[in]  BusNumber                    - The SPEEDY Bus Number.
  @param[in]  Slave                        - The Slave.
  @param[in]  SlaveAddress                 - The Slave Address.
  @param[out] Data                         - The Output Data.

  @return EFI_SUCCESS                      - Successfully Read Data from the Specified Slave.
  @return EFI_INVALID_PARAMETER            - The Data Parameter is NULL.
  @return EFI_NOT_FOUND                    - The Specified SPEEDY Bus does not Exist.
  @return EFI_TIMEOUT                      - The SPEEDY Command took too Long to be Processed.
  @return EFI_PROTOCOL_ERROR               - The SPEEDY RX Bits are Faulty.
  @return EFI_CRC_ERROR                    - The SPEEDY RX is Glitched.
  @return EFI_DEVICE_ERROR                 - The SPEEDY Bus Returned an Unknown Error.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_SPEEDY_READ) (
  IN  UINT8  BusNumber,
  IN  UINT8  Slave,
  IN  UINT8  Address,
  OUT UINT8 *Data
  );

/**
  This Function Reads Data from the Specified Slave.

  @param[in]  BusNumber                    - The SPEEDY Bus Number.
  @param[in]  Slave                        - The Slave.
  @param[in]  SlaveAddress                 - The Slave Address.
  @param[in]  DataCount                    - The Amount of Data to Read.
  @param[out] Data                         - The Output Data.

  @return EFI_SUCCESS                      - Successfully Read Data from the Specified Slave.
  @return EFI_INVALID_PARAMETER            - The Data Parameter is NULL and/or DataCount is 0.
  @return EFI_NOT_FOUND                    - The Specified SPEEDY Bus does not Exist.
  @return EFI_TIMEOUT                      - The SPEEDY Command took too Long to be Processed.
  @return EFI_PROTOCOL_ERROR               - The SPEEDY RX Bits are Faulty.
  @return EFI_CRC_ERROR                    - The SPEEDY RX is Glitched.
  @return EFI_DEVICE_ERROR                 - The SPEEDY Bus Returned an Unknown Error.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_SPEEDY_BURST_READ) (
  IN  UINT8  BusNumber,
  IN  UINT8  Slave,
  IN  UINT8  Address,
  IN  UINT8  DataCount,
  OUT UINT8 *Data
  );

/**
  This Function Writes the Specified Data to the Specified Slave.

  @param[in] BusNumber                     - The SPEEDY Bus Number.
  @param[in] Slave                         - The Slave.
  @param[in] SlaveAddress                  - The Slave Address.
  @param[in] Data                          - The Input Data.

  @return EFI_SUCCESS                      - Successfully Wrote the Specified Data to the Specified Slave.
  @return EFI_INVALID_PARAMETER            - The Data Parameter is 0.
  @return EFI_NOT_FOUND                    - The Specified SPEEDY Bus does not Exist.
  @return EFI_TIMEOUT                      - The SPEEDY Command took too Long to be Processed.
  @return EFI_PROTOCOL_ERROR               - The SPEEDY RX Bits are Faulty.
  @return EFI_CRC_ERROR                    - The SPEEDY RX is Glitched.
  @return EFI_DEVICE_ERROR                 - The SPEEDY Bus Returned an Unknown Error.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_SPEEDY_WRITE) (
  IN UINT8 BusNumber,
  IN UINT8 Slave,
  IN UINT8 Address,
  IN UINT8 Data
  );

/**
  This Function Writes the Specified Data to the Specified Slave.

  @param[in] BusNumber                     - The SPEEDY Bus Number.
  @param[in] Slave                         - The Slave.
  @param[in] SlaveAddress                  - The Slave Address.
  @param[in] DataCount                     - The Amount of Data to Write.
  @param[in] Data                          - The Input Data.

  @return EFI_SUCCESS                      - Successfully Wrote the Specified Data to the Specified Slave.
  @return EFI_INVALID_PARAMETER            - The Data Parameter is NULL and/or DataCount is 0.
  @return EFI_NOT_FOUND                    - The Specified SPEEDY Bus does not Exist.
  @return EFI_TIMEOUT                      - The SPEEDY Command took too Long to be Processed.
  @return EFI_PROTOCOL_ERROR               - The SPEEDY RX Bits are Faulty.
  @return EFI_CRC_ERROR                    - The SPEEDY RX is Glitched.
  @return EFI_DEVICE_ERROR                 - The SPEEDY Bus Returned an Unknown Error.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_SPEEDY_BURST_WRITE) (
  IN UINT8  BusNumber,
  IN UINT8  Slave,
  IN UINT8  Address,
  IN UINT8  DataCount,
  IN UINT8 *Data
  );

//
// Define Protocol Functions
//
typedef struct {
  EFI_SPEEDY_READ        Read;
  EFI_SPEEDY_BURST_READ  BurstRead;
  EFI_SPEEDY_WRITE       Write;
  EFI_SPEEDY_BURST_WRITE BurstWrite;
} EFI_SPEEDY_PROTOCOL;

#endif /* _EFI_SPEEDY_H_ */
