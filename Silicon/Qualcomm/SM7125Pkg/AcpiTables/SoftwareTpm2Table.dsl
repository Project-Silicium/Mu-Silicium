/*
 * Intel ACPI Component Architecture
 * AML/ASL+ Disassembler version 20200925 (64-bit version)
 * Copyright (c) 2000 - 2020 Intel Corporation
 * 
 * Disassembly of SoftwareTpm2Table.aml, Wed Jun  5 21:21:17 2024
 *
 * ACPI Data Table [TPM2]
 *
 * Format: [HexOffset DecimalOffset ByteLength]  FieldName : FieldValue
 */

[000h 0000   4]                    Signature : "TPM2"    [Trusted Platform Module hardware interface table]
[004h 0004   4]                 Table Length : 00000044
[008h 0008   1]                     Revision : 03
[009h 0009   1]                     Checksum : B0
[00Ah 0010   6]                       Oem ID : "MSFT  "
[010h 0016   8]                 Oem Table ID : "fTPM    "
[018h 0024   4]                 Oem Revision : 00000001
[01Ch 0028   4]              Asl Compiler ID : "DM  "
[020h 0032   4]        Asl Compiler Revision : 20141014

[024h 0036   4]                     Reserved : 00000000
[028h 0040   8]              Control Address : 0000000000000000
[030h 0048   4]                 Start Method : 00000007

Raw Table Data: Length 68 (0x44)

    0000: 54 50 4D 32 44 00 00 00 03 B0 4D 53 46 54 20 20  // TPM2D.....MSFT  
    0010: 66 54 50 4D 20 20 20 20 01 00 00 00 44 4D 20 20  // fTPM    ....DM  
    0020: 14 10 14 20 00 00 00 00 00 00 00 00 00 00 00 00  // ... ............
    0030: 07 00 00 00 45 54 41 4C 55 4D 49 53 00 00 00 00  // ....ETALUMIS....
    0040: 00 00 00 00                                      // ....
