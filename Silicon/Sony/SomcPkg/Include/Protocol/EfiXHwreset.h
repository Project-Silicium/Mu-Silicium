#ifndef _EFI_X_HWRESET_H_
#define _EFI_X_HWRESET_H_

//
// Global GUID for the XHwreset Protocol
//
#define EFI_X_HWRESET_PROTOCOL_GUID { 0x4684800A, 0x2755, 0x4EDC, { 0xB4, 0x43, 0x7F, 0x8C, 0xEB, 0x32, 0x39, 0xD1 } }

//
// Declare forward reference to the XHwreset Protocol
//
typedef struct _EFI_X_HWRESET_PROTOCOL EFI_X_HWRESET_PROTOCOL;

/**
  This Function Enables XHwreset.

  @return Status        The EFI_STATUS returned by the Function.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_ENABLE_XHWRESET) (VOID);

//
// Define Protocol Functions
//
struct _EFI_X_HWRESET_PROTOCOL {
  UINT64               Revision;
  EFI_ENABLE_XHWRESET  Enable;

  // NOTE: There are maybe more Functions.
};

extern EFI_GUID gXHwresetProtocolGuid;

#endif /* _EFI_X_HWRESET_H_ */
