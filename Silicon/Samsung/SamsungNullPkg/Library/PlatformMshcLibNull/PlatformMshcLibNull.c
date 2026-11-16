#include <Library/DebugLib.h>
#include <Library/MshcHostBridge.h>

UINTN
MshcBoardGetHostCount (VOID)
{
  return 0;
}

EFI_STATUS
MshcBoardInit (
  IN     UINTN      Index,
  IN OUT MSHC_HOST *Host)
{
  (VOID)Index;
  Host->Coherent = FALSE;
  DEBUG ((EFI_D_ERROR, "MSHC: PlatformMshcLib not implemented for this SoC\n"));
  return EFI_UNSUPPORTED;
}

EFI_STATUS
MshcBoardSetCiuClock (
  IN OUT MSHC_HOST *Host,
  IN     UINTN      Frequency)
{
  (VOID)Host;
  (VOID)Frequency;
  return EFI_UNSUPPORTED;
}

EFI_STATUS
MshcBoardSetVoltage (
  IN OUT MSHC_HOST *Host,
  IN     UINT32     Millivolts)
{
  (VOID)Host;
  (VOID)Millivolts;
  return EFI_UNSUPPORTED;
}

EFI_STATUS
MshcBoardPowerCycle (IN OUT MSHC_HOST *Host)
{
  (VOID)Host;
  return EFI_UNSUPPORTED;
}

VOID
MshcBoardPostReset (IN OUT MSHC_HOST *Host)
{
  (VOID)Host;
}

BOOLEAN
MshcBoardIsCardPresent (IN MSHC_HOST *Host)
{
  (VOID)Host;
  return FALSE;
}

BOOLEAN
MshcBoardIsReadOnly (IN MSHC_HOST *Host)
{
  (VOID)Host;
  return FALSE;
}
