  FILE FREEFORM = A1E235DE-E825-4591-9623-C43175811826 {
    SECTION UI = "SecParti.cfg"
    SECTION RAW = Binaries/renoir/RawFiles/SecParti.cfg
  }

  FILE FREEFORM = 21E9BDD9-6C3F-4F10-84A5-BBEC322741F1 {
    SECTION UI = "uefipil.cfg"
    SECTION RAW = Binaries/renoir/RawFiles/uefipil.cfg
  }

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  FILE FREEFORM = 690F46C1-0ECA-4790-AF19-955AA4D2018A {
    SECTION UI = "Panel_K9_36_02_0a_amoled_dsc_cmd.xml"
    SECTION RAW = Binaries/renoir/RawFiles/Panel_K9_36_02_0a_amoled_dsc_cmd.xml
  }

  FILE FREEFORM = 491E95D4-1E50-11EB-9C28-4F6FFBD193AE {
    SECTION UI = "Panel_K9_36_02_0a_p1_amoled_dsc_cmd.xml"
    SECTION RAW = Binaries/renoir/RawFiles/Panel_K9_36_02_0a_p1_amoled_dsc_cmd.xml
  }

  FILE FREEFORM = 3B1087BA-3F50-11EB-8910-43910AA72327 {
    SECTION UI = "Panel_K9_36_02_0a_p2_amoled_dsc_cmd.xml"
    SECTION RAW = Binaries/renoir/RawFiles/Panel_K9_36_02_0a_p2_amoled_dsc_cmd.xml
  }

  FILE FREEFORM = 0FC966CA-44C4-11EB-A973-CF565D676119 {
    SECTION UI = "Panel_K9_36_02_0a_mp_amoled_dsc_cmd.xml"
    SECTION RAW = Binaries/renoir/RawFiles/Panel_K9_36_02_0a_mp_amoled_dsc_cmd.xml
  }

  FILE FREEFORM = 246C2EE8-5732-11EB-AFF5-D749A0928057 {
    SECTION UI = "Panel_K9_42_02_0b_mp_amoled_dsc_cmd.xml"
    SECTION RAW = Binaries/renoir/RawFiles/Panel_K9_42_02_0b_mp_amoled_dsc_cmd.xml
  }
!endif

  FILE FREEFORM = 45FE4B7C-150C-45DA-A021-4BEB2048EC6F {
    SECTION UI = "QcomChargerCfg.cfg"
    SECTION RAW = Binaries/renoir/RawFiles/QcomChargerCfg.cfg
  }

  FILE FREEFORM = F780C779-DD7C-47CD-BD1A-5EB414C51704 {
    SECTION UI = "BATTERY.PROVISION"
    SECTION RAW = Binaries/renoir/RawFiles/BATTERY.PROVISION
  }
