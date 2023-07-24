/** @file -- MsWheaEarlyStorageLib.c

This header defines APIs to utilize special memory for MsWheaReport during
early stage.

Copyright (C) Microsoft Corporation. All rights reserved.
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi/UefiBaseType.h>

#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MsWheaEarlyStorageLib.h>
#include <Library/MemoryMapHelperLib.h>

ARM_MEMORY_REGION_DESCRIPTOR_EX LogBuffer;

#define MS_WHEA_EARLY_STORAGE_OFFSET  0x40

#define MS_WHEA_EARLY_STORAGE_HEADER_SIZE  (sizeof(MS_WHEA_EARLY_STORAGE_HEADER))
#define MS_WHEA_EARLY_STORAGE_DATA_OFFSET  MS_WHEA_EARLY_STORAGE_HEADER_SIZE

/**

This routine has the highest privilege to read any byte(s) on the CMOS

@param[in]  Ptr                       The pointer to hold read data
@param[in]  Size                      The size of intended read data
@param[in]  Offset                    The offset of read data, starting from the beginning of CMOS

@retval EFI_SUCCESS                   Operation is successful
@retval EFI_INVALID_PARAMETER         Null pointer or zero or over length request detected

**/
STATIC
EFI_STATUS
__MsWheaCMOSRawRead (
  VOID   *Ptr,
  UINT8  Size,
  UINT8  Offset
  )
{
  UINT8       mIndex;
  UINT8       i;
  UINT8       *mBuf;
  EFI_STATUS  Status;

  mBuf = Ptr;
  if ((mBuf == NULL) ||
      (Size == 0) ||
      ((UINT8)(PcdGet32 (PcdMsWheaReportEarlyStorageCapacity) - Size) < Offset))
  {
    Status = EFI_INVALID_PARAMETER;
    goto Cleanup;
  }

  if (LogBuffer.Address == 0) {
    LocateMemoryMapAreaByName("Log Buffer", &LogBuffer);
  }

  if (LogBuffer.Address == 0) {
    Status = EFI_INVALID_PARAMETER;
    goto Cleanup;
  }

  for (i = 0; i < Size; i++) {
    mIndex = Offset + i;
    mBuf[i] = MmioRead8(LogBuffer.Address + mIndex);
  }

  Status = EFI_SUCCESS;

Cleanup:
  return Status;
}

/**

This routine has the highest privilege to write any byte(s) on the CMOS

@param[in]  Ptr                       The pointer to hold intended written data
@param[in]  Size                      The size of intended written data
@param[in]  Offset                    The offset of written data, starting from the beginning of CMOS

@retval EFI_SUCCESS                   Operation is successful
@retval EFI_INVALID_PARAMETER         Null pointer or zero or over length request detected

**/
STATIC
EFI_STATUS
__MsWheaCMOSRawWrite (
  VOID   *Ptr,
  UINT8  Size,
  UINT8  Offset
  )
{
  UINT8       mIndex;
  UINT8       i;
  UINT8       *mBuf;
  EFI_STATUS  Status;

  mBuf = Ptr;
  if ((mBuf == NULL) ||
      (Size == 0) ||
      ((UINT8)(PcdGet32 (PcdMsWheaReportEarlyStorageCapacity) - Size) < Offset))
  {
    Status = EFI_INVALID_PARAMETER;
    goto Cleanup;
  }

  for (i = 0; i < Size; i++) {
    mIndex = Offset + i;
    MmioWrite8(LogBuffer.Address + mIndex, mBuf[i]);
  }

  Status = EFI_SUCCESS;

Cleanup:
  return Status;
}

/**

This routine has the highest privilege to 'clear' any byte(s) on the CMOS

@param[in]  Size                      The size of intended clear region
@param[in]  Offset                    The offset of clear data, starting from the beginning of CMOS

@retval EFI_SUCCESS                   Operation is successful
@retval EFI_INVALID_PARAMETER         Null pointer or zero or over length request detected

**/
STATIC
EFI_STATUS
__MsWheaCMOSRawClear (
  UINT8  Size,
  UINT8  Offset
  )
{
  UINT8       mIndex;
  UINT8       i;
  EFI_STATUS  Status;

  if ((Size == 0) ||
      ((UINT8)(PcdGet32 (PcdMsWheaReportEarlyStorageCapacity) - Size) < Offset))
  {
    Status = EFI_INVALID_PARAMETER;
    goto Cleanup;
  }

  for (i = 0; i < Size; i++) {
    mIndex = Offset + i;
    MmioWrite8(LogBuffer.Address + mIndex, PcdGet8 (PcdMsWheaEarlyStorageDefaultValue));
  }

  Status = EFI_SUCCESS;
Cleanup:
  return Status;
}

