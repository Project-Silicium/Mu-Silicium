//
// Copyright (c) 2021-2023 DuoWoA authors. All Rights Reserved.
//
// Module Name:
//
//     SSDT.asl
//
// Abstract:
//
//     Contains device definitions for the Secondary System
//     Description Table (SSDT).
//
// Environment:
//
//     Advanced Configuration and Power Interface (ACPI)
//
// Revision History:
//
//     2023-01-22 - @gus33000 - Initial revision
//
// License:
//
//     SPDX-License-Identifier: MIT
//

DefinitionBlock ("", "SSDT", 2, "MSFT  ", "", 0x00000001)
{
    Scope (\_SB)
    {
        Device (SSDE)
        {
            Name (_HID, "SSDE")  // _HID: Hardware ID
        }
    }
}

