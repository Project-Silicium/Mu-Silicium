/** @file

  Patches NTOSKRNL to not cause a SError when reading/writing ACTLR_EL1
  Patches NTOSKRNL to not cause a SError when reading/writing AMCNTENSET0_EL0
  Patches NTOSKRNL to not cause a bugcheck when attempting to use
  PSCI_MEMPROTECT Due to an issue in QHEE

  Based on https://github.com/SamuelTulach/rainbow

  Copyright (c) 2021 Samuel Tulach
  Copyright (c) 2022-2023 DuoWoA authors

  SPDX-License-Identifier: MIT

**/
#ifndef _NTDEF_H_
#define _NTDEF_H_

/*
 * All sorts of defines related to Windows kernel
 * https://www.vergiliusproject.com/
 * Windows SDK/DDK
 */

typedef struct _UNICODE_STRING {
  UINT16  Length;
  UINT16  MaximumLength;
  CHAR16 *Buffer;
} UNICODE_STRING, *PUNICODE_STRING;

typedef struct _KLDR_DATA_TABLE_ENTRY {
  struct _LIST_ENTRY            InLoadOrderLinks;
  VOID                         *ExceptionTable;
  UINT32                        ExceptionTableSize;
  VOID                         *GpValue;
  struct _NON_PAGED_DEBUG_INFO *NonPagedDebugInfo;
  VOID                         *DllBase;
  VOID                         *EntryPoint;
  UINT32                        SizeOfImage;
  struct _UNICODE_STRING        FullDllName;
  struct _UNICODE_STRING        BaseDllName;
  UINT32                        Flags;
  UINT16                        LoadCount;
  union {
    UINT16 SignatureLevel : 4;
    UINT16 SignatureType : 3;
    UINT16 Unused : 9;
    UINT16 EntireField;
  } u1;
  VOID  *SectionPointer;
  UINT32 CheckSum;
  UINT32 CoverageSectionSize;
  VOID  *CoverageSection;
  VOID  *LoadedImports;
  VOID  *Spare;
  UINT32 SizeOfImageNotRounded;
  UINT32 TimeDateStamp;
} KLDR_DATA_TABLE_ENTRY, *PKLDR_DATA_TABLE_ENTRY;

typedef struct _LOADER_PARAMETER_BLOCK {
  UINT32             OsMajorVersion;
  UINT32             OsMinorVersion;
  UINT32             Size;
  UINT32             OsLoaderSecurityVersion;
  struct _LIST_ENTRY LoadOrderListHead;
  struct _LIST_ENTRY MemoryDescriptorListHead;
  struct _LIST_ENTRY BootDriverListHead;
  struct _LIST_ENTRY EarlyLaunchListHead;
  struct _LIST_ENTRY CoreDriverListHead;
  struct _LIST_ENTRY CoreExtensionsDriverListHead;
  struct _LIST_ENTRY TpmCoreDriverListHead;
} LOADER_PARAMETER_BLOCK, *PLOADER_PARAMETER_BLOCK;

#define CONTAINING_RECORD(address, type, field)                                \
  ((type *)((char *)(address) - (UINT64)(&((type *)0)->field)))

enum WinloadContext { ApplicationContext, FirmwareContext };

typedef struct _IMAGE_FILE_HEADER {
  UINT16 Machine;
  UINT16 NumberOfSections;
  UINT32 TimeDateStamp;
  UINT32 PointerToSymbolTable;
  UINT32 NumberOfSymbols;
  UINT16 SizeOfOptionalHeader;
  UINT16 Characteristics;
} IMAGE_FILE_HEADER, *PIMAGE_FILE_HEADER;

typedef struct _IMAGE_DATA_DIRECTORY {
  UINT32 VirtualAddress;
  UINT32 Size;
} IMAGE_DATA_DIRECTORY, *PIMAGE_DATA_DIRECTORY;

#define IMAGE_NUMBEROF_DIRECTORY_ENTRIES 16

