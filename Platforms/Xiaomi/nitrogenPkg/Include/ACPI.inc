FILE FREEFORM = 7E374E25-8E01-4FEE-87F2-390C23C606CD {
  SECTION RAW = Blackbolt/APIC.aml
# SECTION RAW = Blackbolt/BERT.aml
# SECTION RAW = Blackbolt/BGRT.aml
# SECTION RAW = Blackbolt/CSRT.aml
!if $(TARGET) == DEBUG
  SECTION RAW = Blackbolt/DBG2.aml
!endif
  SECTION RAW = nitrogen/DSDT.aml
  SECTION RAW = Blackbolt/FACP.aml
# SECTION RAW = Blackbolt/FPDT.aml
  SECTION RAW = Blackbolt/GTDT.aml
# SECTION RAW = Blackbolt/IORT.aml
# SECTION RAW = Blackbolt/MCFG.aml
# SECTION RAW = Blackbolt/MSDM.aml
# SECTION RAW = Blackbolt/PPTT.aml
# SECTION RAW = Blackbolt/SPCR.aml
# SECTION RAW = Blackbolt/TPM2.aml
# SECTION RAW = Blackbolt/XSDT.aml
  SECTION UI = "AcpiTables"
}
