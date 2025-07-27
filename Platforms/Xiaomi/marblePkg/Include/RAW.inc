// Panel configuration for Poco F5 (marble) - m16t_36_0d_0b AMOLED
FILE FREEFORM = A91D838E-A5FA-4138-825D-455E2303079E {
  SECTION UI = "BDS_Menu.cfg"
  SECTION RAW = Binaries/marble/RawFiles/BDS_Menu.cfg
}

FILE FREEFORM = A1E235DE-E825-4591-9623-C43175811826 {
  SECTION UI = "SecParti.cfg"
  SECTION RAW = Binaries/marble/RawFiles/SecParti.cfg
}

// Primary panel: m16t_36_0d_0b AMOLED 120Hz DSC Video Mode
FILE FREEFORM = 8CAE2C55-FC5A-410B-812B-8CA22F572F36 {
  SECTION UI = "Panel_m16t_36_0d_0b_amoled_dsc_vid.xml"
  SECTION RAW = Binaries/marble/RawFiles/Panel_m16t_36_0d_0b_amoled_dsc_vid.xml
}

// Alternative command mode for same panel
FILE FREEFORM = 13AC9DC9-0092-4374-AE1A-06FF35950FD0 {
  SECTION UI = "Panel_m16t_36_0d_0b_amoled_dsc_cmd.xml"
  SECTION RAW = Binaries/marble/RawFiles/Panel_m16t_36_0d_0b_amoled_dsc_cmd.xml
}

// 120Hz specific configuration
FILE FREEFORM = FC6315F9-F8FE-4222-A1E3-226DF55D7592 {
  SECTION UI = "Panel_m16t_36_0d_0b_120hz_dsc_vid.xml"
  SECTION RAW = Binaries/marble/RawFiles/Panel_m16t_36_0d_0b_120hz_dsc_vid.xml
}

// 120Hz command mode
FILE FREEFORM = 95E7B040-26A2-4511-9ABB-1D95D6DA7082 {
  SECTION UI = "Panel_m16t_36_0d_0b_120hz_dsc_cmd.xml"
  SECTION RAW = Binaries/marble/RawFiles/Panel_m16t_36_0d_0b_120hz_dsc_cmd.xml
}

// 60Hz fallback mode
FILE FREEFORM = 8BC595B7-391A-4A9A-8256-5B9983B30392 {
  SECTION UI = "Panel_m16t_36_0d_0b_60hz_dsc_vid.xml"
  SECTION RAW = Binaries/marble/RawFiles/Panel_m16t_36_0d_0b_60hz_dsc_vid.xml
}

// 60Hz command mode fallback
FILE FREEFORM = 19B58ACD-B1F8-4078-B9A5-A33584B680E2 {
  SECTION UI = "Panel_m16t_36_0d_0b_60hz_dsc_cmd.xml"
  SECTION RAW = Binaries/marble/RawFiles/Panel_m16t_36_0d_0b_60hz_dsc_cmd.xml
}

// HDR10+ configuration
FILE FREEFORM = 0AE323D6-2EBE-11E8-8349-9BA636A0D80C {
  SECTION UI = "Panel_m16t_36_0d_0b_hdr10_plus.xml"
  SECTION RAW = Binaries/marble/RawFiles/Panel_m16t_36_0d_0b_hdr10_plus.xml
}

// Dolby Vision configuration
FILE FREEFORM = 87273B42-311D-11E8-9324-DFB2DADFC3D1 {
  SECTION UI = "Panel_m16t_36_0d_0b_dolby_vision.xml"
  SECTION RAW = Binaries/marble/RawFiles/Panel_m16t_36_0d_0b_dolby_vision.xml
}

// Panel simulation config for testing
FILE FREEFORM = 03E9256F-E621-48D8-8576-7E3B4FCF2D41 {
  SECTION UI = "PanelSimConfig.csv"
  SECTION RAW = Binaries/marble/RawFiles/PanelSimConfig.csv
}

// UEFI PIL configuration
FILE FREEFORM = 21E9BDD9-6C3F-4F10-84A5-BBEC322741F1 {
  SECTION UI = "uefipil.cfg"
  SECTION RAW = Binaries/marble/RawFiles/uefipil.cfg
}

// Charger configuration
FILE FREEFORM = 45FE4B7C-150C-45DA-A021-4BEB2048EC6F {
  SECTION UI = "QcomChargerCfg.cfg"
  SECTION RAW = Binaries/marble/RawFiles/QcomChargerCfg.cfg
}
