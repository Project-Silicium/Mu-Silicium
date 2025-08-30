#ifndef _EFI_I2C_H_
#define _EFI_I2C_H_

#include <Protocol/EfiGpio.h>

//
// Global GUID for the I2C Protocol
//
#define EFI_I2C_PROTOCOL_GUID { 0xF7762E08, 0xAECA, 0x4160, { 0xBC, 0xBE, 0x18, 0xEF, 0x46, 0xD2, 0x7C, 0x4E } }

//
// Declare forward reference to the I2C Protocol
//
typedef struct _EFI_I2C_PROTOCOL EFI_I2C_PROTOCOL;

typedef struct i2c_host {
  INTN     Id;
  UINT32   Delay;
  GpioBank *SclBank;
  UINT32   SclBankOffset;
  UINT32   SclPin;
  GpioBank *SdaBank;
  UINT32   SdaBankOffset;
  UINT32   SdaPin;
} I2CHost;

//
// Define Protocol Functions
//

/**
  This Function reads from the specific I2C Host.

  @param[in] I2CHost                       - The target I2C host.
  @param[in] SlaveAddress                  - The address to the slave device.
  @param[in] WordAddress                   - The word address.
  @param[in] DataPointer                   - The pointer of where to store data to.

  @return EFI_SUCCESS                      - Successfully read from I2C.
  @return EFI_TIMEOUT                      - I2C recieved no ACK, most likely to timeout.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_I2C_READ) (
  I2CHost Host,
  UINT32  SlaveAddres,
  UINT32  WordAddress,
  UINT8   *DataPointer
  );

/**
  This Function reads from the specific I2C Host multiple times.

  @param[in] I2CHost                       - The target I2C host.
  @param[in] SlaveAddress                  - The address to the slave device.
  @param[in] WordAddress                   - The word address.
  @param[in] DataPointer                   - The pointer of where to store data to.

  @return EFI_SUCCESS                      - Successfully read from I2C.
  @return EFI_TIMEOUT                      - I2C recieved no ACK, most likely to timeout.
**/    
typedef   
EFI_STATUS    
(EFIAPI *EFI_I2C_MULTI_READ) (
  I2CHost Host, 
  UINT32  SlaveAddres,
  UINT32  WordAddress,
  UINT8   *DataPointer
  );

/**
  This Function does a burst read from the specific I2C Host.

  @param[in] I2CHost                       - The target I2C host.
  @param[in] SlaveAddress                  - The address to the slave device.
  @param[in] WordAddress                   - The word address.
  @param[in] DataPointer                   - The pointer of where to store data to.
  @param[in] Length                        - The length of data to be read.

  @return EFI_SUCCESS                      - Successfully read from I2C.
  @return EFI_TIMEOUT                      - I2C recieved no ACK, most likely to timeout.
**/       
typedef       
EFI_STATUS
(EFIAPI *EFI_I2C_BURST_READ) (
  I2CHost Host,
  UINT32  SlaveAddres,
  UINT32  WordAddress,
  UINT8   *DataPointer,
  UINT8   Length
  );

/**
  This Function reads from a specific word address on the specific I2C Host.

  @param[in] I2CHost                       - The target I2C host.
  @param[in] SlaveAddress                  - The address to the slave device.
  @param[in] WordAddress                   - The word address.
  @param[in] DataPointer                   - The pointer of where to store data to.

  @return EFI_SUCCESS                      - Successfully read from I2C.
  @return EFI_TIMEOUT                      - I2C recieved no ACK, most likely to timeout.
**/       
typedef       
EFI_STATUS
(EFIAPI *EFI_I2C_WORD_ADDRESS_READ) (
  I2CHost Host,
  UINT32  SlaveAddres,
  UINT32  WordAddress,
  UINT32   *DataPointer
  );

/**
  This Function reads from a specific word address on the specific I2C Host multiple times.

  @param[in] I2CHost                       - The target I2C host.
  @param[in] SlaveAddress                  - The address to the slave device.
  @param[in] WordAddress                   - The word address.
  @param[in] DataPointer                   - The pointer of where to store data to.

  @return EFI_SUCCESS                      - Successfully read from I2C.
  @return EFI_TIMEOUT                      - I2C recieved no ACK, most likely to timeout.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_I2C_MULTI_ADDRESS_READ) (
  I2CHost Host,
  UINT32  SlaveAddres,
  UINT16  WordAddress,
  UINT16  *DataPointer
  );

/**
  This Function does a burst read from the specific I2C Host for a specific command.

  @param[in] I2CHost                       - The target I2C host.
  @param[in] SlaveAddress                  - The address to the slave device.
  @param[in] WordAddress                   - The word address.
  @param[in] DataPointer                   - The pointer of where to store data to.
  @param[in] Length                        - The length of data to be read.

  @return EFI_SUCCESS                      - Successfully read from I2C.
  @return EFI_TIMEOUT                      - I2C recieved no ACK, most likely to timeout.
**/    
typedef   
EFI_STATUS
(EFIAPI *EFI_I2C_BURST_READ_CMD) (
  I2CHost Host,
  UINT32 SlaveAddress,
  UINT8 *DataPointer,
  UINT8 Length
  );

