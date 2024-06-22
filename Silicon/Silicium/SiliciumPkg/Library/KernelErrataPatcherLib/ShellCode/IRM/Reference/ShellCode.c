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
  LIST_ENTRY                    InLoadOrderLinks;
  VOID                         *ExceptionTable;
  UINT32                        ExceptionTableSize;
  VOID                         *GpValue;
  struct _NON_PAGED_DEBUG_INFO *NonPagedDebugInfo;
  VOID                         *DllBase;
  VOID                         *EntryPoint;
  UINT32                        SizeOfImage;
  UNICODE_STRING                FullDllName;
  UNICODE_STRING                BaseDllName;
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
  UINT32     OsMajorVersion;
  UINT32     OsMinorVersion;
  UINT32     Size;
  UINT32     OsLoaderSecurityVersion;
  LIST_ENTRY LoadOrderListHead;
  LIST_ENTRY MemoryDescriptorListHead;
  LIST_ENTRY BootDriverListHead;
  LIST_ENTRY EarlyLaunchListHead;
  LIST_ENTRY CoreDriverListHead;
  LIST_ENTRY CoreExtensionsDriverListHead;
  LIST_ENTRY TpmCoreDriverListHead;
} LOADER_PARAMETER_BLOCK, *PLOADER_PARAMETER_BLOCK;

#define CONTAINING_RECORD(address, type, field)                                \
  ((type *)((char *)(address) - (unsigned long long)(&((type *)0)->field)))

#define ARM64_INSTRUCTION_LENGTH 4
#define ARM64_TOTAL_INSTRUCTION_LENGTH(x) (ARM64_INSTRUCTION_LENGTH * x)
#define ARM64_BRANCH_LOCATION_INSTRUCTION(CurrentOffset, TargetOffset)         \
  (0x94000000u |                                                               \
   ((UINT32)((TargetOffset - CurrentOffset) / ARM64_INSTRUCTION_LENGTH) &      \
    0x7FFFFFFu))

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

  EFI_PHYSICAL_ADDRESS GICINSTR = 0;
  EFI_PHYSICAL_ADDRESS REALESTATEFUNC = 0;

  for (LIST_ENTRY *entry = (&loaderBlock->LoadOrderListHead)->ForwardLink;
       entry != (&loaderBlock->LoadOrderListHead); entry = entry->ForwardLink) {

    PKLDR_DATA_TABLE_ENTRY kernelModule =
        CONTAINING_RECORD(entry, KLDR_DATA_TABLE_ENTRY, InLoadOrderLinks);

    EFI_PHYSICAL_ADDRESS base = (EFI_PHYSICAL_ADDRESS)kernelModule->DllBase;
    UINTN                size = kernelModule->SizeOfImage;

    for (EFI_PHYSICAL_ADDRESS current = base; current < base + size;
         current += sizeof(UINT32)) {
      if (*(UINT32 *)current == 0xD518CBAA) { // msr icc_sgi1r_el1, x10
        GICINSTR = current;
      }

      if (*(UINT64 *)current == 0x0009001600090003) {
        REALESTATEFUNC = current;
      }

      if (GICINSTR != 0 && REALESTATEFUNC != 0) {
        break;
      }
    }
  }

  if (GICINSTR != 0 && REALESTATEFUNC != 0) {
        UINT64 CodeLocation = REALESTATEFUNC - ARM64_TOTAL_INSTRUCTION_LENGTH(42); // Arbitrary, function is huge
        *(UINT32 *)GICINSTR = ARM64_BRANCH_LOCATION_INSTRUCTION(GICINSTR, CodeLocation);

        *(UINT64 *)(CodeLocation)                                      = 0x910003FDA9BF7BFD; // stp x29, x30, [sp, #-0x10]! | mov x29, sp
        *(UINT64 *)(CodeLocation + ARM64_TOTAL_INSTRUCTION_LENGTH(2))  = 0x9257F94AB640024A; // tbz x10, #0x28, #0x50       | and x10, x10, #0xfffffeffffffffff
        *(UINT64 *)(CodeLocation + ARM64_TOTAL_INSTRUCTION_LENGTH(4))  = 0xAA0A03F6B240014A; // orr x10, x10, #1            | mov x22, x10
        *(UINT64 *)(CodeLocation + ARM64_TOTAL_INSTRUCTION_LENGTH(6))  = 0x92780D29D53800A9; // mrs x9, mpidr_el1           | and x9, x9, #0xf00
        *(UINT64 *)(CodeLocation + ARM64_TOTAL_INSTRUCTION_LENGTH(8))  = 0x52800115D2800014; // movz x20, #0                | movz w21, #0x8
        *(UINT64 *)(CodeLocation + ARM64_TOTAL_INSTRUCTION_LENGTH(10)) = 0x540000A0EB49229F; // cmp x20, x9, lsr #8         | b.eq #0x40
        *(UINT64 *)(CodeLocation + ARM64_TOTAL_INSTRUCTION_LENGTH(12)) = 0xAA0802CAD370BE88; // lsl x8, x20, #0x10          | orr x10, x22, x8
        *(UINT64 *)(CodeLocation + ARM64_TOTAL_INSTRUCTION_LENGTH(14)) = 0xD5033F9FD518CBAA; // msr icc_sgi1r_el1, x10      | dsb sy
        *(UINT64 *)(CodeLocation + ARM64_TOTAL_INSTRUCTION_LENGTH(16)) = 0x510006B591000694; // add x20, x20, #1            | sub w21, w21, #1
        *(UINT64 *)(CodeLocation + ARM64_TOTAL_INSTRUCTION_LENGTH(18)) = 0x1400000235FFFF15; // cbnz w21, #0x28             | b #0x54
        *(UINT64 *)(CodeLocation + ARM64_TOTAL_INSTRUCTION_LENGTH(20)) = 0xA8C17BFDD518CBAA; // msr icc_sgi1r_el1, x10      | ldp x29, x30, [sp], #0x10
        *(UINT32 *)(CodeLocation + ARM64_TOTAL_INSTRUCTION_LENGTH(22)) = 0xD65F03C0;         // ret
  }

  DoSomething(OsLoaderBlock, KernelAddress);
}