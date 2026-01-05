#include <Library/MemoryMapLib.h>

STATIC
EFI_MEMORY_REGION_DESCRIPTOR_EX
gMemoryRegionDescriptorEx[] = {
  // Name, Address, Length, HobOption, ResourceType, ResourceAttribute, MemoryType, ArmAttribute

  // Registers regions - Map entire lower 2GB as MMIO like NintendoSwitchPkg
  {"MMIO",               0x00000000, 0x80000000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},

  // DDR Regions (starting at 0x80000000) - ORIGINAL WORKING LAYOUT
  {"UEFI FD",            0x80000000, 0x00200000, AddMem, SYS_MEM, SYS_MEM_CAP, BsData, WRITE_BACK},
  {"CPU Vectors",        0x80200000, 0x00001000, AddMem, SYS_MEM, SYS_MEM_CAP, BsData, WRITE_BACK},
  {"UEFI Stack",         0x80201000, 0x00040000, AddMem, SYS_MEM, SYS_MEM_CAP, BsData, WRITE_BACK},
  {"DXE Heap",           0x80241000, 0x03C00000, AddMem, SYS_MEM, SYS_MEM_CAP, BsData, WRITE_BACK},
  
  // Main RAM Partition - ORIGINAL ADDRESS
  {"RAM Partition",      0x83E81000, 0x71B7F000, AddMem, SYS_MEM, SYS_MEM_CAP, Conv,   WRITE_BACK},
  
  // Display Reserved - ORIGINAL ADDRESS (Hekate sets this, don't change!)
  {"Display Reserved",   0xF5A00000, 0x00384000, AddMem, MEM_RES, SYS_MEM_CAP, Reserv, WRITE_THROUGH},
  
  // High RAM Partition (MMU Bypass fix)
  {"RAM Partition 2",    0xF5D84000, 0x00001000, AddMem, SYS_MEM, SYS_MEM_CAP, Conv,   WRITE_BACK},

  // Terminator for MMU
  {"Terminator", 0, 0, 0, 0, 0, 0, 0}
};

EFI_MEMORY_REGION_DESCRIPTOR_EX*
GetMemoryMap ()
{
  return gMemoryRegionDescriptorEx;
}
