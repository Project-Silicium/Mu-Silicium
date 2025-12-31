#ifndef _ERRATA_PATCHES_LIB_H_
#define _ERRATA_PATCHES_LIB_H_

EFI_STATUS
ApplyPlatformErrataPatches (
  IN EFI_PHYSICAL_ADDRESS Base,
  IN UINTN                Length
  );

UINT8*
GetPlatformTransferToKernelShellCode (OUT UINTN *ShellCodeSize);

#endif /* _ERRATA_PATCHES_LIB_H_ */