/**

This routine returns the maximum number of bytes that can be stored in the early storage area.

@retval Count    The maximum number of bytes that can be stored in the MS WHEA store.

**/
UINT8
EFIAPI
MsWheaEarlyStorageGetMaxSize (
  VOID
  )
{
  return (UINT8)((PcdGet32 (PcdMsWheaReportEarlyStorageCapacity) - (MS_WHEA_EARLY_STORAGE_OFFSET)) & 0xFF);
}

/**

This routine reads the specified data region from the MS WHEA store.

@param[in]  Ptr                       The pointer to hold intended read data
@param[in]  Size                      The size of intended read data
@param[in]  Offset                    The offset of read data, ranging from 0 to PcdMsWheaReportEarlyStorageCapacity

@retval EFI_SUCCESS                   Operation is successful
@retval EFI_INVALID_PARAMETER         Null pointer or zero or over length request detected
@retval EFI_UNSUPPORTED               The function is unimplemented

**/
EFI_STATUS
EFIAPI
MsWheaEarlyStorageRead (
  VOID   *Ptr,
  UINT8  Size,
  UINT8  Offset
  )
{
  EFI_STATUS  Status;

  if (Offset >= MsWheaEarlyStorageGetMaxSize ()) {
    Status = EFI_INVALID_PARAMETER;
    goto Cleanup;
  }

  Status = __MsWheaCMOSRawRead (Ptr, Size, MS_WHEA_EARLY_STORAGE_OFFSET + Offset);

Cleanup:
  return Status;
}

/**

This routine writes the specified data region from the MS WHEA store.

@param[in]  Ptr                       The pointer to hold intended written data
@param[in]  Size                      The size of intended written data
@param[in]  Offset                    The offset of written data, ranging from 0 to PcdMsWheaReportEarlyStorageCapacity

@retval EFI_SUCCESS                   Operation is successful
@retval EFI_INVALID_PARAMETER         Null pointer or zero or over length request detected
@retval EFI_UNSUPPORTED               The function is unimplemented

**/
EFI_STATUS
EFIAPI
MsWheaEarlyStorageWrite (
  VOID   *Ptr,
  UINT8  Size,
  UINT8  Offset
  )
{
  EFI_STATUS  Status;

  if (Offset >= MsWheaEarlyStorageGetMaxSize ()) {
    Status = EFI_INVALID_PARAMETER;
    goto Cleanup;
  }

  Status = __MsWheaCMOSRawWrite (Ptr, Size, MS_WHEA_EARLY_STORAGE_OFFSET + Offset);

Cleanup:
  return Status;
}

/**

This routine clears the specified data region from the MS WHEA store to PcdMsWheaEarlyStorageDefaultValue.

@param[in]  Size                      The size of intended clear data
@param[in]  Offset                    The offset of clear data, ranging from 0 to PcdMsWheaReportEarlyStorageCapacity

@retval EFI_SUCCESS                   Operation is successful
@retval EFI_INVALID_PARAMETER         Null pointer or zero or over length request detected
@retval EFI_UNSUPPORTED               The function is unimplemented

**/
EFI_STATUS
EFIAPI
MsWheaEarlyStorageClear (
  UINT8  Size,
  UINT8  Offset
  )
{
  EFI_STATUS  Status;

  if (Offset >= MsWheaEarlyStorageGetMaxSize ()) {
    Status = EFI_INVALID_PARAMETER;
    goto Cleanup;
  }

  Status = __MsWheaCMOSRawClear (Size, MS_WHEA_EARLY_STORAGE_OFFSET + Offset);

Cleanup:
  return Status;
}

/**

This is a helper function that returns the maximal capacity for header excluded data.

@retval Count    The maximum number of bytes that can be stored in the MS WHEA store.

**/
UINT8
EFIAPI
MsWheaESGetMaxDataCount (
  VOID
  )
{
  return (UINT8)((MsWheaEarlyStorageGetMaxSize () - (MS_WHEA_EARLY_STORAGE_DATA_OFFSET)) & 0xFF);
}

