/** @file
 *BootMemu

  Copyright (C) Microsoft Corporation. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _BOOT_MENU_H_
#define _BOOT_MENU_H_

// The following defines are from MsBootMenuGuid.h.  Keep these in mind when altering
// the values used in this formset. The dynamic question id's are QUESTION_START + boot order index

#define MS_BOOT_ORDER_FORM_ID  0x1000

#include <Guid/MsBootMenuGuid.h>   // defines MS_BOOT_* items

// The following defines are defined in Guid/MedModuleHii.h, but this header
// doesn't play well with the VFR compiler.
#define EFI_OTHER_DEVICE_CLASS            0x20
#define EFI_GENERAL_APPLICATION_SUBCLASS  0x01

#define MAX_BOOT_OPTIONS_SUPPORTED  6           // Maximum number of boot options to display in listbox

#define MS_BOOT_ORDER_VARID     0x0031
#define MS_BOOT_SETTINGS_VARID  0x0033
#define MS_BOOT_GRAYOUT_VARID   0x0034
#define MS_BOOT_SUPPRESS_VARID  0x0035

#define MS_BOOT_ORDER_INIT_KEY  0x0041

#define MS_BOOT_DEVICE_QUESTION_ID  0x0050
#define MS_BOOT_ORDER_QUESTION_ID   0x0051

#define MS_ENABLE_IPV6_QUESTION_ID             0x0061
#define MS_ENABLE_ALT_BOOT_QUESTION_ID         0x0062
#define MS_ENABLE_BOOT_ORDER_LOCK_QUESTION_ID  0x0063
#define MS_ENABLE_USB_BOOT_QUESTION_ID         0x0064
//
// These are the VFR compiler generated data representing our VFR data.
//
extern UINT8  BootMenuVfrBin[];

#define LABEL_ORDER_OPTIONS  0x1021
#define LABEL_ORDER_END      0x1022

// Grid class Start delimeter (GUID opcode).
//
#define GRID_START_OPCODE_GUID                                             \
  {                                                                                \
    0xc0b6e247, 0xe140, 0x4b4d, { 0xa6, 0x4, 0xc3, 0xae, 0x1f, 0xa6, 0xcc, 0x12 }  \
  }

// Grid class End delimeter (GUID opcode).
//
#define GRID_END_OPCODE_GUID                                               \
  {                                                                                \
    0x30879de9, 0x7e69, 0x4f1b, { 0xb5, 0xa5, 0xda, 0x15, 0xbf, 0x6, 0x25, 0xce }  \
  }

// Grid class select cell location (GUID opcode).
//
#define GRID_SELECT_CELL_OPCODE_GUID                                       \
  {                                                                                \
    0x3147b040, 0xeac3, 0x4b9f, { 0xb5, 0xec, 0xc2, 0xe2, 0x88, 0x45, 0x17, 0x4e } \
  }

typedef struct {
  UINT32    OrderOptions[MAX_BOOT_OPTIONS_SUPPORTED];
} ORDER_MENU_CONFIGURATION;

typedef struct {
  BOOLEAN    IPv6;
  BOOLEAN    AltBoot;
  BOOLEAN    BootOrderLock;
  BOOLEAN    EnableUsbBoot;
} SETTINGS_MENU_CONFIGURATION;

typedef struct {
  BOOLEAN    IPv6;
  BOOLEAN    AltBoot;
  BOOLEAN    BootOrderLock;
  BOOLEAN    EnableUsbBoot;
  //
  BOOLEAN    RestrictedAccessString;
} SETTINGS_GRAYOUT_CONFIGURATION;

typedef struct {
  BOOLEAN    IPv6;
  BOOLEAN    AltBoot;
  BOOLEAN    BootOrderLock;
  BOOLEAN    EnableUsbBoot;
} SETTINGS_SUPPRESS_CONFIGURATION;

#endif // _BOOT_MENU_H_
