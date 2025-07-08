#include <Library/AslUpdateLib.h>

VOID
DeviceUpdateAcpiTables ()
{
  UINT32 PLST = 0x2;

  UpdateNameAslCode (SIGNATURE_32('P', 'L', 'S', 'T'), &PLST, 4);
}
