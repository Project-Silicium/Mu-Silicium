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
#ifndef _QCOM_SMEM_H_
#define _QCOM_SMEM_H_

//
// Global ID for the Smem Protocol
//
#define QCOM_SMEM_PROTOCOL_GUID { 0x7d896303, 0xde4b, 0x43ec, { 0xa0, 0x48, 0x49, 0xff, 0xd2, 0x9d, 0x9e, 0x97 } }

//
// Declare forward referenced to the Smem Protocol
//
typedef struct _QCOM_SMEM_PROTOCOL QCOM_SMEM_PROTOCOL;

struct SmemProcCommon {
  UINT32 Command;
  UINT32 Status;
  UINT32 Data1;
  UINT32 Data2;
};

struct SmemHeapInfo {
  BOOLEAN Initialized;
  BOOLEAN FreeOffset;
  BOOLEAN HeapRemaining;
  BOOLEAN Reserved;
};

struct SmemAllocationInfo {
  UINT32 Allocated;
  UINT32 Offset;
  UINT32 BaseExt;
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
  struct SmemHeapInfo       heap_info;
  struct SmemAllocationInfo AllocationInfo[SMEM_MAX_SIZE];
};

/**
  This Functions Reads the Smem Allocation Entry of the Memory Type.

  @param MemType         The Smem Memory Type.
  @param Buffer          Structure of a Device.
  @param Len             The Size of the Structure.

  @return EFI_SUCCESS    The Smem Allocation Entry has been Read Successfully.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_READ_SMEM_ALLOCATION_ENTRY)(
  SmemMemType MemType,
  VOID       *Buffer,
  UINT32      Len
  );

/**
  This Functions Gets the Smem Allocation Entry of the Memory Type.

  @param MemType         The Smem Memory Type.

  @return Pointer        Pointer to the Smem Item.
**/
typedef
VOID*
(EFIAPI *EFI_GET_SMEM_ALLOCATION_ENTRY)(
  SmemMemType MemType
  );

//
// Define Protocol Functions
//
struct _QCOM_SMEM_PROTOCOL {
  EFI_READ_SMEM_ALLOCATION_ENTRY ReadSmemAllocationEntry;
  EFI_GET_SMEM_ALLOCATION_ENTRY  GetSmemAllocationEntry;
};

extern EFI_GUID gQcomSmemProtocolGuid;

#endif /* _QCOM_SMEM_H_ */