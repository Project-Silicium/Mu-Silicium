#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>

STATIC
VOID
DummyNotify (
  IN EFI_EVENT Event,
  IN VOID     *Context) 
{
  // Dummy Function Needed for Event Notification Callback
}

EFI_STATUS
EFIAPI
InitPeripherals (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status = EFI_SUCCESS;

  // Start the USB Port Controller
  if (FixedPcdGetBool (PcdStartUsbController)) {
    EFI_EVENT InitEvent;

    Status = gBS->CreateEventEx (EVT_NOTIFY_SIGNAL, TPL_CALLBACK, DummyNotify, NULL, &gUsbControllerInitGuid, &InitEvent);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "Failed to Create USB Port Controller Start Event! Status = %r\n", Status));
    } else {
      gBS->SignalEvent (InitEvent);
      gBS->CloseEvent  (InitEvent);
    }
  }

  // Init SD Card Slot
  if (FixedPcdGetBool (PcdInitCardSlot)) {
    EFI_EVENT InitEvent;

    Status = gBS->CreateEventEx (EVT_NOTIFY_SIGNAL, TPL_CALLBACK, DummyNotify, NULL, &gSDCardInitGuid, &InitEvent);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "Failed to Create SD Card Init Event! Status = %r\n", Status));
    } else {
      gBS->SignalEvent (InitEvent);
      gBS->CloseEvent  (InitEvent);
    }
  }

  return Status;
}
