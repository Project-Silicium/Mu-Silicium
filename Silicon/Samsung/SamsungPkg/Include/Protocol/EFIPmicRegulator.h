#ifndef _EFI_PMIC_REGULATOR_H_
#define _EFI_PMIC_REGULATOR_H_

#include <Device/Pmic.h>

/**
  This Function Enables/Disables The Specified BUCK Regulator.

  @param[in] Id                            - The PMIC ID.
  @param[in] BuckNumber                    - The BUCK Regulator Number.
  @param[in] Enable                        - The Enable Toggle.

  @return EFI_SUCCESS                      - The Specified BUCK Regulator was Successfully Enabled/Disabled.
  @return EFI_UNSUPPORTED                  - The Specified PMIC does not Support this Operation.
  @return EFI_NOT_READY                    - The Specified PMIC hasn't been Init.
  @return EFI_NOT_FOUND                    - The Specified PMIC isn't Supported.
  @return EFI_NOT_FOUND                    - The Specified BUCK Regulator does not Exist.
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
  @param[in] LdoNumber                     - The LDO Regulator Number.
  @param[in] Mode                          - The LDO Regulator Mode.
  @param[in] Enable                        - The Enable Toggle.

  @return EFI_SUCCESS                      - The Specified LDO Regulator was Successfully Enabled/Disabled.
  @return EFI_UNSUPPORTED                  - The Specified PMIC does not Support this Operation.
  @return EFI_INVALID_PARAMETER            - The "Mode" Parameter is Invalid for the Specified PMIC.
  @return EFI_NOT_READY                    - The Specified PMIC hasn't been Init.
  @return EFI_NOT_FOUND                    - The Specified PMIC isn't Supported.
  @return EFI_NOT_FOUND                    - The Specified LDO Regulator does not Exist.
  ...
**/
typedef
EFI_STATUS
(EFIAPI *EFI_PMIC_SET_LDO) (
  IN EFI_PMIC_ID       Id,
  IN UINT8             LdoNumber,
  IN EFI_PMIC_LDO_MODE Mode,
  IN BOOLEAN           Enable
  );

/**
  This Function sets the Output Voltage of the Specified LDO Regulator.

  The Regulator keeps whatever Enable State it already had, so this may be
  Called either before or after Enabling it.

  @param[in] Id                            - The PMIC ID.
  @param[in] LdoNumber                     - The LDO Regulator Number.
  @param[in] Microvolts                    - The Wanted Output Voltage.

  @return EFI_SUCCESS                      - The Voltage was Successfully Set.
  @return EFI_UNSUPPORTED                  - The Specified PMIC does not Support this Operation.
  @return EFI_INVALID_PARAMETER            - The Voltage is Outside what the Regulator can Produce,
                                             or is not a Whole Number of Steps.
  @return EFI_NOT_READY                    - The Specified PMIC hasn't been Init.
  @return EFI_NOT_FOUND                    - The Specified PMIC or LDO Regulator does not Exist.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_PMIC_SET_LDO_VOLTAGE) (
  IN EFI_PMIC_ID Id,
  IN UINT8       LdoNumber,
  IN UINT32      Microvolts
  );

/**
  This Function reads the Output Voltage the Specified LDO Regulator is Set to.

  @param[in]  Id                           - The PMIC ID.
  @param[in]  LdoNumber                    - The LDO Regulator Number.
  @param[out] Microvolts                   - The Voltage the Regulator is Set to.

  @return EFI_SUCCESS                      - The Voltage was Successfully Read.
  @return EFI_UNSUPPORTED                  - The Specified PMIC does not Support this Operation.
  @return EFI_NOT_READY                    - The Specified PMIC hasn't been Init.
  @return EFI_NOT_FOUND                    - The Specified PMIC or LDO Regulator does not Exist.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_PMIC_GET_LDO_VOLTAGE) (
  IN  EFI_PMIC_ID Id,
  IN  UINT8       LdoNumber,
  OUT UINT32     *Microvolts
  );

//
// Define Protocol
//
typedef struct {
  EFI_PMIC_SET_BUCK SetBuck;
  EFI_PMIC_SET_LDO         SetLdo;
  EFI_PMIC_SET_LDO_VOLTAGE SetLdoVoltage;
  EFI_PMIC_GET_LDO_VOLTAGE GetLdoVoltage;
} EFI_PMIC_REGULATOR_PROTOCOL;

#endif /* _EFI_PMIC_REGULATOR_H_ */
