#include <Library/DebugLib.h>
#include <Library/UfsCalAdapterLib.h>

UfsCalError UfsCalPrePmc (struct UfsCalParam *p)
{
  DEBUG ((EFI_D_ERROR, "UFS: PlatformUfsLib not implemented for this SoC\n"));
  return UFS_CAL_ERROR;
}

UfsCalError UfsCalPostPmc (struct UfsCalParam *p)
{
  DEBUG ((EFI_D_ERROR, "UFS: PlatformUfsLib not implemented for this SoC\n"));
  return UFS_CAL_ERROR;
}

UfsCalError UfsCalPostLink (struct UfsCalParam *p)
{
  DEBUG ((EFI_D_ERROR, "UFS: PlatformUfsLib not implemented for this SoC\n"));
  return UFS_CAL_ERROR;
}

UfsCalError UfsCalPreLink (struct UfsCalParam *p)
{
  DEBUG ((EFI_D_ERROR, "UFS: PlatformUfsLib not implemented for this SoC\n"));
  return UFS_CAL_ERROR;
}

UINT8 UfsCalGetTargetBoard (VOID)
{
  DEBUG ((EFI_D_ERROR, "UFS: PlatformUfsLib not implemented for this SoC\n"));
  return UFS_CAL_ERROR;
}

UfsCalError UfsCalInit (struct UfsCalParam *p)
{
  DEBUG ((EFI_D_ERROR, "UFS: PlatformUfsLib not implemented for this SoC\n"));
  return UFS_CAL_ERROR;
}