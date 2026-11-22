/**
  Copyright (c) 2011-2014, ARM Limited. All rights reserved.
  Copyright (c) 2014, Linaro Limited. All rights reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/PcdLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/MemoryMapHelperLib.h>
#include <Library/HobLib.h>

#include "ShimLibraries/EFIShim.h"

#include "PlatformPei.h"

EFI_PHYSICAL_ADDRESS
GetXblSchedulerStubProtocol (
  VOID
  );

// DXE may reach XBLCore enqueue paths; seed empty ready queues.
#define XBL_SCHED_QUEUE_BASE    0xC684B8C0ULL
#define XBL_SCHED_QUEUE_COUNT   32ULL
#define XBL_SCHED_QUEUE_STRIDE  0x40ULL

//
// XBLCore FDT globals are populated directly: its own DTB init reaches
// DebugPrint/lock paths that are unsafe in this boot flow.
//
#define XBL_GDTB_HANDLE_ADDR        0xC684D100ULL
#define XBL_FDT_DESCRIPTOR_PTR      0xC684D0F8ULL
#define XBL_FDT_DTB_HANDLE          0xC684D100ULL
#define XBL_FDT_HANDLE_TABLE        0xC684D9F8ULL
#define XBL_FDT_DESC_DTB_PTR_OFF    0x58ULL
#define XBL_FDT_DESC_DTB_SIZE_OFF   0x60ULL
#define XBL_FDT_DESC_SIZE           0x80U
#define XBL_IMEM_COOKIE_BASE        0x14680000ULL
#define XBL_IMEM_DTB_PTR_OFFSET     0x58ULL
#define XBL_IMEM_DTB_SIZE_OFFSET    0x60ULL
#define XBL_DTB_SCAN_BASE           0x81960000ULL
#define XBL_DTB_SCAN_END            0x81A60000ULL
#define XBL_FD_SCAN_BASE            0xC6800000ULL
#define XBL_FD_SCAN_END             0xC6840000ULL
#define FDT_MAGIC                   0xD00DFEEDU
#define FDT_HEADER_MIN_SIZE         0x28U
#define FDT_MAX_SIZE                0x00100000U

//
// XBLCore code dereferences TPIDR_EL1, but XBLCore never ran here. Seed the
// core-0 thread object and current-thread pointer so those reads are valid.
//
#define XBL_THREAD_BASE             0xC6847BD8ULL
#define XBL_THREAD_STRIDE           0x330ULL
#define XBL_THREAD_SIZE             0x330U
#define XBL_THREAD_CORE             0U
#define XBL_SCHED_LIST_HEAD         0xC684B8A8ULL
#define XBL_THREAD_CURRENT_ADDR     0xC6851268ULL
#define XBL_THREAD_WAITBUF_SIZE     0x100U
#define XBL_THREAD_MAGIC            0x74687264U  // "thrd"
#define XBL_THREAD_WAIT_MAGIC       0x77616974U  // "wait"

STATIC
BOOLEAN
IsSafeXblDtbRange (
  IN EFI_PHYSICAL_ADDRESS  Address,
  IN UINT32                Size
  )
{
  EFI_PHYSICAL_ADDRESS  End;

  if (Size < FDT_HEADER_MIN_SIZE || Size > FDT_MAX_SIZE) {
    return FALSE;
  }

  if (Address < XBL_DTB_SCAN_BASE) {
    return FALSE;
  }

  End = Address + Size;
  if (End < Address) {
    return FALSE;
  }

  // Only MMU-mapped windows are safe to read from PEI.
  if ((Address >= XBL_DTB_SCAN_BASE) && (End <= XBL_DTB_SCAN_END)) {
    return TRUE;
  }

  if ((Address >= XBL_FD_SCAN_BASE) && (End <= XBL_FD_SCAN_END)) {
    return TRUE;
  }

  return FALSE;
}

STATIC
BOOLEAN
IsSafeXblDtbReadAddress (
  IN EFI_PHYSICAL_ADDRESS  Address,
  IN UINT32                Length
  )
{
  EFI_PHYSICAL_ADDRESS  End;

  if (Length == 0) {
    return FALSE;
  }

  End = Address + Length;
  if (End < Address) {
    return FALSE;
  }

  if ((Address >= XBL_DTB_SCAN_BASE) && (End <= XBL_DTB_SCAN_END)) {
    return TRUE;
  }

  if ((Address >= XBL_FD_SCAN_BASE) && (End <= XBL_FD_SCAN_END)) {
    return TRUE;
  }

  return FALSE;
}

STATIC
BOOLEAN
ContainsAsciiToken (
  IN CONST UINT8  *Buffer,
  IN UINTN         BufferSize,
  IN CONST CHAR8  *Token
  )
{
  UINTN  TokenLen;
  UINTN  Index;

  if (Buffer == NULL || Token == NULL) {
    return FALSE;
  }

  TokenLen = AsciiStrLen (Token);
  if (TokenLen == 0 || TokenLen > BufferSize) {
    return FALSE;
  }

  for (Index = 0; Index + TokenLen <= BufferSize; Index++) {
    if (CompareMem (Buffer + Index, Token, TokenLen) == 0) {
      return TRUE;
    }
  }

  return FALSE;
}

STATIC
BOOLEAN
ValidateXblDtbCandidate (
  IN  EFI_PHYSICAL_ADDRESS  Address,
  IN  UINT32                DescriptorSize,
  OUT UINT32                *TotalSize
  )
{
  UINT32  Magic;
  UINT32  Size;

  if (TotalSize != NULL) {
    *TotalSize = 0;
  }

  if (!IsSafeXblDtbReadAddress (Address, sizeof (UINT32) * 2)) {
    return FALSE;
  }

  Magic = SwapBytes32 (*(CONST UINT32 *)(UINTN)Address);
  if (Magic != FDT_MAGIC) {
    return FALSE;
  }

  Size = SwapBytes32 (*(CONST UINT32 *)(UINTN)(Address + 4));
  if (Size < FDT_HEADER_MIN_SIZE || Size > FDT_MAX_SIZE) {
    return FALSE;
  }

  if ((DescriptorSize != 0) && (DescriptorSize < FDT_MAX_SIZE) && (DescriptorSize < Size)) {
    return FALSE;
  }

  if (!IsSafeXblDtbRange (Address, Size)) {
    return FALSE;
  }

  if (!ContainsAsciiToken ((CONST UINT8 *)(UINTN)Address, Size, "mailbox-config")) {
    return FALSE;
  }

  if (TotalSize != NULL) {
    *TotalSize = Size;
  }

  return TRUE;
}

STATIC
BOOLEAN
FindXblDtbInRange (
  IN  EFI_PHYSICAL_ADDRESS  ScanBase,
  IN  EFI_PHYSICAL_ADDRESS  ScanEnd,
  OUT EFI_PHYSICAL_ADDRESS  *FoundAddress,
  OUT UINT32                *FoundSize
  )
{
  EFI_PHYSICAL_ADDRESS  Address;
  UINT32                Size;

  if (FoundAddress != NULL) {
    *FoundAddress = 0;
  }
  if (FoundSize != NULL) {
    *FoundSize = 0;
  }

  if (ScanEnd <= ScanBase || (ScanEnd - ScanBase) < FDT_HEADER_MIN_SIZE) {
    return FALSE;
  }

  for (Address = ScanBase; Address + FDT_HEADER_MIN_SIZE <= ScanEnd; Address += 4) {
    if (SwapBytes32 (*(CONST UINT32 *)(UINTN)Address) != FDT_MAGIC) {
      continue;
    }

    if (ValidateXblDtbCandidate (Address, 0, &Size)) {
      if (FoundAddress != NULL) {
        *FoundAddress = Address;
      }
      if (FoundSize != NULL) {
        *FoundSize = Size;
      }

      return TRUE;
    }
  }

  return FALSE;
}

STATIC
VOID
InitializeXblSchedulerQueues (
  VOID
  )
{
  UINT64  Index;

  for (Index = 0; Index < XBL_SCHED_QUEUE_COUNT; Index++) {
    UINT64 *Node;

    Node = (UINT64 *)(UINTN)(XBL_SCHED_QUEUE_BASE + (Index * XBL_SCHED_QUEUE_STRIDE));
    Node[0] = (UINT64)(UINTN)Node;
    Node[1] = (UINT64)(UINTN)Node;
  }
}

STATIC
VOID
InitializeXblThreadState (
  VOID
  )
{
  UINT8   *Thread;
  UINT8   *WaitBuf;

  Thread = (UINT8 *)(UINTN)(XBL_THREAD_BASE + (XBL_THREAD_CORE * XBL_THREAD_STRIDE));

  // Seed the core-0 thread object with XBLCore's magic/state fields.
  ZeroMem (Thread, XBL_THREAD_SIZE);
  *(UINT32 *)(Thread + 0x000) = XBL_THREAD_MAGIC;

  *(UINT64 *)(Thread + 0x008) = XBL_SCHED_LIST_HEAD;
  *(UINT64 *)(Thread + 0x010) = XBL_SCHED_LIST_HEAD;
  *(UINT64 *)(UINTN)XBL_SCHED_LIST_HEAD      = (UINT64)(UINTN)(Thread + 0x008);
  *(UINT64 *)(UINTN)(XBL_SCHED_LIST_HEAD + 8) = (UINT64)(UINTN)(Thread + 0x008);

  *(UINT32 *)(Thread + 0x028) = 0x1f;
  *(UINT32 *)(Thread + 0x02c) = 2;    // state: ready
  *(UINT32 *)(Thread + 0x034) = 1;    // flags
  *(UINT32 *)(Thread + 0x038) = XBL_THREAD_CORE;
  *(UINT32 *)(Thread + 0x03c) = XBL_THREAD_CORE;

  // Zeroed wait slots mean "first slot free" for XBLCore's scheduler scan.
  WaitBuf = AllocatePool (XBL_THREAD_WAITBUF_SIZE);
  if (WaitBuf == NULL) {
    DEBUG ((EFI_D_ERROR, "XBL Thread: failed to allocate wait buffer\n"));
    return;
  }
  ZeroMem (WaitBuf, XBL_THREAD_WAITBUF_SIZE);
  *(UINT64 *)(Thread + 0x268) = (UINT64)(UINTN)WaitBuf;
  *(UINT64 *)(Thread + 0x270) = XBL_THREAD_WAITBUF_SIZE;

  *(UINT32 *)(Thread + 0x290) = XBL_THREAD_WAIT_MAGIC;
  *(UINT64 *)(Thread + 0x298) = (UINT64)(UINTN)(Thread + 0x298);
  *(UINT64 *)(Thread + 0x2a0) = (UINT64)(UINTN)(Thread + 0x298);
  *(UINT32 *)(Thread + 0x2a8) = 0;
  *(UINT64 *)(Thread + 0x2d0) = 0;

  // Publish the current-thread pointer used by XBLCore's getter.
  *(UINT64 *)(UINTN)XBL_THREAD_CURRENT_ADDR = (UINT64)(UINTN)Thread;

  // XBLCore dereferences TPIDR_EL1; zero would fault.
  __asm__ volatile (
    "msr tpidr_el1, %0\n\t"
    "isb"
    :
    : "r" ((UINTN)Thread)
    : "memory"
    );
}

STATIC
VOID
InitializeXblDtbHandle (
  VOID
  )
{
  UINT64                     *DtbHandle;
  EFI_PHYSICAL_ADDRESS         Candidate;
  EFI_PHYSICAL_ADDRESS         SelectedAddress;
  UINT32                       DescriptorSize;
  UINT32                       SelectedSize;
  VOID                         *DtbCopy;
  VOID                         *Descriptor;

  DtbHandle = (UINT64 *)(UINTN)XBL_GDTB_HANDLE_ADDR;
  if (*DtbHandle != 0) {
    return;
  }

  SelectedAddress = 0;
  SelectedSize    = 0;

  // IMEM cookie descriptor.
  Candidate      = *(CONST UINT64 *)(UINTN)(XBL_IMEM_COOKIE_BASE + XBL_IMEM_DTB_PTR_OFFSET);
  DescriptorSize = *(CONST UINT32 *)(UINTN)(XBL_IMEM_COOKIE_BASE + XBL_IMEM_DTB_SIZE_OFFSET);

  if ((Candidate != 0) &&
      IsSafeXblDtbReadAddress (Candidate, sizeof (UINT32) * 2) &&
      ValidateXblDtbCandidate (Candidate, DescriptorSize, &SelectedSize)) {
    SelectedAddress = Candidate;
  }

  // Mapped XBL memory window.
  if (SelectedAddress == 0) {
    FindXblDtbInRange (XBL_DTB_SCAN_BASE, XBL_DTB_SCAN_END, &SelectedAddress, &SelectedSize);
  }

  // Early FD fallback for the embedded mailbox DTB.
  if (SelectedAddress == 0) {
    FindXblDtbInRange (XBL_FD_SCAN_BASE, XBL_FD_SCAN_END, &SelectedAddress, &SelectedSize);
  }

  if (SelectedAddress == 0) {
    DEBUG ((EFI_D_ERROR, "XBL DTB: failed to locate mailbox-config FDT; XBLCore FDT state left unset\n"));
    return;
  }

  Descriptor = AllocatePool (XBL_FDT_DESC_SIZE);
  if (Descriptor == NULL) {
    DEBUG ((EFI_D_ERROR, "XBL DTB: failed to allocate XBLCore FDT descriptor\n"));
    return;
  }
  ZeroMem (Descriptor, XBL_FDT_DESC_SIZE);

  // Copy to persistent memory: XBL-owned blobs may be reclaimed in DXE.
  // SEC allocator rejects >64 KB AllocatePool, so use AllocatePages.
  DtbCopy = AllocatePages (EFI_SIZE_TO_PAGES (SelectedSize));
  if (DtbCopy != NULL) {
    CopyMem (DtbCopy, (CONST VOID *)(UINTN)SelectedAddress, SelectedSize);
  } else {
    DtbCopy = (VOID *)(UINTN)SelectedAddress;
  }

  *(UINT64 *)(Descriptor + XBL_FDT_DESC_DTB_PTR_OFF)  = (UINT64)(UINTN)DtbCopy;
  *(UINT64 *)(Descriptor + XBL_FDT_DESC_DTB_SIZE_OFF) = SelectedSize;

  *(UINT64 *)(UINTN)XBL_FDT_DESCRIPTOR_PTR = (UINT64)(UINTN)Descriptor;
  *(UINT64 *)(UINTN)XBL_FDT_DTB_HANDLE     = (UINT64)(UINTN)DtbCopy;
  *(UINT64 *)(UINTN)XBL_FDT_HANDLE_TABLE   = (UINT64)(UINTN)DtbCopy;
  *DtbHandle                              = (UINT64)(UINTN)DtbCopy;

  // Calling fdt_set_blob_handle() faults here (uninitialized XBLCore TLS).
}

EFI_SHIM_LIBRARY_LOADER
ShLibraryLoader = {
  0x00010001,
  ShimInstallLib,
  ShimLoadLib
};

VOID
BuildXblHobs (
  IN EFI_PHYSICAL_ADDRESS SchedulerInterfaceAddr,
  IN EFI_PHYSICAL_ADDRESS DtbExtensionAddr)
{
  EFI_MEMORY_REGION_DESCRIPTOR InfoBlkRegion        = {0};
  UINTN                        ShimLibLoaderAddress = (UINTN)&ShLibraryLoader;
  BOOLEAN                      Prodmode             = FALSE;

  LocateMemoryRegionByName ("Info Blk", &InfoBlkRegion);
  if (InfoBlkRegion.Address == 0) {
    LocateMemoryRegionByName ("Info_Blk", &InfoBlkRegion);
  }

  // Verify Memory Region
  if (!InfoBlkRegion.Address) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate \"Info Blk\" Memory Region!\n"));
  } else {
    // Build Info Blk HOB
    BuildGuidDataHob (&gEfiInfoBlkHobGuid, &InfoBlkRegion.Address, sizeof (InfoBlkRegion.Address));
  }

  // Build Shim HOB
  BuildGuidDataHob (&gEfiShimLibraryHobGuid, &ShimLibLoaderAddress, sizeof (ShimLibLoaderAddress));

  // Build Prodmode HOB
  BuildGuidDataHob (&gEfiProdmodeHobGuid, &Prodmode, sizeof (Prodmode));

  // Must precede DXE drivers that call into XBLCore TLS code.
  InitializeXblThreadState ();

  // Provide XBLCore's raw DTB handle for MailboxDxe.
  InitializeXblDtbHandle ();

  // The real XBLCore scheduler faults when invoked; publish the stub HOB.
  if (SchedulerInterfaceAddr) {
    EFI_PHYSICAL_ADDRESS StubSchedulerAddr = GetXblSchedulerStubProtocol ();

    InitializeXblSchedulerQueues ();

    BuildGuidDataHob (&gEfiSchedulerInterfaceHobGuid, &StubSchedulerAddr, sizeof (StubSchedulerAddr));
  }

  // Build DTB Extension HOB
  if (DtbExtensionAddr) {
    BuildGuidDataHob (&gEfiDtbExtensionHobGuid, &DtbExtensionAddr, sizeof (DtbExtensionAddr));
  }
}

EFI_STATUS
EFIAPI
PlatformPeim ()
{
  // Set Default Values
  EFI_PHYSICAL_ADDRESS SchedulerInterfaceAddr = 0;
  EFI_PHYSICAL_ADDRESS DtbExtensionAddr       = 0;

  // Build FV HOB
  BuildFvHob (PcdGet64 (PcdFvBaseAddress), PcdGet32 (PcdFvSize));

  // Get XBL HOB Addresses
  GetXblHobAddresses (&SchedulerInterfaceAddr, &DtbExtensionAddr);

  // Build XBL HOBs
  BuildXblHobs (SchedulerInterfaceAddr, DtbExtensionAddr);

  return EFI_SUCCESS;
}
