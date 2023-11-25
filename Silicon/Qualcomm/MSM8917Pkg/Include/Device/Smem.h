/*
 * Copyright (c) 2009, Google Inc.
 * All rights reserved.
 *
 * Copyright (c) 2009-2018, The Linux Foundation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
#ifndef _SMEM_H_
#define _SMEM_H_

struct SmemProcCommon {
  UINT32 Command;
  UINT32 Status;
  UINT32 Data1;
  UINT32 Data2;
};

struct SmemHeapInfo {
  UINT32 Initialized;
  UINT32 FreeOffset;
  UINT32 HeapRemaining;
  UINT32 Reserved;
};

struct SmemAllocationInfo {
  UINT32 Allocated;
  UINT32 Offset;
  UINT32 Size;
  UINT32 BaseExt;
};

typedef enum {
  PMIC_IS_PM6610,
  PMIC_IS_PM6620,
  PMIC_IS_PM6640,
  PMIC_IS_PM6650,
  PMIC_IS_PM7500,
  PMIC_IS_PANORAMIX,
  PMIC_IS_PM6652,
  PMIC_IS_PM6653,
  PMIC_IS_PM6658,
  PMIC_IS_EPIC,
  PMIC_IS_HAN,
  PMIC_IS_KIP,
  PMIC_IS_WOOKIE,
  PMIC_IS_PM8058,
  PMIC_IS_PM8028,
  PMIC_IS_PM8901,
  PMIC_IS_PM8027 ,
  PMIC_IS_ISL_9519,
  PMIC_IS_PM8921,
  PMIC_IS_PM8018,
  PMIC_IS_PM8015,
  PMIC_IS_PM8014,
  PMIC_IS_PM8821,
  PMIC_IS_PM8038,
  PMIC_IS_PM8922,
  PMIC_IS_PM8917,
  PMIC_IS_INVALID = 0x7fffffff,
} PmicModelTypeA;

typedef enum {
  PMIC_IS_UNKNOWN   = 0,
  PMIC_IS_PM8941    = 1,
  PMIC_IS_PM8841    = 2,
  PMIC_IS_PM8019    = 3,
  PMIC_IS_PM8026    = 4,
  PMIC_IS_PM8110    = 5,
  PMIC_IS_PM8916    = 11,
  PMIC_IS_PM8909    = 13,
  PMIC_IS_PMI8950   = 17,
  PMIC_IS_PMI8994   = 10,
  PMIC_IS_PMI8996   = 19,
  PMIC_IS_PM660     = 27,
  PMIC_IS_PMI632    = 37,
} PmicModelTypeB;

struct SmemBoardInfoV3 {
  UINT32 Format;
  UINT32 MSMID;
  UINT32 MSMVersion;
  CHAR16 BuildID[32];
  UINT32 RawMSMID;
  UINT32 RawMSMVersion;
  UINT32 HWPlatform;
};

struct SmemBoardInfoV4 {
  struct SmemBoardInfoV3 BoardInfoV3;
  UINT32 PlatformVersion;
  UINT32 BufferAlign;                      // Need for 8 bytes alignment while reading from shared memory.
};

struct SmemBoardInfoV5 {
  struct SmemBoardInfoV3 BoardInfoV3;
  UINT32 PlatformVersion;
  UINT32 FusedChip;
};

struct SmemBoardInfoV6 {
  struct SmemBoardInfoV3 BoardInfoV3;
  UINT32 PlatformVersion;
  UINT32 FusedChip;
  UINT32 PlatformSubtype;
  UINT32 BufferAlign;                     // Need for 8 bytes alignment while reading from shared memory.
};

struct SmemBoardInfoV7 {
  struct SmemBoardInfoV3 BoardInfoV3;
  UINT32 PlatformVersion;
  UINT32 FusedChip;
  UINT32 PlatformSubtype;
  UINT32 PmicType;
  UINT32 PmicVersion;
  UINT32 BufferAlign;                     // Need for 8 bytes alignment while reading from shared memory.
};

struct SmemPmicInfo {
  UINT32 PmicType;
  UINT32 PmicVersion;
};

struct SmemBoardInfoV8 {
  struct SmemBoardInfoV3 BoardInfoV3;
  UINT32 PlatformVersion;
  UINT32 FusedChip;
  UINT32 PlatformSubtype;
  struct SmemPmicInfo PmicInfo[3];
};

struct SmemBoardInfoV9 {
  struct SmemBoardInfoV3 BoardInfoV3;
  UINT32 PlatformVersion;
  UINT32 FusedChip;
  UINT32 PlatformSubtype;
  struct SmemPmicInfo PmicInfo[3];
  UINT32 FoundryID;                       // Used as foundry_id only for v9  
};

struct SmemBoardInfoV10 {
  struct SmemBoardInfoV3 BoardInfoV3;
  UINT32 PlatformVersion;
  UINT32 FusedChip;
  UINT32 PlatformSubtype;
  struct SmemPmicInfo PmicInfo[3];
  UINT32 FoundryID;                       // Used as foundry_id only for v9  
  UINT32 ChipSerial;                      // Used as serial number for v10 
};

struct SmemBoardInfoV11 {
  struct SmemBoardInfoV3 BoardInfoV3;
  UINT32 PlatformVersion;
  UINT32 FusedChip;
  UINT32 PlatformSubtype;
  struct SmemPmicInfo PmicInfo[3];       // Depreciated
  UINT32 FoundryID;                      // Used as foundry_id only for v9  
  UINT32 ChipSerial;                     // Used as serial number for v10 
  UINT32 NumPmics;                       // Number of pmics in array 
  UINT32 PmicArrayOffset;                // Offset from base of structure to array of pmic info types 
};

// Chip Information
enum {
  UNKNOWN   = 0,
  MDM9200   = 57,
  MDM9600   = 58,
  MSM8260   = 70,
  MSM8660   = 71,
  APQ8060   = 86,
  MSM8960   = 87,
  MSM7225A  = 88,
  MSM7625A  = 89,
  MSM7227A  = 90,
  MSM7627A  = 91,
  ESM7227A  = 92,
  ESM7225A  = 96,
  ESM7627A  = 97,
  MSM7225AA = 98,
  MSM7625AA = 99,
  ESM7225AA = 100,
  MSM7227AA = 101,
  MSM7627AA = 102,
  ESM7227AA = 103,
  APQ8064   = 109,
  MSM8930   = 116,
  MSM8630   = 117,
  MSM8230   = 118,
  APQ8030   = 119,
  MSM8627   = 120,
  MSM8227   = 121,
  MSM8660A  = 122,
  MSM8260A  = 123,
  APQ8060A  = 124,
  MSM8974   = 126,
  MSM8225   = 127,
  MSM8625   = 129,
  MPQ8064   = 130,
  MSM7225AB = 131,
  MSM7625AB = 132,
  ESM7225AB = 133,
  MDM9625   = 134,
  MSM7125A  = 135,
  MSM7127A  = 136,
  MSM8125A  = 137,
  MSM8960AB = 138,
  APQ8060AB = 139,
  MSM8260AB = 140,
  MSM8660AB = 141,
  MSM8930AA = 142,
  MSM8630AA = 143,
  MSM8230AA = 144,
  MSM8626   = 145,
  MSM8610   = 147,
  MDM9225   = 149,
  MDM9225M  = 150,
  MDM9625M  = 152,
  APQ8064AB = 153, // aka V2 PRIME
  MSM8930AB = 154,
  MSM8630AB = 155,
  MSM8230AB = 156,
  APQ8030AB = 157,
  MSM8226   = 158,
  MSM8826   = 159,
  APQ8030AA = 160,
  MSM8110   = 161,
  MSM8210   = 162,
  MSM8810   = 163,
  MSM8212   = 164,
  MSM8612   = 165,
  MSM8812   = 166,
  MSM8125   = 167,
  MDM9310   = 171,
  APQ8064AA = 172, // aka V2 SLOW_PRIME
  APQ8084   = 178,
  MSM8130   = 179,
  MSM8130AA = 180,
  MSM8130AB = 181,
  MSM8627AA = 182,
  MSM8227AA = 183,
  APQ8074   = 184,
  MSM8274   = 185,
  MSM8674   = 186,
  MDM9635   = 187,
  FSM9900   = 188,
  FSM9905   = 189,
  FSM9955   = 190,
  FSM9950   = 191,
  FSM9915   = 192,
  FSM9910   = 193,
  MSM8974AC = 194,
  MSM8126   = 198,
  APQ8026   = 199,
  MSM8926   = 200,
  MSM8326   = 205,
  MSM8916   = 206,
  MSM8994   = 207,
  APQ8074AA = 208,
  APQ8074AB = 209,
  APQ8074AC = 210,
  MSM8274AA = 211,
  MSM8274AB = 212,
  MSM8274AC = 213,
  MSM8674AA = 214,
  MSM8674AB = 215,
  MSM8674AC = 216,
  MSM8974AA = 217,
  MSM8974AB = 218,
  APQ8028   = 219,
  MSM8128   = 220,
  MSM8228   = 221,
  MSM8528   = 222,
  MSM8628   = 223,
  MSM8928   = 224,
  MSM8510   = 225,
  MSM8512   = 226,
  MSM8936   = 233,
  MDM9640   = 234,
  MSM8939   = 239,
  APQ8036   = 240,
  APQ8039   = 241,
  MSM8236   = 242,
  MSM8636   = 243,
  MSM8909   = 245,
  MSM8996   = 246,
  APQ8016   = 247,
  MSM8216   = 248,
  MSM8116   = 249,
  MSM8616   = 250,
  MSM8992   = 251,
  APQ8092   = 252,
  APQ8094   = 253,
  FSM9008   = 254,
  FSM9010   = 255,
  FSM9016   = 256,
  FSM9055   = 257,
  MSM8209   = 258,
  MSM8208   = 259,
  MDM9209   = 260,
  MDM9309   = 261,
  MDM9609   = 262,
  MSM8239   = 263,
  APQ8009   = 265,
  MSM8952   = 264,
  MSM8956   = 266,
  MSM8929   = 268,
  MSM8629   = 269,
  MSM8229   = 270,
  APQ8029   = 271,
  APQ8056   = 274,
  MSM8609   = 275,
  FSM9916   = 276,
  APQ8076   = 277,
  MSM8976   = 278,
  MDM9650   = 279,
  MDM9655   = 283,
  MDM9250   = 284,
  MDM9255   = 285,
  MDM9350   = 286,
  APQ8052   = 289,
  MDM9607   = 290,
  MDM8207   = 296,
  MDM9207   = 297,
  MDM9307   = 298,
  MDM9628   = 299,
  APQ8096   = 291,
  MSM8953   = 293,
  MSM8937   = 294,
  APQ8037   = 295,
  MSM8940   = 313,
  MSM8996L  = 302,
  MSM8917   = 303,
  MSM8920   = 320,
  APQ8053   = 304,
  MSM8996SG = 305,
  APQ8017   = 307,
  MSM8217   = 308,
  MSM8617   = 309,
  MSM8996AU = 310,
  APQ8096AU = 311,
  APQ8096SG = 312,
  SDX201    = 314,
  SDX202    = 333,
  MSM8909W  = 300,
  APQ8009W  = 301,
  SDM450    = 338,
  MDM9206   = 322,
  SDA450    = 351,
  SDM632    = 349,
  SDA632    = 350,
  SDM429    = 354,
  SDM439    = 353,
  SDA429    = 364,
  SDA439    = 363,
  QM215     = 386
};

enum Platform {
  HW_PLATFORM_UNKNOWN = 0,
  HW_PLATFORM_SURF    = 1,
  HW_PLATFORM_FFA     = 2,
  HW_PLATFORM_FLUID   = 3,
  HW_PLATFORM_SVLTE   = 4,
  HW_PLATFORM_QT      = 6,
  HW_PLATFORM_MTP_MDM = 7,
  HW_PLATFORM_MTP     = 8,
  HW_PLATFORM_LIQUID  = 9,
  HW_PLATFORM_DRAGON  = 10,
  HW_PLATFORM_QRD     = 11,
  HW_PLATFORM_IPC     = 12,
  HW_PLATFORM_HRD     = 13,
  HW_PLATFORM_DTV     = 14,
  HW_PLATFORM_RUMI    = 15,
  HW_PLATFORM_VIRTIO  = 16,
  HW_PLATFORM_BTS     = 19,
  HW_PLATFORM_RCM     = 21,
  HW_PLATFORM_DMA     = 22,
  HW_PLATFORM_STP     = 23,
  HW_PLATFORM_SBC     = 24,
  HW_PLATFORM_ADP     = 25,
  HW_PLATFORM_ATP     = 33,
  HW_PLATFORM_32BITS  = 0x7FFFFFFF,
};

enum PlatformSubtype {
  HW_PLATFORM_SUBTYPE_UNKNOWN           = 0,
  HW_PLATFORM_SUBTYPE_MDM               = 1,
  HW_PLATFORM_SUBTYPE_8974PRO_PM8084    = 1,
  HW_PLATFORM_SUBTYPE_CSFB              = 1,
  HW_PLATFORM_SUBTYPE_SVLTE1            = 2,
  HW_PLATFORM_SUBTYPE_IOT               = 2,
  HW_PLATFORM_SUBTYPE_SAP               = 2,
  HW_PLATFORM_SUBTYPE_SAP_NOPMI         = 3,
  HW_PLATFORM_SUBTYPE_SVLTE2A           = 3,
  HW_PLATFORM_SUBTYPE_MTP_WEAR          = 5,
  HW_PLATFORM_SUBTYPE_SGLTE             = 6,
  HW_PLATFORM_SUBTYPE_DSDA              = 7,
  HW_PLATFORM_SUBTYPE_DSDA2             = 8,
  HW_PLATFORM_SUBTYPE_SGLTE2            = 9,
  HW_PLATFORM_SUBTYPE_SWOC_TP_CIRC      = 12,
  HW_PLATFORM_SUBTYPE_POLARIS           = 64,
  HW_PLATFORM_SUBTYPE_SWOC_WEAR         = 9,
  HW_PLATFORM_SUBTYPE_SWOC_NOWGR_CIRC   = 13,
  HW_PLATFORM_SUBTYPE_8909_PM660        = 15,
  HW_PLATFORM_SUBTYPE_8909_COMPAL_ALPHA = 19,
  HW_PLATFORM_SUBTYPE_8909_PM660_V1     = 18,
  HW_PLATFORM_SUBTYPE_INTRINSIC_SOM     = 20,
  HW_PLATFORM_SUBTYPE_32BITS            = 0x7FFFFFFF
};

typedef enum {
  SMEM_SPINLOCK_ARRAY             = 7,
  SMEM_AARM_PARTITION_TABLE       = 9,
  SMEM_CHANNEL_ALLOC_TBL          = 13,
  SMEM_SMD_BASE_ID                = 14,
  SMEM_APPS_BOOT_MODE             = 106,
  SMEM_BOARD_INFO_LOCATION        = 137,
  SMEM_SMD_FIFO_BASE_ID           = 338,
  SMEM_USABLE_RAM_PARTITION_TABLE = 402,
  SMEM_POWER_ON_STATUS_INFO       = 403,
  SMEM_RLOCK_AREA                 = 404,
  SMEM_BOOT_INFO_FOR_APPS         = 418,
  SMEM_FIRST_VALID_TYPE           = SMEM_SPINLOCK_ARRAY,
  SMEM_LAST_VALID_TYPE            = SMEM_BOOT_INFO_FOR_APPS,
  SMEM_MAX_SIZE                   = SMEM_BOOT_INFO_FOR_APPS + 1,
} SmemMemType;

struct Smem {
  struct SmemProcCommon     ProcCommon[4];
  UINT32 VersionInfo[32];
  struct SmemHeapInfo       HeapInfo;
  struct SmemAllocationInfo AllocationInfo[SMEM_MAX_SIZE];
};

#endif // _SMEM_H_ 