/**

This routine finds a contiguous memory that has default value of specified size in data region
from the MS WHEA store.

@param[in]  Size                      The size of intended clear data
@param[out] Offset                    The pointer to receive returned offset value, starting from
                                      Early MS_WHEA_EARLY_STORAGE_DATA_OFFSET

@retval EFI_SUCCESS                   Operation is successful
@retval EFI_OUT_OF_RESOURCES          Null pointer or zero or over length request detected
@retval EFI_UNSUPPORTED               The function is unimplemented

**/
EFI_STATUS
EFIAPI
MsWheaESFindSlot (
  IN UINT8  Size,
  IN UINT8  *Offset
  )
{
  EFI_STATUS                    Status = EFI_OUT_OF_RESOURCES;
  MS_WHEA_EARLY_STORAGE_HEADER  Header;

  MsWheaEarlyStorageRead (&Header, MS_WHEA_EARLY_STORAGE_HEADER_SIZE, 0);

  if (Header.ActiveRange + Size <= MsWheaESGetMaxDataCount ()) {
    *Offset = (UINT8)Header.ActiveRange;
    Status  = EFI_SUCCESS;
  }

  return Status;
}

/**

This routine checks the checksum of early storage region: starting from the signature of header to
the last byte of active range (excluding checksum field).

@param[in]  Header                    Whea Early Store Header
@param[out] Checksum                  Checksum of the Whea Early Store

@retval EFI_SUCCESS                   Checksum is now valid
@retval EFI_INVALID_PARAMETER         Checksum or Header were NULL
@retval EFI_BAD_BUFFER_SIZE           Header active range was too large
@retval EFI_UNSUPPORTED               The function is unimplemented

**/
EFI_STATUS
EFIAPI
MsWheaESCalculateChecksum16 (
  MS_WHEA_EARLY_STORAGE_HEADER  *Header,
  UINT16                        *Checksum
  )
{
  UINT16      Data;
  UINT32      Index;
  UINT16      Sum;
  EFI_STATUS  Status;

  DEBUG ((DEBUG_INFO, "%a Calculate sum...\n", __FUNCTION__));

  Status = EFI_SUCCESS;

  if ((Checksum == NULL) || (Header == NULL)) {
    Status = EFI_INVALID_PARAMETER;
    goto Cleanup;
  } else if ((Header->ActiveRange > MsWheaEarlyStorageGetMaxSize ()) ||
             ((Header->ActiveRange & BIT0) != 0))
  {
    Status = EFI_BAD_BUFFER_SIZE;
    goto Cleanup;
  }

  // Clear the checksum field for calculation then restore...
  *Checksum        = Header->Checksum;
  Header->Checksum = 0;
  Sum              = CalculateSum16 ((UINT16 *)Header, MS_WHEA_EARLY_STORAGE_HEADER_SIZE);
  Header->Checksum = *Checksum;

  for (Index = 0; Index < Header->ActiveRange; Index += sizeof (Data)) {
    Status = MsWheaEarlyStorageRead (&Data, (UINT8)sizeof (Data), (UINT8)(MS_WHEA_EARLY_STORAGE_DATA_OFFSET + Index));
    if (EFI_ERROR (Status) != FALSE) {
      DEBUG ((DEBUG_ERROR, "%a: Reading Early Storage %d failed %r\n", __FUNCTION__, Index, Status));
      goto Cleanup;
    }

    Sum = Sum + Data;
  }

  *Checksum = (UINT16)(0x10000 - Sum);

Cleanup:
  return Status;
}

