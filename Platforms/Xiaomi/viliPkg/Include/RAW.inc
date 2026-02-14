  FILE FREEFORM = A1E235DE-E825-4591-9623-C43175811826 {
    SECTION UI = "SecParti.cfg"
    SECTION RAW = Binaries/vili/RawFiles/SecParti.cfg
  }

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  FILE FREEFORM = CB049ADC-62C3-11EB-96A1-6701D6051FE4 {
    SECTION UI = "Panel_K3S_42_02_0a_amoled_dsc_cmd.xml"
    SECTION RAW = Binaries/vili/RawFiles/Panel_K3S_42_02_0a_amoled_dsc_cmd.xml
  }

  FILE FREEFORM = 2FCC8E00-2785-4F64-AF5E-C3910984E8AD {
    SECTION UI = "Panel_K3S_42_02_0b_amoled_dsc_cmd.xml"
    SECTION RAW = Binaries/vili/RawFiles/Panel_K3S_42_02_0b_amoled_dsc_cmd.xml
  }
!endif

  FILE FREEFORM = 21E9BDD9-6C3F-4F10-84A5-BBEC322741F1 {
    SECTION UI = "uefipil.cfg"
    SECTION RAW = Binaries/vili/RawFiles/uefipil.cfg
  }

  FILE FREEFORM = 45FE4B7C-150C-45DA-A021-4BEB2048EC6F {
    SECTION UI = "QcomChargerCfg.cfg"
    SECTION RAW = Binaries/vili/RawFiles/QcomChargerCfg.cfg
  }

  FILE FREEFORM = F780C779-DD7C-47CD-BD1A-5EB414C51704 {
    SECTION UI = "BATTERY.PROVISION"
    SECTION RAW = Binaries/vili/RawFiles/BATTERY.PROVISION
  }
