[000h 0000 004h]                   Signature : "APIC"    [Multiple APIC Description Table (MADT)]
[004h 0004 004h]                Table Length : 000002ED
[008h 0008 001h]                    Revision : 05
[009h 0009 001h]                    Checksum : 00     /* Incorrect checksum, should be 1E */
[00Ah 0010 006h]                      Oem ID : "QCOM  "
[010h 0016 008h]                Oem Table ID : "QCOMEDK2"
[018h 0024 004h]                Oem Revision : 00007125
[01Ch 0028 004h]             Asl Compiler ID : "QCOM"
[020h 0032 004h]       Asl Compiler Revision : 00000001

[024h 0036 004h]          Local Apic Address : 00000000
[028h 0040 004h]       Flags (decoded below) : 00000000
                         PC-AT Compatibility : 0

[02Ch 0044 001h]               Subtable Type : 0B [Generic Interrupt Controller]
[02Dh 0045 001h]                      Length : 50
[02Eh 0046 002h]                    Reserved : 0000
[030h 0048 004h]        CPU Interface Number : 00000000
[034h 0052 004h]               Processor UID : 00000000
[038h 0056 004h]       Flags (decoded below) : 00000001
                           Processor Enabled : 1
          Performance Interrupt Trigger Mode : 0
          Virtual GIC Interrupt Trigger Mode : 0
[03Ch 0060 004h]    Parking Protocol Version : 00000000
[040h 0064 004h]       Performance Interrupt : 00000015
[044h 0068 008h]              Parked Address : 0000000000000000
[04Ch 0076 008h]                Base Address : 0000000000000000
[054h 0084 008h]    Virtual GIC Base Address : 0000000000000000
[05Ch 0092 008h] Hypervisor GIC Base Address : 0000000000000000
[064h 0100 004h]       Virtual GIC Interrupt : 00000018
[068h 0104 008h]  Redistributor Base Address : 0000000000000000
[070h 0112 008h]                   ARM MPIDR : 0000000000000000
[078h 0120 001h]            Efficiency Class : 00
[079h 0121 001h]                    Reserved : 00
[07Ah 0122 002h]      SPE Overflow Interrupt : 0000
[07Ch 0124 002h]              TRBE Interrupt : 500B

[07Ch 0124 001h]               Subtable Type : 0B [Generic Interrupt Controller]
[07Dh 0125 001h]                      Length : 50
[07Eh 0126 002h]                    Reserved : 0000
[080h 0128 004h]        CPU Interface Number : 00000001
[084h 0132 004h]               Processor UID : 00000001
[088h 0136 004h]       Flags (decoded below) : 00000001
                           Processor Enabled : 1
          Performance Interrupt Trigger Mode : 0
          Virtual GIC Interrupt Trigger Mode : 0
[08Ch 0140 004h]    Parking Protocol Version : 00000000
[090h 0144 004h]       Performance Interrupt : 00000015
[094h 0148 008h]              Parked Address : 0000000000000000
[09Ch 0156 008h]                Base Address : 0000000000000000
[0A4h 0164 008h]    Virtual GIC Base Address : 0000000000000000
[0ACh 0172 008h] Hypervisor GIC Base Address : 0000000000000000
[0B4h 0180 004h]       Virtual GIC Interrupt : 00000018
[0B8h 0184 008h]  Redistributor Base Address : 0000000000000000
[0C0h 0192 008h]                   ARM MPIDR : 0000000000000100
[0C8h 0200 001h]            Efficiency Class : 00
[0C9h 0201 001h]                    Reserved : 00
[0CAh 0202 002h]      SPE Overflow Interrupt : 0000
[0CCh 0204 002h]              TRBE Interrupt : 500B

[0CCh 0204 001h]               Subtable Type : 0B [Generic Interrupt Controller]
[0CDh 0205 001h]                      Length : 50
[0CEh 0206 002h]                    Reserved : 0000
[0D0h 0208 004h]        CPU Interface Number : 00000002
[0D4h 0212 004h]               Processor UID : 00000002
[0D8h 0216 004h]       Flags (decoded below) : 00000001
                           Processor Enabled : 1
          Performance Interrupt Trigger Mode : 0
          Virtual GIC Interrupt Trigger Mode : 0
