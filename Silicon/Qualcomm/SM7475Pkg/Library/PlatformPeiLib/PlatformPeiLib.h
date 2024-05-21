#ifndef _PLATFORM_PEI_LIB_H_
#define _PLATFORM_PEI_LIB_H_

typedef
EFI_STATUS
(*GET_CONFIG_STRING) (
  CHAR8 *Key,
  CHAR8 *Value,
  UINTN *ValBuffSize
  );

typedef
EFI_STATUS
(*GET_CONFIG_VAL) (
  CHAR8  *Key,
  UINT32 *Value
  );

typedef
EFI_STATUS
(*GET_CONFIG_VAL64) (
  CHAR8  *Key,
  UINT64 *Value
  );

typedef
EFI_STATUS
(*GET_MEM_INFO_BY_NAME) (
  CHAR8                           *RegionName,
  ARM_MEMORY_REGION_DESCRIPTOR_EX *MemRegions
  );

typedef
EFI_STATUS
(*GET_MEM_INFO_BY_ADDRESS) (
  UINT64                           RegionBaseAddress,
  ARM_MEMORY_REGION_DESCRIPTOR_EX *MemRegions
  );

typedef struct {
  UINT32                  LibVersion;
  GET_MEM_INFO_BY_NAME    GetMemInfoByName;
  GET_CONFIG_STRING       GetCfgInfoString;
  GET_CONFIG_VAL          GetCfgInfoVal;
  GET_CONFIG_VAL64        GetCfgInfoVal64;
  GET_MEM_INFO_BY_ADDRESS GetMemInfoByAddress;
} UefiCfgLibType;

typedef
UINTN
(*SIO_READ) (
  OUT UINT8 *Buffer,
  IN UINTN   NumberOfBytes
  );

typedef
UINTN
(*SIO_WRITE) (
  IN UINT8 *Buffer,
  IN UINTN  NumberOfBytes
  );

typedef
BOOLEAN
(*SIO_POLL) (VOID);

typedef
UINTN
(*SIO_DRAIN) (VOID);

typedef
UINTN
(*SIO_FLUSH) (VOID);

typedef
UINTN
(*SIO_CONTROL) (
  IN UINTN Arg,
  IN UINTN Param
  );

typedef
EFI_STATUS
(*SIO_SETATTRIBUTES) (
  IN OUT UINT64             *BaudRate,
  IN OUT UINT32             *ReceiveFifoDepth,
  IN OUT UINT32             *Timeout,
  IN OUT EFI_PARITY_TYPE    *Parity,
  IN OUT UINT8              *DataBits,
  IN OUT EFI_STOP_BITS_TYPE *StopBits
  );

typedef struct {
  UINT32            LibVersion;
  SIO_READ          Read;
  SIO_WRITE         Write;
  SIO_POLL          Poll;
  SIO_DRAIN         Drain;
  SIO_FLUSH         Flush;
  SIO_CONTROL       Control;
  SIO_SETATTRIBUTES SetAttributes;
} SioPortLibType;

typedef
EFI_STATUS
(*INSTALL_LIB) (
  IN CHAR8 *LibName,
  IN UINT32 LibVersion,
  IN VOID  *LibIntf
  );

typedef
EFI_STATUS
(*LOAD_LIB) (
  IN  CHAR8 *LibName,
  IN  UINT32 LibVersion,
  OUT VOID **LibIntfPtr
  );

typedef struct {
  UINT32      LoaderVersion;
  INSTALL_LIB InstallLib;
  LOAD_LIB    LoadLib;
} ShLibLoaderType;

typedef struct {
  UINT64 Reversion;
  VOID *SecFdtInitRootHandle;
  VOID *unknow_func0;
  VOID *unknow_func1;
  VOID *dtb_get_reg_info;
  VOID *unknow_func2;
  VOID *SecFdtGetNodeHandle;
  VOID *unknow_func3;
  VOID *unknow_func4;
  VOID *unknow_func5;
  VOID *unknow_func6;
  VOID *unknow_func7;
  VOID *unknow_func8;
  VOID *unknow_func9;
  VOID *unknow_func10;
  VOID *unknow_func11;
  VOID *unknow_func12;
  VOID *unknow_func13;
  VOID *unknow_func14;
  VOID *unknow_func15;
  VOID *unknow_func16;
  VOID *fdt_get_uint32_prop_list;
  VOID *unknow_func17;
  VOID *unknow_func18;
  VOID *unknow_func19;
  VOID *unknow_func20;
  VOID *unknow_func21;
  VOID *unknow_func22;
  VOID *unknow_func23;
  VOID *unknow_func24;
  VOID *unknow_func25;
  VOID *unknow_func26;
  VOID *unknow_func27;
} EFI_DTB_EXTN_PROTOCOL;

#define EFI_SHIM_LIBRARY_GUID                                                  \
  {                                                                            \
    0xbedaeabc, 0x5e70, 0x4d66,                                                \
    {                                                                          \
      0x97, 0x33, 0x21, 0x3d, 0x07, 0x2b, 0x9d, 0x04                           \
    }                                                                          \
  }

#define EFI_INFORMATION_BLOCK_GUID                                             \
  {                                                                            \
    0x90a49afd, 0x422f, 0x08ae,                                                \
    {                                                                          \
      0x96, 0x11, 0xe7, 0x88, 0xd3, 0x80, 0x48, 0x45                           \
    }                                                                          \
  }

#define EFI_FV_DECOMPRESS_GUID                                                 \
  {                                                                            \
    0x12dbd93d, 0x402a, 0x416e,                                                \
    {                                                                          \
      0xec, 0x20, 0x5f, 0x80, 0xcf, 0x5f, 0x7f, 0xe7                           \
    }                                                                          \
  }

#define EFI_SCHED_INTF_GUID                                                    \
  {                                                                            \
    0x8ec2bd8d, 0x56d7, 0x49ef,                                                \
    {                                                                          \
      0x87, 0x96, 0x63, 0x17, 0x78, 0xf8, 0xeb, 0xf8                           \
    }                                                                          \
  }

#define EFI_DTB_EXTN_GUID                                                      \
  {                                                                            \
    0x3A06E1E8, 0x61F6, 0x11EB,                                                \
    {                                                                          \
      0xBB, 0xED, 0x4B, 0x47, 0x6E, 0x2F, 0xF6, 0xA7                           \
    }                                                                          \
  }

#endif /* _PLATFORM_PEI_LIB_H_ */