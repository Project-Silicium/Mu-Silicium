/**************************
 * Description:
   Add RAM Partitions.
   Read Ram Partitions Info by EFI_RAMPARTITION_PROTOCOL and add them.


 * Reference Codes
 * abl/edk2/QcomModulePkg/Library/BootLib/Board.c

 - License:
 * Copyright (c) 2015-2018, 2020, The Linux Foundation. All rights reserved.
 * Copyright (c) 2022. Sunflower2333. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * * Redistributions of source code must retain the above copyright
 *  notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided
 *  with the distribution.
 *   * Neither the name of The Linux Foundation nor the names of its
 * contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

**************************/

#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/ArmMmuLib.h>
#include <Library/MemoryMapHelperLib.h>
#include <Library/SortLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DeviceMemoryMapLib.h>

#include <libfdt.h>

BOOLEAN AnyRamPartitionAdded = FALSE;

typedef struct {
  UINT64 Start;
  UINT64 Size;
} MEMORY_NODE;

MEMORY_NODE*
GetMemoryNodes (
  IN  CONST VOID *fdt,
  OUT UINTN      *NodeCount)
{
  MEMORY_NODE *Nodes     = NULL;
  UINTN        Count     = 0;
  INT32        Node      = 0;
  INT32        AddrCells = 0;
  INT32        SizeCells = 0;
  UINTN	       CurrentSize = 0;
  INT32	       Len         = 0;

  // Find Root FDT Node
  Node = fdt_path_offset (fdt, "/");
  if (Node < 0) {
    DEBUG ((EFI_D_ERROR, "Failed to Find FDT Root Node! Status = %a\n", fdt_strerror (Node)));
    return NULL;
  }

  // Get Address & Size Cells
  AddrCells = fdt_address_cells (fdt, Node);
  SizeCells = fdt_size_cells    (fdt, Node);
  if (AddrCells < 0 || SizeCells < 0) {
    DEBUG((EFI_D_ERROR, "Invalid Address & Size Cells!\n"));
    return NULL;
  }

  // Get all Subnodes
  fdt_for_each_subnode (Node, fdt, Node) {
    // Get Device Type
    CONST CHAR8 *DeviceType = fdt_getprop (fdt, Node, "device_type", NULL);

    // Check for Memory Type
    if (DeviceType && !AsciiStrCmp (DeviceType, "memory")) {
      // Get Reg Properties
      CONST UINT32 *Reg = fdt_getprop (fdt, Node, "reg", &Len);

      if (!Reg) {
        DEBUG ((EFI_D_ERROR, "Failed to Read 'reg' Property! Status = %a\n", fdt_strerror(Len)));
        return NULL;
      }

      // Calculate Sizes
      INT32 RegSize = (AddrCells + SizeCells) * sizeof (UINT32);
      INT32 NumRegs = Len / RegSize;
      UINTN NewSize = (Count + NumRegs) * sizeof (MEMORY_NODE);

      // Reallocate Memory
      Nodes = ReallocatePool (CurrentSize, NewSize, Nodes);

      CurrentSize = NewSize;

      // Get Start & Size Addresses
      for (INT32 i = 0; i < NumRegs; i++) {
        UINT64 Start = 0;
        UINT64 Size  = 0;

        for (INT32 j = 0; j < AddrCells; j++) {
          Start = (Start << 32) | fdt32_to_cpu (Reg[i * (AddrCells + SizeCells) + j]);
        }

        for (INT32 j = 0; j < SizeCells; j++) {
          Size = (Size << 32) | fdt32_to_cpu (Reg[i * (AddrCells + SizeCells) + AddrCells + j]);
        }

        Nodes[Count].Start = Start;
        Nodes[Count].Size  = Size;

        Count++;
      }
    }
  }

  *NodeCount = Count;

  return Nodes;
}

