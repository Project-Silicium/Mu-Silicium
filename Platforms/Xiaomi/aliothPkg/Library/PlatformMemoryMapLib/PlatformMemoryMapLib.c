#include <Library/BaseLib.h>
#include <Library/PlatformMemoryMapLib.h>

static ARM_MEMORY_REGION_DESCRIPTOR_EX gDeviceMemoryDescriptorEx[] = {
    /* Name               Address     Length      HobOption        ResourceAttribute    ArmAttributes
                                                          ResourceType          MemoryType */
    /* DDR Regions */
	{"Hypervisor",			0x80000000, 0x00600000, AddMem, SYS_MEM, SYS_MEM_CAP, Reserv, NS_DEVICE},		// Added for Windows boot
	{"RAM Partition",		0x80600000, 0x00260000, AddMem, SYS_MEM, SYS_MEM_CAP, Conv,   WRITE_BACK_XN},	// Added for filling gap
	{"AOP CMD DB",			0x80860000, 0x00020000, AddMem, MEM_RES, UNCACHEABLE, Reserv, UNCACHED_UNBUFFERED_XN},
	{"RAM Partition",		0x80880000, 0x00004000, AddMem, SYS_MEM, SYS_MEM_CAP, Conv,   WRITE_BACK_XN},	// Added for filling gap
	{"XBL Log Buffer",		0x80884000, 0x00010000, AddMem, SYS_MEM, SYS_MEM_CAP, Reserv, WRITE_BACK_XN},
	{"RAM Partition",		0x80894000, 0x0006C000, AddMem, SYS_MEM, SYS_MEM_CAP, Conv,   WRITE_BACK_XN},	// Added for filling gap
	{"SMEM",				0x80900000, 0x00200000, AddMem, MEM_RES, UNCACHEABLE, Reserv, UNCACHED_UNBUFFERED_XN},
//	{"TZApps Reserved",		0x82400000, 0x03A00000, HobOnlyNoCacheSetting, MEM_RES, UNCACHEABLE, Reserv, UNCACHED_UNBUFFERED_XN}, // Untouched TZApps partition
	{"TZApps Reserved",		0x80B00000, 0x05500000, HobOnlyNoCacheSetting, MEM_RES, UNCACHEABLE, Reserv, UNCACHED_UNBUFFERED_XN}, // Expanded to fill whole gap
	{"PIL Reserved",		0x86000000, 0x0D200000, AddMem, MEM_RES, UNCACHEABLE, Reserv, UNCACHED_UNBUFFERED_XN},
	{"RAM Partition",		0x93200000, 0x05700000, AddMem, SYS_MEM, SYS_MEM_CAP, Conv,   WRITE_BACK_XN},	// Added for filling gap
	{"DXE Heap",			0x98900000, 0x03300000, AddMem, SYS_MEM, SYS_MEM_CAP, Conv,   WRITE_BACK_XN},
	{"Sched Heap",			0x9BC00000, 0x00400000, AddMem, SYS_MEM, SYS_MEM_CAP, BsData, WRITE_BACK_XN},
	{"Display Reserved",	0x9C000000, 0x02400000, AddMem, MEM_RES, SYS_MEM_CAP, Reserv, WRITE_THROUGH_XN},
	{"DBI Dump",			0x9E400000, 0x00F00000, NoHob,  MMAP_IO, INITIALIZED, Conv,   UNCACHED_UNBUFFERED_XN},
	{"RAM Partition",		0x9F300000, 0x00500000, AddMem, SYS_MEM, SYS_MEM_CAP, Conv,   WRITE_BACK_XN},	// Added for filling gap
	{"FV Region",			0x9F800000, 0x00200000, AddMem, SYS_MEM, SYS_MEM_CAP, BsData, WRITE_BACK_XN},
	{"RAM Partition",		0x9FA00000, 0x00200000, AddMem, SYS_MEM, SYS_MEM_CAP, Conv,   WRITE_BACK_XN},	// Added for filling gap
	{"UEFI FD",				0x9FC00000, 0x00300000, AddMem, SYS_MEM, SYS_MEM_CAP, BsData, WRITE_BACK},
	{"SEC Heap",			0x9FF00000, 0x0008C000, AddMem, SYS_MEM, SYS_MEM_CAP, BsData, WRITE_BACK_XN},
	{"CPU Vectors",			0x9FF8C000, 0x00001000, AddMem, SYS_MEM, SYS_MEM_CAP, BsData, WRITE_BACK},
	{"MMU PageTables",		0x9FF8D000, 0x00003000, AddMem, SYS_MEM, SYS_MEM_CAP, BsData, WRITE_BACK_XN},
	{"UEFI Stack",			0x9FF90000, 0x00040000, AddMem, SYS_MEM, SYS_MEM_CAP, BsData, WRITE_BACK_XN},
	{"RAM Partition",		0x9FFD0000, 0x00027000, AddMem, SYS_MEM, SYS_MEM_CAP, Conv,   WRITE_BACK_XN},	// Added for filling gap
	{"Log Buffer",			0x9FFF7000, 0x00008000, AddMem, SYS_MEM, SYS_MEM_CAP, RtData, WRITE_BACK_XN},
	{"Info Blk",			0x9FFFF000, 0x00001000, AddMem, SYS_MEM, SYS_MEM_CAP, RtData, WRITE_BACK_XN},

#if DEVICE_RAM == 6
/*==================================================6GB RAM Setup==================================================*/
	/* 	On 6GB device /sys/firmware/fdt
		memory {
				ddr_device_type = <0x08>;
				device_type = "memory";
				reg = <0x00 0x80000000 0x00 0x3cc00000 0x00 0xc0000000 0x01 0x40000000>;
		};
		DDR Bank 0: 0x80000000 + 0x3CC00000 = 0xBCC00000
	*/
	
    // This RAM parition starts just after Info Blk and ends with DDR Bank 0
	{"RAM Partition",		0xA0000000, 0x1CC00000,	AddMem,	SYS_MEM, SYS_MEM_CAP, Conv,   WRITE_BACK_XN},
	// DDR Bank 0 end
	
    // Memory hole between DDR Bank 0 end and DDR Bank 1 start: 0xBCC00000 - 0xBFFFFFFF (1 byte for spacing)
	// Size: 0x33FFFFF (52 MB)
	
	// DDR Bank 1 start
	{"RAM Partition",		0xC0000000, 0x80000000,	AddMem,	SYS_MEM, SYS_MEM_CAP, Conv,   WRITE_BACK_XN},
	// As RAM starts at 0x80000000 (2GB) and this is a 6GB device, so it must ends at 8GB, then 0x200000000 = 0x140000000 + 0xC0000000
    {"RAM Partition",		0x140000000,0xC0000000,	AddMem,	SYS_MEM, SYS_MEM_CAP, Conv,   WRITE_BACK_XN},
/*===============================================================================================================*/
#elif DEVICE_RAM == 8
/*==================================================8GB RAM Setup==================================================*/
	/* 	On 8GB device /sys/firmware/fdt
		memory {
				ddr_device_type = <0x08>;
				device_type = "memory";
				reg = <0x00 0x80000000 0x00 0x3bb00000 0x00 0xc0000000 0x01 0xc0000000>;
		};
		DDR Bank 0: 0x80000000 + 0x3BB00000 = 0xBBB00000
	*/
	
    // This RAM parition starts just after Info Blk and ends with DDR Bank 0
	{"RAM Partition",		0xA0000000, 0x1BB00000,  AddMem,  SYS_MEM, SYS_MEM_CAP, Conv,   WRITE_BACK_XN},
	// DDR Bank 0 end
	
    // Memory hole between DDR Bank 0 end and DDR Bank 1 start: 0xBBB00000 - 0xBFFFFFFF (1 byte for spacing)
	// Size: 0x44FFFFF (69 MB)
	
	// DDR Bank 1 start
	{"RAM Partition",		0xC0000000, 0x80000000,	AddMem,	SYS_MEM, SYS_MEM_CAP, Conv,   WRITE_BACK_XN},
    {"RAM Partition",		0x140000000,0xC0000000, AddMem,	SYS_MEM, SYS_MEM_CAP, Conv,   WRITE_BACK_XN},
	// As RAM starts at 0x80000000 (2GB) and this is a 8GB device, so it must ends at 10GB, then 0x280000000 = 0x200000000 + 0x80000000
    {"RAM Partition",		0x200000000,0x80000000, AddMem,	SYS_MEM, SYS_MEM_CAP, Conv,   WRITE_BACK_XN},
/*===============================================================================================================*/

#elif DEVICE_RAM == 12
/*==================================================12GB RAM Setup==================================================*/
	/* 	On 12GB device /sys/firmware/fdt
		memory {
			ddr_device_type = <0x08>;
			device_type = "memory";
			reg = <0x00 0x80000000 0x00 0x39900000 0x02 0x00 0x01 0x80000000 0x00 0xc0000000 0x01 0x40000000>;
		};
		DDR Bank 0: 0x80000000 + 0x39900000 = 0xB9900000
	*/
	
    // This RAM parition starts just after Info Blk and ends with DDR Bank 0
	{"RAM Partition",		0xA0000000, 0x19900000,  AddMem,  SYS_MEM, SYS_MEM_CAP, Conv,   WRITE_BACK_XN},
	// DDR Bank 0 end
	
    // Memory hole between DDR Bank 0 end and DDR Bank 1 start: 0xB9900000 - 0xBFFFFFFF (1 byte for spacing)
	// Size: 0x66FFFFF (103 MB)
	
	// DDR Bank 1 start
	{"RAM Partition",		0xC0000000, 0x80000000,	AddMem,	SYS_MEM, SYS_MEM_CAP, Conv,   WRITE_BACK_XN},
    {"RAM Partition",		0x140000000,0xC0000000,	AddMem,	SYS_MEM, SYS_MEM_CAP, Conv,   WRITE_BACK_XN},
    {"RAM Partition",		0x200000000,0x80000000,	AddMem,	SYS_MEM, SYS_MEM_CAP, Conv,   WRITE_BACK_XN},
    {"RAM Partition",		0x280000000,0x80000000,	AddMem,	SYS_MEM, SYS_MEM_CAP, Conv,   WRITE_BACK_XN},
	// As RAM starts at 0x80000000 (2GB) and this is a 12GB device, so it must ends at 14GB, then 0x380000000 = 0x300000000 + 0x80000000
    {"RAM Partition",		0x300000000,0x80000000,	AddMem,	SYS_MEM, SYS_MEM_CAP, Conv,   WRITE_BACK_XN},
/*===============================================================================================================*/
#endif

	/* Other memory regions */
	{"IMEM Base",			0x14680000, 0x00040000, NoHob,  MMAP_IO, INITIALIZED, Conv,   NS_DEVICE},
	{"IMEM Cookie Base",	0x146BF000, 0x00001000, AddDev, MMAP_IO, INITIALIZED, Conv,   NS_DEVICE},

	/* Register regions */
	{"IPC_ROUTER_TOP",		0x00400000, 0x00100000, AddDev, MMAP_IO, UNCACHEABLE, MmIO, NS_DEVICE},
	{"SECURITY CONTROL",	0x00780000, 0x00007000, AddDev, MMAP_IO, UNCACHEABLE, MmIO, NS_DEVICE},
	{"QUPV3_2_GSI",			0x00800000, 0x000D0000, AddDev, MMAP_IO, UNCACHEABLE, MmIO, NS_DEVICE},
	{"QUPV3_0_GSI",			0x00900000, 0x000D0000, AddDev, MMAP_IO, UNCACHEABLE, MmIO, NS_DEVICE},
	{"QUPV3_1_GSI",			0x00A00000, 0x000D0000, AddDev, MMAP_IO, UNCACHEABLE, MmIO, NS_DEVICE},
	{"PRNG_CFG_PRNG",		0x00790000, 0x00010000, AddDev, MMAP_IO, UNCACHEABLE, MmIO, NS_DEVICE},
	{"CRYPTO0 CRYPTO",		0x01DC0000, 0x00040000, AddDev, MMAP_IO, UNCACHEABLE, MmIO, NS_DEVICE},
	{"TCSR_TCSR_REGS",		0x01FC0000, 0x00030000, AddDev, MMAP_IO, UNCACHEABLE, MmIO, NS_DEVICE},
	{"GPU_GMU_CX_BLK",		0x02C7D000, 0x00002000, AddDev, MMAP_IO, UNCACHEABLE, MmIO, NS_DEVICE},
	{"GPU_CC",				0x02C90000, 0x0000A000, AddDev, MMAP_IO, UNCACHEABLE, MmIO, NS_DEVICE},
	{"QUPV3_SSC_GSI",		0x05A00000, 0x000D0000, AddDev, MMAP_IO, UNCACHEABLE, MmIO, NS_DEVICE},
	{"PERIPH_SS",			0x08800000, 0x00200000, AddDev, MMAP_IO, UNCACHEABLE, MmIO, NS_DEVICE},
	{"USB30_PRIM",			0x0A600000, 0x0011B000, AddDev, MMAP_IO, UNCACHEABLE, MmIO, NS_DEVICE},
	{"USB_RUMI",			0x0A720000, 0x00010000, AddDev, MMAP_IO, UNCACHEABLE, MmIO, NS_DEVICE},
	{"USB30_SEC",			0x0A800000, 0x0011B000, AddDev, MMAP_IO, UNCACHEABLE, MmIO, NS_DEVICE},
	{"AOSS",				0x0B000000, 0x04000000, AddDev, MMAP_IO, UNCACHEABLE, MmIO, NS_DEVICE},
	{"TLMM_WEST",			0x0F100000, 0x00300000, AddDev, MMAP_IO, UNCACHEABLE, MmIO, NS_DEVICE},
	{"TLMM_SOUTH",			0x0F500000, 0x00300000, AddDev, MMAP_IO, UNCACHEABLE, MmIO, NS_DEVICE},
	{"TLMM_NORTH",			0x0F900000, 0x00300000, AddDev, MMAP_IO, UNCACHEABLE, MmIO, NS_DEVICE},
	{"SMMU",				0x15000000, 0x000D0000, AddDev, MMAP_IO, UNCACHEABLE, MmIO, NS_DEVICE},
	{"APSS_HM",				0x17800000, 0x0d981000, AddDev, MMAP_IO, UNCACHEABLE, MmIO, NS_DEVICE},
	{"Terminator", 0, 0, 0, 0, 0, 0, 0}
};

ARM_MEMORY_REGION_DESCRIPTOR_EX *GetPlatformMemoryMap()
{
  return gDeviceMemoryDescriptorEx;
}
