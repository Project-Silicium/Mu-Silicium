#ifndef _EFI_PMIC_EXTRA_H_
#define _EFI_PMIC_EXTRA_H_

#include <Device/Pmic.h>

/**
  This Function Enables/Disables WTSR of the Specified PMIC.

  @param[in] Id                            - The PMIC ID.
  @param[in] Enable                        - The Enable Toggle.

  @return EFI_SUCCESS                      - WTSR was Successfully Enabled/Disabled.
  @return EFI_UNSUPPORTED                  - The Specified PMIC does not Support this Operation.
  @return EFI_NOT_READY                    - The Specified PMIC hasn't been Init.
  @return EFI_NOT_FOUND                    - The Specified PMIC isn't Supported.
  ...
**/
typedef
EFI_STATUS
(EFIAPI *EFI_PMIC_SET_WTSR) (
  IN EFI_PMIC_ID Id,
  IN BOOLEAN     Enable
  );

/**
  This Function Enables/Disables SMPL of the Specified PMIC.

  @param[in] Id                            - The PMIC ID.
  @param[in] Enable                        - The Enable Toggle.

  @return EFI_SUCCESS                      - SMPL was Successfully Enabled/Disabled.
  @return EFI_UNSUPPORTED                  - The Specified PMIC does not Support this Operation.
  @return EFI_NOT_READY                    - The Specified PMIC hasn't been Init.
  @return EFI_NOT_FOUND                    - The Specified PMIC isn't Supported.
  ...
**/
typedef
EFI_STATUS
(EFIAPI *EFI_PMIC_SET_SMPL) (
  IN EFI_PMIC_ID Id,
  IN BOOLEAN     Enable
  );

/**
  This Function Powers Down the Specified PMIC.

  @return EFI_SUCCESS                      - SMPL was Successfully Enabled/Disabled.
  @return EFI_UNSUPPORTED                  - The Specified PMIC does not Support this Operation.
  @return EFI_NOT_READY                    - The Specified PMIC hasn't been Init.
  @return EFI_NOT_FOUND                    - The Specified PMIC isn't Supported.
  ...
**/
typedef
EFI_STATUS
(EFIAPI *EFI_PMIC_POWER_DOWN) (
  IN EFI_PMIC_ID Id
  );

//
// Define Protocol
//
typedef struct {
  EFI_PMIC_SET_WTSR   SetWtsr;
  EFI_PMIC_SET_SMPL   SetSmpl;
  EFI_PMIC_POWER_DOWN PowerDown;
} EFI_PMIC_EXTRA_PROTOCOL;

#endif /* _EFI_PMIC_EXTRA_H_ */
