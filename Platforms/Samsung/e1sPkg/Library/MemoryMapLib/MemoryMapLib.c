#include <Library/MemoryMapLib.h>

STATIC
EFI_MEMORY_REGION_DESCRIPTOR
gMemoryDescriptor[] = {
  // Name, Address, Length, HobOption, ResourceType, ResourceAttribute, MemoryType, ArmAttribute

  // DDR Regions
  {"EL2 Code",           0xC1400000, 0x00200000, AddMem, SYS_MEM, SYS_MEM_CAP, BsCode, WRITE_BACK},
  {"UEFI FD",            0xF4800000, 0x00200000, AddMem, SYS_MEM, SYS_MEM_CAP, BsData, WRITE_BACK},
  {"UEFI Stack",         0xF4A00000, 0x00040000, AddMem, SYS_MEM, SYS_MEM_CAP, BsData, WRITE_BACK},
  {"DXE Heap",           0xF4A40000, 0x03C00000, AddMem, SYS_MEM, SYS_MEM_CAP, Conv,   WRITE_BACK},
  {"Display Reserved",   0xFA200000, 0x009A4000, AddMem, MEM_RES, SYS_MEM_CAP, Reserv, WRITE_THROUGH},

  // iRAM Regions
  {"IRAM",               0x02020000, 0x00018000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   DEVICE},
  {"NS_IRAM",            0x02038000, 0x00004000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},

  // Register Regions
  {"Chip Info",          0x10000000, 0x00010000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"Watchdog Timer",     0x10040000, 0x00001000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"GICD",               0x10200000, 0x00010000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"GICR",               0x10240000, 0x00140000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"SYSREG PERIC0_1",    0x10821000, 0x00001000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"SYSREG PERIC0_2",    0x10822000, 0x00001000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"SYSREG PERIC1_1",    0x11021000, 0x00001000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"SYSREG PERIC1_2",    0x11022000, 0x00001000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"SYSREG PERIC2_1",    0x11821000, 0x00001000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"SYSREG PERIC2_2",    0x11822000, 0x00001000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"PMU",                0x12860000, 0x00010000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"SYSREG CMGP",        0x14022000, 0x00001000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"SYSREG UFS",         0x17020000, 0x00001000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"SYSREG HSI0",        0x17820000, 0x00010000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"DRM Decon",          0x19100000, 0x00004000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"Clock Controller",   0x22B20000, 0x00008000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE}
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
