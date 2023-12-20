/** @file GpioButtons.h

  This module installs the MsButtonServicesProtocol.

  Copyright (C) Microsoft Corporation. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef _GPIO_BUTTONS_H_
#define _GPIO_BUTTONS_H_

typedef enum {
  NoButtons     = SCAN_NULL,
  VolUpButton   = SCAN_UP,
  VolDownButton = SCAN_DOWN
} BUTTON_STATE;

typedef struct {
  MS_BUTTON_SERVICES_PROTOCOL ButtonServicesProtocol;
  BUTTON_STATE                ButtonState;
} GPIO_BUTTON_SERVICES_PROTOCOL;

GPIO_BUTTON_SERVICES_PROTOCOL *gBsp = NULL;

#define MS_BSP_FROM_BSP(a) BASE_CR(a, GPIO_BUTTON_SERVICES_PROTOCOL, ButtonServicesProtocol)

#endif /* _GPIO_BUTTONS_H_ */