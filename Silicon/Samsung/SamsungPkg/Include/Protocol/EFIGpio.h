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

#include <GpioBankId.h>

//
// GPIO Functions
//
typedef enum {
  FUNCTION_INPUT,
  FUNCTION_OUTPUT,
  FUNCTION_2,

  FUNCTION_NUM
} EFI_GPIO_FUNCTION;

//
// GPIO Pull Modes
//
typedef enum {
  PULL_NONE,
  PULL_DOWN,
  PULL_UP,

  PULL_NUM
} EFI_GPIO_PULL_MODE;

/**
  This Function gets the current State of the Specified GPIO Pin.

  @param[in]  BankId                       - The GPIO Bank ID of the GPIO Pin.
  @param[in]  BankNumber                   - The GPIO Bank Number of the GPIO Pin.
  @param[in]  Pin                          - The GPIO Pin.
  @param[out] State                        - The current State.

  @return EFI_SUCCESS                      - Successfully got the current State of the Specified GPIO Pin.
  @return EFI_INVALID_PARAMETER            - The Pin Parameter is larger than 7.
  @return EFI_NOT_FOUND                    - The Specified GPIO Bank does not Exist.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_GET_GPIO_PIN) (
  IN  EFI_GPIO_BANK_ID  BankId,
  IN  UINT8             BankNumber,
  IN  UINT8             Pin,
  OUT BOOLEAN          *State
  );

/**
  This Function Enables/Disables the Specified GPIO Pin.

  @param[in] BankId                        - The GPIO Bank ID of the GPIO Pin.
  @param[in] BankNumber                    - The GPIO Bank Number of the GPIO Pin.
  @param[in] Pin                           - The GPIO Pin.
  @param[in] Enable                        - The new State.

  @return EFI_SUCCESS                      - Successfully Enabled/Disabled the Specified GPIO Pin.
  @return EFI_INVALID_PARAMETER            - The Pin Parameter is larger than 7.
  @return EFI_NOT_FOUND                    - The Specified GPIO Bank does not Exist.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_SET_GPIO_PIN)(
  IN EFI_GPIO_BANK_ID  BankId,
  IN UINT8             BankNumber,
  IN UINT8             Pin,
  IN BOOLEAN          *Enable
  );

/**
  This Function sets the Specified Function of the Specified GPIO Pin.

  @param[in] BankId                        - The GPIO Bank ID of the GPIO Pin.
  @param[in] BankNumber                    - The GPIO Bank Number of the GPIO Pin.
  @param[in] Pin                           - The GPIO Pin.
  @param[in] Function                      - The Function.

  @return EFI_SUCCESS                      - Successfully set the Function of the Specified GPIO Pin.
  @return EFI_INVALID_PARAMETER            - The Pin Parameter is larger than 7 or the Function is Unknown.
  @return EFI_NOT_FOUND                    - The Specified GPIO Bank does not Exist.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_SET_GPIO_PIN_FUNCTION) (
  IN EFI_GPIO_BANK_ID  BankId,
  IN UINT8             BankNumber,
  IN UINT8             Pin,
  IN EFI_GPIO_FUNCTION Function
  );

/**
  This Function sets the Specified Pull Mode of the Specified GPIO Pin.

  @param[in] BankId                        - The GPIO Bank ID of the GPIO Pin.
  @param[in] BankNumber                    - The GPIO Bank Number of the GPIO Pin.
  @param[in] Pin                           - The GPIO Pin.
  @param[in] Pull                          - The new Pull Mode.

  @return EFI_SUCCESS                      - Successfully set the new Direction of the Specified GPIO Pin.
  @return EFI_INVALID_PARAMETER            - The Pin Parameter is larger than 7 or the Pull Mode is Unknown.
  @return EFI_NOT_FOUND                    - The Specified GPIO Bank does not Exist.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_SET_GPIO_PIN_PULL) (
  IN EFI_GPIO_BANK_ID   BankId,
  IN UINT8              BankNumber,
  IN UINT8              Pin,
  IN EFI_GPIO_PULL_MODE Pull
  );

//
// Define Protocol
//
typedef struct {
  EFI_GET_GPIO_PIN          GetPin;
  EFI_SET_GPIO_PIN          SetPin;
  EFI_SET_GPIO_PIN_FUNCTION SetPinFunction;
  EFI_SET_GPIO_PIN_PULL     SetPinPull;
} EFI_GPIO_PROTOCOL;

#endif /* _EFI_GPIO_H_ */
