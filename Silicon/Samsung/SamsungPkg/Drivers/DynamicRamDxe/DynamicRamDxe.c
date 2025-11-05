/**
  Copyright (c) 2015-2018, 2020, The Linux Foundation. All rights reserved.
  Copyright (c) 2022. Sunflower2333. All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

  - Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.

  - Redistributions in binary form must reproduce the above
    copyright notice, this list of conditions and the following
    disclaimer in the documentation and/or other materials provided
    with the distribution.

  - Neither the name of The Linux Foundation nor the names of its
    contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
  WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
  ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
  BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
  BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
  OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
  IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**/

#include <libfdt.h>

#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryMapLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/MemoryMapHelperLib.h>
#include <Library/ArmMmuLib.h>
#include <Library/IoLib.h>

//
// DTB Memory Node
//
typedef struct memory_node {
  UINT64 Address;
  UINT64 Length;
} MEMORY_NODE;

EFI_STATUS
GetMemoryNodes (
  IN  CONST VOID   *Fdt,
  OUT MEMORY_NODE **MemoryNodes,
  OUT UINTN        *NodeCount)
{
  MEMORY_NODE *Nodes       = NULL;
  UINTN        Count       = 0;
  INT32        Node        = 0;
  INT32        AddrCells   = 0;
  INT32        SizeCells   = 0;
  UINTN	       CurrentSize = 0;
  INT32	       Length      = 0;

  // Find Root FDT Node
  Node = fdt_path_offset (Fdt, "/");
  if (Node < 0) {
    DEBUG ((EFI_D_ERROR, "Failed to Find FDT Root Node! FdtStatus = %a\n", fdt_strerror (Node)));
    return EFI_NOT_FOUND;
  }

  // Get Address & Size Cells
  AddrCells = fdt_address_cells (Fdt, Node);
  SizeCells = fdt_size_cells    (Fdt, Node);
  if (AddrCells < 0 || SizeCells < 0) {
    DEBUG((EFI_D_ERROR, "Invalid Address & Size Cells!\n"));
    return EFI_INVALID_PARAMETER;
  }

  // Go thru each Subnode
  fdt_for_each_subnode (Node, Fdt, Node) {
    // Get Current Node
    CONST CHAR8 *CurrentNode = fdt_getprop (Fdt, Node, "device_type", NULL);

    // Check for memory Node
    if (CurrentNode && !AsciiStrCmp (CurrentNode, "memory")) {
      // Get Address & Size Address
      CONST UINT32 *Register = fdt_getprop (Fdt, Node, "reg", &Length);
      if (!Register) {
        DEBUG ((EFI_D_ERROR, "Failed to Read 'reg' Property! Status = %a\n", fdt_strerror (Length)));
        return EFI_NOT_FOUND;
      }

      // Calculate Sizes
      INT32 RegSize = (AddrCells + SizeCells) * sizeof (UINT32);
      INT32 NumRegs = Length / RegSize;
      UINTN NewSize = (Count + NumRegs) * sizeof (MEMORY_NODE);

      // Reallocate Memory
      Nodes = ReallocatePool (CurrentSize, NewSize, Nodes);

      // Update Current Size
      CurrentSize = NewSize;

      // Calculate Start & Size Addresses
      for (INT32 i = 0; i < NumRegs; i++) {
        UINT64 Address = 0;
        UINT64 Length  = 0;

        // Calculate Start Address
        for (INT32 j = 0; j < AddrCells; j++) {
          Address = (Address << 32) | fdt32_to_cpu (Register[i * (AddrCells + SizeCells) + j]);
        }

        // Calculate Size Address
        for (INT32 j = 0; j < SizeCells; j++) {
          Length = (Length << 32) | fdt32_to_cpu (Register[i * (AddrCells + SizeCells) + AddrCells + j]);
        }

        // Add new Start & Size Address
        Nodes[Count].Address = Address;
        Nodes[Count].Length  = Length;

        // Increase Count
        Count++;
      }
    }
  }

  // Pass Output
  *NodeCount   = Count;
  *MemoryNodes = Nodes;

  return EFI_SUCCESS;
}

VOID
GetReversedMemoryMap (
  OUT EFI_MEMORY_REGION_DESCRIPTOR_EX *ReversedMemoryMap,
  IN  UINTN                            ItemCount)
{
  EFI_MEMORY_REGION_DESCRIPTOR_EX *MemoryMap = GetMemoryMap ();
  EFI_MEMORY_REGION_DESCRIPTOR_EX  Temp      = {0};
  UINTN                            Start     = 0;
  UINTN                            End       = ItemCount - 1;

  // Pass Memory Map
  for (UINTN i = 0; i < ItemCount; i++) {
    ReversedMemoryMap[i] = MemoryMap[i];
  }

  // Rearange Memory Map
  while (Start < End) {
    Temp = ReversedMemoryMap[Start];

    // Set new Start & End Address
    ReversedMemoryMap[Start] = ReversedMemoryMap[End];
    ReversedMemoryMap[End] = Temp;

    Start++;
    End--;
  }
}

