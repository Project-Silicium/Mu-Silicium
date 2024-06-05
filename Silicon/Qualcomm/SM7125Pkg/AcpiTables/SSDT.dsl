/*
 * Intel ACPI Component Architecture
 * AML/ASL+ Disassembler version 20200925 (64-bit version)
 * Copyright (c) 2000 - 2020 Intel Corporation
 * 
 * Disassembling to symbolic ASL+ operators
 *
 * Disassembly of SSDT.aml, Wed Jun  5 21:21:26 2024
 *
 * Original Table Header:
 *     Signature        "SSDT"
 *     Length           0x0000003D (61)
 *     Revision         0x02
 *     Checksum         0x5E
 *     OEM ID           "MSFT  "
 *     OEM Table ID     ""
 *     OEM Revision     0x00000001 (1)
 *     Compiler ID      "MSFT"
 *     Compiler Version 0x05000000 (83886080)
 */
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

