#ifndef _EFI_X_DEVICE_TREE_H_
#define _EFI_X_DEVICE_TREE_H_

//
// Global GUID for the XDeviceTree Protocol
//
#define EFI_X_DEVICE_TREE_PROTOCOL_GUID { 0xB5902DCC, 0x5B30, 0x4678, { 0xB9, 0x09, 0x99, 0x52, 0xF1, 0xF3, 0xA5, 0x69 } }

//
// Declare forward reference to the XDeviceTree Protocol
//
typedef struct _EFI_X_DEVICE_TREE_PROTOCOL EFI_X_DEVICE_TREE_PROTOCOL;

/**
  This Function Sets the XDevice Tree Load Address.

  @param[in] LoadAddr     The Load Address.

  @return    Status       The EFI_STATUS returned by the Function.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_SET_LOAD_ADDR) (
  IN UINT8 LoadAddr
  );

/**
  This Function Gets the XDevice Tree Load Address.

  @param[out] LoadAddr    The Load Address.

  @return     Status      The EFI_STATUS returned by the Function.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_GET_LOAD_ADDR) (
  OUT UINT8 *LoadAddr
  );

//
// Define Protocol Functions
//
struct _EFI_X_DEVICE_TREE_PROTOCOL {
  UINT64             Revision;
  EFI_SET_LOAD_ADDR  SetLoadAddr;
  EFI_GET_LOAD_ADDR  GetLoadAddr;

  // NOTE: There are maybe more Functions.
};

extern EFI_GUID gXDeviceTreeProtocolGuid;

#endif /* _EFI_X_DEVICE_TREE_H_ */