/**
This routine adds an MS_WHEA_EARLY_STORAGE_ENTRY_V0 record to the WHEA early store region. The header
checksum and active range will be updated in the process.

@param[in]  MsWheaEntry             The MS_WHEA_EARLY_STORAGE_ENTRY_V0 to be added

@retval     EFI_SUCCESS             The record was added
@retval     EFI_OUT_OF_RESOURCES    The CMOS ES region is full
@retval     EFI_INVALID_PARAMETER   MsWheaEntry was NULL

**/
STATIC
EFI_STATUS
MsWheaESAddRecordV0Internal (
  IN MS_WHEA_EARLY_STORAGE_ENTRY_V0  *MsWheaEntry
  )
{
  UINT8                         Offset = 0;
  EFI_STATUS                    Status;
  MS_WHEA_EARLY_STORAGE_HEADER  Header;
  UINT16                        Checksum;

  if (MsWheaEntry == NULL) {
    Status = EFI_INVALID_PARAMETER;
    goto Cleanup;
  }

  Status = MsWheaESFindSlot (
             sizeof (MS_WHEA_EARLY_STORAGE_ENTRY_V0),
             &Offset
             );

  if (EFI_ERROR (Status)) {
    goto Cleanup;
  }

  Status = MsWheaEarlyStorageWrite (
             MsWheaEntry,
             sizeof (MS_WHEA_EARLY_STORAGE_ENTRY_V0),
             MS_WHEA_EARLY_STORAGE_DATA_OFFSET + Offset
             );

  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "%a: Clear V0 Early Storage failed at %d %r\n",
      __FUNCTION__,
      Offset,
      Status
      ));

    goto Cleanup;
  }

  Status = MsWheaEarlyStorageRead (
             &Header,
             MS_WHEA_EARLY_STORAGE_HEADER_SIZE,
             0
             );

  if (EFI_ERROR (Status)) {
    goto Cleanup;
  }

  Header.ActiveRange += sizeof (MS_WHEA_EARLY_STORAGE_ENTRY_V0);

  Status = MsWheaESCalculateChecksum16 (
             &Header,
             &Checksum
             );

  if (EFI_ERROR (Status)) {
    goto Cleanup;
  }

  Header.Checksum = Checksum;

  Status = MsWheaEarlyStorageWrite (
             &Header,
             MS_WHEA_EARLY_STORAGE_HEADER_SIZE,
             0
             );

  ZeroMem (
    &Header,
    sizeof (Header)
    );

  Status = MsWheaEarlyStorageRead (
             &Header,
             MS_WHEA_EARLY_STORAGE_HEADER_SIZE,
             0
             );

  if (Header.Checksum != Checksum) {
    DEBUG ((
      DEBUG_ERROR,
      "%a: - Checksum Write Failed. Actual: %d, Expected: %d\n",
      __FUNCTION__,
      Header.Checksum,
      Checksum
      ));
  }

Cleanup:
  return Status;
}

/**
This routine adds an MS_WHEA_EARLY_STORAGE_ENTRY_V0 record to the WHEA early store region using the supplied
metadata. The header checksum and active range will be updated in the process.

@param[in]  UINT32    ErrorStatusValue
@param[in]  UINT64    AdditionalInfo1
@param[in]  UINT64    AdditionalInfo2
@param[in]  EFI_GUID  *ModuleId
@param[in]  EFI_GUID  *PartitionId

@retval     EFI_SUCCESS             The record was added
@retval     EFI_OUT_OF_RESOURCES    The CMOS ES region is full
@retval     EFI_UNSUPPORTED         The function is unimplemented

**/
EFI_STATUS
EFIAPI
MsWheaESAddRecordV0 (
  IN  UINT32    ErrorStatusValue,
  IN  UINT64    AdditionalInfo1,
  IN  UINT64    AdditionalInfo2,
  IN  EFI_GUID  *ModuleId OPTIONAL,
  IN  EFI_GUID  *PartitionId OPTIONAL
  )
{
  MS_WHEA_EARLY_STORAGE_ENTRY_V0  WheaV0;

  ZeroMem (
    &WheaV0,
    sizeof (WheaV0)
    );

  WheaV0.Rev              = MS_WHEA_REV_0;
  WheaV0.ErrorStatusValue = ErrorStatusValue;
  WheaV0.AdditionalInfo1  = AdditionalInfo1;
  WheaV0.AdditionalInfo2  = AdditionalInfo2;

  if (ModuleId != NULL) {
    CopyMem (
      &WheaV0.ModuleID,
      ModuleId,
      sizeof (EFI_GUID)
      );
  } else {
    CopyMem (
      &WheaV0.ModuleID,
      &gEfiCallerIdGuid,
      sizeof (EFI_GUID)
      );
  }

  if (PartitionId != NULL) {
    CopyMem (
      &WheaV0.PartitionID,
      PartitionId,
      sizeof (EFI_GUID)
      );
  }

  return MsWheaESAddRecordV0Internal (&WheaV0);
}
