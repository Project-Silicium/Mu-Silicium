/** @file

  Patches NTOSKRNL to not cause a SError when reading/writing ACTLR_EL1
  Patches NTOSKRNL to not cause a SError when reading/writing AMCNTENSET0_EL0
  Patches NTOSKRNL to not cause a bugcheck when attempting to use
  PSCI_MEMPROTECT Due to an issue in QHEE

  Shell Code to patch kernel mode components before NTOSKRNL

  Copyright (c) 2022-2023 DuoWoA authors

  SPDX-License-Identifier: MIT

**/

/*
 * All sorts of defines related to Windows kernel
 * https://www.vergiliusproject.com/
 * Windows SDK/DDK
 */

typedef unsigned long long UINT64;
typedef long long          INT64;
typedef unsigned int       UINT32;
typedef int                INT32;
typedef unsigned short     UINT16;
typedef unsigned short     CHAR16;
typedef short              INT16;
typedef unsigned char      BOOLEAN;
typedef unsigned char      UINT8;
typedef char               CHAR8;
typedef signed char        INT8;

///
/// Unsigned value of native width.  (4 bytes on supported 32-bit processor
/// instructions, 8 bytes on supported 64-bit processor instructions)
///
typedef UINT64 UINTN;

///
/// Signed value of native width.  (4 bytes on supported 32-bit processor
/// instructions, 8 bytes on supported 64-bit processor instructions)
///
typedef INT64 INTN;

///
/// Datum is read-only.
///
#define CONST const

///
/// Datum is scoped to the current file or function.
///
#define STATIC static

///
/// Undeclared type.
///
#define VOID void

//
// Modifiers for Data Types used to self document code.
// This concept is borrowed for UEFI specification.
//

///
/// Datum is passed to the function.
///
#define IN

///
/// Datum is returned from the function.
///
#define OUT

///
/// Passing the datum to the function is optional, and a NULL
/// is passed if the value is not supplied.
///
#define OPTIONAL

//
// 8-bytes unsigned value that represents a physical system address.
//
typedef UINT64 PHYSICAL_ADDRESS;

///
/// LIST_ENTRY structure definition.
///
typedef struct _LIST_ENTRY LIST_ENTRY;

typedef UINT64 EFI_PHYSICAL_ADDRESS;

///
/// _LIST_ENTRY structure definition.
///
struct _LIST_ENTRY {
  LIST_ENTRY *ForwardLink;
  LIST_ENTRY *BackLink;
};

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

enum WinloadContext { ApplicationContext, FirmwareContext };

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

#define ARM64_INSTRUCTION_LENGTH 4
#define ARM64_TOTAL_INSTRUCTION_LENGTH(x) (ARM64_INSTRUCTION_LENGTH * x)

typedef void (*NT_OS_LOADER_ARM64_TRANSFER_TO_KERNEL)(
    VOID *OsLoaderBlock, VOID *KernelAddress);

VOID DoSomething(VOID *OsLoaderBlock, VOID *KernelAddress)
{
  ((NT_OS_LOADER_ARM64_TRANSFER_TO_KERNEL)KernelAddress)(
      OsLoaderBlock, KernelAddress);
}

VOID OslArm64TransferToKernel(VOID *OsLoaderBlock, VOID *KernelAddress)
{
  PLOADER_PARAMETER_BLOCK loaderBlock = (PLOADER_PARAMETER_BLOCK)OsLoaderBlock;

  for (LIST_ENTRY *entry = (&loaderBlock->LoadOrderListHead)->ForwardLink;
       entry != (&loaderBlock->LoadOrderListHead); entry = entry->ForwardLink) {

    PKLDR_DATA_TABLE_ENTRY kernelModule =
        CONTAINING_RECORD(entry, KLDR_DATA_TABLE_ENTRY, InLoadOrderLinks);

    EFI_PHYSICAL_ADDRESS base = (EFI_PHYSICAL_ADDRESS)kernelModule->DllBase;
    UINTN                size = kernelModule->SizeOfImage;

    for (EFI_PHYSICAL_ADDRESS current = base; current < base + size;
         current += sizeof(UINT32)) {
      if ((*(UINT32 *)current & 0xD5381020) == 0xD5381020) { // mrs x?, actlr_el1
        *(UINT32 *)current = (*(UINT32 *)current & ~(0xD5381020)) | 0xD2800000; // movz x?, #0
      }
      else if ((*(UINT32 *)current & 0xD5181020) == 0xD5181020) { // msr actlr_el1, x?
        *(UINT32 *)current = 0xD503201F;      // nop
      }
    }
  }

  DoSomething(OsLoaderBlock, KernelAddress);
}