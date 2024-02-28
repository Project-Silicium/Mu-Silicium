#ifndef _EFI_X_RESET_REASON_H_
#define _EFI_X_RESET_REASON_H_

//
// Global GUID for the XReset Reason Protocol
//
#define EFI_X_RESET_REASON_PROTOCOL_GUID { 0x98243104, 0xA34F, 0x4C01, { 0x8F, 0x99, 0xDD, 0xE1, 0x84, 0x9F, 0x23, 0x90 } }

//
// Declare forward reference to the XReset Reason Protocol
//
typedef struct _EFI_X_RESET_REASON_PROTOCOL EFI_X_RESET_REASON_PROTOCOL;

/**
  This Function Clears the current Reset Reason of the Device.

  @param[in]  Unknown         Unknown Value.

  @return     Status          The EFI_STATUS returned by the Function.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_CLEAR_RESET_REASON) (
  IN UINTN Unknown
  );

/**
  This Function Gets the Reset Reason of the Device.

  @param[out] Reason          The Reason of the Reset.

  @return     Status          The EFI_STATUS returned by the Function.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_GET_RESET_REASON) (
  OUT UINTN *Reason
  );

/**
  This Function Sets the Reset Reason for the Device.

  @param[in] Reason           The Reason for the Reset.

  @return    Status           The EFI_STATUS returned by the Function.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_SET_RESET_REASON) (
  IN UINTN Reason
  );

//
// Define Protocol Functions
//
struct _EFI_X_RESET_REASON_PROTOCOL {
  UINT64                  Revision;
  VOID                   *UnknownFunc1;
  VOID                   *UnknownFunc2;
  EFI_GET_RESET_REASON    GetResetReason;
  EFI_SET_RESET_REASON    SetResetReason;
  
  // NOTE: There are maybe more Functions
};

extern EFI_GUID gXResetReasonProtocolGuid;

#endif /* _EFI_X_RESET_REASON_H_ */
