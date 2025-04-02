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

// GPA Pull Modes
#define GPA_PULL_NONE   0
#define GPA_PULL_DOWN   1
#define GPA_PULL_UP     3

// GPA DRV Speed Values
#define GPA_DRV_FAST    0
#define GPA_DRV_SLOW    1

// GPA Directions
#define GPA_INPUT       0
#define GPA_OUTPUT      1

//
// Global GUID for the GPIO Protocol
//
#define EFI_GPIO_PROTOCOL_GUID { 0x1C592EB4, 0x22F9, 0x4174, { 0x81, 0xB7, 0x66, 0x11, 0xB6, 0xCA, 0x4F, 0xC9 } }

//
// Declare forward Reference to the GPIO Protocol
//
typedef struct _EFI_GPIO_PROTOCOL EFI_GPIO_PROTOCOL;

//
// GPIO Bank Structure
//
typedef struct gpio_bank {
  UINT32 Con;
  UINT32 Dat;
  UINT32 Pull;
  UINT32 Drv;
  UINT32 PdnCon;
  UINT32 PdnPull;
  UINT8  Reserved[8];
} GpioBank;

/**
  This Function Configure the Defined Pin.

  @param[in] Bank                          - The Exynos Gpa Bank.
  @param[in] Offset                        - The Bank Offset.
  @param[in] Pin                           - The Gpa Pin.
  @param[in] Config                        - The Configuration.

  @return EFI_SUCCESS                      - Successfully Configured defined Pin.
  @return EFI_NOT_FOUND                    - The GPIO Base Address is not Mapped.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_CONFIGURE_PIN) (
  IN GpioBank *Bank,
  IN UINT32    Offset,
  IN INT32     Pin,
  IN INT32     Config
  );

/**
  This Function Sets the Direction to Output on the defined Pin.

  @param[in] Bank                          - The Exynos Gpa Bank.
  @param[in] Offset                        - The Bank Offset.
  @param[in] Pin                           - The Gpa Pin.
  @param[in] Enable                        - Enable Output

  @return EFI_SUCCESS                      - Successfully set Pin Out Direction.
  @return EFI_NOT_FOUND                    - The GPIO Base Address is not Mapped.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_SET_DIRECTION_OUTPUT) (
  IN GpioBank *Bank,
  IN UINT32    Offset,
  IN INT32     Pin,
  IN BOOLEAN   Enable
  );

/**
  This Function Sets the Direction to Input on the defined Pin.

  @param[in] Bank                          - The Exynos Gpa Bank.
  @param[in] Offset                        - The Bank Offset.
  @param[in] Pin                           - The Gpa Pin.

  @return EFI_SUCCESS                      - Successfully set Pin In Direction.
  @return EFI_NOT_FOUND                    - The GPIO Base Address is not Mapped.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_SET_DIRECTION_INPUT) (
  IN GpioBank *Bank,
  IN UINT32    Offset,
  IN INT32     Pin
  );

/**
  This Function Gets the Current State of the defined Pin.

  @param[in]  Bank                         - The Exynos Gpa Bank.
  @param[in]  Offset                       - The Bank Offset.
  @param[in]  Pin                          - The Gpa Pin.
  @param[out] State                        - The State of the Pin.

  @return EFI_SUCCESS                      - Successfully got the Pin State.
  @return EFI_NOT_FOUND                    - The GPIO Base Address is not Mapped.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_GET_PIN) (
  IN  GpioBank *Bank,
  IN  UINT32    Offset,
  IN  INT32     Pin,
  OUT UINT32   *State
  );

/**
  This Function Sets the Pull Mode to the defined Pin.

  @param[in] Bank                          - The Exynos Gpa Bank.
  @param[in] Offset                        - The Bank Offset.
  @param[in] Pin                           - The Gpa Pin.
  @param[in] Mode                          - The Mode.

  @return EFI_SUCCESS                      - Successfully Set defined Pull Mode.
  @return EFI_INVALID_PARAMETER            - The Pull Mode Parameter is Invalid.
  @return EFI_NOT_FOUND                    - The GPIO Base Address is not Mapped.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_SET_PULL) (
  IN GpioBank *Bank,
  IN UINT32    Offset,
  IN INT32     Pin,
  IN INT32     Mode
  );

/**
  This Function Sets the DRV Mode to the defined Pin.

  @param[in] Bank                          - The Exynos Gpa Bank.
  @param[in] Offset                        - The Bank Offset.
  @param[in] Pin                           - The Gpa Pin.
  @param[in] Mode                          - The Mode.

  @return EFI_SUCCESS                      - Successfully set DRV Mode.
  @return EFI_NOT_FOUND                    - The GPIO Base Address is not Mapped.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_SET_DRV) (
  IN GpioBank *Bank,
  IN UINT32    Offset,
  IN INT32     Pin,
  IN INT32     Mode
  );

/**
  This Function Sets the Speed of the defined Pin.

  @param[in] Bank                          - The Exynos Gpa Bank.
  @param[in] Offset                        - The Bank Offset.
  @param[in] Pin                           - The Gpa Pin.
  @param[in] Mode                          - The Mode.

  @return EFI_SUCCESS                      - Successfully Set defined Speed Mode.
  @return EFI_INVALID_PARAMETER            - The Speed Mode Parameter is Invalid.
  @return EFI_NOT_FOUND                    - The GPIO Base Address is not Mapped.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_SET_RATE) (
  IN GpioBank *Bank,
  IN UINT32    Offset,
  IN INT32     Pin,
  IN INT32     Mode
  );

//
// Define Protocol Functions
//
struct _EFI_GPIO_PROTOCOL {
  EFI_CONFIGURE_PIN        ConfigurePin;
  EFI_SET_DIRECTION_OUTPUT SetDirectionOutput;
  EFI_SET_DIRECTION_INPUT  SetDirectionInput;
  EFI_GET_PIN              GetPin;
  EFI_SET_PULL             SetPull;
  EFI_SET_DRV              SetDrv;
  EFI_SET_RATE             SetRate;
};

extern EFI_GUID gEfiGpioProtocolGuid;

#endif /* _EFI_GPIO_H_ */
