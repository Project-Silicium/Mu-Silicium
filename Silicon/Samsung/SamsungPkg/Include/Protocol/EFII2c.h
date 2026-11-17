#ifndef _EFI_I2C_H_
#define _EFI_I2C_H_

/**
  This Function Initialises the Specified Legacy I2C Bus.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_I2C_INIT_BUS) (
  IN UINT8 BusNumber
  );

/**
  This Function Reads a 32 Bit Value from a Slave Register.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_I2C_READ_32) (
  IN  UINT8   BusNumber,
  IN  UINT8   SlaveAddr,
  IN  UINT32  SlaveReg,
  OUT UINT32 *Data
  );

/**
  This Function Writes a 32 Bit Value to a Slave Register.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_I2C_WRITE_32) (
  IN UINT8  BusNumber,
  IN UINT8  SlaveAddr,
  IN UINT32 SlaveReg,
  IN UINT32 Data
  );

/**
  This Function Reads a Byte from a Slave Register.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_I2C_READ) (
  IN  UINT8   BusNumber,
  IN  UINT8   SlaveAddr,
  IN  UINT32  SlaveReg,
  OUT UINT8  *Data
  );

/**
  This Function Writes a Byte to a Slave Register.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_I2C_WRITE) (
  IN UINT8  BusNumber,
  IN UINT8  SlaveAddr,
  IN UINT32 SlaveReg,
  IN UINT8  Data
  );

/**
  This Function Reads a Number of Bytes from a Slave Register.

  SlaveAddr is a 7 Bit Address. Only the Low Byte of SlaveReg is placed on
  the Bus.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_I2C_READ_BUFFER) (
  IN  UINT8   BusNumber,
  IN  UINT8   SlaveAddr,
  IN  UINT32  SlaveReg,
  OUT UINT8  *Data,
  IN  UINTN   DataLen
  );

/**
  This Function Writes a Number of Bytes to a Slave Register.

  SlaveAddr is a 7 Bit Address. Only the Low Byte of SlaveReg is placed on
  the Bus.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_I2C_WRITE_BUFFER) (
  IN UINT8   BusNumber,
  IN UINT8   SlaveAddr,
  IN UINT32  SlaveReg,
  IN UINT8  *Data,
  IN UINTN   DataLen
  );

//
// Define Protocol
//
typedef struct {
  EFI_I2C_INIT_BUS     InitBus;
  EFI_I2C_READ_32      Read32;
  EFI_I2C_WRITE_32     Write32;
  EFI_I2C_READ         Read;
  EFI_I2C_WRITE        Write;
  EFI_I2C_READ_BUFFER  ReadBuffer;
  EFI_I2C_WRITE_BUFFER WriteBuffer;
} EFI_I2C_PROTOCOL;

#endif /* _EFI_I2C_H_ */
