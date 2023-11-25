#ifndef _QCOM_SMEM_H_
#define _QCOM_SMEM_H_

#include <Device/Smem.h>

//
// Global ID for the Smem Protocol
//
#define QCOM_SMEM_PROTOCOL_GUID { 0x7d896303, 0xde4b, 0x43ec, { 0xa0, 0x48, 0x49, 0xff, 0xd2, 0x9d, 0x9e, 0x97 } }

//
// Declare forward referenced to the Smem Protocol
//
typedef struct _QCOM_SMEM_PROTOCOL QCOM_SMEM_PROTOCOL;

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

/**
  This Functions Writes the Smem Allocation Entry of the Memory Type.

  @param MemType         The Smem Memory Type.
  @param Buffer          Structure of a Device.
  @param Size            The Size of the Entry.

  @return EFI_SUCCESS    The Smem Allocation Entry has been Writen Successfully.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_WRITE_SMEM_ALLOCATION_ENTRY)(
  SmemMemType MemType,
  VOID       *Buffer,
  UINT32      Size
  );

/**
  This Functions Reads the Smem Allocation Offset of the Memory Type.

  @param MemType         The Smem Memory Type.
  @param Buffer          Structure of a Device.
  @param Len             The Size of the Structure.
  @param Offset          The Offset of the Memory Type.

  @return EFI_SUCCESS    The Smem Allocation Offset has been Read Successfully.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_READ_SMEM_ALLOCATION_OFFSET)(
  SmemMemType MemType,
  VOID       *Buffer,
  UINT32      Len,
  UINT32      Offset
  );

//
// Define Protocol Functions
//
struct _QCOM_SMEM_PROTOCOL {
  EFI_READ_SMEM_ALLOCATION_ENTRY  ReadSmemAllocationEntry;
  EFI_GET_SMEM_ALLOCATION_ENTRY   GetSmemAllocationEntry;
  EFI_WRITE_SMEM_ALLOCATION_ENTRY WriteSmemAllocationEntry;
  EFI_READ_SMEM_ALLOCATION_OFFSET ReadSmemAllocationOffset;
};

extern EFI_GUID gQcomSmemProtocolGuid;

#endif /* _QCOM_SMEM_H_ */