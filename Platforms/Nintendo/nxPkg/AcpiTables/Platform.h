/** @file
 *
 *  Copyright (c) 2013-2017, ARM Limited. All rights reserved.
 *
 *  This program and the accompanying materials
 *  are licensed and made available under the terms and conditions of the BSD
 *License which accompanies this distribution.  The full text of the license may
 *be found at http://opensource.org/licenses/bsd-license.php
 *
 *  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR
 *IMPLIED.
 *
 **/

#ifndef __NINTENDO_SWITCH_H__
#define __NINTENDO_SWITCH_H__

//
// ACPI table information used to initialize tables.
//
#define EFI_ACPI_ARM_OEM_ID 'N', 'V', 'I', 'D', 'I', 'A' // OEMID 6 bytes long
#define EFI_ACPI_ARM_OEM_TABLE_ID                                              \
  SIGNATURE_64('N', 'I', 'N', 'T', 'E', 'N', 'D',                              \
               'O') // OEM table id 8 bytes long
#define EFI_ACPI_ARM_OEM_REVISION 0x20210314
#define EFI_ACPI_ARM_CREATOR_ID SIGNATURE_32('I', 'M', 'B', 'U')
#define EFI_ACPI_ARM_CREATOR_REVISION 0x00000004

// A macro to initialise the common header part of EFI ACPI tables as defined by
// EFI_ACPI_DESCRIPTION_HEADER structure.
#define ARM_ACPI_HEADER(Signature, Type, Revision)                             \
  {                                                                            \
    Signature,                         /* UINT32  Signature */                 \
        sizeof(Type),                  /* UINT32  Length */                    \
        Revision,                      /* UINT8   Revision */                  \
        0,                             /* UINT8   Checksum */                  \
        {EFI_ACPI_ARM_OEM_ID},         /* UINT8   OemId[6] */                  \
        EFI_ACPI_ARM_OEM_TABLE_ID,     /* UINT64  OemTableId */                \
        EFI_ACPI_ARM_OEM_REVISION,     /* UINT32  OemRevision */               \
        EFI_ACPI_ARM_CREATOR_ID,       /* UINT32  CreatorId */                 \
        EFI_ACPI_ARM_CREATOR_REVISION, /* UINT32  CreatorRevision */           \
  }

#endif
