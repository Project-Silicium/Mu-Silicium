/**
  Copyright (C) Samsung Electronics Co. LTD

  This software is proprietary of Samsung Electronics.
  No part of this software, either material or conceptual may be copied or distributed, transmitted,
  transcribed, stored in a retrieval system or translated into any human or computer language in any form by any means,
  electronic, mechanical, manual or otherwise, or disclosed
  to third parties without the express written permission of Samsung Electronics.

  Alternatively, this program is free software in case of open source project
  you can redistribute it and/or modify
  it under the terms of the GNU General Public License version 2 as
  published by the Free Software Foundation.
**/

#ifndef _EFI_GPIO_H_
#define _EFI_GPIO_H_

#include <Library/GpioBankLib.h>

// GPIO Pull Modes
#define GPIO_PULL_NONE 0
#define GPIO_PULL_DOWN 1
#define GPIO_PULL_UP   3

// GPIO DRV Speed Values
#define GPIO_DRV_FAST 0
#define GPIO_DRV_SLOW 1

// GPIO Directions
#define GPIO_INPUT  0
#define GPIO_OUTPUT 1

//
// Global GUID for the Exynos GPIO Protocol
//
#define EFI_EXYNOS_GPIO_PROTOCOL_GUID { 0x1C592EB4, 0x22F9, 0x4174, { 0x81, 0xB7, 0x66, 0x11, 0xB6, 0xCA, 0x4F, 0xC9 } }

//
// Declare forward Reference to the Exynos GPIO Protocol
//
typedef struct _EFI_EXYNOS_GPIO_PROTOCOL EFI_EXYNOS_GPIO_PROTOCOL;

/**
  This Function Configures the Defined GPIO Pin.

  @param[in] BankNumber                    - The Bank Number.
  @param[in] BankId                        - The Bank ID.
  @param[in] Pin                           - The GPIO Pin.
  @param[in] Config                        - The new Configuration.

  @return EFI_SUCCESS                      - Successfully Configured defined Pin.
  @return EFI_NOT_FOUND                    - The Specified GPIO Bank does not Exist.
  @return EFI_NO_MAPPING                   - The Specified Pinctrl isn't Mapped in Memory.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_CONFIGURE_PIN) (
  IN UINT8 BankNumber,
  IN UINT8 BankId,
  IN UINT8 Pin,
  IN UINT8 Config
  );

/**
  This Function Sets the Direction to Output of the defined GPIO Pin.

  @param[in] BankNumber                    - The Bank Number.
  @param[in] BankId                        - The Bank ID.
  @param[in] Pin                           - The GPIO Pin.
  @param[in] Enable                        - Enable/Disable GPIO Pin.

  @return EFI_SUCCESS                      - Successfully Configured defined Pin.
  @return EFI_NOT_FOUND                    - The Specified GPIO Bank does not Exist.
  @return EFI_NO_MAPPING                   - The Specified Pinctrl isn't Mapped in Memory.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_SET_DIRECTION_OUTPUT) (
  IN UINT8   BankNumber,
  IN UINT8   BankId,
  IN UINT8   Pin,
  IN BOOLEAN Enable
  );

/**
  This Function Sets the Direction to Input of the defined GPIO Pin.

  @param[in] BankNumber                    - The Bank Number.
  @param[in] BankId                        - The Bank ID.
  @param[in] Pin                           - The GPIO Pin.

  @return EFI_SUCCESS                      - Successfully Configured defined Pin.
  @return EFI_NOT_FOUND                    - The Specified GPIO Bank does not Exist.
  @return EFI_NO_MAPPING                   - The Specified Pinctrl isn't Mapped in Memory.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_SET_DIRECTION_INPUT) (
  IN UINT8 BankNumber,
  IN UINT8 BankId,
  IN UINT8 Pin
  );

/**
  This Function Gets the Current State of the defined GPIO Pin.

  @param[in]  BankNumber                   - The Bank Number.
  @param[in]  BankId                       - The Bank ID.
  @param[in]  Pin                          - The GPIO Pin.
  @param[out] State                        - The current State of the GPIO Pin.

  @return EFI_SUCCESS                      - Successfully Configured defined Pin.
  @return EFI_NOT_FOUND                    - The Specified GPIO Bank does not Exist.
  @return EFI_NO_MAPPING                   - The Specified Pinctrl isn't Mapped in Memory.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_GET_PIN) (
  IN  UINT8  BankNumber,
  IN  UINT8  BankId,
  IN  UINT8  Pin,
  OUT UINT8 *State
  );

/**
  This Function Sets the Pull of the defined GPIO Pin.

  @param[in] BankNumber                    - The Bank Number.
  @param[in] BankId                        - The Bank ID.
  @param[in] Pin                           - The GPIO Pin.
  @param[in] Mode                          - The new Mode.

  @return EFI_SUCCESS                      - Successfully Configured defined Pin.
  @return EFI_NOT_FOUND                    - The Specified GPIO Bank does not Exist.
  @return EFI_NO_MAPPING                   - The Specified Pinctrl isn't Mapped in Memory.
  @return EFI_INVALID_PARAMETER            - The Specified Mode is Invalid.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_SET_PULL) (
  IN UINT8 BankNumber,
  IN UINT8 BankId,
  IN UINT8 Pin,
  IN UINT8 Mode
  );

/**
  This Function Sets the DRV of the defined GPIO Pin.

  @param[in] BankNumber                    - The Bank Number.
  @param[in] BankId                        - The Bank ID.
  @param[in] Pin                           - The GPIO Pin.
  @param[in] Mode                          - The new Mode.

  @return EFI_SUCCESS                      - Successfully Configured defined Pin.
  @return EFI_NOT_FOUND                    - The Specified GPIO Bank does not Exist.
  @return EFI_NO_MAPPING                   - The Specified Pinctrl isn't Mapped in Memory.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_SET_DRV) (
  IN UINT8 BankNumber,
  IN UINT8 BankId,
  IN UINT8 Pin,
  IN UINT8 Mode
  );

/**
  This Function Sets the Rate of the defined GPIO Pin.

  @param[in] BankNumber                    - The Bank Number.
  @param[in] BankId                        - The Bank ID.
  @param[in] Pin                           - The GPIO Pin.
  @param[in] Mode                          - The new Mode.

  @return EFI_SUCCESS                      - Successfully Configured defined Pin.
  @return EFI_NOT_FOUND                    - The Specified GPIO Bank does not Exist.
  @return EFI_NO_MAPPING                   - The Specified Pinctrl isn't Mapped in Memory.
  @return EFI_INVALID_PARAMETER            - The Specified Mode is Invalid.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_SET_RATE) (
  IN UINT8 BankNumber,
  IN UINT8 BankId,
  IN UINT8 Pin,
  IN UINT8 Mode
  );

//
// Define Protocol Functions
//
struct _EFI_EXYNOS_GPIO_PROTOCOL {
  EFI_CONFIGURE_PIN        ConfigurePin;
  EFI_SET_DIRECTION_OUTPUT SetDirectionOutput;
  EFI_SET_DIRECTION_INPUT  SetDirectionInput;
  EFI_GET_PIN              GetPin;
  EFI_SET_PULL             SetPull;
  EFI_SET_DRV              SetDrv;
  EFI_SET_RATE             SetRate;
};

extern EFI_GUID gEfiExynosGpioProtocolGuid;

#endif /* _EFI_GPIO_H_ */
