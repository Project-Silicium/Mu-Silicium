#ifndef _EFI_X_OEM_CERT_H_
#define _EFI_X_OEM_CERT_H_

//
// Global GUID for the XOemCert Protocol
//
#define EFI_X_OEM_CERT_PROTOCOL_GUID { 0x39953D72, 0xF33A, 0x4291, { 0xBE, 0xAF, 0x95, 0x58, 0xBE, 0xF6, 0xF6, 0x6A } }

//
// Declare forward reference to the XOemCert Protocol
//
typedef struct _EFI_X_OEM_CERT_PROTOCOL EFI_X_OEM_CERT_PROTOCOL;

/**
  This Function Sets the OEM Certificate.

  @param Unknown1      Unknown Variable.
  @param Unknown2      Unknown Variable.

  @return Status       The EFI_STATUS returned by the Function.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_SET_OEM_CERTIFICATE) (
  UINT8 Unknown1,
  UINTN Unknown2
  );

//
// Define Protocol Functions
//
struct _EFI_X_OEM_CERT_PROTOCOL {
  UINT64                   Revision;
  EFI_SET_OEM_CERTIFICATE  SetCertificate;

  // NOTE: There are maybe more Functions.
};

extern EFI_GUID gXOemCertProtocolGuid;

#endif /* _EFI_X_OEM_CERT_H_ */
