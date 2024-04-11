#ifndef _EFI_TA_H_
#define _EFI_TA_H_

//
// Global GUID for the TA Protocol
//
#define EFI_TA_PROTOCOL_GUID { 0x464F3A3F, 0xEBDF, 0x4233, { 0xB8, 0xC0, 0xC0, 0xD2, 0xA1, 0xC2, 0xB0, 0x06 } }

//
// Declare forward reference to the TA Protocol
//
typedef struct _EFI_TA_PROTOCOL EFI_TA_PROTOCOL;

/**
  This Function Opens the TA Partition.

  @param Unknown1      Unknown Variable.
  @param Unknown2      Unknown Variable.
**/
typedef
VOID
(EFIAPI *EFI_OPEN_PARTITION) (
  UINTN Unknown1,
  UINTN Unknown2
  );

/**
  This Function Closes TA Partitions. (I Guess)
**/
typedef
VOID
(EFIAPI *EFI_CLOSE) (VOID);

/**
  This Function Gets the Unit Size of the Device.

  @param[out] Size     The Unit Size.
**/
typedef
VOID
(EFIAPI *EFI_GET_UNIT_SIZE) (
  UINTN Size
  );

/**
  This Function Reads Data from TA. (I Guess)

  @param Unknown1      Unknown Variable.
  @param Unknown2      Unknown Variable.
  @param Unknown3      Unknown Variable.
**/
typedef
VOID
(EFIAPI *EFI_READ_DATA) (
  UINTN Unknown1,
  UINT8 Unknown2,
  UINTN Unknown3
  );

/**
  This Function Writes Data to TA. (I Guess)

  @param Unknown1      Unknown Variable.
  @param Unknown2      Unknown Variable.
  @param Unknown3      Unknown Variable.
**/
typedef
VOID
(EFIAPI *EFI_WRITE_DATA) (
  UINTN Unknown1,
  UINT8 Unknown2,
  UINTN Unknown3
  );

/**
  This Function Deletes a Unit.

  @param[in] UnitID    The Unit ID to Delete (I Guess).
**/
typedef
VOID
(EFIAPI *EFI_DELETE_UNIT) (
  UINTN UnitID
  );

//
// Define Protocol Functions
//
struct _EFI_TA_PROTOCOL {
  UINT64              Revision;

  // NOTE: Might be in wrong Order.

  EFI_OPEN_PARTITION  OpenPartition;
  EFI_CLOSE           Close;
  EFI_GET_UNIT_SIZE   GetUnitSize;
  EFI_READ_DATA       ReadData;
  EFI_WRITE_DATA      WriteData;
  EFI_DELETE_UNIT     DeleteUnit;

  // NOTE: There are maybe more Functions.
};

extern EFI_GUID gTAProtocolGuid;

#endif /* _EFI_TA_H_ */
