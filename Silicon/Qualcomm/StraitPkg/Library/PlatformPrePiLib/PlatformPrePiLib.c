#include <Library/PlatformPrePiLib.h>
#include <Library/ArmSmmuDetachLib.h>

VOID
PlatformInitialize ()
{
  CONST UINT16 MdpStreams[] = { 0x820, 0x821, 0x83C, 0x83D, 0xC60, 0xC61, 0xC63, 0xC64, 0xC80 };

  // Detach IOMMU Domains
  ArmSmmuDetach (MdpStreams, ARRAY_SIZE (MdpStreams));
}
