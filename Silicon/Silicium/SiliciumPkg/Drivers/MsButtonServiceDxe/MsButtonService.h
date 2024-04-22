/**
  Copyright (C) Microsoft Corporation. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _MS_BUTTON_SERVICE_H_
#define _MS_BUTTON_SERVICE_H_

#if HAS_BUILD_IN_KEYBOARD == 1
typedef enum {
  NoButtons     = SCAN_NULL,
  ESCButton     = SCAN_ESC,
  EntfButton    = SCAN_DELETE
} BUTTON_STATE;
#else
typedef enum {
  NoButtons     = SCAN_NULL,
  VolUpButton   = SCAN_UP,
  VolDownButton = SCAN_DOWN
} BUTTON_STATE;
#endif

typedef struct {
  MS_BUTTON_SERVICES_PROTOCOL ButtonServicesProtocol;
  BUTTON_STATE                ButtonState;
} MS_BUTTON_SERVICES;

MS_BUTTON_SERVICES *gButtonService = NULL;

#define MS_BSP_FROM_BSP(a) BASE_CR(a, MS_BUTTON_SERVICES, ButtonServicesProtocol)

#endif /* _MS_BUTTON_SERVICE_DXE_H_ */
