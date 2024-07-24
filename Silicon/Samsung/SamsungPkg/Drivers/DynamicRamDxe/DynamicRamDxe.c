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
#include <Library/libfdt.h>

BOOLEAN AnyRamPartitionAdded = FALSE;

typedef struct {
  UINT64 StartAddress;
  UINT64 Size;
} MEMORY_NODE;

MEMORY_NODE*
GetMemoryNodes(const void *fdt, UINTN *NodeCount) {
  INT32 Node;
  INT32 AddrCells, SizeCells;
  CONST UINT32 *Reg;
  INT32 Len;
  UINTN Count = 0;
  MEMORY_NODE *Nodes = NULL;
  UINTN CurrentSize = 0;

  Node = fdt_path_offset(fdt, "/");

  if (Node < 0) {
    DEBUG((EFI_D_ERROR, "Error finding root node: %a\n", fdt_strerror(Node)));
    ASSERT(FALSE);
  }

  AddrCells = fdt_address_cells(fdt, Node);
  SizeCells = fdt_size_cells(fdt, Node);
  if (AddrCells < 0 || SizeCells < 0) {
      DEBUG((EFI_D_ERROR, "Error finding address or size cells\n"));
      ASSERT(FALSE);
  }

  fdt_for_each_subnode(Node, fdt, Node) {
    CONST CHAR8 *DeviceType = fdt_getprop(fdt, Node, "device_type", NULL);

    if (DeviceType && AsciiStrCmp(DeviceType, "memory") == 0) {
      Reg = fdt_getprop(fdt, Node, "reg", &Len);

      if (!Reg) {
        DEBUG((EFI_D_ERROR, "Error reading 'reg' property: %a\n", fdt_strerror(Len)));
        ASSERT(FALSE);
      }

      INT32 RegSize = (AddrCells + SizeCells) * sizeof(UINT32);
      INT32 NumRegs = Len / RegSize;
      UINTN NewSize = (Count + NumRegs) * sizeof(MEMORY_NODE);

      Nodes = ReallocatePool(CurrentSize, NewSize, Nodes);
      CurrentSize = NewSize;

      for (INT32 i = 0; i < NumRegs; i++) {
        UINT64 Start = 0, Size = 0;

        for (INT32 j = 0; j < AddrCells; j++) {
          Start = (Start << 32) | fdt32_to_cpu(Reg[i * (AddrCells + SizeCells) + j]);
        }

        for (INT32 j = 0; j < SizeCells; j++) {
          Size = (Size << 32) | fdt32_to_cpu(Reg[i * (AddrCells + SizeCells) + AddrCells + j]);
        }

        Nodes[Count].StartAddress = Start;
        Nodes[Count].Size = Size;
        Count++;
      }
    }
  }

  *NodeCount = Count;
  return Nodes;
}

VOID
ReverseMemoryNodes(MEMORY_NODE *Nodes, UINTN NodeCount) {
  for (UINTN i = 0; i < NodeCount / 2; i++) {
    MEMORY_NODE Temp = Nodes[i];
    Nodes[i] = Nodes[NodeCount - 1 - i];
    Nodes[NodeCount - 1 - i] = Temp;
  }
}

