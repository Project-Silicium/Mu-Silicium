[000h 0000 004h]                   Signature : "DBG2"    [Debug Port Table type 2]
[004h 0004 004h]                Table Length : 0000015C
[008h 0008 001h]                    Revision : 01
[009h 0009 001h]                    Checksum : 00     /* Incorrect checksum, should be B5 */
[00Ah 0010 006h]                      Oem ID : "QCOM  "
[010h 0016 008h]                Oem Table ID : "QCOMEDK2"
[018h 0024 004h]                Oem Revision : 00007125
[01Ch 0028 004h]             Asl Compiler ID : "QCOM"
[020h 0032 004h]       Asl Compiler Revision : 00000001

[024h 0036 004h]                 Info Offset : 0000002C
[028h 0040 004h]                  Info Count : 00000002

[02Ch 0044 001h]                    Revision : 01
[02Dh 0045 002h]                      Length : 0046
[02Fh 0047 001h]              Register Count : 01
[030h 0048 002h]             Namepath Length : 000A
[032h 0050 002h]             Namepath Offset : 0026
[034h 0052 002h]             OEM Data Length : 0000 [Optional field not present]
[036h 0054 002h]             OEM Data Offset : 0000 [Optional field not present]
[038h 0056 002h]                   Port Type : 8000
[03Ah 0058 002h]                Port Subtype : 0011
[03Ch 0060 002h]                    Reserved : 0000
[03Eh 0062 002h]         Base Address Offset : 0016
[040h 0064 002h]         Address Size Offset : 0022

[042h 0066 00Ch]       Base Address Register : [Generic Address Structure]
[042h 0066 001h]                    Space ID : 00 [SystemMemory]
[043h 0067 001h]                   Bit Width : 20
[044h 0068 001h]                  Bit Offset : 00
[045h 0069 001h]        Encoded Access Width : 20 [Unknown Width Encoding]
[046h 0070 008h]                     Address : 0000000000A88000

[04Eh 0078 004h]                Address Size : 00001000

[052h 0082 00Ah]                    Namepath : "\_SB.UARD"

[072h 0114 001h]                    Revision : 01
[073h 0115 002h]                      Length : 00EA
[075h 0117 001h]              Register Count : 02
[076h 0118 002h]             Namepath Length : 000A
[078h 0120 002h]             Namepath Offset : 0036
[07Ah 0122 002h]             OEM Data Length : 0094
[07Ch 0124 002h]             OEM Data Offset : 0056
[07Eh 0126 002h]                   Port Type : 8003
[080h 0128 002h]                Port Subtype : 5143
[082h 0130 002h]                    Reserved : 0000
[084h 0132 002h]         Base Address Offset : 0016
[086h 0134 002h]         Address Size Offset : 002E

[088h 0136 00Ch]       Base Address Register : [Generic Address Structure]
[088h 0136 001h]                    Space ID : 00 [SystemMemory]
[089h 0137 001h]                   Bit Width : 20
[08Ah 0138 001h]                  Bit Offset : 00
[08Bh 0139 001h]        Encoded Access Width : 20 [Unknown Width Encoding]
[08Ch 0140 008h]                     Address : 000000000A600000


[094h 0148 00Ch]       Base Address Register : [Generic Address Structure]
[094h 0148 001h]                    Space ID : 00 [SystemMemory]
[095h 0149 001h]                   Bit Width : 20
[096h 0150 001h]                  Bit Offset : 00
[097h 0151 001h]        Encoded Access Width : 20 [Unknown Width Encoding]
[098h 0152 008h]                     Address : 000000000A600000

[0A0h 0160 004h]                Address Size : 000FFFFF
[0A4h 0164 004h]                Address Size : 00001000

[0A8h 0168 00Ah]                    Namepath : "\_SB.USB0"
[0C8h 0200 094h]                    OEM Data : 44 42 47 32 5C 01 00 00 01 00 51 43 4F 4D 20 20 /* DBG2\.....QCOM   */\
/* 0D8h 0216  16 */                            51 43 4F 4D 45 44 4B 32 80 71 00 00 51 43 4F 4D /* QCOMEDK2.q..QCOM */\
/* 0E8h 0232  16 */                            01 00 00 00 2C 00 00 00 02 00 00 00 01 46 00 01 /* ....,........F.. */\
/* 0F8h 0248  16 */                            0A 00 26 00 00 00 00 00 00 80 11 00 00 00 16 00 /* ..&............. */\
/* 108h 0264  16 */                            22 00 00 20 00 20 00 80 A8 00 00 00 00 00 00 10 /* ".. . .......... */\
/* 118h 0280  16 */                            00 00 5C 5F 53 42 2E 55 41 52 44 00 00 00 00 00 /* ..\_SB.UARD..... */\
/* 128h 0296  16 */                            00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 /* ................ */\
/* 138h 0312  16 */                            00 00 01 EA 00 02 0A 00 36 00 94 00 56 00 03 80 /* ........6...V... */\
/* 148h 0328  16 */                            43 51 00 00 16 00 2E 00 00 20 00 20 00 00 60 0A /* CQ....... . ..`. */\
/* 158h 0344   4 */                            00 00 00 00                                     /* .... */\
