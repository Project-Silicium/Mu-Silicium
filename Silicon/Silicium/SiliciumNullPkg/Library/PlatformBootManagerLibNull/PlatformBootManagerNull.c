#include <Library/DeviceBootManagerLib.h>

EFI_HANDLE
EFIAPI
DeviceBootManagerBeforeConsole (
  OUT EFI_DEVICE_PATH_PROTOCOL  **DevicePath,
  OUT BDS_CONSOLE_CONNECT_ENTRY **PlatformConsoles)
{
  return NULL;
}

EFI_DEVICE_PATH_PROTOCOL**
EFIAPI
DeviceBootManagerAfterConsole ()
{
  return NULL;
}

EFI_DEVICE_PATH_PROTOCOL**
EFIAPI
DeviceBootManagerOnDemandConInConnect ()
{
  return NULL;
}

VOID
EFIAPI
DeviceBootManagerProcessBootCompletion (OUT EFI_BOOT_MANAGER_LOAD_OPTION *BootOption)
{
  return;
}

EFI_STATUS
EFIAPI
DeviceBootManagerPriorityBoot (IN OUT EFI_BOOT_MANAGER_LOAD_OPTION *BootOption)
{
  return EFI_SUCCESS;
}

VOID
EFIAPI
DeviceBootManagerUnableToBoot ()
{
  return;
}

VOID
EFIAPI
DeviceBootManagerBdsEntry ()
{
  return;
}
