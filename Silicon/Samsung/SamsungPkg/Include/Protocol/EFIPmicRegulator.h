#ifndef _EFI_PMIC_REGULATOR_H_
#define _EFI_PMIC_REGULATOR_H_

/**
  This Function Enables/Disables The Specified BUCK Regulator.

  @param[in] Id                            - The PMIC ID.
  @param[in] BuckNumber                    - The BUCK Regulator Number.
  @param[in] Enable                        - The Enable Toggle.

  @return EFI_SUCCESS                      - The Specified BUCK Regulator was Successfully Enabled/Disabled.
  @return EFI_UNSUPPORTED                  - The Specified PMIC does not Support this Operation.
  @return EFI_INVALID_PARAMETER            - The "BuckNumber" Parameter Exceeds the Max Supported BUCK Regulators.
  @return EFI_NOT_READY                    - The Specified PMIC hasn't been Init.
  @return EFI_NOT_FOUND                    - The Specified PMIC isn't Supported.
  ...
**/
typedef
EFI_STATUS
(EFIAPI *EFI_PMIC_SET_BUCK) (
  IN EFI_PMIC_ID Id,
  IN UINT8       BuckNumber,
  IN BOOLEAN     Enable
  );

/**
  This Function Enables/Disables The Specified LDO Regulator.

  @param[in] Id                            - The PMIC ID.
  @param[in] BuckNumber                    - The LDO Regulator Number.
  @param[in] Mode                          - The LDO Regulator Mode.
  @param[in] Enable                        - The Enable Toggle.

  @return EFI_SUCCESS                      - The Specified LDO Regulator was Successfully Enabled/Disabled.
  @return EFI_UNSUPPORTED                  - The Specified PMIC does not Support this Operation.
  @return EFI_INVALID_PARAMETER            - The "BuckNumber" Parameter Exceeds the Max Supported LDO Regulators.
  @return EFI_NOT_READY                    - The Specified PMIC hasn't been Init.
  @return EFI_NOT_FOUND                    - The Specified PMIC isn't Supported.
  ...
**/
typedef
EFI_STATUS
(EFIAPI *EFI_PMIC_SET_LDO) (
  IN EFI_PMIC_ID Id,
  IN UINT8       LdoNumber,
  IN UINT8       Mode,
  IN BOOLEAN     Enable
  );

//
// Define Protocol
//
typedef struct {
  EFI_PMIC_SET_BUCK SetBuck;
  EFI_PMIC_SET_LDO  SetLdo;
} EFI_PMIC_REGULATOR_PROTOCOL;

#endif /* _EFI_PMIC_REGULATOR_H_ */
