#ifndef _ERRATA_PATCHES_LIB_H_
#define _ERRATA_PATCHES_LIB_H_

/**
  This Function Applies Platform Specific Patches.

  @param[in] Base                          - The Memory Region Base of "winload.efi".
  @param[in] Length                        - The Memory Region Length of "winload.efi".
**/
VOID
ApplyPlatformErrataPatches (
  IN EFI_PHYSICAL_ADDRESS Base,
  IN UINTN                Length
  );

/**
  This Function gets the Platform Specific Shell Code.

  @param[out] ShellCode                    - The Shell Code.
  @param[out] ShellCodeSize                - The Size of the Shell Code.
**/
VOID
GetPlatformShellCode (
  OUT UINT8 **ShellCode,
  OUT UINTN  *ShellCodeSize
  );

#endif /* _ERRATA_PATCHES_LIB_H_ */
