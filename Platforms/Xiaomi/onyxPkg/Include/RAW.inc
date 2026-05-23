  FILE FREEFORM = A1E235DE-E825-4591-9623-C43175811826 {
    SECTION UI = "SecParti.cfg"
    SECTION RAW = Binaries/onyx/RawFiles/SecParti.cfg
  }

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  FILE FREEFORM = 03E9256F-E621-48D8-8576-7E3B4FCF2D41 {
    SECTION UI = "PanelSimConfig.csv"
    SECTION RAW = Binaries/onyx/RawFiles/PanelSimConfig.csv
  }

  FILE FREEFORM = 6FAB14C9-1A4C-4BD3-ACA1-CA921219C3CD {
    SECTION UI = "Panel_O10U_36_02_0b_amoled_dsc_vid.xml"
    SECTION RAW = Binaries/onyx/RawFiles/Panel_O10U_36_02_0b_amoled_dsc_vid.xml
  }

  FILE FREEFORM = FE1A8D61-88F5-4B88-9496-46950122A811 {
    SECTION UI = "Panel_O10U_42_02_0a_amoled_dsc_vid.xml"
    SECTION RAW = Binaries/onyx/RawFiles/Panel_O10U_42_02_0a_amoled_dsc_vid.xml
  }
!endif

  FILE FREEFORM = 21E9BDD9-6C3F-4F10-84A5-BBEC322741F1 {
    SECTION UI = "uefipil.cfg"
    SECTION RAW = Binaries/onyx/RawFiles/uefipil.cfg
  }

