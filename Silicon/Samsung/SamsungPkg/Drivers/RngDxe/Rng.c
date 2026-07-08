#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/ArmSmcLib.h>

#include <Protocol/Rng.h>

#include "CryptoManager.h"

EFI_STATUS
RngGetInfo (
  IN     EFI_RNG_PROTOCOL  *This,
  IN OUT UINTN             *RngAlgoListSize,
  OUT    EFI_RNG_ALGORITHM *RngAlgoList)
{
  // Verify Parameters
  if (RngAlgoListSize == NULL || RngAlgoList == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  // Set RNG Algorithm GUID
  CopyGuid (&RngAlgoList[0], &gEfiRngAlgorithmRaw);

  // Set RNG Algorithm List Size
  *RngAlgoListSize = sizeof (EFI_RNG_ALGORITHM);

  return EFI_SUCCESS;
}

EFI_STATUS
RngGetRng (
  IN  EFI_RNG_PROTOCOL  *This,
  IN  EFI_RNG_ALGORITHM *RngAlgo,
  IN  UINTN              RngValueLength,
  OUT UINT8             *RngValue)
{
  UINT64 TrngValue[2];
  UINT64 SmcStatus;
  UINTN  ChunkSize;

  // Verify Parameters
  if (RngValue == NULL || !RngValueLength) {
    return EFI_INVALID_PARAMETER;
  }

  // Verify RNG Algorithm
  if (RngAlgo != NULL && !CompareGuid (RngAlgo, &gEfiRngAlgorithmRaw)) {
    return EFI_UNSUPPORTED;
  }

  // Repeat for Value Length
  for (UINTN i = 0; i < RngValueLength; i += 16) {
    // Set Chunk Size
    ChunkSize = (RngValueLength - i > 16) ? 16 : (RngValueLength - i);

    // Reset TRNG Value
    ZeroMem (TrngValue, sizeof (TrngValue));

    // Get TRNG Value
    SmcStatus = ArmCallSmc0 (SMC_CM_GET_TRN, &TrngValue[0], &TrngValue[1], NULL);
    if (SmcStatus != 0) {
      return EFI_DEVICE_ERROR;
    }

    // Copy Random Byte Chunk
    CopyMem (&RngValue[i], TrngValue, ChunkSize);
  }

  // Clear TRNG Value
  ZeroMem (TrngValue, sizeof (TrngValue));

  return EFI_SUCCESS;
}

STATIC EFI_RNG_PROTOCOL mRng = {
  RngGetInfo,
  RngGetRng
};

EFI_STATUS
EFIAPI
RegisterRng (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  // Register RNG Protocol
  return gBS->InstallProtocolInterface (&ImageHandle, &gEfiRngProtocolGuid, EFI_NATIVE_INTERFACE, &mRng);
}
