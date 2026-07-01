#ifndef _MTK_PMIC_H_
#define _MTK_PMIC_H_

//
// Declare forward Reference to the PMIC Protocol
//
typedef struct _MTK_PMIC_PROTOCOL MTK_PMIC_PROTOCOL;

/**
  This Function Checks if button is pressed.

  @param[out] Pressed                      - The Pressed.
**/
typedef
VOID
(EFIAPI *MTK_PMIC_BUTTON_PRESSED) (
  OUT BOOLEAN *Pressed
  );

/**
  This Function enables or disables regulator.

  @param[in] Name                          - Name of the regulator.
  @param[in] Enable                        - TRUE to enable regulator, FALSE to disable it.
**/
typedef
EFI_STATUS
(EFIAPI *MTK_PMIC_REG_SET_ENABLE) (
  IN CONST CHAR8 *Name,
  IN BOOLEAN Enable
  );

/**
  This Function retrieves state of regulator.

  @param[in]  Name                         - Name of the regulator.
  @param[out] Enabled                      - TRUE if regulator enabled, FALSE if disabled.
**/
typedef
EFI_STATUS
(EFIAPI *MTK_PMIC_REG_IS_ENABLED) (
  IN  CONST CHAR8 *Name,
  OUT BOOLEAN *Enabled
  );

/**
  This Function sets regulator voltage.

  @param[in] Name                          - Name of the regulator.
  @param[in] Voltage                       - Desired regulator voltage in uV.
**/
typedef
EFI_STATUS
(EFIAPI *MTK_PMIC_REG_SET_VOLTAGE) (
  IN CONST CHAR8 *Name,
  IN UINT32 Voltage
  );

/**
  This Function retrieves voltage of regulator.

  @param[in]  Name                          - Name of the regulator.
  @param[out] Voltage                       - Current regulator voltage in uV.
**/
typedef
EFI_STATUS
(EFIAPI *MTK_PMIC_REG_GET_VOLTAGE) (
  IN  CONST CHAR8 *Name,
  OUT UINT32 *Voltage
  );

//
// Define Protocol Functions
//
struct _MTK_PMIC_PROTOCOL {
  MTK_PMIC_BUTTON_PRESSED PowerButtonPressed;
  MTK_PMIC_BUTTON_PRESSED HomeButtonPressed;
  MTK_PMIC_REG_SET_ENABLE RegulatorSetEnable;
  MTK_PMIC_REG_IS_ENABLED RegulatorIsEnabled;
  MTK_PMIC_REG_SET_VOLTAGE RegulatorSetVoltage;
  MTK_PMIC_REG_GET_VOLTAGE RegulatorGetVoltage;
};

extern EFI_GUID gMediaTekPmicProtocolGuid;

#endif /* _MTK_PMIC_H_ */