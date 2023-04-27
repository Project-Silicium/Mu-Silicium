/** @file
  Silicon Policy C Struct Header File for GFX.

  Copyright (c) Microsoft Corporation.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef POLICY_DATA_STRUCT_GFX_H_
#define POLICY_DATA_STRUCT_GFX_H_

#pragma pack(1)

#define   GFX_PORT_MAX_CNT  2
#define   GFX_POLICY_SIZE   (sizeof (GFX_POLICY_DATA) * GFX_PORT_MAX_CNT)

typedef struct {
  /* Power state of GFX port */
  UINT8    Power_State_Port;
} GFX_POLICY_DATA;

#pragma pack()

#endif
