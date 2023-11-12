/** @file
DfciMenu.h

Header file for DfciMenu

Copyright (C) Microsoft Corporation. All rights reserved.
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef __DFCI_MENU_H__
#define __DFCI_MENU_H__

#include <Guid/DfciMenuGuid.h>   // defines DFCI_* items
// The following defines are defined in Guid/MdeModuleHii.h, but this header
// doesn't play well with the VFR compiler.
#define EFI_OTHER_DEVICE_CLASS            0x20
#define EFI_GENERAL_APPLICATION_SUBCLASS  0x01

// The following defines are from DfciMenuGuid.h.  Keep these in mind when altering
// the values used in this formset.
//
// #define DFCI_MENU_FORM_ID       0x2000
// #define DFCI_MENU_FORMSET_GUID  0x3b82283d, 0x7add, 0x4c6a, {0xad, 0x2b, 0x71, 0x9b, 0x8d, 0x7b, 0x77, 0xc9}

#define DFCI_MENU_VARID                        0x2100
#define DFCI_MENU_CONFIGURE_FORM_ID            0x2200
#define DFCI_MENU_RECOVERY_INFO_FORM_ID        0x2300
#define DFCI_MENU_INIT_QUESTION_ID             0x2400
#define DFCI_MENU_INIT2_QUESTION_ID            0x2500
#define DFCI_MENU_INIT3_QUESTION_ID            0x2600
#define DFCI_MENU_HTTP_UPDATE_NOW_QUESTION_ID  0x2700
#define DFCI_MENU_USB_UPDATE_NOW_QUESTION_ID   0x2800
#define DFCI_MENU_RECOVERY_INFO_QUESTION_ID    0x2900
#define DFCI_MENU_RECOVERY_NOW_QUESTION_ID     0x2A00
#define DFCI_MENU_ZUM_OPT_IN_QUESTION_ID       0x2B00
#define DFCI_MENU_ZUM_OPT_OUT_QUESTION_ID      0x2C00
#define DFCI_MENU_CONFIGURE_QUESTION_ID        0x2D00
#define DFCI_MENU_USB_INSTALL_NOW_QUESTION_ID  0x2E00

// These are the VFR compiler generated data representing our VFR data.
//
extern UINT8  DfciMenuVfrBin[];

// Values for Dfci Menu Configuration
#define MENU_TRUE   0x01
#define MENU_FALSE  0x00

typedef struct {
  UINT8    DfciRecoveryEnabled;
  UINT8    DfciHttpRecoveryEnabled;
  UINT8    DfciZeroTouchEnabled;
  UINT8    DfciZeroTouchCertAvailable;
  UINT8    DfciZeroTouchOptGrayOut;
  UINT8    DfciOwnerEnabled;
  UINT8    DfciUserEnabled;
  UINT8    DfciUser1Enabled;
  UINT8    DfciUser2Enabled;
  UINT8    DfciFriendlyName;
  UINT8    DfciTenantName;
  UINT8    DfciOptInChanged;
} DFCI_MENU_CONFIGURATION;

// Grid class form browser extensions.  These should be ignored
// by any browser not implementing the Grid Class.
//
//
#define GRID_CLASS_START_OPCODE_GUID                                             \
  {                                                                                \
    0xc0b6e247, 0xe140, 0x4b4d, { 0xa6, 0x4, 0xc3, 0xae, 0x1f, 0xa6, 0xcc, 0x12 }  \
  }

// Grid class End delimeter (GUID opcode).
//
#define GRID_CLASS_END_OPCODE_GUID                                               \
  {                                                                                \
    0x30879de9, 0x7e69, 0x4f1b, { 0xb5, 0xa5, 0xda, 0x15, 0xbf, 0x6, 0x25, 0xce }  \
  }

// Grid class select cell location (GUID opcode).
//
#define GIRD_CLASS_SELECT_CELL_OPCODE_GUID                                       \
  {                                                                                \
    0x3147b040, 0xeac3, 0x4b9f, { 0xb5, 0xec, 0xc2, 0xe2, 0x88, 0x45, 0x17, 0x4e } \
  }

#endif // __DFCI_MENU_H__
