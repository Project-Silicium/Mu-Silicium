#ifndef _EFI_BOARD_INFO_H_
#define _EFI_BOARD_INFO_H_

//
// Global GUID for the Board Info Protocol
//
#define EFI_BOARD_INFO_PROTOCOL_GUID { 0x0C60E64E, 0x6587, 0x4B3A, { 0xBD, 0x51, 0x11, 0x47, 0xF2, 0xCC, 0x9C, 0x0B } }

//
// Declare forward reference to the Board Info Protocol
//
typedef struct _EFI_BOARD_INFO_PROTOCOL EFI_BOARD_INFO_PROTOCOL;

/**
  This Function gets the Hardware Revision.

  @param[out] Revision                     - The Hardware Revision.

  @return EFI_SUCCESS                      - Successfully got the Hardware Revision.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_GET_HARDWARE_REVISION) (
  OUT UINT32 *Revision
  );

//
// Define Protocol Functions
//
struct _EFI_BOARD_INFO_PROTOCOL {
  UINT32                    Revision;
  EFI_GET_HARDWARE_REVISION GetHardwareRevision;
};

extern EFI_GUID gEfiBoardInfoProtocolGuid;

#endif /* _EFI_BOARD_INFO_H_ */
