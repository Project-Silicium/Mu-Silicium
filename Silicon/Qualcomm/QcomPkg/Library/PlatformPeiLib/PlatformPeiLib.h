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

#endif /* _PLATFORM_PEI_LIB_H_ */
