#include <Library/PlatformPrePiLib.h>
#include <Library/ArmSmmuDetachLib.h>

VOID
PlatformInitialize ()
{
  CONST UINT16 MdpStreams[] = { 0x820, 0x821, 0x83C, 0x83D };

  // Detach IOMMU Domains
  ArmSmmuDetach (MdpStreams, ARRAY_SIZE (MdpStreams));
}
