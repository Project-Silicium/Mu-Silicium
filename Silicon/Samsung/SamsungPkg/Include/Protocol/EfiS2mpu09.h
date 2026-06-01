#ifndef _EFI_S2MPU09_H_
#define _EFI_S2MPU09_H_

//
// Global GUID for the S2MPU09 Protocol
//
#define EFI_S2MPU09_PROTOCOL_GUID { 0x75CF3316, 0xCD37, 0x41FE, { 0xAC, 0xA1, 0xB7, 0x18, 0x0B, 0x75, 0x5A, 0xA3 } }

//
// Declare forward reference to the S2mpu09 Protocol
//
typedef struct _EFI_S2MPU09_PROTOCOL EFI_S2MPU09_PROTOCOL;

/**
  This Function Disables WTSR of the PMIC.

  @return EFI_SUCCESS                      - Successfully Disabled WTSR.
  @return EFI_TIMEOUT                      - There was a Timeout in Reading/Writing the Register.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_S2MPU09_DISABLE_WTSR) ();

/**
  This Function Disables SMPL of the PMIC.

  @return EFI_SUCCESS                      - Successfully Disabled SMPL.
  @return EFI_TIMEOUT                      - There was a Timeout in Reading/Writing the Register.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_S2MPU09_DISABLE_SMPL) ();

//
// Define Protocol Functions
//
struct _EFI_S2MPU09_PROTOCOL {
  EFI_S2MPU09_DISABLE_WTSR DisableWtsr;
  EFI_S2MPU09_DISABLE_SMPL DisableSmpl;
};

extern EFI_GUID gEfiS2mpu09ProtocolGuid;

#endif /* _EFI_S2MPU09_H_ */
