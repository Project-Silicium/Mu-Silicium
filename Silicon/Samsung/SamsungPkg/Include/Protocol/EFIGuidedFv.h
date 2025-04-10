#ifndef _EFI_GUIDED_FV_H_
#define _EFI_GUIDED_FV_H_

//
// Global GUID for the Guided Fv Protocol
//
#define EFI_GUIDED_FV_PROTOCOL_GUID { 0xC8B55839, 0xD79B, 0x49E6, { 0xB3, 0x26, 0xE6, 0x31, 0xA1, 0xDF, 0x86, 0x6F } }

//
// Declare forward reference to the Guided Fv Protocol
//
typedef struct _EFI_GUIDED_FV_PROTOCOL EFI_GUIDED_FV_PROTOCOL;

/**
  This Function Executes the defined UEFI App from the defined FV.

  @param[in] FvGuid                        - The GUID of the FV Partition.
  @param[in] AppName                       - The Name of the UEFI App.
  @param[in] AppArguments                  - The Arguments for the UEFI App.

  @return EFI_SUCCESS                      - Successfully Executed the UEFI App.
  @return EFI_INVALID_PARAMETER            - FvGuid or AppName is NULL.
  @return EFI_OUT_OF_RESOURCES             - Failed to Allocate Memory.
  @return EFI_NOT_FOUND                    - The defined UEFI App or FV does not Exist.
  ...
**/
typedef
EFI_STATUS
(EFIAPI *EFI_LAUNCH_APP_FROM_GUIDED_FV) (
  IN EFI_GUID *FvGuid,
  IN CHAR8    *AppName,
  IN CHAR8    *AppArguments
  );

//
// Define Protocol Functions
//
struct _EFI_GUIDED_FV_PROTOCOL {
  EFI_LAUNCH_APP_FROM_GUIDED_FV  LaunchAppFromGuidedFv;
  VOID                          *LoadFilefromGuidedFv;
};

extern EFI_GUID gEfiGuidedFvProtocolGuid;

#endif /* _EFI_GUIDED_FV_H_ */
