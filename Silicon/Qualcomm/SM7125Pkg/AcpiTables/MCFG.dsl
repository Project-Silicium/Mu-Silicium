/*
 * Intel ACPI Component Architecture
 * AML/ASL+ Disassembler version 20230628 (64-bit version)
 * Copyright (c) 2000 - 2023 Intel Corporation
 * 
 * Disassembly of MCFG.aml, Thu Feb 29 21:51:11 2024
 *
 * ACPI Data Table [MCFG]
 *
 * Format: [HexOffset DecimalOffset ByteLength]  FieldName : FieldValue (in hex)
 */

[000h 0000 004h]                   Signature : "MCFG"    [Memory Mapped Configuration Table]
[004h 0004 004h]                Table Length : 0000006C
[008h 0008 001h]                    Revision : 01
[009h 0009 001h]                    Checksum : 00     /* Incorrect checksum, should be 2C */
[00Ah 0010 006h]                      Oem ID : "QCOM  "
[010h 0016 008h]                Oem Table ID : "QCOMEDK2"
[018h 0024 004h]                Oem Revision : 00007125
[01Ch 0028 004h]             Asl Compiler ID : "QCOM"
[020h 0032 004h]       Asl Compiler Revision : 00000001

[024h 0036 008h]                    Reserved : 0000000000000000

[02Ch 0044 008h]                Base Address : 0000000060000000
[034h 0052 002h]        Segment Group Number : 0000
[036h 0054 001h]            Start Bus Number : 00
[037h 0055 001h]              End Bus Number : 01
[038h 0056 004h]                    Reserved : 00000000

[03Ch 0060 008h]                Base Address : 0000000068000000
[044h 0068 002h]        Segment Group Number : 0001
[046h 0070 001h]            Start Bus Number : 00
[047h 0071 001h]              End Bus Number : 01
[048h 0072 004h]                    Reserved : 00000000

[04Ch 0076 008h]                Base Address : 0000000070000000
[054h 0084 002h]        Segment Group Number : 0002
[056h 0086 001h]            Start Bus Number : 00
[057h 0087 001h]              End Bus Number : 01
[058h 0088 004h]                    Reserved : 00000000

[05Ch 0092 008h]                Base Address : 0000000040000000
[064h 0100 002h]        Segment Group Number : 0003
[066h 0102 001h]            Start Bus Number : 00
[067h 0103 001h]              End Bus Number : 01
[068h 0104 004h]                    Reserved : 00000000

Raw Table Data: Length 108 (0x6C)

    0000: 4D 43 46 47 6C 00 00 00 01 00 51 43 4F 4D 20 20  // MCFGl.....QCOM  
    0010: 51 43 4F 4D 45 44 4B 32 80 71 00 00 51 43 4F 4D  // QCOMEDK2.q..QCOM
    0020: 01 00 00 00 00 00 00 00 00 00 00 00 00 00 00 60  // ...............`
    0030: 00 00 00 00 00 00 00 01 00 00 00 00 00 00 00 68  // ...............h
    0040: 00 00 00 00 01 00 00 01 00 00 00 00 00 00 00 70  // ...............p
    0050: 00 00 00 00 02 00 00 01 00 00 00 00 00 00 00 40  // ...............@
    0060: 00 00 00 00 03 00 00 01 00 00 00 00              // ............
