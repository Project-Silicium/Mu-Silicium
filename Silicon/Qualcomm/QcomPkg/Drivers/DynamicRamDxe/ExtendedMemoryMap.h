#include <Library/DeviceMemoryMapLib.h>

STATIC
ARM_MEMORY_REGION_DESCRIPTOR_EX
gExtendedMemoryDescriptor[] = {
  // Name, Address, Length, HobOption, ResourceType, ResourceAttribute, MemoryType, ArmAttribute

  {"RAM Partition", 0x00000000, 0x00000000, NoHob, SYS_MEM, SYS_MEM_CAP, Conv, WRITE_BACK_XN},
  {"RAM Partition", 0x00000000, 0x00000000, NoHob, SYS_MEM, SYS_MEM_CAP, Conv, WRITE_BACK_XN},
  {"RAM Partition", 0x00000000, 0x00000000, NoHob, SYS_MEM, SYS_MEM_CAP, Conv, WRITE_BACK_XN},
  {"RAM Partition", 0x00000000, 0x00000000, NoHob, SYS_MEM, SYS_MEM_CAP, Conv, WRITE_BACK_XN},
  {"RAM Partition", 0x00000000, 0x00000000, NoHob, SYS_MEM, SYS_MEM_CAP, Conv, WRITE_BACK_XN},
  {"RAM Partition", 0x00000000, 0x00000000, NoHob, SYS_MEM, SYS_MEM_CAP, Conv, WRITE_BACK_XN},
  {"RAM Partition", 0x00000000, 0x00000000, NoHob, SYS_MEM, SYS_MEM_CAP, Conv, WRITE_BACK_XN},
  {"RAM Partition", 0x00000000, 0x00000000, NoHob, SYS_MEM, SYS_MEM_CAP, Conv, WRITE_BACK_XN},
};
