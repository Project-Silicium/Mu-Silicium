#include <Uefi.h>
#include <Library/MsdcImplLib.h>

MSDC_PLATFORM_INFO gPlatformInfo = {
  .NumberOfHosts  = 0,
};

VOID
GetSourceClockRate (
  UINT32 Index,
  UINTN *Hz)
{
  if (Hz != NULL) {
    *Hz = 0;
  }
}

VOID
SourceClockControl (
  UINT32 Index,
  BOOLEAN Enable)
{
}

VOID
ClockControl (
  UINT32 Index,
  BOOLEAN Enable)
{
}

VOID
PowerControl (
  UINT32 Index,
  BOOLEAN Enable)
{
}

VOID
InitGpio (
  UINT32 Index)
{
}

RETURN_STATUS
EFIAPI
MsdcLibConstructor (VOID)
{
  return EFI_SUCCESS;
}
