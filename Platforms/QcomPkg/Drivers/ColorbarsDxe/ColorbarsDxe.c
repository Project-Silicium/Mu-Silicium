#include <Guid/EventGroup.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/CacheMaintenanceLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PrintLib.h>
#include <Library/SerialPortLib.h>
#include <Library/TimerLib.h>
#include <Library/UefiBootManagerLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>

#include <Protocol/Cpu.h>
#include <Protocol/DevicePathFromText.h>
#include <Protocol/EmbeddedGpio.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/PlatformBootManager.h>

#include <Library/DeviceStateLib.h>

EFI_STATUS
EFIAPI
InitializeColorbars(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable)
{
  DEVICE_STATE State;

  State = 0;

  State |= DEVICE_STATE_SECUREBOOT_OFF;
  //State |= DEVICE_STATE_MANUFACTURING_MODE;
  //State |= DEVICE_STATE_DEVELOPMENT_BUILD_ENABLED;
  //State |= DEVICE_STATE_SOURCE_DEBUG_ENABLED;
  //State |= DEVICE_STATE_UNDEFINED;
  //State |= DEVICE_STATE_UNIT_TEST_MODE;

  /*State |= DEVICE_STATE_PLATFORM_MODE_0;
  State |= DEVICE_STATE_PLATFORM_MODE_1;
  State |= DEVICE_STATE_PLATFORM_MODE_2;
  State |= DEVICE_STATE_PLATFORM_MODE_3;
  State |= DEVICE_STATE_PLATFORM_MODE_4;
  State |= DEVICE_STATE_PLATFORM_MODE_5;
  State |= DEVICE_STATE_PLATFORM_MODE_6;
  State |= DEVICE_STATE_PLATFORM_MODE_7;*/

  AddDeviceState(State);

  return EFI_SUCCESS;
}