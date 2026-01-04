!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  FILE FREEFORM = 97CEB64B-29D5-42DF-9755-036D253C4B63 {
    SECTION UI = "Panel_G7B_37_02_0a_lcd_dsc_vid.xml"
    SECTION RAW = Binaries/phoenix/RawFiles/Panel_G7B_37_02_0a_lcd_dsc_vid.xml
  }

  FILE FREEFORM = 32E62D13-ADEA-41A0-896E-86F6FC5E9926 {
    SECTION UI = "Panel_G7B_42_02_0b_lcd_dsc_vid.xml"
    SECTION RAW = Binaries/phoenix/RawFiles/Panel_G7B_42_02_0b_lcd_dsc_vid.xml
  }
  
  FILE FREEFORM = 32E62D13-ADEA-41A0-896E-86F6FC5E9815 {
    SECTION UI = "Panel_G7B_37_02_0b_lcd_dsc_vid.xml"
    SECTION RAW = Binaries/phoenix/RawFiles/Panel_G7B_37_02_0b_lcd_dsc_vid.xml
  }
!endif

  FILE FREEFORM = A1E235DE-E825-4591-9623-C43175811826 {
    SECTION UI = "SecParti.cfg"
    SECTION RAW = Binaries/phoenix/RawFiles/SecParti.cfg
  }

  FILE FREEFORM = 21E9BDD9-6C3F-4F10-84A5-BBEC322741F1 {
    SECTION UI = "uefipil.cfg"
    SECTION RAW = Binaries/phoenix/RawFiles/uefipil.cfg
  }

  FILE FREEFORM = 45FE4B7C-150C-45DA-A021-4BEB2048EC6F {
    SECTION UI = "QcomChargerCfg.cfg"
    SECTION RAW = Binaries/phoenix/RawFiles/QcomChargerCfg.cfg
  }
