#ifndef _RAM_PARTITION_TABLE_LIB_H_
#define _RAM_PARTITION_TABLE_LIB_H_

//
// SMEM RAM Partition Defines
//
#define SMEM_USABLE_RAM_PARTITION_TABLE 402

#define RAM_NUM_PART_ENTRIES            32
#define RAM_PART_MAGIC1                 0x9DA5E0A8
#define RAM_PART_MAGIC2                 0xAF9EC4E2
#define RAM_PART_SYS_MEMORY             1
#define RAM_PART_SDRAM                  14

#define GENERIC_RAM_BASE                FixedPcdGet64 (PcdSystemMemoryBase)
#define RAM_PARTITION_BASE              FixedPcdGet64 (PcdRamPartitionBase)

//
// RAM Partition Entry Structure
//
typedef struct ram_partition_entry {
  CHAR8  Name[16]; 
  UINT64 Base;
  UINT64 Length;
  UINT32 Attribute;
  UINT32 Category;
  UINT32 Domain;
  UINT32 Type;
  UINT32 NumPartitions;
  UINT32 HWInfo;
  UINT8  HighestBankBit;
  UINT8  Reserved0;
  UINT8  Reserved1;
  UINT8  Reserved2;
  UINT32 MinPasrSize;
  UINT64 AvailableLength;
} RamPartitionEntry;

//
// RAM Partition Table Structure
//
typedef struct ram_partition_table {
  UINT32            Magic1;
  UINT32            Magic2;
  UINT32            Version;
  UINT32            Reserved1;
  UINT32            NumPartitions;
  UINT32            Reserved2;
  RamPartitionEntry RamPartitionEntry[RAM_NUM_PART_ENTRIES];
} RamPartitionTable;

EFI_STATUS
GetRamPartitions (
  OUT RamPartitionTable **RamPartTable,
  OUT UINT32             *Version
  );

#endif /* _RAM_PARTITION_TABLE_LIB_H_ */
