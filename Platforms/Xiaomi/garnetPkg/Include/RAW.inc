  FILE FREEFORM = A1E235DE-E825-4591-9623-C43175811826 {
    SECTION UI = "SecParti.cfg"
    SECTION RAW = Binaries/garnet/RawFiles/SecParti.cfg
  }

  FILE FREEFORM = 21E9BDD9-6C3F-4F10-84A5-BBEC322741F1 {
    SECTION UI = "uefipil.cfg"
    SECTION RAW = Binaries/garnet/RawFiles/uefipil.cfg
  }

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  FILE FREEFORM = 8290042E-261F-4B29-A7DA-E7C7EB0A0B90 {
    SECTION UI = "Panel_N16_42_02_0b_amoled_dsc_vid.xml"
    SECTION RAW = Binaries/garnet/RawFiles/Panel_N16_42_02_0b_amoled_dsc_vid.xml
  }

  FILE FREEFORM = 9E42A4A0-81D4-4942-9761-3BD3183C6B02 {
    SECTION UI = "Panel_N16_36_0d_0a_amoled_dsc_vid.xml"
    SECTION RAW = Binaries/garnet/RawFiles/Panel_N16_36_0d_0a_amoled_dsc_vid.xml
  }

  FILE FREEFORM = AE5921A4-0FD4-4B15-88B1-1B9C4EA1FDC4 {
    SECTION UI = "Panel_N16_41_02_0c_amoled_dsc_vid.xml"
    SECTION RAW = Binaries/garnet/RawFiles/Panel_N16_41_02_0c_amoled_dsc_vid.xml
  }
!endif

  FILE FREEFORM = 45FE4B7C-150C-45DA-A021-4BEB2048EC6F {
    SECTION UI = "QcomChargerCfg.cfg"
    SECTION RAW = Binaries/garnet/RawFiles/QcomChargerCfg.cfg
  }
