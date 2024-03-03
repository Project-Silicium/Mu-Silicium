[000h 0000 004h]                   Signature : "FACP"    [Fixed ACPI Description Table (FADT)]
[004h 0004 004h]                Table Length : 00000114
[008h 0008 001h]                    Revision : 05
[009h 0009 001h]                    Checksum : 00     /* Incorrect checksum, should be 30 */
[00Ah 0010 006h]                      Oem ID : "QCOM  "
[010h 0016 008h]                Oem Table ID : "QCOMEDK2"
[018h 0024 004h]                Oem Revision : 00007125
[01Ch 0028 004h]             Asl Compiler ID : "QCOM"
[020h 0032 004h]       Asl Compiler Revision : 00000001

[024h 0036 004h]                FACS Address : 00000000
[028h 0040 004h]                DSDT Address : 00000000
[02Ch 0044 001h]                       Model : 00
[02Dh 0045 001h]                  PM Profile : 08 [Tablet]
[02Eh 0046 002h]               SCI Interrupt : 0000
[030h 0048 004h]            SMI Command Port : 00000000
[034h 0052 001h]           ACPI Enable Value : 00
[035h 0053 001h]          ACPI Disable Value : 00
[036h 0054 001h]              S4BIOS Command : 00
[037h 0055 001h]             P-State Control : 00
[038h 0056 004h]    PM1A Event Block Address : 00000000
[03Ch 0060 004h]    PM1B Event Block Address : 00000000
[040h 0064 004h]  PM1A Control Block Address : 00000000
[044h 0068 004h]  PM1B Control Block Address : 00000000
[048h 0072 004h]   PM2 Control Block Address : 00000000
[04Ch 0076 004h]      PM Timer Block Address : 00000000
[050h 0080 004h]          GPE0 Block Address : 00000000
[054h 0084 004h]          GPE1 Block Address : 00000000
[058h 0088 001h]      PM1 Event Block Length : 00
[059h 0089 001h]    PM1 Control Block Length : 00
[05Ah 0090 001h]    PM2 Control Block Length : 00
[05Bh 0091 001h]       PM Timer Block Length : 00
[05Ch 0092 001h]           GPE0 Block Length : 00
[05Dh 0093 001h]           GPE1 Block Length : 00
[05Eh 0094 001h]            GPE1 Base Offset : 00
[05Fh 0095 001h]                _CST Support : 00
[060h 0096 002h]                  C2 Latency : 0000
[062h 0098 002h]                  C3 Latency : 0000
[064h 0100 002h]              CPU Cache Size : 0000
[066h 0102 002h]          Cache Flush Stride : 0000
[068h 0104 001h]           Duty Cycle Offset : 00
[069h 0105 001h]            Duty Cycle Width : 00
[06Ah 0106 001h]         RTC Day Alarm Index : 00
[06Bh 0107 001h]       RTC Month Alarm Index : 00
[06Ch 0108 001h]           RTC Century Index : 00
[06Dh 0109 002h]  Boot Flags (decoded below) : 0000
               Legacy Devices Supported (V2) : 0
            8042 Present on ports 60/64 (V2) : 0
                        VGA Not Present (V4) : 0
                      MSI Not Supported (V4) : 0
                PCIe ASPM Not Supported (V4) : 0
                   CMOS RTC Not Present (V5) : 0
[06Fh 0111 001h]                    Reserved : 00
[070h 0112 004h]       Flags (decoded below) : 00300000
      WBINVD instruction is operational (V1) : 0
              WBINVD flushes all caches (V1) : 0
                    All CPUs support C1 (V1) : 0
                  C2 works on MP system (V1) : 0
            Control Method Power Button (V1) : 0
            Control Method Sleep Button (V1) : 0
        RTC wake not in fixed reg space (V1) : 0
            RTC can wake system from S4 (V1) : 0
                        32-bit PM Timer (V1) : 0
                      Docking Supported (V1) : 0
               Reset Register Supported (V2) : 0
                            Sealed Case (V3) : 0
                    Headless - No Video (V3) : 0
        Use native instr after SLP_TYPx (V3) : 0
              PCIEXP_WAK Bits Supported (V4) : 0
                     Use Platform Timer (V4) : 0
               RTC_STS valid on S4 wake (V4) : 0
                Remote Power-on capable (V4) : 0
                 Use APIC Cluster Model (V4) : 0
     Use APIC Physical Destination Mode (V4) : 0
                       Hardware Reduced (V5) : 1
                      Low Power S0 Idle (V5) : 1

[074h 0116 00Ch]              Reset Register : [Generic Address Structure]
[074h 0116 001h]                    Space ID : 03 [EmbeddedControl]
[075h 0117 001h]                   Bit Width : 00
[076h 0118 001h]                  Bit Offset : 00
[077h 0119 001h]        Encoded Access Width : 03 [DWord Access:32]
[078h 0120 008h]                     Address : 00000000009020B4

