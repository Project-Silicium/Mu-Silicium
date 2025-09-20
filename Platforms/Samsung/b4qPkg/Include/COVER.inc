  FILE FREEFORM = PCD (gSiliciumPkgTokenSpaceGuid.PcdCoverBootLogoFile) {
    SECTION RAW = b4qPkg/Include/Resources/CoverBootLogo.bmp
    SECTION UI = "Cover Boot Logo"
  }

  FILE FREEFORM = PCD (gSiliciumPkgTokenSpaceGuid.PcdCoverNoBootOSFile) {
    SECTION RAW = b4qPkg/Include/Resources/CoverNoBootOS.bmp
    SECTION UI = "Cover No Operating System"
  }

  FILE FREEFORM = PCD (gSiliciumPkgTokenSpaceGuid.PcdCoverMassStorageFile) {
    SECTION RAW = b4qPkg/Include/Resources/CoverMassStorage.bmp
    SECTION UI = "Cover Mass Storage"
  }
