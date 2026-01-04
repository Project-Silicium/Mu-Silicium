FILE FREEFORM = 7E374E25-8E01-4FEE-87F2-390C23C606CD {
  SECTION RAW = Kodiak/APIC.aml
# SECTION RAW = Kodiak/BERT.aml
# SECTION RAW = Kodiak/CSRT.aml
!if $(TARGET) == DEBUG
# SECTION RAW = Kodiak/DBG2.aml
!endif
  SECTION RAW = spacewar/DSDT.aml
  SECTION RAW = Common/SSDT.aml
  SECTION RAW = Kodiak/FACP.aml
# SECTION RAW = Kodiak/FPDT.aml
  SECTION RAW = Kodiak/GTDT.aml
# SECTION RAW = Kodiak/IORT.aml
# SECTION RAW = Kodiak/MCFG.aml
# SECTION RAW = Kodiak/MSDM.aml
# SECTION RAW = Kodiak/PPTT.aml
# SECTION RAW = Kodiak/SPCR.aml
  SECTION UI = "AcpiTables" 
}
