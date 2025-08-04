FILE FREEFORM = 7E374E25-8E01-4FEE-87F2-390C23C606CD {
  SECTION RAW = Kailua/APIC.aml
# SECTION RAW = Kailua/BERT.aml
# SECTION RAW = Kailua/BGRT.aml
# SECTION RAW = Kailua/CSRT.aml
!if $(TARGET) == DEBUG
  SECTION RAW = Kailua/DBG2.aml
!endif
  SECTION RAW = nx729j/DSDT.aml
  SECTION RAW = Kailua/FACP.aml
# SECTION RAW = Kailua/FPDT.aml
  SECTION RAW = Kailua/GTDT.aml
# SECTION RAW = Kailua/IORT.aml
# SECTION RAW = Kailua/MCFG.aml
# SECTION RAW = Kailua/MSDM.aml
  SECTION RAW = Kailua/PPTT.aml
# SECTION RAW = Kailua/SPCR.aml
# SECTION RAW = Kailua/TPM2.aml
# SECTION RAW = Kailua/XSDT.aml
  SECTION UI = "AcpiTables"
}
