FILE FREEFORM = 7E374E25-8E01-4FEE-87F2-390C23C606CD {
  SECTION RAW = Palawan/APIC.aml
# SECTION RAW = Palawan/BERT.aml
# SECTION RAW = Palawan/BGRT.aml
# SECTION RAW = Palawan/CSRT.aml
!if $(TARGET) == DEBUG
# SECTION RAW = Palawan/DBG2.aml
!endif
  SECTION RAW = Palawan/DSDT_minimal.aml
  SECTION RAW = Palawan/FACP.aml
# SECTION RAW = Palawan/FPDT.aml
  SECTION RAW = Palawan/GTDT.aml
# SECTION RAW = Palawan/IORT.aml
# SECTION RAW = Palawan/MCFG.aml
# SECTION RAW = Palawan/MSDM.aml
# SECTION RAW = Palawan/PPTT.aml
# SECTION RAW = Palawan/SPCR.aml
# SECTION RAW = Palawan/TPM2.aml
# SECTION RAW = Palawan/XSDT.aml
  SECTION UI = "AcpiTables"
}
