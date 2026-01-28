#include <Library/PlatformPrePiLib.h>
#include <Library/ArmSmmuDetachLib.h>

VOID
PlatformInitialize ()
{
  CONST UINT16 MdpStreams[] = { 0x800, 0x801, 0x804 };

  // Detach IOMMU Domains
  ArmSmmuDetach (MdpStreams, ARRAY_SIZE (MdpStreams));
}