VOID
AddRamPartition (
  IN EFI_PHYSICAL_ADDRESS Base,
  IN UINT64               Length)
{
  EFI_STATUS Status;

  if (Length <= 0) { return; }

  // Add New Memory Space
  Status = gDS->AddMemorySpace (EfiGcdMemoryTypeSystemMemory, Base, Length, 0xF);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Add Memory Space for 0x%08llx 0x%08llx! Status = %r\n", Base, Length, Status));
    return;
  }

  // Set New Memory Attributes
  Status = ArmSetMemoryAttributes (Base, Length, EFI_RESOURCE_SYSTEM_MEMORY, 0);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Set Memory Attributes for 0x%08llx 0x%08llx! Status = %r\n", Base, Length, Status));
    return;
  }

  // Set Memory Space Attributes
  Status = gDS->SetMemorySpaceAttributes (Base, Length, EFI_MEMORY_WB);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Set Memory Space Attributes for 0x%08llx 0x%08llx! Status = %r\n", Base, Length, Status));
    return;
  }

  DEBUG ((EFI_D_WARN, "Successfully Added RAM Parition: 0x%08llx 0x%08llx\n", Base, Length));
}

EFI_STATUS
EFIAPI
AddRamPartitions (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS                       Status;
  EFI_MEMORY_REGION_DESCRIPTOR_EX  FdtPointerRegion;
  MEMORY_NODE                     *Nodes;
  UINTN                            NodeCount;

  // Get FDT Pointer
  Status = LocateMemoryMapAreaByName ("FDT Pointer", &FdtPointerRegion);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_WARN, "No FDT Pointer Available, Assuming Manual RAM Partitions were Added.\n"));
    return EFI_NOT_FOUND;
  }

  // Get FDT Base Address
  CONST VOID *Fdt = (CONST VOID*)(UINTN)MmioRead32 (FdtPointerRegion.Address);

  // Get all Memory Nodes
  Status = GetMemoryNodes (Fdt, &Nodes, &NodeCount);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Reserve all Memory Nodes
  for (UINTN i = 0; i < NodeCount / 2; i++) {
    MEMORY_NODE Temp = Nodes[i];

    Nodes[i] = Nodes[NodeCount - 1 - i];
    Nodes[NodeCount - 1 - i] = Temp;
  }

  // Add all RAM Partitions
  for (UINTN i = 0; i < NodeCount; i++) {
    EFI_PMEMORY_REGION_DESCRIPTOR_EX  MemoryDescriptorEx  = GetMemoryMap ();
    CHAR8                            *FirstMappedItemName = MemoryDescriptorEx[0].Name;
    BOOLEAN                           IsMapped            = FALSE;

    // Check if RAM is already Mapped
    while (MemoryDescriptorEx->Length != 0) {
      if (MemoryDescriptorEx->Address + MemoryDescriptorEx->Length >= Nodes[i].Address + Nodes[i].Length) {
        IsMapped = TRUE;
        break;
      }

      MemoryDescriptorEx++;
    }

    // Map RAM Partition
    if (!IsMapped) {
      EFI_MEMORY_REGION_DESCRIPTOR_EX *MemoryMap    = GetMemoryMap ();
      UINTN                            MemMapLength = 1;

      // Skip Mapped Memory
      while (MemoryMap->Length != 0) {
        MemMapLength++;
        MemoryMap++;
      }

      // Get Last RAM Partition
      EFI_MEMORY_REGION_DESCRIPTOR_EX LastRAMPartition;
      EFI_MEMORY_REGION_DESCRIPTOR_EX ReversedMemoryMap[MemMapLength];

      // Get Reserved Memory Map
      GetReversedMemoryMap (ReversedMemoryMap, MemMapLength);

      // Check if Memory is already Mapped
      for (UINTN j = 0; j < MemMapLength; j++) {
        if (!AsciiStrCmp (FirstMappedItemName, ReversedMemoryMap[j].Name)) {
          break;
        }

        if (!AsciiStrCmp ("RAM Partition", ReversedMemoryMap[j].Name)) {
          LastRAMPartition = ReversedMemoryMap[j];
          break;
        }
      }

      if (LastRAMPartition.Address + LastRAMPartition.Length > Nodes[i].Address) {
        // Calculate Start & Size Address
        UINT64 NewStartAddr = LastRAMPartition.Address + LastRAMPartition.Length;
        UINT64 NewSize = (Nodes[i].Address + Nodes[i].Length) - NewStartAddr;

        // Add RAM Partition
        AddRamPartition (NewStartAddr, NewSize);
      } else {
        // Add RAM Partitions
        AddRamPartition (Nodes[i].Address, Nodes[i].Length);
      }
    }
  }

  return EFI_SUCCESS;
}
