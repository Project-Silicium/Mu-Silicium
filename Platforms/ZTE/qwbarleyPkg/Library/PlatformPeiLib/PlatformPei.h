#ifndef _PLATFORM_PEI_H_
#define _PLATFORM_PEI_H_

VOID
GetXblHobAddresses (
  OUT EFI_PHYSICAL_ADDRESS *SchedulerInterfaceAddr,
  OUT EFI_PHYSICAL_ADDRESS *DtbExtensionAddr
  );

EFI_STATUS
ShimInstallLib (
  IN CHAR8  *LibName,
  IN UINT32  LibVersion,
  IN VOID   *LibIntf
  );

EFI_STATUS
ShimLoadLib (
  IN  CHAR8   *LibName,
  IN  UINT32   LibVersion,
  OUT VOID   **LibIntf
  );

EFI_STATUS
LocateConfigurationEntryString (
  IN  CHAR8 *EntryName,
  OUT CHAR8 *EntryValue,
  IN  UINTN *EntryValueLength
  );

UINTN
SerialPortFlush ();

UINTN
SerialPortControl (
  IN UINTN Arg,
  IN UINTN Param
  );

UINTN
SerialPortDrain ();

#endif /* _PLATFORM_PEI_H_ */
