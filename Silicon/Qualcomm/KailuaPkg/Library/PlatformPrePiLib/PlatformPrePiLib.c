#include <Library/PlatformPrePiLib.h>
#include <Library/ArmSmmuDetachLib.h>

VOID
PlatformInitialize ()
{
  CONST UINT16 MdpStreams[] = { 0x1C00, 0x1C01, 0x1C03, 0x1C04, 0x2801, 0x2C01 };

  // Detach IOMMU Domains
  ArmSmmuDetach (MdpStreams, ARRAY_SIZE (MdpStreams));
}