[0DCh 0220 004h]    Parking Protocol Version : 00000000
[0E0h 0224 004h]       Performance Interrupt : 00000015
[0E4h 0228 008h]              Parked Address : 0000000000000000
[0ECh 0236 008h]                Base Address : 0000000000000000
[0F4h 0244 008h]    Virtual GIC Base Address : 0000000000000000
[0FCh 0252 008h] Hypervisor GIC Base Address : 0000000000000000
[104h 0260 004h]       Virtual GIC Interrupt : 00000018
[108h 0264 008h]  Redistributor Base Address : 0000000000000000
[110h 0272 008h]                   ARM MPIDR : 0000000000000200
[118h 0280 001h]            Efficiency Class : 00
[119h 0281 001h]                    Reserved : 00
[11Ah 0282 002h]      SPE Overflow Interrupt : 0000
[11Ch 0284 002h]              TRBE Interrupt : 500B

[11Ch 0284 001h]               Subtable Type : 0B [Generic Interrupt Controller]
[11Dh 0285 001h]                      Length : 50
[11Eh 0286 002h]                    Reserved : 0000
[120h 0288 004h]        CPU Interface Number : 00000003
[124h 0292 004h]               Processor UID : 00000003
[128h 0296 004h]       Flags (decoded below) : 00000001
                           Processor Enabled : 1
          Performance Interrupt Trigger Mode : 0
          Virtual GIC Interrupt Trigger Mode : 0
[12Ch 0300 004h]    Parking Protocol Version : 00000000
[130h 0304 004h]       Performance Interrupt : 00000015
[134h 0308 008h]              Parked Address : 0000000000000000
[13Ch 0316 008h]                Base Address : 0000000000000000
[144h 0324 008h]    Virtual GIC Base Address : 0000000000000000
[14Ch 0332 008h] Hypervisor GIC Base Address : 0000000000000000
[154h 0340 004h]       Virtual GIC Interrupt : 00000018
[158h 0344 008h]  Redistributor Base Address : 0000000000000000
[160h 0352 008h]                   ARM MPIDR : 0000000000000300
[168h 0360 001h]            Efficiency Class : 00
[169h 0361 001h]                    Reserved : 00
[16Ah 0362 002h]      SPE Overflow Interrupt : 0000
[16Ch 0364 002h]              TRBE Interrupt : 500B

[16Ch 0364 001h]               Subtable Type : 0B [Generic Interrupt Controller]
[16Dh 0365 001h]                      Length : 50
[16Eh 0366 002h]                    Reserved : 0000
[170h 0368 004h]        CPU Interface Number : 00000004
[174h 0372 004h]               Processor UID : 00000004
[178h 0376 004h]       Flags (decoded below) : 00000001
                           Processor Enabled : 1
          Performance Interrupt Trigger Mode : 0
          Virtual GIC Interrupt Trigger Mode : 0
[17Ch 0380 004h]    Parking Protocol Version : 00000000
[180h 0384 004h]       Performance Interrupt : 00000015
[184h 0388 008h]              Parked Address : 0000000000000000
[18Ch 0396 008h]                Base Address : 0000000000000000
[194h 0404 008h]    Virtual GIC Base Address : 0000000000000000
[19Ch 0412 008h] Hypervisor GIC Base Address : 0000000000000000
[1A4h 0420 004h]       Virtual GIC Interrupt : 00000018
[1A8h 0424 008h]  Redistributor Base Address : 0000000000000000
[1B0h 0432 008h]                   ARM MPIDR : 0000000000000400
[1B8h 0440 001h]            Efficiency Class : 00
[1B9h 0441 001h]                    Reserved : 00
[1BAh 0442 002h]      SPE Overflow Interrupt : 0000
[1BCh 0444 002h]              TRBE Interrupt : 500B

[1BCh 0444 001h]               Subtable Type : 0B [Generic Interrupt Controller]
[1BDh 0445 001h]                      Length : 50
[1BEh 0446 002h]                    Reserved : 0000
[1C0h 0448 004h]        CPU Interface Number : 00000005
[1C4h 0452 004h]               Processor UID : 00000005
[1C8h 0456 004h]       Flags (decoded below) : 00000001
                           Processor Enabled : 1
          Performance Interrupt Trigger Mode : 0
          Virtual GIC Interrupt Trigger Mode : 0
[1CCh 0460 004h]    Parking Protocol Version : 00000000
[1D0h 0464 004h]       Performance Interrupt : 00000015
[1D4h 0468 008h]              Parked Address : 0000000000000000
[1DCh 0476 008h]                Base Address : 0000000000000000
[1E4h 0484 008h]    Virtual GIC Base Address : 0000000000000000
[1ECh 0492 008h] Hypervisor GIC Base Address : 0000000000000000
[1F4h 0500 004h]       Virtual GIC Interrupt : 00000018
[1F8h 0504 008h]  Redistributor Base Address : 0000000000000000
[200h 0512 008h]                   ARM MPIDR : 0000000000000500
[208h 0520 001h]            Efficiency Class : 00
[209h 0521 001h]                    Reserved : 00
[20Ah 0522 002h]      SPE Overflow Interrupt : 0000
[20Ch 0524 002h]              TRBE Interrupt : 500B

