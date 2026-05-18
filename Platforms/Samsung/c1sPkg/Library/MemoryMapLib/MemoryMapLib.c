#include <Library/MemoryMapLib.h>

STATIC
EFI_MEMORY_REGION_DESCRIPTOR
gMemoryDescriptor[] = {
  // Name, Address, Length, HobOption, ResourceType, ResourceAttribute, MemoryType, ArmAttribute

  // DDR Regions
  {"EL2 Code",           0xC1400000, 0x00200000, AddMem, SYS_MEM, SYS_MEM_CAP, Reserv, WRITE_BACK},
  {"UEFI FD",            0xE8000000, 0x00200000, AddMem, SYS_MEM, SYS_MEM_CAP, BsData, WRITE_BACK},
  {"UEFI Stack",         0xE8200000, 0x00040000, AddMem, SYS_MEM, SYS_MEM_CAP, BsData, WRITE_BACK},
  {"DXE Heap",           0xF2194000, 0x03C00000, AddMem, SYS_MEM, SYS_MEM_CAP, Conv,   WRITE_BACK},
  {"Display Reserved",   0xF1000000, 0x01194000, AddMem, MEM_RES, SYS_MEM_CAP, Reserv, WRITE_THROUGH},
  {"RAM Partition",      0xF2194000, 0x00001000, AddMem, SYS_MEM, SYS_MEM_CAP, Conv,   WRITE_BACK}, // TEMP: Required for MMU to Pass.

  // iRAM Regions
  {"IRAM",               0x02020000, 0x00018000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   DEVICE},
  {"NS_IRAM",            0x02038000, 0x00039000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},

  // Register Regions
  {"Chip Info",          0x10000000, 0x00001000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"GICD",               0x10101000, 0x00001000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"GICR",               0x10102000, 0x00006000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"Sysreg PERIC0",      0x10420000, 0x00002000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"Pinctrl PERIC0",     0x10430000, 0x00001000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"Sysreg PERIC1",      0x10720000, 0x00003000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"Pinctrl PERIC1",     0x10730000, 0x00001000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"Pinctrl HSI1",       0x13040000, 0x00001000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"Pinctrl HSI2",       0x13C30000, 0x00001000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"Pinctrl VTS",        0x15580000, 0x00001000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"Pinctrl ALIVE",      0x15850000, 0x00001000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"PMU",                0x15860000, 0x00004000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"Speedy 1",           0x15940000, 0x00002000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"Speedy 2",           0x15950000, 0x00002000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"Sysreg CMGP",        0x15C20000, 0x00003000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"Pinctrl CMGP",       0x15C30000, 0x00001000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"Decon",              0x19050000, 0x00010000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"Clock Controller",   0x1A330000, 0x00008000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE}
};

VOID
GetMemoryMap (
  OUT EFI_MEMORY_REGION_DESCRIPTOR **MemoryDescriptor,
  OUT UINT8                         *MemoryDescriptorCount)
{
  // Pass Data
  *MemoryDescriptor      = gMemoryDescriptor;
  *MemoryDescriptorCount = ARRAY_SIZE (gMemoryDescriptor);
}
