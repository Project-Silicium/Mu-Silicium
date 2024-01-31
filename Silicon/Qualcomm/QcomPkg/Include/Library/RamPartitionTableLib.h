#ifndef _RAM_PARTITIONS_LIB_H
#define _RAM_PARTITIONS_LIB_H

//
// SMEM RAM Partition Defines
//
#define SMEM_USABLE_RAM_PARTITION_TABLE 402

#define RAM_NUM_PART_ENTRIES            32

#define RAM_PART_MAGIC1                 0x9DA5E0A8
#define RAM_PART_MAGIC2                 0xAF9EC4E2

#define RAM_PART_SYS_MEMORY             1
#define RAM_PART_SDRAM                  14

#define GENERIC_RAM_BASE                FixedPcdGet64(PcdSystemMemoryBase)
#define RAM_PARTITION_BASE              FixedPcdGet64(PcdRamPartitionBase)

//
// RAM Partition Entry Structure
//
struct RAMPartitionEntry {
  char   Name[16]; 
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
};

//
// RAM Partition Table Structure
//
struct RAMPartitionTable{
  UINT32                   Magic1;
  UINT32                   Magic2;
  UINT32                   Reserved1;
  UINT32                   Reserved2;
  UINT32                   Version;
  UINT32                   NumPartitions;
  struct RAMPartitionEntry RAMPartitionEntry[RAM_NUM_PART_ENTRIES];
};

EFI_STATUS
GetRamPartitions (
  OUT struct RAMPartitionTable **RAMPartitionTable,
  OUT UINT32                    *NumPartitions,
  OUT UINT32                    *Version
);

#endif /* _RAM_PARTITIONS_LIB_H */