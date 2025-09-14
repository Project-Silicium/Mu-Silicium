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

//
// Define Protocol Functions
//
struct _MTK_PMIC_PROTOCOL {
  MTK_PMIC_BUTTON_PRESSED PowerButtonPressed;
  MTK_PMIC_BUTTON_PRESSED HomeButtonPressed;
};

extern EFI_GUID gMediaTekPmicProtocolGuid;

#endif /* _MTK_PMIC_H_ */