[20Ch 0524 001h]               Subtable Type : 0B [Generic Interrupt Controller]
[20Dh 0525 001h]                      Length : 50
[20Eh 0526 002h]                    Reserved : 0000
[210h 0528 004h]        CPU Interface Number : 00000006
[214h 0532 004h]               Processor UID : 00000006
[218h 0536 004h]       Flags (decoded below) : 00000001
                           Processor Enabled : 1
          Performance Interrupt Trigger Mode : 0
          Virtual GIC Interrupt Trigger Mode : 0
[21Ch 0540 004h]    Parking Protocol Version : 00000000
[220h 0544 004h]       Performance Interrupt : 00000015
[224h 0548 008h]              Parked Address : 0000000000000000
[22Ch 0556 008h]                Base Address : 0000000000000000
[234h 0564 008h]    Virtual GIC Base Address : 0000000000000000
[23Ch 0572 008h] Hypervisor GIC Base Address : 0000000000000000
[244h 0580 004h]       Virtual GIC Interrupt : 00000018
[248h 0584 008h]  Redistributor Base Address : 0000000000000000
[250h 0592 008h]                   ARM MPIDR : 0000000000000600
[258h 0600 001h]            Efficiency Class : 01
[259h 0601 001h]                    Reserved : 00
[25Ah 0602 002h]      SPE Overflow Interrupt : 0000
[25Ch 0604 002h]              TRBE Interrupt : 500B

[25Ch 0604 001h]               Subtable Type : 0B [Generic Interrupt Controller]
[25Dh 0605 001h]                      Length : 50
[25Eh 0606 002h]                    Reserved : 0000
[260h 0608 004h]        CPU Interface Number : 00000007
[264h 0612 004h]               Processor UID : 00000007
[268h 0616 004h]       Flags (decoded below) : 00000001
                           Processor Enabled : 1
          Performance Interrupt Trigger Mode : 0
          Virtual GIC Interrupt Trigger Mode : 0
[26Ch 0620 004h]    Parking Protocol Version : 00000000
[270h 0624 004h]       Performance Interrupt : 00000015
[274h 0628 008h]              Parked Address : 0000000000000000
[27Ch 0636 008h]                Base Address : 0000000000000000
[284h 0644 008h]    Virtual GIC Base Address : 0000000000000000
[28Ch 0652 008h] Hypervisor GIC Base Address : 0000000000000000
[294h 0660 004h]       Virtual GIC Interrupt : 00000018
[298h 0664 008h]  Redistributor Base Address : 0000000000000000
[2A0h 0672 008h]                   ARM MPIDR : 0000000000000700
[2A8h 0680 001h]            Efficiency Class : 01
[2A9h 0681 001h]                    Reserved : 00
[2AAh 0682 002h]      SPE Overflow Interrupt : 0000
[2ACh 0684 002h]              TRBE Interrupt : 190C

[2ACh 0684 001h]               Subtable Type : 0C [Generic Interrupt Distributor]
[2ADh 0685 001h]                      Length : 19
[2AEh 0686 002h]                    Reserved : 0000
[2B0h 0688 004h]       Local GIC Hardware ID : 00000000
[2B4h 0692 008h]                Base Address : 0000000017A00000
[2BCh 0700 004h]              Interrupt Base : 00000000
[2C0h 0704 001h]                     Version : 03
[2C1h 0705 003h]                    Reserved : 000000

[2C5h 0709 001h]               Subtable Type : 0E [Generic Interrupt Redistributor]
[2C6h 0710 001h]                      Length : 10
[2C7h 0711 002h]                    Reserved : 0000
[2C9h 0713 008h]                Base Address : 0000000017A60000
[2D1h 0721 004h]                      Length : 00100000

[2D5h 0725 001h]               Subtable Type : 0D [Generic MSI Frame]
[2D6h 0726 001h]                      Length : 18
[2D7h 0727 002h]                    Reserved : 0000
[2D9h 0729 004h]                MSI Frame ID : 00000000
[2DDh 0733 008h]                Base Address : 0000000017A10000
[2E5h 0741 004h]       Flags (decoded below) : 00000001
                                  Select SPI : 1
[2E9h 0745 002h]                   SPI Count : 0080
[2EBh 0747 002h]                    SPI Base : 0340