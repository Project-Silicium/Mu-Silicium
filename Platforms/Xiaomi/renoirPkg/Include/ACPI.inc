FILE FREEFORM = 7E374E25-8E01-4FEE-87F2-390C23C606CD {
  SECTION RAW = Cedros/APIC.aml
# SECTION RAW = Cedros/BERT.aml
# SECTION RAW = Cedros/CSRT.aml
!if $(ENABLE_SECUREBOOT) == 0
  SECTION RAW = Cedros/DBG2.aml
!endif
  SECTION RAW = renoir/DSDT.aml
# SECTION RAW = Common/SSDT.aml
  SECTION RAW = Cedros/FACP.aml
# SECTION RAW = Cedros/FPDT.aml
  SECTION RAW = Cedros/GTDT.aml
  SECTION RAW = Cedros/IORT.aml
# SECTION RAW = Cedros/MCFG.aml
# SECTION RAW = Cedros/MSDM.aml
# SECTION RAW = Cedros/PPTT.aml
# SECTION RAW = Cedros/SPCR.aml
  SECTION UI = "AcpiTables" 
}