VOID
GetReversedMemoryMap(ARM_MEMORY_REGION_DESCRIPTOR_EX* ReversedMemoryMap, UINTN itemCount) {
  ARM_MEMORY_REGION_DESCRIPTOR_EX* MemoryMap = GetDeviceMemoryMap();

  for (UINTN i = 0; i < itemCount; i++) {
    ReversedMemoryMap[i] = MemoryMap[i];
  }

  UINTN start = 0;
  UINTN end = itemCount - 1;
  ARM_MEMORY_REGION_DESCRIPTOR_EX temp;

  while (start < end) {
    temp = ReversedMemoryMap[start];
    ReversedMemoryMap[start] = ReversedMemoryMap[end];
    ReversedMemoryMap[end] = temp;

    start++;
    end--;
  }
}
VOID
AddRamPartition (
  IN EFI_PHYSICAL_ADDRESS Base,
  IN UINT64               Length,
  IN UINT64               ArmAttributes,
  IN EFI_MEMORY_TYPE      EfiMemoryType)
{
  EFI_STATUS          Status;
  EFI_GCD_MEMORY_TYPE EfiGcdMemoryType;

  if (Length <= 0) { return; }

  switch (EfiMemoryType) {
    case EfiReservedMemoryType:
        EfiGcdMemoryType = EfiGcdMemoryTypeReserved;
        break;

    case EfiConventionalMemory:
        EfiGcdMemoryType = EfiGcdMemoryTypeSystemMemory;
        break;

    default:
        EfiGcdMemoryType = EfiGcdMemoryTypeReserved;
        break;
  }

  // Add New Memory Space
  Status = gDS->AddMemorySpace (EfiGcdMemoryType, Base, Length, 0xF);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Add Memory Space for 0x%08llx 0x%08llx! Status = %r\n", Base, Length, Status));
    return;
  }

  // Set New Memory Attributes
  Status = ArmSetMemoryAttributes (Base, Length, ArmAttributes, 0);
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
  EFI_STATUS                      Status = EFI_SUCCESS;
  ARM_MEMORY_REGION_DESCRIPTOR_EX FdtPointer;

  Status = LocateMemoryMapAreaByName ("FDT", &FdtPointer);
  if (!EFI_ERROR (Status)) {
    CONST VOID *FDT = (CONST VOID*)(uintptr_t)MmioRead32(FdtPointer.Address);
    UINTN NodeCount;
    MEMORY_NODE *Nodes;

    Nodes = GetMemoryNodes(FDT, &NodeCount);
    ReverseMemoryNodes(Nodes, NodeCount);

    for (UINTN i = 0; i < NodeCount; i++) {
      BOOLEAN isMapped = FALSE;

      PARM_MEMORY_REGION_DESCRIPTOR_EX MemoryDescriptorEx = GetDeviceMemoryMap ();
      CHAR8 *FirstMappedItemName = MemoryDescriptorEx[0].Name;

      while (MemoryDescriptorEx->Length != 0) {
          if(MemoryDescriptorEx->Address + MemoryDescriptorEx->Length >= Nodes[i].StartAddress + Nodes[i].Size) {
              DEBUG((EFI_D_ERROR, "Node %d has already been mapped by area %a\n", i, MemoryDescriptorEx->Name));
              isMapped = true;
              break;
          }
          MemoryDescriptorEx++;
      }

          if (!isMapped) {
              // Get memory map length
              UINTN memMapLength = 1;
              ARM_MEMORY_REGION_DESCRIPTOR_EX* MemoryMap = GetDeviceMemoryMap();

              while(MemoryMap->Length != 0)
              {
                memMapLength++;
                MemoryMap++;
              }

              // Get last RAM Partition
              ARM_MEMORY_REGION_DESCRIPTOR_EX LastRAMPartition;
              ARM_MEMORY_REGION_DESCRIPTOR_EX ReversedMemoryMap[memMapLength];
              GetReversedMemoryMap(ReversedMemoryMap, memMapLength);

              for(UINTN j = 0; j < memMapLength; j++) {
                if(AsciiStrCmp(FirstMappedItemName, ReversedMemoryMap[j].Name) == 0) break;
                  if (AsciiStrCmp("RAM Partition", ReversedMemoryMap[j].Name) == 0) {
                    LastRAMPartition = ReversedMemoryMap[j];
                    break;
                  }
              }

              if(LastRAMPartition.Address + LastRAMPartition.Length > Nodes[i].StartAddress) {
                UINT64 NewStartAddr = LastRAMPartition.Address + LastRAMPartition.Length;
                UINT64 NewSize = (Nodes[i].StartAddress + Nodes[i].Size) - NewStartAddr;

                AddRamPartition(NewStartAddr, NewSize, SYS_MEM_CAP, EfiConventionalMemory);
              }
              else {
                    AddRamPartition(Nodes[i].StartAddress, Nodes[i].Size, SYS_MEM_CAP, EfiConventionalMemory);
              }
          }
     }
}

  if (!AnyRamPartitionAdded) {
    DEBUG ((EFI_D_ERROR, "No RAM Partitions were Added! Stoping UEFI now.\n"));
    ASSERT_EFI_ERROR (EFI_NOT_READY);
  }

  return EFI_SUCCESS;
}
