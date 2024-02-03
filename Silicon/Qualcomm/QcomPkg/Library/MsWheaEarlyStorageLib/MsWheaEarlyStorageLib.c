/**
  This Header Defines APIs to Utilize Special Memory for MsWheaReport during Early Stage.

  Copyright (C) Microsoft Corporation. All rights reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/MsWheaEarlyStorageLib.h>
#include <Library/MemoryMapHelperLib.h>

ARM_MEMORY_REGION_DESCRIPTOR_EX LogBuffer;

#define MS_WHEA_EARLY_STORAGE_OFFSET        0x40

#define MS_WHEA_EARLY_STORAGE_HEADER_SIZE  (sizeof(MS_WHEA_EARLY_STORAGE_HEADER))
#define MS_WHEA_EARLY_STORAGE_DATA_OFFSET   MS_WHEA_EARLY_STORAGE_HEADER_SIZE

/**
  This Routine has the Highest Privilege to Read any Byte(s) on the CMOS.

  @param Ptr                      - The Pointer to Hold Read Data.
  @param Size                     - The Size of Intended Read Data.
  @param Offset                   - The Offset of Read Data, Starting from the Beginning of CMOS.

  @return Status                  - The EFI_STATUS returned by this Function.
**/
STATIC
EFI_STATUS
__MsWheaCMOSRawRead (
  VOID *Ptr,
  UINT8 Size,
  UINT8 Offset)
{
  UINT8 *mBuf = Ptr;

  if ((mBuf == NULL) || (Size == 0) || ((UINT8)(PcdGet32 (PcdMsWheaReportEarlyStorageCapacity) - Size) < Offset)) {
    return EFI_INVALID_PARAMETER;
  }

  if (LogBuffer.Address == 0) {
    LocateMemoryMapAreaByName("Log Buffer", &LogBuffer);
  }

  if (LogBuffer.Address == 0) {
    return EFI_UNSUPPORTED;
  }

  for (UINT8 i = 0; i < Size; i++) {
    UINT8 mIndex = Offset + i;
    mBuf[i] = MmioRead8 (LogBuffer.Address + mIndex);
  }

  return EFI_SUCCESS;
}

/**
  This Routine has the Highest Privilege to Write any Byte(s) on the CMOS.

  @param Ptr                      - The Pointer to Hold Write Data.
  @param Size                     - The Size of Intended Write Data.
  @param Offset                   - The Offset of Written Data, Starting from the Beginning of CMOS.

  @return Status                  - The EFI_STATUS returned by this Function.
**/
STATIC
EFI_STATUS
__MsWheaCMOSRawWrite (
  VOID *Ptr,
  UINT8 Size,
  UINT8 Offset)
{
  UINT8 *mBuf = Ptr;

  if ((mBuf == NULL) || (Size == 0) || ((UINT8)(PcdGet32 (PcdMsWheaReportEarlyStorageCapacity) - Size) < Offset)) {
    return EFI_INVALID_PARAMETER;
  }

  for (UINT8 i = 0; i < Size; i++) {
    UINT8 mIndex = Offset + i;
    MmioWrite8 (LogBuffer.Address + mIndex, mBuf[i]);
  }

  return EFI_SUCCESS;
}

/**
  This Routine has the Highest Privilege to 'Clear' any Byte(s) on the CMOS.

  @param Size                     - The Size of Intended Clear Region.
  @param Offset                   - The Offset of Clear Data, Starting from the Beginning of CMOS.

  @return Status                  - The EFI_STATUS returned by this Function.
**/
STATIC
EFI_STATUS
__MsWheaCMOSRawClear (
  UINT8 Size,
  UINT8 Offset)
{
  if ((Size == 0) || ((UINT8)(PcdGet32 (PcdMsWheaReportEarlyStorageCapacity) - Size) < Offset)) {
    return EFI_INVALID_PARAMETER;
  }

  for (UINT8 i = 0; i < Size; i++) {
    UINT8 mIndex = Offset + i;
    MmioWrite8 (LogBuffer.Address + mIndex, PcdGet8 (PcdMsWheaEarlyStorageDefaultValue));
  }

  return EFI_SUCCESS;
}

/**
  This Routine Returns the Maximum Number of Bytes that can be Stored in the Early Storage Area.

  @retval Count                   - The Maximum Number of Bytes that can be Stored in the Ms Whea Store.
**/
UINT8
EFIAPI
MsWheaEarlyStorageGetMaxSize (VOID)
{
  UINT32 Capacity = PcdGet32 (PcdMsWheaReportEarlyStorageCapacity);

  if (Capacity < MS_WHEA_EARLY_STORAGE_OFFSET) {
    ASSERT (Capacity >= MS_WHEA_EARLY_STORAGE_OFFSET);
    return 0;
  }

  return (UINT8)((Capacity - (MS_WHEA_EARLY_STORAGE_OFFSET)) & 0xFF);
}

