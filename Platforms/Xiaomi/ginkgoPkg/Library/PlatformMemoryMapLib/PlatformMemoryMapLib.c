#include <Library/BaseLib.h>
#include <Library/PlatformMemoryMapLib.h>

static ARM_MEMORY_REGION_DESCRIPTOR_EX gDeviceMemoryDescriptorEx[] = {
	/* Name			   Address	 Length	  HobOption		ResourceAttribute	ArmAttributes
														  ResourceType		  MemoryType */

	/* DDR Regions */
	{"RAM Partition",		0x40000000, 0x05700000, AddMem, SYS_MEM, SYS_MEM_CAP, Conv, WRITE_BACK_XN}, // 87.00 MB
	{"HYP",					0x45700000, 0x00600000, AddMem, SYS_MEM, SYS_MEM_CAP, Reserv, WRITE_BACK_XN},
	{"RAM Partition",		0x45D00000, 0x00220000, AddMem, SYS_MEM, SYS_MEM_CAP, Conv, WRITE_BACK_XN}, // 2.13 MB
	{"AOP CMD DB",			0x45F20000, 0x00020000, AddMem, MEM_RES, WRITE_COMBINEABLE, Reserv, UNCACHED_UNBUFFERED_XN},
	{"SMEM",				0x46000000, 0x00200000, AddMem, MEM_RES, WRITE_COMBINEABLE, Reserv, UNCACHED_UNBUFFERED_XN},
	{"PIL Reserved",		0x4AB00000, 0x0D200000, AddMem, MEM_RES, WRITE_COMBINEABLE, Reserv, UNCACHED_UNBUFFERED_XN},
	{"DXE Heap",			0x57D00000, 0x03400000, AddMem, SYS_MEM, SYS_MEM_CAP, Conv,   WRITE_BACK},
	{"DBI Dump",			0x5B100000, 0x00A00000, NoHob,  MMAP_IO, INITIALIZED, Conv,   UNCACHED_UNBUFFERED_XN},
	{"Sched Heap",			0x5BC00000, 0x00400000, AddMem, SYS_MEM, SYS_MEM_CAP, BsData, WRITE_BACK_XN},
	{"Display Reserved",	0x5C000000, 0x01000000, AddMem, MEM_RES, SYS_MEM_CAP, Reserv, WRITE_THROUGH_XN},
	{"FV Region",			0x5F800000, 0x00200000, AddMem, SYS_MEM, SYS_MEM_CAP, BsData, WRITE_BACK_XN},
	{"RAM Partition",		0x5FA00000, 0x00200000, AddMem, SYS_MEM, SYS_MEM_CAP, Conv, WRITE_BACK_XN}, // 2.00 MB
	{"UEFI FD",				0x5FC00000, 0x00300000, AddMem, SYS_MEM, SYS_MEM_CAP, BsData, WRITE_BACK},
	{"SEC Heap",			0x5FF00000, 0x0008C000, AddMem, SYS_MEM, SYS_MEM_CAP, BsData, WRITE_BACK_XN},
	{"CPU Vectors",			0x5FF8C000, 0x00001000, AddMem, SYS_MEM, SYS_MEM_CAP, BsData, WRITE_BACK},
	{"MMU PageTables",		0x5FF8D000, 0x00003000, AddMem, SYS_MEM, SYS_MEM_CAP, BsData, WRITE_BACK_XN},
	{"UEFI Stack",			0x5FF90000, 0x00040000, AddMem, SYS_MEM, SYS_MEM_CAP, BsData, WRITE_BACK_XN},
	{"Log Buffer",			0x5FFF7000, 0x00008000, AddMem, SYS_MEM, SYS_MEM_CAP, RtData, WRITE_BACK_XN},
	{"Info Blk",			0x5FFFF000, 0x00001000, AddMem, SYS_MEM, SYS_MEM_CAP, RtData, WRITE_BACK_XN},

	#if DEVICE_RAM == 3
/*==================================================3GB RAM Setup==================================================*/
	/*	On 3GB device /sys/firmware/fdt

		memory {
			ddr_device_type = <0x07>;
			device_type = "memory";
			reg = <0x00 0x40000000 0x00 0x3e580000 0x00 0x80000000 0x00 0x80000000>;
		};
		
					  Start		   Lenght		 End
		DDR Bank 0: 0x40000000 + 0x3e580000 = 0x7E580000
	*/

	/* This RAM partition starts just after Info Blk and ends with DDR Bank 0
	It starts at 0x5FFFF000 + 0x00001000 = 0x60000000
	It ends at 0x7E580000, so its length must be 0x7E580000 - 0x60000000 = 0x1E580000 */
	{"RAM Partition",		0x69600000, 0x1E580000, AddMem, SYS_MEM, SYS_MEM_CAP, Conv, WRITE_BACK_XN}, // 485.50 MB

	// Memory hole between DDR Bank 0 end and DDR Bank 1 start: 0x7E580000 - 0x80000000
	// Size: 0x1A80000 (26.50 MB, or 27,136.00 KB)

	// Now starts DDR Bank 1 at 0x80000000 with 0x80000000 length
	{"RAM Partition",		0x80000000, 0x80000000, AddMem, SYS_MEM, SYS_MEM_CAP, Conv, WRITE_BACK_XN}, // 2048.00 MB

	/* And we reached the max ram size.
	As RAM starts at 0x40000000 (1GB) and this is a 3 GB device (0xC0000000), RAM must end at 0xC0000000 + 0x40000000 = 0x100000000 = 0x80000000 + 0x80000000 */
/*=================================================================================================================*/

	#elif DEVICE_RAM == 4
/*==================================================4GB RAM Setup==================================================*/
	/*	On 4GB device /sys/firmware/fdt

		memory {
			ddr_device_type = <0x07>;
			device_type = "memory";
			reg = <0x00 0x40000000 0x00 0x3dd00000 0x00 0xc0000000 0x00 0x80000000 0x00 0x80000000 0x00 0x40000000>;
		};
		
					  Start		   Lenght		 End
		DDR Bank 0: 0x40000000 + 0x3dd00000 = 0x7DD00000
	*/

	/* This RAM partition starts just after Info Blk and ends with DDR Bank 0
	It starts at 0x5FFFF000 + 0x00001000 = 0x60000000
	It ends at 0x7DD00000, so its length must be 0x7DD00000 - 0x60000000 = 0x1DD00000 */
	{"RAM Partition",		0x60000000, 0x1DD00000, AddMem, SYS_MEM, SYS_MEM_CAP, Conv, WRITE_BACK_XN}, // 477.00 MB

	// Memory hole between DDR Bank 0 end and DDR Bank 1 start: 0x7DD00000 - 0x80000000
	// Size: 0x2300000 (35.00 MB, or 35,840.00 KB)

	// Now starts DDR Bank 1 at 0x80000000 with 0x40000000 length
	{"RAM Partition",		0x80000000, 0x40000000, AddMem, SYS_MEM, SYS_MEM_CAP, Conv, WRITE_BACK_XN}, // 1024.00 MB

	// Now starts DDR Bank 2 at 0xc0000000 with 0x80000000 length
	{"RAM Partition",		0xC0000000, 0x80000000, AddMem, SYS_MEM, SYS_MEM_CAP, Conv, WRITE_BACK_XN}, // 2048.00 MB

	/* And we reached the max ram size.
	As RAM starts at 0x40000000 (1GB) and this is a 4 GB device (0x100000000), RAM must end at 0x100000000 + 0x40000000 = 0x140000000 = 0xC0000000 + 0x80000000 */
/*================================================================================================================*/

	#elif DEVICE_RAM == 6
/*==================================================6GB RAM Setup==================================================*/
	/*	On 6GB device /sys/firmware/fdt IT MUST BE

		memory {
			ddr_device_type = <0x07>;
			device_type = "memory";
			reg = <0x00 0x40000000 0x00 0x3cc00000 0x00 0xc0000000 0x00 0x80000000 0x00 0x80000000 0x00 0x40000000>;
		};
		
					  Start		   Lenght		 End
		DDR Bank 0: 0x40000000 + 0x3CC00000 = 0x7CC00000
	*/

	/* This RAM partition starts just after Info Blk and ends with DDR Bank 0
	It starts at 0x5FFFF000 + 0x00001000 = 0x60000000
	It ends at 0x7CC00000, so its length must be 0x7CC00000 - 0x60000000 = 0x1CC00000 */
	{"RAM Partition",		0x60000000, 0x1CC00000, AddMem, SYS_MEM, SYS_MEM_CAP, Conv, WRITE_BACK_XN}, // 460.00 MB

	// Memory hole between DDR Bank 0 end and DDR Bank 1 start: 0x7CC00000 - 0x80000000
	// Size: 0x2300000 (52.00 MB, or 53,248.00 KB)

	// Now starts DDR Bank 1 at 0x80000000 with 0x40000000 length
	{"RAM Partition",		0x80000000, 0x40000000, AddMem, SYS_MEM, SYS_MEM_CAP, Conv, WRITE_BACK_XN}, // 1024.00 MB

	// Now starts DDR Bank 2 at 0xc0000000 with 0x80000000 length
	{"RAM Partition",		0xC0000000, 0x80000000, AddMem, SYS_MEM, SYS_MEM_CAP, Conv, WRITE_BACK_XN}, // 2048.00 MB
	{"RAM Partition",		0x140000000,0x80000000, AddMem, SYS_MEM, SYS_MEM_CAP, Conv, WRITE_BACK_XN}, // 2048.00 MB

	/* And we reached the max ram size.
	As RAM starts at 0x40000000 (1GB) and this is a 6 GB device (0x180000000), RAM must end at 0x180000000 + 0x40000000 = 0x1C0000000 = 0x140000000 + 0x80000000 */
/*================================================================================================================*/
	#endif

	/* Other memory regions */
	{"AOP_SS_MSG_RAM",		0x045F0000, 0x00007000, NoHob,  MMAP_IO, INITIALIZED, Conv,   NS_DEVICE},
	{"IMEM Base",			0x0C100000, 0x00026000, NoHob,  MMAP_IO, INITIALIZED, Conv,   NS_DEVICE},
	{"IMEM Cookie Base",	0x0C125000, 0x00001000, AddDev, MMAP_IO, INITIALIZED, Conv,   NS_DEVICE},

	/* Register regions */
	{"GCC CLK CTL",			0x01400000, 0x00200000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
	{"DISP_CC_DISP_CC",		0x05F00000, 0x00020000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
	{"APCS_ALIAS0_GLB",		0x0F111000, 0x00001000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
	{"OSM_RAIL",			0x0F520000, 0x00020000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
	{"MMCX_CPR3",			0x01648000, 0x00008000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
	{"SECURITY CONTROL",	0x01B40000, 0x00010000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
	{"PRNG_CFG_PRNG",		0x01B50000, 0x00010000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
	{"QUPV3_0_GSI",			0x04A00000, 0x000D0000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
	{"QUPV3_1_GSI",			0x04C00000, 0x000D0000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
	{"UFS UFS REGS",		0x04800000, 0x00020000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
	{"CRYPTO0 CRYPTO",		0x01B00000, 0x00040000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
	{"TCSR_TCSR_REGS",		0x003C0000, 0x00040000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
	{"GPU_GMU_CX_BLK",		0x0597D000, 0x0000C000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
	{"GPU_CC",				0x05990000, 0x00009000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
	{"TLMM_WEST",			0x00500000, 0x00300000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
	{"TLMM_EAST",			0x00D00000, 0x00300000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
	{"TLMM_SOUTH",			0x00900000, 0x00300000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
	{"PERIPH_SS",			0x04700000, 0x00200000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
	{"USB30_PRIM",			0x04E00000, 0x00200000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
	{"VIDEO_CC",			0x05B00000, 0x00020000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
	{"MDSS",				0x05E00000, 0x00200000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
	{"SLP_CNTR",			0x04403000, 0x00001000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
	{"TSENS0",				0x04410000 ,0x00001000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
	{"TSENS0_TM",			0x04411000, 0x00001000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
	{"PSHOLD",				0x0440B000, 0x00001000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
	{"SMMU",				0x0C600000, 0x00080000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
	{"APSS_GIC500_GICD",	0x0F200000, 0x00010000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
	{"APSS_GIC500_GICR",	0x0F300000, 0x00020000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
	{"APSS_WDT_TMR1",		0x0F017000, 0x00001000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
	{"APSS_ACTPM_WRAP",		0x0F500000, 0x000B0000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
	{"QTIMER",				0x0F020000, 0x00110000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
	{"USB2",				0x01610000, 0x00010000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
	{"PMIC ARB SPMI",		0x01C00000, 0x02800000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
	{"MCCC_MCCC_MSTR",		0x0447D000, 0x00001000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},

	/* Terminator for MMU */
	{"Terminator", 0, 0, 0, 0, 0, 0, 0}};

ARM_MEMORY_REGION_DESCRIPTOR_EX *GetPlatformMemoryMap()
{
	return gDeviceMemoryDescriptorEx;
}