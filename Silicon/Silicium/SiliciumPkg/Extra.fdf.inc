  #
  # Remaining UEFI Drivers
  #
  INF MdeModulePkg/Universal/Acpi/BootGraphicsResourceTableDxe/BootGraphicsResourceTableDxe.inf
  INF MdeModulePkg/Universal/SetupBrowserDxe/SetupBrowserDxe.inf

  #
  # UEFI Applications
  #
  INF MdeModulePkg/Application/BootManagerMenuApp/BootManagerMenuApp.inf
  INF PcBdsPkg/MsBootPolicy/MsBootPolicy.inf
  INF SiliciumPkg/Applications/UFP/ufpdevicefw.inf

  #
  # RAW Files
  #
!if $(ENABLE_SECUREBOOT) == 1
  FILE FREEFORM = PCD (gSiliciumPkgTokenSpaceGuid.PcdSiPolicyFile) {
    SECTION RAW = SiliciumPkg/Include/Resources/SecureBoot/SiPolicy.p7b
    SECTION UI = "SiPolicy.p7b"
  }
!endif

  FILE FREEFORM = PCD (gOemPkgTokenSpaceGuid.PcdLogoFile) {
    SECTION RAW = SiliciumPkg/Include/Resources/BootLogo.bmp
    SECTION UI = "BootLogo.bmp"
  }

  FILE FREEFORM = PCD (gSiliciumPkgTokenSpaceGuid.PcdNoBootOSFile) {
    SECTION RAW = SiliciumPkg/Include/Resources/NoBootOS.bmp
    SECTION UI = "NoBootOS.bmp"
  }

  FILE FREEFORM = PCD (gSiliciumPkgTokenSpaceGuid.PcdMassStorageBmpFile) {
    SECTION RAW = SiliciumPkg/Include/Resources/MassStorage.bmp
    SECTION UI = "MassStorage.bmp"
  }
