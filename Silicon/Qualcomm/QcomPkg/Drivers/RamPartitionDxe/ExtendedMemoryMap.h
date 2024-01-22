#include <Library/DeviceMemoryMapLib.h>

static ARM_MEMORY_REGION_DESCRIPTOR_EX gExtendedMemoryDescriptor[] = {
  /* Name               Address     Length      HobOption        ResourceAttribute    ArmAttributes
                                                          ResourceType          MemoryType */
{"RAM Partition", 0x00000000, 0x00, NoHob, SYS_MEM, SYS_MEM_CAP, Conv, WRITE_BACK_XN},
{"RAM Partition", 0x00000000, 0x00, NoHob, SYS_MEM, SYS_MEM_CAP, Conv, WRITE_BACK_XN},
{"RAM Partition", 0x00000000, 0x00, NoHob, SYS_MEM, SYS_MEM_CAP, Conv, WRITE_BACK_XN},
{"RAM Partition", 0x00000000, 0x00, NoHob, SYS_MEM, SYS_MEM_CAP, Conv, WRITE_BACK_XN},
{"RAM Partition", 0x00000000, 0x00, NoHob, SYS_MEM, SYS_MEM_CAP, Conv, WRITE_BACK_XN},
{"RAM Partition", 0x00000000, 0x00, NoHob, SYS_MEM, SYS_MEM_CAP, Conv, WRITE_BACK_XN},
{"RAM Partition", 0x00000000, 0x00, NoHob, SYS_MEM, SYS_MEM_CAP, Conv, WRITE_BACK_XN},
{"RAM Partition", 0x00000000, 0x00, NoHob, SYS_MEM, SYS_MEM_CAP, Conv, WRITE_BACK_XN},
};