[080h 0128 001h]        Value to cause reset : 01
[081h 0129 002h]   ARM Flags (decoded below) : 0001
                              PSCI Compliant : 1
                       Must use HVC for PSCI : 0

[083h 0131 001h]         FADT Minor Revision : 00
[084h 0132 008h]                FACS Address : 0000000000000000
[08Ch 0140 008h]                DSDT Address : 0000000000000000
[094h 0148 00Ch]            PM1A Event Block : [Generic Address Structure]
[094h 0148 001h]                    Space ID : 00 [SystemMemory]
[095h 0149 001h]                   Bit Width : 00
[096h 0150 001h]                  Bit Offset : 00
[097h 0151 001h]        Encoded Access Width : 00 [Undefined/Legacy]
[098h 0152 008h]                     Address : 0000000000000000

[0A0h 0160 00Ch]            PM1B Event Block : [Generic Address Structure]
[0A0h 0160 001h]                    Space ID : 00 [SystemMemory]
[0A1h 0161 001h]                   Bit Width : 00
[0A2h 0162 001h]                  Bit Offset : 00
[0A3h 0163 001h]        Encoded Access Width : 00 [Undefined/Legacy]
[0A4h 0164 008h]                     Address : 0000000000000000

[0ACh 0172 00Ch]          PM1A Control Block : [Generic Address Structure]
[0ACh 0172 001h]                    Space ID : 00 [SystemMemory]
[0ADh 0173 001h]                   Bit Width : 00
[0AEh 0174 001h]                  Bit Offset : 00
[0AFh 0175 001h]        Encoded Access Width : 00 [Undefined/Legacy]
[0B0h 0176 008h]                     Address : 0000000000000000

[0B8h 0184 00Ch]          PM1B Control Block : [Generic Address Structure]
[0B8h 0184 001h]                    Space ID : 00 [SystemMemory]
[0B9h 0185 001h]                   Bit Width : 00
[0BAh 0186 001h]                  Bit Offset : 00
[0BBh 0187 001h]        Encoded Access Width : 00 [Undefined/Legacy]
[0BCh 0188 008h]                     Address : 0000000000000000

[0C4h 0196 00Ch]           PM2 Control Block : [Generic Address Structure]
[0C4h 0196 001h]                    Space ID : 00 [SystemMemory]
[0C5h 0197 001h]                   Bit Width : 00
[0C6h 0198 001h]                  Bit Offset : 00
[0C7h 0199 001h]        Encoded Access Width : 00 [Undefined/Legacy]
[0C8h 0200 008h]                     Address : 0000000000000000

[0D0h 0208 00Ch]              PM Timer Block : [Generic Address Structure]
[0D0h 0208 001h]                    Space ID : 00 [SystemMemory]
[0D1h 0209 001h]                   Bit Width : 00
[0D2h 0210 001h]                  Bit Offset : 00
[0D3h 0211 001h]        Encoded Access Width : 00 [Undefined/Legacy]
[0D4h 0212 008h]                     Address : 0000000000000000

[0DCh 0220 00Ch]                  GPE0 Block : [Generic Address Structure]
[0DCh 0220 001h]                    Space ID : 00 [SystemMemory]
[0DDh 0221 001h]                   Bit Width : 00
[0DEh 0222 001h]                  Bit Offset : 00
[0DFh 0223 001h]        Encoded Access Width : 00 [Undefined/Legacy]
[0E0h 0224 008h]                     Address : 0000000000000000

[0E8h 0232 00Ch]                  GPE1 Block : [Generic Address Structure]
[0E8h 0232 001h]                    Space ID : 00 [SystemMemory]
[0E9h 0233 001h]                   Bit Width : 00
[0EAh 0234 001h]                  Bit Offset : 00
[0EBh 0235 001h]        Encoded Access Width : 00 [Undefined/Legacy]
[0ECh 0236 008h]                     Address : 0000000000000000


[0F4h 0244 00Ch]      Sleep Control Register : [Generic Address Structure]
[0F4h 0244 001h]                    Space ID : 00 [SystemMemory]
[0F5h 0245 001h]                   Bit Width : 00
[0F6h 0246 001h]                  Bit Offset : 00
[0F7h 0247 001h]        Encoded Access Width : 00 [Undefined/Legacy]
[0F8h 0248 008h]                     Address : 0000000000000000

[100h 0256 00Ch]       Sleep Status Register : [Generic Address Structure]
[100h 0256 001h]                    Space ID : 00 [SystemMemory]
[101h 0257 001h]                   Bit Width : 00
[102h 0258 001h]                  Bit Offset : 00
[103h 0259 001h]        Encoded Access Width : 00 [Undefined/Legacy]
[104h 0260 008h]                     Address : 0000000000000000

[10Ch 0268 008h]               Hypervisor ID : 000000004D4F4351