FILE FREEFORM = 7E374E25-8E01-4FEE-87F2-390C23C606CD {
  SECTION RAW = Blackbolt/APIC.aml
# SECTION RAW = Blackbolt/BERT.aml
# SECTION RAW = Blackbolt/CSRT.aml
!if $(TARGET) == DEBUG
  SECTION RAW = Blackbolt/DBG2.aml
!endif
  SECTION RAW = Blackbolt/DSDT.aml
  SECTION RAW = nitrogen/SSDT.aml
  SECTION RAW = Blackbolt/FACP.aml
# SECTION RAW = Blackbolt/FPDT.aml
  SECTION RAW = Blackbolt/GTDT.aml
# SECTION RAW = Blackbolt/IORT.aml
# SECTION RAW = Blackbolt/MCFG.aml
# SECTION RAW = Blackbolt/MSDM.aml
# SECTION RAW = Blackbolt/PPTT.aml
# SECTION RAW = Blackbolt/SPCR.aml
  SECTION UI = "AcpiTables"
}
