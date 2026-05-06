/**
  Copyright (c) 2023 - 2024, Arm Limited. All rights reserved.
  Copyright (c) Microsoft Corporation. All rights reserved.
  Copyright (C) DuoWoA authors. All rights reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/RngLib.h>

#include <Protocol/Rng.h>

//
// Global Variables
//
STATIC EFI_RNG_PROTOCOL *mRngProtocol;
STATIC VOID             *RegisteredRngProtocol;

STATIC
EFI_STATUS
GenerateRandomNumber (
  OUT UINT8 *Buffer,
  IN  UINTN  BufferSize)
{
  EFI_STATUS Status;

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

  // Verify Argument
  ASSERT (Rand != NULL);

  // Get Random Number
  Status = GenerateRandomNumber ((UINT8 *)Rand, sizeof (UINT16));
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

  // Verify Argument
  ASSERT (Rand != NULL);

  // Get Random Number
  Status = GenerateRandomNumber ((UINT8 *)Rand, sizeof (UINT32));
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

  // Verify Argument
  ASSERT (Rand != NULL);

  // Get Random Number
  Status = GenerateRandomNumber ((UINT8 *)Rand, sizeof (UINT64));
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

  // Verify Argument
  ASSERT (Rand != NULL);

  // Get Random Number
  Status = GenerateRandomNumber ((UINT8 *)Rand, sizeof (UINT64) * 2);
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
GetRngProtocol (
  IN EFI_EVENT  Event,
  IN VOID      *Context)
{
  EFI_STATUS  Status;
  EFI_HANDLE *HandleBuffer;
  UINTN       HandleCount;

  // Locate RNG Protocol Handle
  Status = gBS->LocateHandleBuffer (ByRegisterNotify, NULL, RegisteredRngProtocol, &HandleCount, &HandleBuffer);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Locate Protocol Handle! Status = %r\n", __FUNCTION__, Status));
    return;
  }

  // Get RNG Protocol from Handle
  Status = gBS->HandleProtocol (HandleBuffer[0], &gEfiRngProtocolGuid, (VOID *)&mRngProtocol);

  // Free Buffer
  FreePool (HandleBuffer);

  // Check for Errors
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to get RNG Protocol from Handle! Status = %r\n", __FUNCTION__, Status));
    return;
  }

  // Close Event
  gBS->CloseEvent (Event);
}

EFI_STATUS
EFIAPI
DxeRngConstructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status;
  EFI_EVENT  CallbackEvent;

  // Locate RNG Protocol
  Status = gBS->LocateProtocol (&gEfiRngProtocolGuid, NULL, (VOID *)&mRngProtocol);
  if (!EFI_ERROR (Status)) {
    return EFI_SUCCESS;
  }

  // Create RNG Protocol Register Call Event
  Status = gBS->CreateEvent (EVT_NOTIFY_SIGNAL, TPL_CALLBACK, GetRngProtocol, NULL, &CallbackEvent);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Create RNG Protocol Callback Event! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  // Register Protocol Notify for RNG Protocol
  Status = gBS->RegisterProtocolNotify (&gEfiRngProtocolGuid, CallbackEvent, &RegisteredRngProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Register RNG Protocol Callback Event! Status = %r\n", __FUNCTION__, Status));

    // Close Event
    gBS->CloseEvent (CallbackEvent);

    return Status;
  }

  return EFI_SUCCESS;
}
