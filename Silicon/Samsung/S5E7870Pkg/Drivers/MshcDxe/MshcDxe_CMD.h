/** @file

  Copyright (c) 2012, Samsung Electronics Co. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _MSHCDXE_CMD_H_
#define _MSHCDXE_CMD_H_


#define HosttoCard              (0x1)
#define CardtoHost              (0x0)

#define ENDMA                   (0x1)
#define ENBLKCNT                (0x1 << 1)
#define RD1WT0                  (0x1 << 4)
#define MUL1SIN0                (0x1 << 5)
#define RSPTYP136               (0x1 << 16)
#define RSPTYP48                (0x2 << 16)
#define RSPTYP48B               (0x3 << 16)
#define ENCMDCRC                (0x1 << 19)
#define ENCMDIDX                (0x1 << 20)
#define DATAPRNT                (0x1 << 21)


#define CMDCOMP                 (0x1)
#define TRNSCOMP                (0x1 << 1)
#define RDYFORWT                (0x1 << 4)
#define RDYFORRD                (0x1 << 5)
#define CARDINSERT              (0x1 << 6)
#define CARDREMOVE              (0x1 << 7)
#define ERRINT                  (0x1 << 15)
#define CMDTOUTERR              (0x1 << 16)
#define CMDCRCERR               (0x1 << 17)
#define CMDEBITERR              (0x1 << 18)
#define CMDIDXERR               (0x1 << 19)
#define DATATOUTERR             (0x1 << 20)
#define DATACRCERR              (0x1 << 21)
#define DATAEBITERR             (0x1 << 2)


/* Command Definitions */
#define INDX(CMD_INDX)          (CMD_INDX & 0x3F)

#define CMD0                    INDX(0)
#define CMD0_INT_EN             (CMDCOMP | CMDEBITERR)

#define CMD1                    (INDX(1) | RSPTYP48)
#define CMD1_INT_EN             (CMDCOMP | CMDEBITERR | CMDTOUTERR)

#define CMD2                    (INDX(2) | ENCMDCRC | RSPTYP136)
#define CMD2_INT_EN             (CMDIDXERR | CMDCRCERR | CMDCOMP | CMDEBITERR | CMDTOUTERR)

#define CMD3                    (INDX(3) | ENCMDIDX | ENCMDCRC | RSPTYP48)
#define CMD3_INT_EN             (CMDIDXERR | CMDCRCERR | CMDCOMP | CMDEBITERR | CMDTOUTERR)

#define CMD5                    (INDX(5) | RSPTYP48)
#define CMD5_INT_EN             (CMDCOMP | CMDEBITERR | CMDTOUTERR)

#define CMD7                    (INDX(7) | ENCMDIDX | ENCMDCRC | RSPTYP48)
#define CMD7_INT_EN             (CMDIDXERR | CMDCRCERR | CMDCOMP | CMDEBITERR | CMDTOUTERR)

#define CMD8                    (INDX(8) | ENCMDIDX | ENCMDCRC | RSPTYP48)
#define CMD8_INT_EN             (CMDIDXERR | CMDCRCERR | CMDCOMP | CMDEBITERR | CMDTOUTERR)
#define CMD8_ARG                (0x0UL << 12 | 0x1 << 8 | 0xCEUL)

#define CMD9                    (INDX(9) | ENCMDCRC | RSPTYP136)
#define CMD9_INT_EN             (CMDCRCERR | CMDCOMP | CMDEBITERR | CMDTOUTERR)

#define CMD13                   (INDX(13) | RSPTYP48)
#define CMD13_INT_EN            (CMDCOMP | CMDEBITERR | CMDTOUTERR)

#define CMD16                   (INDX(16) | ENCMDIDX | RSPTYP48)
#define CMD16_INT_EN            (CMDIDXERR | CMDCRCERR | CMDCOMP | CMDEBITERR | CMDTOUTERR)

#define CMD17                   (INDX(17) | DATAPRNT | ENCMDIDX | ENCMDCRC | RSPTYP48 | RD1WT0)
#define CMD17_INT_EN            (CMDIDXERR | CMDCRCERR | CMDCOMP | TRNSCOMP | RDYFORRD | CMDTOUTERR | DATATOUTERR | DATACRCERR | DATAEBITERR | CMDEBITERR)

#define CMD18                   (INDX(18) | DATAPRNT | ENCMDIDX | ENCMDCRC | RSPTYP48 )
#define CMD18_INT_EN            (CMDIDXERR | CMDCRCERR | CMDCOMP | TRNSCOMP | RDYFORRD | CMDTOUTERR | DATATOUTERR | DATACRCERR | DATAEBITERR | CMDEBITERR)

#define CMD23                   (INDX(23) | ENCMDIDX | ENCMDCRC | RSPTYP48)
#define CMD23_INT_EN            (CMDIDXERR | CMDCRCERR | CMDCOMP | CMDEBITERR | CMDTOUTERR)

#define CMD24                   (INDX(24) | DATAPRNT | ENCMDIDX | ENCMDCRC | RSPTYP48)
#define CMD24_INT_EN            (CMDIDXERR | CMDCRCERR | CMDCOMP | TRNSCOMP | RDYFORWT | CMDTOUTERR | DATATOUTERR | DATACRCERR | DATAEBITERR | CMDEBITERR)

#define CMD25                   (INDX(25) | DATAPRNT | ENCMDIDX | ENCMDCRC | RSPTYP48)
#define CMD25_INT_EN            (CMDIDXERR | CMDCRCERR | CMDCOMP | TRNSCOMP | RDYFORWT | CMDTOUTERR | DATATOUTERR | DATACRCERR | DATAEBITERR | CMDEBITERR)

#define CMD35                   (INDX(35) | ENCMDCRC | RSPTYP48)
#define CMD36                   (INDX(36) | ENCMDCRC | RSPTYP48)
#define CMD38                   (INDX(38) | RSPTYP48)

#define CMD55                   (INDX(55) | ENCMDIDX | ENCMDCRC | RSPTYP48)
#define CMD55_INT_EN            (CMDIDXERR | CMDCRCERR | CMDCOMP | CMDEBITERR | CMDTOUTERR)

#define ACMD41                  (INDX(41) | RSPTYP48)
#define ACMD41_INT_EN           (CMDIDXERR | CMDCRCERR | CMDCOMP | CMDEBITERR | CMDTOUTERR)

#define ACMD6                   (INDX(6) | RSPTYP48)
#define ACMD6_INT_EN            (CMDIDXERR | CMDCRCERR | CMDCOMP | CMDEBITERR | CMDTOUTERR)

#define CMD62                   (INDX(62) | RSPTYP48)
#define CMD62_INT_EN            (CMDIDXERR | CMDCRCERR | CMDCOMP | CMDEBITERR | CMDTOUTERR)

#endif
