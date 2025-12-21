/**
  Copyright (c) 2023 - 2024, Arm Limited. All rights reserved.
  Copyright (c) Microsoft Corporation. All rights reserved.
  Copyright (C) DuoWoA authors. All rights reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/RngLib.h>

#include <Protocol/Rng.h>

#include "Rng.h"

//
// Global Variables
//
STATIC EFI_RNG_PROTOCOL *mRngProtocol;
STATIC VOID             *mRngRegistration;

STATIC
EFI_STATUS
GenerateRandomNumberViaNist800Algorithm (
  OUT UINT8 *Buffer,
  IN  UINTN  BufferSize)
{
  EFI_STATUS Status;

  // Verify Arguments
  if (Buffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  // Iterate over other available algorithms.
  for (UINT8 i = 0; i < ARRAY_SIZE (mSecureHashAlgorithms); i++) {
    // Verify Hash Algorithm State
    if (!mSecureHashAlgorithms[i].Available) {
      continue;
    }

    // Get Random Number
    Status = mRngProtocol->GetRNG (mRngProtocol, mSecureHashAlgorithms[i].Guid, BufferSize, Buffer);
    if (!EFI_ERROR (Status)) {
      return EFI_SUCCESS;
    }
  }

  // Get Random Number
  Status = mRngProtocol->GetRNG (mRngProtocol, NULL, BufferSize, Buffer);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to get Random Number! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  return EFI_SUCCESS;
}

BOOLEAN
EFIAPI
GetRandomNumber16 (OUT UINT16 *Rand)
{
  EFI_STATUS Status;

  // Verify RNG State
  if (mRngProtocol == NULL) {
    return FALSE;
  }

  // Verify Arguments
  if (Rand == NULL) {
    return FALSE;
  }

  // Get Random Number
  Status = GenerateRandomNumberViaNist800Algorithm ((UINT8 *)Rand, sizeof (UINT16));
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  return TRUE;
}

BOOLEAN
EFIAPI
GetRandomNumber32 (OUT UINT32 *Rand)
{
  EFI_STATUS Status;

  // Verify RNG State
  if (mRngProtocol == NULL) {
    return FALSE;
  }

  // Verify Arguments
  if (Rand == NULL) {
    return FALSE;
  }

  // Get Random Number
  Status = GenerateRandomNumberViaNist800Algorithm ((UINT8 *)Rand, sizeof (UINT32));
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  return TRUE;
}

BOOLEAN
EFIAPI
GetRandomNumber64 (OUT UINT64 *Rand)
{
  EFI_STATUS Status;

  // Verify RNG State
  if (mRngProtocol == NULL) {
    return FALSE;
  }

  // Verify Arguments
  if (Rand == NULL) {
    return FALSE;
  }

  // Get Random Number
  Status = GenerateRandomNumberViaNist800Algorithm ((UINT8 *)Rand, sizeof (UINT64));
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  return TRUE;
}

BOOLEAN
EFIAPI
GetRandomNumber128 (OUT UINT64 *Rand)
{
  EFI_STATUS Status;

  // Verify RNG State
  if (mRngProtocol == NULL) {
    return FALSE;
  }

  // Verify Arguments
  if (Rand == NULL) {
    return FALSE;
  }

  // Get Random Number
  Status = GenerateRandomNumberViaNist800Algorithm ((UINT8 *)Rand, sizeof (UINT64) * 2);
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  return TRUE;
}

EFI_STATUS
EFIAPI
GetRngGuid (OUT GUID *RngGuid)
{
  return EFI_UNSUPPORTED;
}

VOID
EFIAPI
PrepareRng (
  IN EFI_EVENT  Event,
  IN VOID      *Context)
{
  EFI_STATUS         Status;
  EFI_RNG_ALGORITHM *RngArray;
  UINTN              RngArraySize;

  // Get RNG Protocol
  if (mRngProtocol == NULL) {
    EFI_HANDLE *HandleBuffer;
    UINTN       HandleCount;

    // Locate RNG Protocol Handle
    Status = gBS->LocateHandleBuffer (ByRegisterNotify, NULL, mRngRegistration, &HandleCount, &HandleBuffer);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%a: Failed to Locate Protocol Handle! Status = %r\n", __FUNCTION__, Status));
      return;
    }

    // Verify Handle Count
    ASSERT (HandleCount == 1);

    // Get RNG Protocol from Handle
    Status = gBS->HandleProtocol (HandleBuffer[0], &gEfiRngProtocolGuid, (VOID *)&mRngProtocol);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%a: Failed to get RNG Protocol from Handle! Status = %r\n", __FUNCTION__, Status));
      return;
    }
  }

  // Get RNG Info
  Status = mRngProtocol->GetInfo (mRngProtocol, &RngArraySize, (VOID *)&RngArray);
  if (Status == EFI_BUFFER_TOO_SMALL) {
    // Verify RNG Array Size
    if (!RngArraySize) {
      DEBUG ((EFI_D_ERROR, "%a: RNG Array Size is 0!\n", __FUNCTION__));
      return;
    }

    // Allocate Memory
    RngArray = AllocateZeroPool (RngArraySize);
    if (RngArray == NULL) {
      DEBUG ((EFI_D_ERROR, "%a: Failed to Allocate Memory for RNG Array!\n", __FUNCTION__));
      return;
    }

    // Get RNG Info again
    Status = mRngProtocol->GetInfo (mRngProtocol, &RngArraySize, RngArray);
    if (EFI_ERROR (Status)) {
      // Free Buffer
      FreePool (RngArray);

      goto InfoError;
    }
  } else if (EFI_ERROR (Status)) {
InfoError:
    DEBUG ((EFI_D_ERROR, "%a: Failed to get RNG Info! Status = %r\n", __FUNCTION__, Status));
    return;
  }

  // Close Event
  if (Event != NULL) {
    gBS->CloseEvent (Event);
  }
}

EFI_STATUS
EFIAPI
DxeRngLibConstructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status;
  EFI_EVENT  RngPrepareEvent;

  // Locate RNG Protocol
  Status = gBS->LocateProtocol (&gEfiRngProtocolGuid, NULL, (VOID *)&mRngProtocol);
  if (!EFI_ERROR (Status)) {
    // Prepare RNG
    PrepareRng (NULL, NULL);

    return EFI_SUCCESS;
  }

  // Create RNG Protocol Register Call Event
  Status = gBS->CreateEvent (EVT_NOTIFY_SIGNAL, TPL_CALLBACK, PrepareRng, NULL, &RngPrepareEvent);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Create File System Callback Event! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  // Register Protocol Notify for RNG Protocol
  Status = gBS->RegisterProtocolNotify (&gEfiRngProtocolGuid, RngPrepareEvent, &mRngRegistration);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Register Protocol Notify for RNG Protocol! Status = %r\n", __FUNCTION__, Status));

    // Close Event
    gBS->CloseEvent (RngPrepareEvent);

    return Status;
  }

  return EFI_SUCCESS;
}
