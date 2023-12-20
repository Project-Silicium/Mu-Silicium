/*
 * Intel ACPI Component Architecture
 * AML/ASL+ Disassembler version 20200925 (64-bit version)
 * Copyright (c) 2000 - 2020 Intel Corporation
 * 
 * Disassembly of MCFG.aml, Wed Dec 20 20:46:26 2023
 *
 * ACPI Data Table [MCFG]
 *
 * Format: [HexOffset DecimalOffset ByteLength]  FieldName : FieldValue
 */

[000h 0000   4]                    Signature : "MCFG"    [Memory Mapped Configuration table]
[004h 0004   4]                 Table Length : 0000003C
[008h 0008   1]                     Revision : 01
[009h 0009   1]                     Checksum : D9
[00Ah 0010   6]                       Oem ID : "QCOM  "
[010h 0016   8]                 Oem Table ID : "QCOMEDK2"
[018h 0024   4]                 Oem Revision : 00000660
[01Ch 0028   4]              Asl Compiler ID : "INTL"
[020h 0032   4]        Asl Compiler Revision : 20200925

[024h 0036   8]                     Reserved : 0000000000000000

[02Ch 0044   8]                 Base Address : 000000001B000000
[034h 0052   2]         Segment Group Number : 0000
[036h 0054   1]             Start Bus Number : 00
[037h 0055   1]               End Bus Number : 01
[038h 0056   4]                     Reserved : 00000000

Raw Table Data: Length 60 (0x3C)

    0000: 4D 43 46 47 3C 00 00 00 01 D9 51 43 4F 4D 20 20  // MCFG<.....QCOM  
    0010: 51 43 4F 4D 45 44 4B 32 60 06 00 00 49 4E 54 4C  // QCOMEDK2`...INTL
    0020: 25 09 20 20 00 00 00 00 00 00 00 00 00 00 00 1B  // %.  ............
    0030: 00 00 00 00 00 00 00 01 00 00 00 00              // ............
