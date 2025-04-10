#ifndef _EFI_CHIP_DATA_H_
#define _EFI_CHIP_DATA_H_

//
// Global GUID for the Chip Data Protocol
//
#define EFI_CHIP_DATA_PROTOCOL_GUID { 0x620A14C6, 0xAE18, 0x4260, { 0xA3, 0x51, 0x66, 0x20, 0x59, 0xC4, 0x88, 0x64 } }

//
// Declare forward reference to the Chip Data Protocol
//
typedef struct _EFI_CHIP_DATA_PROTOCOL EFI_CHIP_DATA_PROTOCOL;

/**
  This Function gets the Chip ID.

  @param[out] Id                           - The Chip ID.
**/
typedef
VOID
(EFIAPI *EFI_GET_ID) (
  OUT UINT64 *Id
  );

/**
  This Function gets the Chip Revision.

  @param[out] Revision                     - The Chip Revision.
**/
typedef
VOID
(EFIAPI *EFI_GET_REVISION) (
  OUT UINT32 Revision[2]
  );

//
// Define Protocol Functions
//
struct _EFI_CHIP_DATA_PROTOCOL {
    EFI_GET_ID       GetId;
    EFI_GET_REVISION GetRevision;
};

extern EFI_GUID gEfiChipDataProtocolGuid;

#endif /* _EFI_CHIP_DATA_H_ */