typedef struct _IMAGE_OPTIONAL_HEADER64 {
  UINT16               Magic;
  UINT8                MajorLinkerVersion;
  UINT8                MinorLinkerVersion;
  UINT32               SizeOfCode;
  UINT32               SizeOfInitializedData;
  UINT32               SizeOfUninitializedData;
  UINT32               AddressOfEntryPoint;
  UINT32               BaseOfCode;
  UINT64               ImageBase;
  UINT32               SectionAlignment;
  UINT32               FileAlignment;
  UINT16               MajorOperatingSystemVersion;
  UINT16               MinorOperatingSystemVersion;
  UINT16               MajorImageVersion;
  UINT16               MinorImageVersion;
  UINT16               MajorSubsystemVersion;
  UINT16               MinorSubsystemVersion;
  UINT32               Win32VersionValue;
  UINT32               SizeOfImage;
  UINT32               SizeOfHeaders;
  UINT32               CheckSum;
  UINT16               Subsystem;
  UINT16               DllCharacteristics;
  UINT64               SizeOfStackReserve;
  UINT64               SizeOfStackCommit;
  UINT64               SizeOfHeapReserve;
  UINT64               SizeOfHeapCommit;
  UINT32               LoaderFlags;
  UINT32               NumberOfRvaAndSizes;
  IMAGE_DATA_DIRECTORY DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
} IMAGE_OPTIONAL_HEADER64, *PIMAGE_OPTIONAL_HEADER64;

typedef struct _IMAGE_NT_HEADERS64 {
  UINT32                  Signature;
  IMAGE_FILE_HEADER       FileHeader;
  IMAGE_OPTIONAL_HEADER64 OptionalHeader;
} IMAGE_NT_HEADERS64, *PIMAGE_NT_HEADERS64, IMAGE_NT_HEADERS,
    *PIMAGE_NT_HEADERS;

typedef struct _IMAGE_DOS_HEADER {
  UINT16 e_magic;    // Magic number
  UINT16 e_cblp;     // Bytes on last page of file
  UINT16 e_cp;       // Pages in file
  UINT16 e_crlc;     // Relocations
  UINT16 e_cparhdr;  // Size of header in paragraphs
  UINT16 e_minalloc; // Minimum extra paragraphs needed
  UINT16 e_maxalloc; // Maximum extra paragraphs needed
  UINT16 e_ss;       // Initial (relative) SS value
  UINT16 e_sp;       // Initial SP value
  UINT16 e_csum;     // Checksum
  UINT16 e_ip;       // Initial IP value
  UINT16 e_cs;       // Initial (relative) CS value
  UINT16 e_lfarlc;   // File address of relocation table
  UINT16 e_ovno;     // Overlay number
  UINT16 e_res[4];   // Reserved words
  UINT16 e_oemid;    // OEM identifier (for e_oeminfo)
  UINT16 e_oeminfo;  // OEM information; e_oemid specific
  UINT16 e_res2[10]; // Reserved words
  INT32  e_lfanew;   // File address of new exe header
} IMAGE_DOS_HEADER, *PIMAGE_DOS_HEADER;

#define IMAGE_DOS_SIGNATURE 0x5A4D
#define IMAGE_NT_SIGNATURE 0x00004550

#define IMAGE_DIRECTORY_ENTRY_EXPORT 0

typedef struct _IMAGE_EXPORT_DIRECTORY {
  UINT32 Characteristics;
  UINT32 TimeDateStamp;
  UINT16 MajorVersion;
  UINT16 MinorVersion;
  UINT32 Name;
  UINT32 Base;
  UINT32 NumberOfFunctions;
  UINT32 NumberOfNames;
  UINT32 AddressOfFunctions;
  UINT32 AddressOfNames;
  UINT32 AddressOfNameOrdinals;
} IMAGE_EXPORT_DIRECTORY, *PIMAGE_EXPORT_DIRECTORY;

#endif /* _NTDEF_H_ */