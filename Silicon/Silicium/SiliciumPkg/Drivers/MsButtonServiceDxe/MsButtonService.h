/**
  Copyright (C) Microsoft Corporation. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _MS_BUTTON_SERVICE_H_
#define _MS_BUTTON_SERVICE_H_

//
// Button States
//
typedef enum {
  NoButtons     = SCAN_NULL,
  VolUpButton   = SCAN_UP,
  VolDownButton = SCAN_DOWN
} BUTTON_STATE;

//
// Ms Button Service
//
typedef struct {
  MS_BUTTON_SERVICES_PROTOCOL ButtonServicesProtocol;
  BUTTON_STATE                ButtonState;
} MS_BUTTON_SERVICES;

//
// Ms BSP Function
//
#define MS_BSP_FROM_BSP(a) BASE_CR (a, MS_BUTTON_SERVICES, ButtonServicesProtocol)

#endif /* _MS_BUTTON_SERVICE_DXE_H_ */
