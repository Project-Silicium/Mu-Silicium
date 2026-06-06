#include <Library/RamManagerLib.h>
#include <Library/MemoryMapHelperLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryAllocationLib.h>

#define EMI_CONA 0x00
#define EMI_CONH 0x38

#define EMI_CONA_DATA_WIDTH_32_EN BIT1

#define EMI_CONA_CH_EN_SHIFT 8
#define EMI_CONA_CH_EN_MASK 0x3

typedef struct {
  UINT8 Channel;
  UINT8 Rank;

  UINT8 ColShift;
  UINT8 RowShift;
  UINT8 RowExtShift;

  UINT8 SizeShift;
  UINT8 SizeMask;

  UINT8 EnableBit;
} EMI_RANK_INFO;

STATIC CONST EMI_RANK_INFO mRankInfo[] = {
  {
    .Channel = 0,
    .Rank    = 0,

    .ColShift    = 4,
    .RowShift    = 12,
    .RowExtShift = 24,

    .SizeShift = 16,
    .SizeMask  = 0xF,

    .EnableBit = 0,
  },
  {
    .Channel = 0,
    .Rank    = 1,

    .ColShift    = 6,
    .RowShift    = 14,
    .RowExtShift = 25,

    .SizeShift = 20,
    .SizeMask  = 0xF,

    .EnableBit = 17,
  },
  {
    .Channel = 1,
    .Rank    = 0,

    .ColShift    = 20,
    .RowShift    = 28,
    .RowExtShift = 4,

    .SizeShift = 24,
    .SizeMask  = 0xF,

    .EnableBit = 0,
  },
  {
    .Channel = 1,
    .Rank    = 1,

    .ColShift    = 22,
    .RowShift    = 30,
    .RowExtShift = 5,

    .SizeShift = 28,
    .SizeMask  = 0xF,

    .EnableBit = 16,
  }
};

STATIC
UINT64
GetRankSize (
  IN UINT32               ConA,
  IN UINT32               ConH,
  IN CONST EMI_RANK_INFO *Info,
  IN BOOLEAN              DataWidthIs32Bit)
{
  UINT64 RankSize;
  UINT8  ColBit;
  UINT8  RowBit;

  // Calculate Rank Size by using size if available
  RankSize = (ConH >> Info->SizeShift) & Info->SizeMask;
  if (RankSize != 0) {
    return RankSize * SIZE_256MB;
  }

  // Get Col and Row Bits
  ColBit = ((ConA >> Info->ColShift) & 0x03) + 9;
  RowBit = ((((ConA >> Info->RowExtShift) & 0x01) << 2) + ((ConA >> Info->RowShift) & 0x03)) + 13;

  // Calculate Rank Size by using Col and Row
  return (1ULL << (RowBit + ColBit)) * ((DataWidthIs32Bit ? 4ULL : 2ULL) * 8ULL);
}

EFI_STATUS
GetUsableMemoryRanges (
  OUT EFI_MEMORY_RANGE **Range,
  OUT UINT8             *RangeCount)
{
  EFI_STATUS Status;
  EFI_MEMORY_REGION_DESCRIPTOR EMIRegion;
  UINT32 ConA;
  UINT32 ConH;
  UINT8 EnabledChannels;
  BOOLEAN DataWidthIs32Bit;
  UINT64 TotalSize = 0;
  EFI_MEMORY_RANGE *LocalRange;

  // Get EMI Memory Region
  Status = LocateMemoryRegionByName ("EMI", &EMIRegion);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate EMI Memory Region! Status = %r\n", Status));
    return Status;
  }

  // Read ConA and ConH
  ConA = MmioRead32 (EMIRegion.Address + EMI_CONA);
  ConH = MmioRead32 (EMIRegion.Address + EMI_CONH);

  // Read Enabled Channels and Data Width
  EnabledChannels = 1 << ((ConA >> EMI_CONA_CH_EN_SHIFT) & EMI_CONA_CH_EN_MASK);
  DataWidthIs32Bit = (ConA & EMI_CONA_DATA_WIDTH_32_EN);

  for (UINTN Index = 0; Index < ARRAY_SIZE (mRankInfo); Index++) {
    CONST EMI_RANK_INFO *Info = &mRankInfo[Index];

    // Skip if the channel is disabled
    if (Info->Channel >= EnabledChannels) {
      continue;
    }

    // Skip if the rank is disabled
    if (Info->EnableBit != 0 && !(ConA & (1 << Info->EnableBit))) {
      continue;
    }

    // Get Rank Size
    UINT64 Size = GetRankSize(ConA, ConH, Info, DataWidthIs32Bit);
    if (EnabledChannels >= 4) {
      Size *= 2;
    }

    // Add to Total Size
    TotalSize += Size;
  }

  // Allocate Memory
  LocalRange = AllocateZeroPool (sizeof (EFI_MEMORY_RANGE));
  if (LocalRange == NULL) {
    DEBUG ((EFI_D_ERROR, "Failed to Allocate Memory for usable memory ranges!\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  // MediaTek doesn't have memory holes
  LocalRange->Address = FixedPcdGet64 (PcdSystemMemoryBase);
  LocalRange->Length = TotalSize;

  // Pass Memory Range Details
  *Range = LocalRange;
  *RangeCount = 1;
  return EFI_SUCCESS;
}