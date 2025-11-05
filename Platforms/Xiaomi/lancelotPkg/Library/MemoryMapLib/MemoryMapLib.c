#include <Library/MemoryMapLib.h>

STATIC
EFI_MEMORY_REGION_DESCRIPTOR_EX
gMemoryRegionDescriptorEx[] = {
  // Name, Address, Length, HobOption, ResourceType, ResourceAttribute, MemoryType, ArmAttribute

  // DDR Regions
  {"CPU Vectors",        0x40000000, 0x00001000, AddMem, SYS_MEM, SYS_MEM_CAP, BsData, WRITE_BACK},
  {"UEFI Stack",         0x40001000, 0x00040000, AddMem, SYS_MEM, SYS_MEM_CAP, BsData, WRITE_BACK},
  {"RAM Partition",      0x40041000, 0x0003F000, AddMem, SYS_MEM, SYS_MEM_CAP, Conv,   WRITE_BACK},
  {"UEFI FD",            0x40080000, 0x00200000, AddMem, SYS_MEM, SYS_MEM_CAP, BsData, WRITE_BACK},
  {"DXE Heap",           0x40280000, 0x03C00000, AddMem, SYS_MEM, SYS_MEM_CAP, Conv,   WRITE_BACK},
  {"RAM Partition",      0x43E80000, 0x08F80000, AddMem, SYS_MEM, SYS_MEM_CAP, Conv,   WRITE_BACK},
  {"ATF Reserved",       0x4CE00000, 0x00060000, AddMem, MEM_RES, SYS_MEM_CAP, Reserv, WRITE_BACK},
  {"TEE Reserved",       0x70000000, 0x05000000, AddMem, MEM_RES, SYS_MEM_CAP, Reserv, WRITE_BACK},
  {"SSPM Reserved",      0x7FF00000, 0x000C0000, AddMem, MEM_RES, SYS_MEM_CAP, Reserv, WRITE_BACK},
  {"SPM Reserved",       0x77FF0000, 0x00010000, AddMem, MEM_RES, SYS_MEM_CAP, Reserv, WRITE_BACK},
  {"Display Reserved",   0x7D9B0000, 0x02250000, AddMem, MEM_RES, SYS_MEM_CAP, Reserv, WRITE_THROUGH},
  {"SCP Reserved",       0x9F900000, 0x00600000, AddMem, MEM_RES, SYS_MEM_CAP, Reserv, WRITE_BACK},
  {"RAM Partition",      0x9FF00000, 0x60100000, AddMem, SYS_MEM, SYS_MEM_CAP, Conv,   WRITE_BACK},

  // Register Regions
  {"GIC Distributor",    0x0C000000, 0x00040000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"GIC Redistributors", 0x0C040000, 0x00200000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"Pinctrl",            0x10005000, 0x00001000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"PMIC Wrapper",       0x1000D000, 0x00001000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"MSDC-0",             0x11230000, 0x00010000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"MSDC-1",             0x11240000, 0x00010000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"MSDC Top-0",         0x11c90000, 0x00001000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"MSDC Top-1",         0x11cd0000, 0x00001000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},

  // Terminator for MMU
  {"Terminator", 0, 0, 0, 0, 0, 0, 0}
};

EFI_MEMORY_REGION_DESCRIPTOR_EX*
GetMemoryMap ()
{
  return gMemoryRegionDescriptorEx;
}