VOID
GetReversedMemoryMap (
  ARM_MEMORY_REGION_DESCRIPTOR_EX *ReversedMemoryMap,
  UINTN                            ItemCount)
{
  ARM_MEMORY_REGION_DESCRIPTOR_EX *MemoryMap;
  ARM_MEMORY_REGION_DESCRIPTOR_EX  Temp;
  UINTN                            Start;
  UINTN                            End;

  MemoryMap = GetDeviceMemoryMap ();
  Start     = 0;
  End       = ItemCount - 1;

  for (UINTN i = 0; i < ItemCount; i++) {
    ReversedMemoryMap[i] = MemoryMap[i];
  }

  while (Start < End) {
    Temp = ReversedMemoryMap[Start];

    ReversedMemoryMap[Start] = ReversedMemoryMap[End];
    ReversedMemoryMap[End] = Temp;

    Start++;
    End--;
  }
}

VOID
ReverseMemoryNodes (
  MEMORY_NODE *Nodes,
  UINTN        NodeCount)
{
  for (UINTN i = 0; i < NodeCount / 2; i++) {
    MEMORY_NODE Temp = Nodes[i];

    Nodes[i] = Nodes[NodeCount - 1 - i];
    Nodes[NodeCount - 1 - i] = Temp;
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

  AnyRamPartitionAdded = TRUE;
}

EFI_STATUS
EFIAPI
AddRamPartitions (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS                      Status;
  ARM_MEMORY_REGION_DESCRIPTOR_EX FdtPointer;
  MEMORY_NODE                    *Nodes;
  UINTN                           NodeCount;

  // Get FDT Pointer
  Status = LocateMemoryMapAreaByName ("FDT Pointer", &FdtPointer);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_WARN, "No FDT Pointer Available, Assuming Manual RAM Partitions were Added.\n"));
    return EFI_NOT_FOUND;
  }

  // Get FDT Base Address
  CONST VOID *FDT = (CONST VOID*)(UINTN)MmioRead32 (FdtPointer.Address);

  // Print FDT Location Address
  DEBUG ((EFI_D_WARN, "FDT Location Address = 0x%llx\n", MmioRead32 (FdtPointer.Address)));

  // Get all Memory Nodes
  Nodes = GetMemoryNodes (FDT, &NodeCount);

  // Reserve all Memory Nodes
  ReverseMemoryNodes (Nodes, NodeCount);

  // Add all RAM Partitions
  for (UINTN i = 0; i < NodeCount; i++) {
    PARM_MEMORY_REGION_DESCRIPTOR_EX  MemoryDescriptorEx  = GetDeviceMemoryMap ();
    CHAR8                            *FirstMappedItemName = MemoryDescriptorEx[0].Name;
    BOOLEAN                           IsMapped            = FALSE;

    while (MemoryDescriptorEx->Length != 0) {
      if (MemoryDescriptorEx->Address + MemoryDescriptorEx->Length >= Nodes[i].Start + Nodes[i].Size) {
        IsMapped = TRUE;
        break;
      }

      MemoryDescriptorEx++;
    }

    if (!IsMapped) {
      // Get Memory Map Length
      ARM_MEMORY_REGION_DESCRIPTOR_EX *MemoryMap    = GetDeviceMemoryMap();
      UINTN                            MemMapLength = 1;

      while (MemoryMap->Length != 0) {
        MemMapLength++;
        MemoryMap++;
      }

      // Get Last RAM Partition
      ARM_MEMORY_REGION_DESCRIPTOR_EX LastRAMPartition;
      ARM_MEMORY_REGION_DESCRIPTOR_EX ReversedMemoryMap[MemMapLength];

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

      // Add RAM Partitions
      if (LastRAMPartition.Address + LastRAMPartition.Length > Nodes[i].Start) {
        // Calculate Start & Size Address
        UINT64 NewStartAddr = LastRAMPartition.Address + LastRAMPartition.Length;
        UINT64 NewSize = (Nodes[i].Start + Nodes[i].Size) - NewStartAddr;

        // Add RAM Partition
        AddRamPartition (NewStartAddr, NewSize);
      } else {
        AddRamPartition (Nodes[i].Start, Nodes[i].Size);
      }
    }
  }

  if (!AnyRamPartitionAdded) {
    DEBUG ((EFI_D_ERROR, "No RAM Partitions were Added! Stoping UEFI now.\n"));
    ASSERT_EFI_ERROR (EFI_NOT_READY);
  }

  return EFI_SUCCESS;
}
