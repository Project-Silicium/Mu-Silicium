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

#ifndef _EFI_EXYNOS_GPA_H_
#define _EFI_EXYNOS_GPA_H_

#define GPA_PER_BANK    8

#define GPA_PULL_NONE   0
#define GPA_PULL_DOWN   1
#define GPA_PULL_UP     3

#define GPA_DRV_FAST    0
#define GPA_DRV_SLOW    1

//
// Global GUID for the Exynos Gpa Protocol
//
#define EFI_EXYNOS_GPA_PROTOCOL_GUID { 0x1C592EB4, 0x22F9, 0x4174, { 0x81, 0xB7, 0x66, 0x11, 0xB6, 0xCA, 0x4F, 0xC9 } }

//
// Declare forward Reference to the Exynos Gpa Protocol
//
typedef struct _EFI_EXYNOS_GPA_PROTOCOL EFI_EXYNOS_GPA_PROTOCOL;

typedef struct exynos_gpa_bank {
  UINT32 Con;
  UINT32 Dat;
  UINT32 Pull;
  UINT32 Drv;
  UINT32 PdnCon;
  UINT32 PdnPull;
  UINT8  Reserved[8];
} ExynosGpaBank;

/**
  This Function Configure the Defined Pin.

  @param Bank               - The Exynos Gpa Bank.
  @param Offset             - The Bank Offset.
  @param Pin                - The Gpa Pin.
  @param Config             - The Configuration.
**/
typedef
VOID
(EFIAPI *EFI_CONFIGURE_PIN) (
  ExynosGpaBank *Bank,
  UINT32         Offset,
  INT32          Pin,
  INT32          Config
  );

/**
  This Function Sets the Direction to Output on the defined Pin.

  @param Bank               - The Exynos Gpa Bank.
  @param Offset             - The Bank Offset.
  @param Pin                - The Gpa Pin.
  @param Enable             - Enable Output?
**/
typedef
VOID
(EFIAPI *EFI_SET_DIRECTION_OUTPUT) (
  ExynosGpaBank *Bank,
  UINT32         Offset,
  INT32          Pin,
  BOOLEAN        Enable
  );

/**
  This Function Sets the Direction to Input on the defined Pin.

  @param Bank               - The Exynos Gpa Bank.
  @param Offset             - The Bank Offset.
  @param Pin                - The Gpa Pin.
**/
typedef
VOID
(EFIAPI *EFI_SET_DIRECTION_INPUT) (
  ExynosGpaBank *Bank,
  UINT32         Offset,
  INT32          Pin
  );

/**
  This Function Gets the Current State of the defined Pin.

  @param Bank               - The Exynos Gpa Bank.
  @param Offset             - The Bank Offset.
  @param Pin                - The Gpa Pin.

  @return State             - The State of the Pin.
**/
typedef
UINT32
(EFIAPI *EFI_GET_GPA) (
  ExynosGpaBank *Bank,
  UINT32         Offset,
  INT32          Pin
  );

/**
  This Function Sets the Pull Mode to the defined Pin.

  @param Bank               - The Exynos Gpa Bank.
  @param Offset             - The Bank Offset.
  @param Pin                - The Gpa Pin.
  @param Mode               - The Mode.
**/
typedef
VOID
(EFIAPI *EFI_SET_PULL) (
  ExynosGpaBank *Bank,
  UINT32         Offset,
  INT32          Pin,
  INT32          Mode
  );

/**
  This Function Sets the Drv Mode to the defined Pin.

  @param Bank               - The Exynos Gpa Bank.
  @param Offset             - The Bank Offset.
  @param Pin                - The Gpa Pin.
  @param Mode               - The Mode.
**/
typedef
VOID
(EFIAPI *EFI_SET_DRV) (
  ExynosGpaBank *Bank,
  UINT32         Offset,
  INT32          Pin,
  INT32          Mode
  );

/**
  This Function Sets the Speed of the defined Pin.

  @param Bank               - The Exynos Gpa Bank.
  @param Offset             - The Bank Offset.
  @param Pin                - The Gpa Pin.
  @param Mode               - The Mode.
**/
typedef
VOID
(EFIAPI *EFI_SET_RATE) (
  ExynosGpaBank *Bank,
  UINT32         Offset,
  INT32          Pin,
  INT32          Mode
  );

//
// Define Protocol Functions
//
struct _EFI_EXYNOS_GPA_PROTOCOL {
  EFI_CONFIGURE_PIN        ConfigurePin;
  EFI_SET_DIRECTION_OUTPUT SetDirectionOutput;
  EFI_SET_DIRECTION_INPUT  SetDirectionInput;
  EFI_GET_GPA              GetGpa;
  EFI_SET_PULL             SetPull;
  EFI_SET_DRV              SetDrv;
  EFI_SET_RATE             SetRate;
};

extern EFI_GUID gEfiExynosGpaProtocolGuid;

#endif /* _EFI_EXYNOS_GPA_H_ */
