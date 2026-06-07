#ifndef _EFI_CHIP_INFO_H_
#define _EFI_CHIP_INFO_H_

/**
  This Function Returns the Chip ID.

  @param[out] Id                           - The Chip ID.

  @return EFI_SUCCESS                      - Successfully got the Chip ID.
  @return EFI_INVALID_PARAMETER            - The Id Parameter is NULL.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_GET_CHIP_ID) (
  OUT UINT64 *Id
  );

/**
  This Function Returns the Major & Minor Chip Revision.

  @param[out] MajorRev                     - The Major Chip Revision.
  @param[out] MinorRev                     - The Minor Chip Revision.

  @return EFI_SUCCESS                      - Successfully got both Chip Revisions.
  @return EFI_INVALID_PARAMETER            - One or more Parameters are NULL.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_GET_CHIP_REVISION) (
  OUT UINT8 *MajorRev,
  OUT UINT8 *MinorRev
  );

//
// Define Protocol
//
typedef struct {
  EFI_GET_CHIP_ID       GetId;
  EFI_GET_CHIP_REVISION GetRevision;
} EFI_CHIP_INFO_PROTOCOL;

#endif /* _EFI_CHIP_INFO_H_ */
