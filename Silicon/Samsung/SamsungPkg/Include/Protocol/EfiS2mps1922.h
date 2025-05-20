#ifndef _EFI_S2MPS_19_22_H_
#define _EFI_S2MPS_19_22_H_

//
// Global GUID for the S2mps1922 Protocol
//
#define EFI_S2MPS_19_22_PROTOCOL_GUID { 0x0C60E64E, 0x6587, 0x4B3A, { 0xBD, 0x51, 0x11, 0x47, 0xF2, 0xCC, 0x9C, 0x0B } }

//
// Declare forward reference to the S2mps1922 Protocol
//
typedef struct _EFI_S2MPS_19_22_PROTOCOL EFI_S2MPS_19_22_PROTOCOL;

/**
  This Function Disables WTSR of the PMIC.

  @return EFI_SUCCESS                      - Successfully Disabled WTSR.
  @return EFI_TIMEOUT                      - There was a Timeout in Reading/Writing the Register.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_DISABLE_WTSR) ();

/**
  This Function Disables SMPL of the PMIC.

  @return EFI_SUCCESS                      - Successfully Disabled SMPL.
  @return EFI_TIMEOUT                      - There was a Timeout in Reading/Writing the Register.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_DISABLE_SMPL) ();

/**
  This Function Turns Off the PMIC.

  @return EFI_SUCCESS                      - Successfully Turned Off the PMIC.
  @return EFI_TIMEOUT                      - There was a Timeout in Writing the Register.
  @return EFI_UNSUPPORTED                  - This SoC does not Support this Function.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_POWER_DOWN) ();

//
// Define Protocol Functions
//
struct _EFI_S2MPS_19_22_PROTOCOL {
  EFI_DISABLE_WTSR DisableWtsr;
  EFI_DISABLE_SMPL DisableSmpl;
  EFI_POWER_DOWN   PowerDown;
};

extern EFI_GUID gEfiS2mps1922ProtocolGuid;

#endif /* _EFI_S2MPS_19_20_H_ */
