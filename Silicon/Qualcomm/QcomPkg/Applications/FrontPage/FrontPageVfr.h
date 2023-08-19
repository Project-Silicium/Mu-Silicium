/**
 * Front Page vfr and c file common definitions

  Copyright (C) Microsoft Corporation. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

//
// THIS FILE MUST ONLY CONTAIN DEFINTIONS THAT CAN BE INTERPRETED
// BY BOTH THE VFR AND C COMPILERS.
//

#ifndef FRONT_PAGE_VFR_H
#define FRONT_PAGE_VFR_H

#define FRONT_PAGE_CLASS     0x0000
#define FRONT_PAGE_SUBCLASS  0x0002

//
// This is the VFR compiler generated header file which defines the
// string identifiers.
//
#define PRINTABLE_LANGUAGE_NAME_STRING_ID  0x0001

// Front Page exposes the following forms
//
// NOTE: Form ID order and values must align with VFR code.
//
#define FRONT_PAGE_FORM_ID_NONE      0x0000
#define FRONT_PAGE_FORM_ID_SECURITY  0x0001
#define FRONT_PAGE_FORM_ID_PCINFO    0x0002
#define FRONT_PAGE_FORM_ID_BOOTMENU  0x0003
#define FRONT_PAGE_FORM_ID_EXIT      0x0004

// Front Page triggers the following actions
//
// NOTE: Form ID order and values must align with VFR code.
//
#define FRONT_PAGE_ACTION_DEFAULTS                 0x1000
#define FRONT_PAGE_ACTION_SEC_CHANGE_SB_CONFIG     0x1003
#define FRONT_PAGE_ACTION_SEC_DISPLAY_SB_WHAT_IS   0x1004
#define FRONT_PAGE_ACTION_SEC_SET_SYSTEM_PASSWORD  0x1006
#define FRONT_PAGE_ACTION_REBOOT_TO_FRONTPAGE      0x1007
#define FRONT_PAGE_ACTION_EXIT_FRONTPAGE           0x1001

#define LABEL_PCINFO_FW_VERSION_TAG_START  0x2000
#define LABEL_PCINFO_FW_VERSION_TAG_END    0x2001

#define LABEL_UPDATE_SECURITY_START  0x2002
#define LABEL_UPDATE_SECURITY_END    0x2003
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

// Front Page Formset Guid
// {7F98EFE9-50AA-4598-B7C1-CB72E1CC5224}
#define FRONT_PAGE_CONFIG_FORMSET_GUID \
  { \
    0x7f98efe9, 0x50aa, 0x4598, { 0xb7, 0xc1, 0xcb, 0x72, 0xe1, 0xcc, 0x52, 0x24 } \
  }

typedef struct _FRONT_PAGE_UI_CONTROLS {
  // Used to indicate that a restart is required prior to altering any
  // security-sensitive controls.
  BOOLEAN    PostSecurityLocks;
} FRONT_PAGE_UI_CONTROLS;

#define FRONT_PAGE_UI_CONTROLS_VARID  0x0072    // Presentation-layer controls.
#define FRONT_PAGE_SECURITY_VARID     0x0074

#endif // FRONT_PAGE_VFR_H
