FILE FREEFORM = 7E374E25-8E01-4FEE-87F2-390C23C606CD {
  SECTION RAW = Napali/APIC.aml
# SECTION RAW = Napali/BERT.aml
#  ECTION RAW = Napali/BGRT.aml
# SECTION RAW = Napali/CSRT.aml
!if $(TARGET) == DEBUG
# SECTION RAW = Napali/DBG2.aml
!endif
  SECTION RAW = Napali/DSDT_minimal.aml
  SECTION RAW = Napali/FACP.aml
# SECTION RAW = Napali/FPDT.aml
  SECTION RAW = Napali/GTDT.aml
# SECTION RAW = Napali/IORT.aml
# SECTION RAW = Napali/MCFG.aml
# SECTION RAW = Napali/MSDM.aml
# SECTION RAW = Napali/PPTT.aml
# SECTION RAW = Napali/SPCR.aml
# SECTION RAW = Napali/TPM2.aml
# SECTION RAW = Napali/XSDT.aml
  SECTION UI = "AcpiTables"
}
