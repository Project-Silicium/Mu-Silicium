#include <Library/IoLib.h>
#include <Library/PlatformPrePiLib.h>
#include <Library/DevicePrePiLib.h>

VOID
PlatformInitialize ()
{
  // Run Device Specific Code
  DeviceInitialize ();
}
