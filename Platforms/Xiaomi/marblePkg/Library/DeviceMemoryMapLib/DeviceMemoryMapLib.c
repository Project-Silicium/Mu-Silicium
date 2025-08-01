#include <Library/DeviceMemoryMapLib.h>

STATIC
ARM_MEMORY_REGION_DESCRIPTOR_EX
gDeviceMemoryDescriptorEx[] = {
  // Name, Address, Length, HobOption, ResourceType, ResourceAttribute, MemoryType, ArmAttribute

  // DDR Regions - corrected based on uefiplat.cfg and iomem from Poco F5 (marble) SM7475
  {"Hypervisor",        0x80000000, 0x00600000, AddMem, SYS_MEM, SYS_MEM_CAP, Reserv, WRITE_BACK_XN},
  {"DT BLOB",           0x80600000, 0x00040000, AddMem, MEM_RES, SYS_MEM_CAP, Reserv, WRITE_BACK_XN},
  {"RAM Partition",     0x80640000, 0x001C0000, AddMem, SYS_MEM, SYS_MEM_CAP, Conv,   WRITE_BACK_XN},
  {"AOP Reserved",      0x80800000, 0x000A0000, AddMem, MEM_RES, UNCACHEABLE, Reserv, UNCACHED_UNBUFFERED_XN},
  {"RAM Partition",     0x808A0000, 0x00060000, AddMem, SYS_MEM, SYS_MEM_CAP, Conv,   WRITE_BACK_XN},
  {"SMEM Reserved",     0x80900000, 0x00200000, AddMem, MEM_RES, UNCACHEABLE, Reserv, UNCACHED_UNBUFFERED_XN},
  {"CUCP Reserved",     0x80B00000, 0x00100000, AddMem, MEM_RES, UNCACHEABLE, Reserv, UNCACHED_UNBUFFERED_XN},

  // System RAM regions directly from iomem
  {"RAM Partition",     0x85200000, 0x00500000, AddMem, SYS_MEM, SYS_MEM_CAP, Conv,   WRITE_BACK_XN},
  {"PIL Reserved",      0x85700000, 0x00700000, AddMem, MEM_RES, UNCACHEABLE, Reserv, UNCACHED_UNBUFFERED_XN},
  {"RAM Partition",     0x85E00000, 0x02200000, AddMem, SYS_MEM, SYS_MEM_CAP, Conv,   WRITE_BACK_XN},
  {"Reserved",          0x88000000, 0x0391C000, AddMem, MEM_RES, UNCACHEABLE, Reserv, UNCACHED_UNBUFFERED_XN},
  {"RAM Partition",     0x8B91C000, 0x000E4000, AddMem, SYS_MEM, SYS_MEM_CAP, Conv,   WRITE_BACK_XN},
  {"Reserved",          0x8BA00000, 0x17400000, AddMem, MEM_RES, UNCACHEABLE, Reserv, UNCACHED_UNBUFFERED_XN},
  {"RAM Partition",     0xA2E00000, 0x03D80000, AddMem, SYS_MEM, SYS_MEM_CAP, Conv,   WRITE_BACK_XN},
  {"Reserved",          0xA6B80000, 0x002C0000, AddMem, MEM_RES, UNCACHEABLE, Reserv, UNCACHED_UNBUFFERED_XN},
  {"RAM Partition",     0xA6E40000, 0x000C0000, AddMem, SYS_MEM, SYS_MEM_CAP, Conv,   WRITE_BACK_XN},
  {"Reserved",          0xA6F00000, 0x00100000, AddMem, MEM_RES, UNCACHEABLE, Reserv, UNCACHED_UNBUFFERED_XN},

  // UEFI Specific regions within System RAM a7000000-dfffffff
  {"UEFI FD",           0xA7000000, 0x00400000, AddMem, SYS_MEM, SYS_MEM_CAP, BsData, WRITE_BACK},
  {"UEFI FD Reserved",  0xA7400000, 0x00200000, AddMem, SYS_MEM, SYS_MEM_CAP, BsData, WRITE_BACK},
  {"CPU Vectors",       0xA7600000, 0x00001000, AddMem, SYS_MEM, SYS_MEM_CAP, BsData, WRITE_BACK},
  {"Info Blk",          0xA7601000, 0x00001000, AddMem, SYS_MEM, SYS_MEM_CAP, RtData, WRITE_BACK_XN},
  {"MMU PageTables",    0xA7602000, 0x00003000, AddMem, SYS_MEM, SYS_MEM_CAP, BsData, WRITE_BACK_XN},
  {"Log Buffer",        0xA7605000, 0x00008000, AddMem, SYS_MEM, SYS_MEM_CAP, RtData, WRITE_BACK_XN},
  {"UEFI Stack",        0xA760D000, 0x00040000, AddMem, SYS_MEM, SYS_MEM_CAP, BsData, WRITE_BACK_XN},
  {"SEC Heap",          0xA764D000, 0x0008C000, AddMem, SYS_MEM, SYS_MEM_CAP, BsData, WRITE_BACK_XN},
  {"Sched Heap",        0xA76D9000, 0x00400000, AddMem, SYS_MEM, SYS_MEM_CAP, BsData, WRITE_BACK_XN},
  {"FV Region",         0xA7AD9000, 0x00400000, AddMem, SYS_MEM, SYS_MEM_CAP, BsData, WRITE_BACK_XN},
  {"UEFI RESV",         0xA7ED9000, 0x00127000, AddMem, SYS_MEM, SYS_MEM_CAP, BsData, WRITE_BACK_XN},
  {"Kernel",            0xA8000000, 0x10000000, AddMem, SYS_MEM, SYS_MEM_CAP, Reserv, WRITE_BACK_XN},

  // Critical missing regions from uefiplat.cfg
  {"Display Reserved",  0xB8000000, 0x02B00000, AddMem, MEM_RES, SYS_MEM_CAP, Reserv, WRITE_THROUGH_XN},
  {"OEM VM",            0xBB000000, 0x05000000, AddMem, SYS_MEM, SYS_MEM_CAP, Reserv, WRITE_BACK_XN},
  {"MTE Reserved",      0xC0000000, 0x20000000, AddDynamicMem, SYS_MEM, SYS_MEM_CAP, Reserv, WRITE_BACK_XN},

  // System RAM allocation within the large a7000000-dfffffff region
  {"VM PIL",            0xE0000000, 0x05700000, AddMem, SYS_MEM, SYS_MEM_CAP, Reserv, WRITE_BACK_XN},

  // System RAM regions from iomem: e0b00000-e55f2fff
  {"RAM Partition",     0xE0B00000, 0x04AF3000, AddMem, SYS_MEM, SYS_MEM_CAP, Conv,   WRITE_BACK_XN},
  {"Reserved",          0xE55F3000, 0x0310D000, AddMem, MEM_RES, UNCACHEABLE, Reserv, UNCACHED_UNBUFFERED_XN},

  // System RAM regions from iomem: e5700000-e87fffff
  {"RAM Partition",     0xE5700000, 0x03100000, AddMem, SYS_MEM, SYS_MEM_CAP, Conv,   WRITE_BACK_XN},
  {"Reserved",          0xE8800000, 0x01000000, AddDynamicMem, MEM_RES, UNCACHEABLE, Reserv, UNCACHED_UNBUFFERED_XN},
  {"Reserved",          0xE9800000, 0x00800000, AddMem, MEM_RES, UNCACHEABLE, Reserv, UNCACHED_UNBUFFERED_XN},
  {"TZApps Reserved",   0xEA000000, 0x07400000, AddDynamicMem, MEM_RES, UNCACHEABLE, Reserv, UNCACHED_UNBUFFERED_XN},

  // UEFI FD regions within System RAM f4600000-ffffffff
  {"Reserved",          0xF1400000, 0x03200000, AddMem, MEM_RES, UNCACHEABLE, Reserv, UNCACHED_UNBUFFERED_XN},
  {"Reserved",          0xF4600000, 0x03A00000, AddMem, MEM_RES, UNCACHEABLE, Reserv, UNCACHED_UNBUFFERED_XN},

  // DXE Heap
  {"DXE Heap",          0xF8000000, 0x07C00000, AddMem, SYS_MEM, SYS_MEM_CAP, Conv,   WRITE_BACK_XN},

  // High memory regions (64-bit space) - directly from iomem with proper reserved regions
  {"RAM Partition",     0x800000000,0x037500000, AddMem, SYS_MEM, SYS_MEM_CAP, Conv,   WRITE_BACK_XN},
  {"Reserved",          0x837500000,0x000600000, AddMem, MEM_RES, UNCACHEABLE, Reserv, UNCACHED_UNBUFFERED_XN},
  {"RAM Partition",     0x837B00000,0x001E90000, AddMem, SYS_MEM, SYS_MEM_CAP, Conv,   WRITE_BACK_XN},
  {"Reserved",          0x839990000,0x006670000, AddMem, MEM_RES, UNCACHEABLE, Reserv, UNCACHED_UNBUFFERED_XN},
  {"RAM Partition",     0x840000000,0x267CB78000, AddMem, SYS_MEM, SYS_MEM_CAP, Conv,   WRITE_BACK_XN},
  {"HYP Reserved",      0xA6CB78000,0x019327000, AddMem, SYS_MEM, SYS_MEM_CAP, Reserv, WRITE_BACK_XN},

  // Other Memory Regions
  {"IMEM Base",         0x14680000, 0x0002A000, NoHob,  MMAP_IO, INITIALIZED, Conv,   NS_DEVICE},
  {"IMEM Cookie Base",  0x146AA000, 0x00016000, AddDev, MMAP_IO, INITIALIZED, Conv,   NS_DEVICE},
  {"DBI Dump",          0xA6000000, 0x00F00000, NoHob,  MMAP_IO, INITIALIZED, Conv,   UNCACHED_UNBUFFERED_XN},

  // Register Regions - SM7475 specific based on iomem and uefiplat.cfg
  // Core system controllers
  {"GCC",               0x00100000, 0x001F5000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"IPC_ROUTER_TOP",    0x00400000, 0x00100000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"SECURITY CONTROL",  0x00780000, 0x00007000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},

  // QUP/I2C Controllers
  {"QUP",               0x00800000, 0x00300000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},

  // Crypto and Security
  {"PRNG_CFG_PRNG",     0x010C0000, 0x0000C000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},

  // Interconnect/NoC
  {"INTERCONNECT_CFG",  0x01500000, 0x00300000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},

  // Storage and Crypto
  {"CRYPTO0 CRYPTO",    0x01DC0000, 0x00040000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"TCSR_TCSR_REGS",    0x01FC0000, 0x00030000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},

  // UFS Storage
  {"UFS_CONTROLLERS",   0x01D80000, 0x00019000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},

  // Peripheral Subsystem
  {"PERIPH_SS",         0x08800000, 0x00100000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},

  // USB Controllers
  {"USB",               0x0A600000, 0x00200000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},

  // AOSS (Always On Subsystem)
  {"AOSS",              0x0B000000, 0x04000000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},

  // PMIC/SPMI
  {"SPMI_CONTROLLERS",  0x0C42D000, 0x0008D000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},

  // GPIO/Pin Control
  {"TLMM",              0x0F000000, 0x01000000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},

  // System MMU
  {"SMMU",              0x15000000, 0x00200000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},

  // CPU Subsystem
  {"APSS_HM",           0x17000000, 0x02000000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},

  // LLCC (Last Level Cache Controller)
  {"LLCC",              0x19000000, 0x01000000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},

  // Security fuses
  {"QFPROM",            0x221C8000, 0x00001000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},

  // PCIe (WiFi)
  {"PCIE_MEM",          0x60200000, 0x03E00000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},

  // Terminator for MMU
  {"Terminator", 0, 0, 0, 0, 0, 0, 0}
};

ARM_MEMORY_REGION_DESCRIPTOR_EX*
GetDeviceMemoryMap ()
{
  return gDeviceMemoryDescriptorEx;
}