/**
  This Routine Reads the Specified Data Region from the Ms Whea Store.

  @param Ptr                      - The Pointer to Hold Intended Read Data.
  @param Size                     - The Size of Intended Read Data.
  @param Offset                   - The Offset of Read Data, Ranging from 0 to PcdMsWheaReportEarlyStorageCapacity.

  @return Status                  - The EFI_STATUS returned by this Function.
**/
EFI_STATUS
EFIAPI
MsWheaEarlyStorageRead (
  VOID *Ptr,
  UINT8 Size,
  UINT8 Offset)
{
  EFI_STATUS  Status;

  if (Offset >= MsWheaEarlyStorageGetMaxSize ()) {
    Status = EFI_INVALID_PARAMETER;
    goto exit;
  }

  Status = __MsWheaCMOSRawRead (Ptr, Size, MS_WHEA_EARLY_STORAGE_OFFSET + Offset);

exit:
  return Status;
}

/**
  This Routine Writes the Specified Data Region from the Ms Whea Store.

  @param Ptr                      - The Pointer to Hold Intended Written Data.
  @param Size                     - The Size of Intended Written Data.
  @param Offset                   - The Offset of Written Data, Ranging from 0 to PcdMsWheaReportEarlyStorageCapacity.

  @return Status                  - The EFI_STATUS returned by this Function.
**/
EFI_STATUS
EFIAPI
MsWheaEarlyStorageWrite (
  VOID *Ptr,
  UINT8 Size,
  UINT8 Offset)
{
  EFI_STATUS  Status;

  if (Offset >= MsWheaEarlyStorageGetMaxSize ()) {
    Status = EFI_INVALID_PARAMETER;
    goto exit;
  }

  Status = __MsWheaCMOSRawWrite (Ptr, Size, MS_WHEA_EARLY_STORAGE_OFFSET + Offset);

exit:
  return Status;
}

/**
  This Routine Clears the Specified Data Region from the Ms Whea Store to PcdMsWheaEarlyStorageDefaultValue.

  @param Size                     - The Size of Intended Clear Data.
  @param Offset                   - The Offset of Clear Data, Ranging from 0 to PcdMsWheaReportEarlyStorageCapacity.

  @return Status                  - The EFI_STATUS returned by this Function.
**/
EFI_STATUS
EFIAPI
MsWheaEarlyStorageClear (
  UINT8 Size,
  UINT8 Offset)
{
  EFI_STATUS  Status;

  if (Offset >= MsWheaEarlyStorageGetMaxSize ()) {
    Status = EFI_INVALID_PARAMETER;
    goto exit;
  }

  Status = __MsWheaCMOSRawClear (Size, MS_WHEA_EARLY_STORAGE_OFFSET + Offset);

exit:
  return Status;
}

/**
  This is a Helper Function that Returns the Maximal Capacity for Header Excluded Data.

  @retval Count                   - The Maximum Number of Bytes that can be Stored in the Ms Whea Store.
**/
UINT8
EFIAPI
MsWheaESGetMaxDataCount (VOID)
{
  UINT8 MaxSize = MsWheaEarlyStorageGetMaxSize ();

  if (MaxSize < MS_WHEA_EARLY_STORAGE_DATA_OFFSET) {
    return 0;
  }

  return (UINT8)((MaxSize - (MS_WHEA_EARLY_STORAGE_DATA_OFFSET)) & 0xFF);
}

/**
  This Routine Finds a Contiguous Memory that has Default Value of Specified Size in Data Region from the Ms Whea Store.

  @param[in] Size                 - The Size of Intended Clear Data.
  @param[in] Offset               - The Pointer to Receive Returned Offset Value, Starting from Early MS_WHEA_EARLY_STORAGE_DATA_OFFSET.

  @return Status                  - The EFI_STATUS returned by this Function.
**/
EFI_STATUS
EFIAPI
MsWheaESFindSlot (
  IN UINT8  Size,
  IN UINT8 *Offset)
{
  MS_WHEA_EARLY_STORAGE_HEADER Header;

  MsWheaEarlyStorageRead (&Header, MS_WHEA_EARLY_STORAGE_HEADER_SIZE, 0);

  if (Header.ActiveRange + Size <= MsWheaESGetMaxDataCount ()) {
    *Offset = (UINT8)Header.ActiveRange;
    return EFI_SUCCESS;
  }

  return EFI_OUT_OF_RESOURCES;
}

