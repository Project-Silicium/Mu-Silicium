#ifndef _EFI_DDR_INFO_H_
#define _EFI_DDR_INFO_H_

//
// Global GUID for the DDR Info Protocol
//
#define EFI_DDR_INFO_PROTOCOL_GUID { 0xBF83E305, 0x8AFE, 0x4BC1, { 0xB1, 0x7D, 0x77, 0x32, 0xDF, 0xC1, 0x20, 0xFF } }

//
// Declare forward reference to the DDR Info Protocol
//
typedef struct _EFI_DDR_INFO_PROTOCOL EFI_DDR_INFO_PROTOCOL;

//
// Memory Manufacturer IDs
//
#define MANUFACTURER_ID_SAMSUNG  1
#define MANUFACTURER_ID_SK_HYNIX 6
#define MANUFACTURER_ID_MICRON   255

//
// Memory Types
//
#define MEMORY_TYPE_LPDDR4  1
#define MEMORY_TYPE_LPDDR4X 2
#define MEMORY_TYPE_LPDDR5  4

//
// DDR Details Table Structure
//
typedef struct ddr_details_table {
  UINT8 ManufacturerId;
  UINT8 MemoryType;
  UINT8 RankNumber;
} DDR_DETAILS_TABLE;

/**
  This Function retrurns the Details of the Memory Chip.

  @return Info                             - The Details of the Memory Chip.
**/
typedef
DDR_DETAILS_TABLE
(EFIAPI *EFI_GET_DDR_DETAILS) ();

/**
  This Function gets the RAM Size of the Memory Chip.

  @return Size                             - The RAM Size of the Memory Chip.
**/
typedef
UINT64
(EFIAPI *EFI_GET_DDR_RAM_SIZE) ();

//
// Define Protocol Functions
//
struct _EFI_DDR_INFO_PROTOCOL {
  EFI_GET_DDR_DETAILS  GetDetails;
  EFI_GET_DDR_RAM_SIZE GetRamSize;
};

extern EFI_GUID gEfiDdrInfoProtocolGuid;

#endif /* _EFI_DDR_INFO_H_ */
