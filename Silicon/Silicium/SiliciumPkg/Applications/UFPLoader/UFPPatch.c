#include <Library/AssemblyUtilsLib.h>
#include <Library/CacheMaintenanceLib.h>

VOID
RemoveSecureBootCheck (
  IN EFI_PHYSICAL_ADDRESS Base,
  IN EFI_PHYSICAL_ADDRESS End)
{
  // Go thru the App Memory Area
  for (EFI_PHYSICAL_ADDRESS Current = Base; Current < End; Current += ARM64_INSTRUCTION_LENGTH) {
    // Verify Function Start
    if (*(UINT64 *)(Current + ARM64_TOTAL_INSTRUCTION_LENGTH (0))  != 0xA9015BF5A9BE53F3) {
      continue;
    }

    // Verify Function End
    if (*(UINT64 *)(Current + ARM64_TOTAL_INSTRUCTION_LENGTH (66)) != 0xD65F03C0A8C253F3) {
      continue;
    }

    // Verify Padding
    if (ARM64_INSTRUCTION (Current - ARM64_TOTAL_INSTRUCTION_LENGTH (1)) != 0x0) {
      continue;
    }

    // Force Disable Function
    ARM64_INSTRUCTION (Current + ARM64_TOTAL_INSTRUCTION_LENGTH (0)) = 0x2A1F03E0;
    ARM64_INSTRUCTION (Current + ARM64_TOTAL_INSTRUCTION_LENGTH (1)) = 0xD65F03C0;

    // Exit Loop
    return;
  }
}

VOID
ApplyUfpPatches (
  IN EFI_PHYSICAL_ADDRESS Base,
  IN UINT64               Length)
{
  // Apply Secure Boot Check Patch
  RemoveSecureBootCheck (Base, (EFI_PHYSICAL_ADDRESS)(Base + Length));

  // Flush Cache
  WriteBackInvalidateDataCacheRange ((VOID *)Base, Length);
  InvalidateInstructionCacheRange   ((VOID *)Base, Length);
}