/**
  This Routine Checks the Checksum of Early Storage Region.

  @param Header                   - Whea Early Store Header.
  @param Checksum                 - Checksum of the Whea Early Store.

  @return Status                  - The EFI_STATUS returned by this Function.
**/
EFI_STATUS
EFIAPI
MsWheaESCalculateChecksum16 (
  MS_WHEA_EARLY_STORAGE_HEADER *Header,
  UINT16                       *Checksum)
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT16     Data   = 0;
  UINT16     Sum    = 0;

  if ((Checksum == NULL) || (Header == NULL)) {
    Status = EFI_INVALID_PARAMETER;
    goto exit;
  } else if ((Header->ActiveRange > MsWheaEarlyStorageGetMaxSize ()) || ((Header->ActiveRange & BIT0) != 0)) {
    Status = EFI_BAD_BUFFER_SIZE;
    goto exit;
  }

  // Clear the Checksum Field for Calculation then Restore
  *Checksum        = Header->Checksum;
  Header->Checksum = 0;
  Sum              = CalculateSum16 ((UINT16 *)Header, MS_WHEA_EARLY_STORAGE_HEADER_SIZE);
  Header->Checksum = *Checksum;

  for (UINT32 Index = 0; Index < Header->ActiveRange; Index += sizeof (Data)) {
    Status = MsWheaEarlyStorageRead (&Data, (UINT8)sizeof (Data), (UINT8)(MS_WHEA_EARLY_STORAGE_DATA_OFFSET + Index));

    if (EFI_ERROR (Status)) { goto exit; }

    Sum = Sum + Data;
  }

  *Checksum = (UINT16)(0x10000 - Sum);

exit:
  return Status;
}

/**
  This Routine Adds an MS_WHEA_EARLY_STORAGE_ENTRY_V0 Record to the Whea Early Store Region.

  @param[in] MsWheaEntry          - The MS_WHEA_EARLY_STORAGE_ENTRY_V0 to be Added.

  @return Status                  - The EFI_STATUS returned by this Function.
**/
STATIC
EFI_STATUS
MsWheaESAddRecordV0Internal (IN MS_WHEA_EARLY_STORAGE_ENTRY_V0 *MsWheaEntry)
{
  EFI_STATUS                   Status   = EFI_SUCCESS;
  MS_WHEA_EARLY_STORAGE_HEADER Header   = NULL;
  UINT16                       Checksum = 0;
  UINT8                        Offset   = 0;

  if (MsWheaEntry == NULL) {
    Status = EFI_INVALID_PARAMETER;
    goto exit;
  }

  Status = MsWheaESFindSlot (sizeof (MS_WHEA_EARLY_STORAGE_ENTRY_V0), &Offset);
  if (EFI_ERROR (Status)) { goto exit; }

  Status = MsWheaEarlyStorageWrite (MsWheaEntry, sizeof (MS_WHEA_EARLY_STORAGE_ENTRY_V0), MS_WHEA_EARLY_STORAGE_DATA_OFFSET + Offset);
  if (EFI_ERROR (Status)) { goto exit; }

  Status = MsWheaEarlyStorageRead (&Header, MS_WHEA_EARLY_STORAGE_HEADER_SIZE, 0);
  if (EFI_ERROR (Status)) { goto exit; }

  Header.ActiveRange += sizeof (MS_WHEA_EARLY_STORAGE_ENTRY_V0);

  Status = MsWheaESCalculateChecksum16 (&Header, &Checksum);
  if (EFI_ERROR (Status)) { goto exit; }

  Header.Checksum = Checksum;

  MsWheaEarlyStorageWrite (&Header, MS_WHEA_EARLY_STORAGE_HEADER_SIZE, 0);

  ZeroMem (&Header, sizeof (Header));

  MsWheaEarlyStorageRead (&Header, MS_WHEA_EARLY_STORAGE_HEADER_SIZE, 0);

exit:
  return Status;
}

EFI_STATUS
EFIAPI
MsWheaESAddRecordV0 (
  IN UINT32    ErrorStatusValue,
  IN UINT64    AdditionalInfo1,
  IN UINT64    AdditionalInfo2,
  IN EFI_GUID *ModuleId         OPTIONAL,
  IN EFI_GUID *PartitionId      OPTIONAL)
{
  MS_WHEA_EARLY_STORAGE_ENTRY_V0  WheaV0;

  ZeroMem (&WheaV0, sizeof (WheaV0));

  WheaV0.Rev              = MS_WHEA_REV_0;
  WheaV0.ErrorStatusValue = ErrorStatusValue;
  WheaV0.AdditionalInfo1  = AdditionalInfo1;
  WheaV0.AdditionalInfo2  = AdditionalInfo2;

  if (ModuleId != NULL) {
    CopyMem (&WheaV0.ModuleID, ModuleId, sizeof (EFI_GUID));
  } else {
    CopyMem (&WheaV0.ModuleID, &gEfiCallerIdGuid, sizeof (EFI_GUID));
  }

  if (PartitionId != NULL) {
    CopyMem (&WheaV0.PartitionID, PartitionId, sizeof (EFI_GUID));
  }

  return MsWheaESAddRecordV0Internal (&WheaV0);
}
