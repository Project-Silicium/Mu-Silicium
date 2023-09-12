/** @file
This is the platform specific implementation of the Thermal Services Library

Copyright (C) Microsoft Corporation. All rights reserved.
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>                                     // UEFI base types

#include <Library/UefiBootServicesTableLib.h>         // gBS
#include <Library/ThermalServicesLib.h>

#include <Protocol/EFITsens.h>

static EFI_TSENS_PROTOCOL *gEfiTsensProtocol = NULL;

EFI_STATUS
EFIAPI
SystemThermalCheck (
  IN  THERMAL_CASE  Case,
  OUT BOOLEAN       *Good
  )
{
  EFI_STATUS Status;
  UINT32     NumGood = 0;
  UINT32     NumBad = 0;
  UINT32     uSensor;
  UINT32     NumSens;
  INT32      CurTemp;
  INT32      MaxTemp;

  if (gEfiTsensProtocol == NULL) {
    Status = gBS->LocateProtocol (&gEfiTsensProtocolGuid, NULL, (VOID **)&gEfiTsensProtocol);
    if (EFI_ERROR (Status)) {
      gEfiTsensProtocol = NULL;
    }
  }

  if (gEfiTsensProtocol == NULL) {
    *Good = TRUE;
  } else {
    Status = gEfiTsensProtocol->GetNumSensors (&NumSens);
    if (EFI_ERROR (Status)) {
      *Good = TRUE;
    } else {
      Status = gEfiTsensProtocol->GetMaxTemp (&MaxTemp);
      if (EFI_ERROR (Status)) {
        *Good = TRUE;
      } else {
        for (uSensor = 0; uSensor < NumSens; uSensor++) {
          Status = gEfiTsensProtocol->GetTemp (uSensor, &CurTemp);
          if (EFI_ERROR (Status)) {
            *Good = TRUE;
            break;
          } else {
            if (CurTemp > MaxTemp || CurTemp == MaxTemp) {
              NumBad = NumBad + 1;
            } else {
              NumGood = NumGood + 1;
            }
          }
        }
        if (*Good != TRUE) {
          if (NumBad > NumGood || NumBad == NumGood) {
            *Good = FALSE;
          } else {
            *Good = TRUE;
          }
        }
      }
    }
  }

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
SystemThermalMitigate (
  IN  THERMAL_CASE  Case,
  IN  UINT32        TimeoutPeriod
  )
{
  return EFI_UNSUPPORTED;
}