/**
 This Function writes from the specific I2C Host.

 @param[in] I2CHost                       - The target I2C host.
 @param[in] SlaveAddress                  - The address to the slave device.
 @param[in] WordAddress                   - The word address.
 @param[in] Data                          - The data to be written.

 @return EFI_SUCCESS                      - Successfully written to I2C.
 @return EFI_TIMEOUT                      - I2C recieved no ACK, most likely to timeout.
 **/
typedef
EFI_STATUS
(EFIAPI *EFI_I2C_WRITE) (
  I2CHost Host,
  UINT32  SlaveAddres,
  UINT32  WordAddress,
  UINT8   Data
);

/**
 This Function does a burst write to the specific I2C Host.

 @param[in] I2CHost                       - The target I2C host.
 @param[in] SlaveAddress                  - The address to the slave device.
 @param[in] WordAddress                   - The word address.
 @param[in] DataPointer                   - The of data to be stored.
 @param[in] Length                        - The length of data to be written.

 @return EFI_SUCCESS                      - Successfully written to I2C.
 @return EFI_TIMEOUT                      - I2C recieved no ACK, most likely to timeout.
 **/
typedef
EFI_STATUS
(EFIAPI *EFI_I2C_BURST_WRITE) (
  I2CHost Host,
  UINT32  SlaveAddres,
  UINT32  WordAddress,
  UINT8   *DataPointer,
  UINT8   Length
);

/**
 This Function writes to a specific word address on the specific I2C Host.

 @param[in] I2CHost                       - The target I2C host.
 @param[in] SlaveAddress                  - The address to the slave device.
 @param[in] WordAddress                   - The word address.
 @param[in] WordData                      - The word data to be written.

 @return EFI_SUCCESS                      - Successfully written to I2C.
 @return EFI_TIMEOUT                      - I2C recieved no ACK, most likely to timeout.
 **/
typedef
EFI_STATUS
(EFIAPI *EFI_I2C_WORD_ADDRESS_WRITE) (
  I2CHost Host,
  UINT32  SlaveAddres,
  UINT32  WordAddress,
  UINT32  WordData
);

/**
 This Function writes to a specific word address on the specific I2C Host.

 @param[in] I2CHost                       - The target I2C host.
 @param[in] SlaveAddress                  - The address to the slave device.
 @param[in] WordAddress                   - The word address.
 @param[in] DataPointer                   - The pointer to the data to be written.

 @return EFI_SUCCESS                      - Successfully written to I2C.
 @return EFI_TIMEOUT                      - I2C recieved no ACK, most likely to timeout.
 **/
typedef
EFI_STATUS
(EFIAPI *EFI_I2C_WRITE_WORD) (
  I2CHost Host,
  UINT32 SlaveAddress,
  UINT32 WordAddress,
  UINT16 *DataPointer
);

/**
 This Function writes to a specific word address on the specific I2C Host multiple times.

 @param[in] I2CHost                       - The target I2C host.
 @param[in] SlaveAddress                  - The address to the slave device.
 @param[in] WordAddress                   - The word address.
 @param[in] Data                          - The data to be written.

 @return EFI_SUCCESS                      - Successfully written to I2C.
 @return EFI_TIMEOUT                      - I2C recieved no ACK, most likely to timeout.
 **/
typedef
EFI_STATUS
(EFIAPI *EFI_I2C_MULTI_ADDRESS_WRITE) (
  I2CHost Host,
  UINT32 SlaveAddress,
  UINT16 WordAddress,
  UINT16 Data
);

/**
 This Function does a burst write to the specific I2C Host for a specific command.

 @param[in] I2CHost                       - The target I2C host.
 @param[in] SlaveAddress                  - The address to the slave device.
 @param[in] WordAddress                   - The word address.
 @param[in] DataPointer                   - The pointer of the data to be written.
 @param[in] Length                        - The length of data to be written.

 @return EFI_SUCCESS                      - Successfully written to I2C.
 @return EFI_TIMEOUT                      - I2C recieved no ACK, most likely to timeout.
 **/
typedef
EFI_STATUS
(EFIAPI *EFI_I2C_BURST_WRITE_CMD) (
  I2CHost Host,
  UINT32 SlaveAddress,
  UINT16 WordAddress,
  UINT8 *DataPointer,
  UINT8 Length
);

struct _EFI_I2C_PROTOCOL {
    EFI_I2C_READ                Read;
    EFI_I2C_MULTI_READ          MultiRead;
    EFI_I2C_BURST_READ          BurstRead;
    EFI_I2C_WORD_ADDRESS_READ   WordAddressRead;
    EFI_I2C_MULTI_ADDRESS_READ  MultiAddressRead;
    EFI_I2C_BURST_READ_CMD      BurstReadCmd;
    EFI_I2C_WRITE               Write;
    EFI_I2C_BURST_WRITE         BurstWrite;
    EFI_I2C_WORD_ADDRESS_WRITE  WordAddressWrite;
    EFI_I2C_WRITE_WORD          WriteWord;
    EFI_I2C_MULTI_ADDRESS_WRITE MultiAddressWrite;
    EFI_I2C_BURST_WRITE_CMD     BurstWriteCmd;
};

extern EFI_GUID gEfiI2CProtocolGuid;

#endif /* _EFI_I2C_H_ */
