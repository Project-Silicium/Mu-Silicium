#include <Library/PlatformPrePiLib.h>
#include <Library/ArmSmmuDetachLib.h>

VOID
PlatformInitialize ()
{
  CONST UINT16 MdpStreams[] = { 0x800, 0x801, 0x804, 0x81C, 0x19A1, 0x19A2, 0x19A3, 0x19A4, 0x19A5, 0x19AB, 0x2801, 0x2C01 };

  // Detach IOMMU Domains
  ArmSmmuDetach (MdpStreams, ARRAY_SIZE (MdpStreams));
}
