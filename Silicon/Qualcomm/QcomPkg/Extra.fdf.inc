!if $(AB_SLOT_SUPPORT) == 1
  INF QcomPkg/GPLDrivers/BootSlotDxe/BootSlotDxe.inf
  INF QcomPkg/GPLApplications/SwitchSlotsApp/SwitchSlotsApp.inf
!endif

  INF QcomPkg/Drivers/PeripheralInitDxe/PeripheralInitDxe.inf

!include SiliciumPkg/Extra.fdf.inc
