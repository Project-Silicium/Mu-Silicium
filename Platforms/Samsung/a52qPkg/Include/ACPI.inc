FILE FREEFORM = 7E374E25-8E01-4FEE-87F2-390C23C606CD {
  SECTION RAW = Rennell/APIC.aml
# SECTION RAW = Rennell/BERT.aml
# SECTION RAW = Rennell/BGRT.aml
  SECTION RAW = Rennell/CSRT.aml
!if $(TARGET) == DEBUG
  SECTION RAW = Rennell/DBG2.aml
!endif
  SECTION RAW = a52q/DSDT.aml
  SECTION RAW = Rennell/FACP.aml
  SECTION RAW = Rennell/FACS.aml
# SECTION RAW = Rennell/FPDT.aml
  SECTION RAW = Rennell/GTDT.aml
  SECTION RAW = Rennell/IORT.aml
  SECTION RAW = Rennell/MCFG.aml
# SECTION RAW = Rennell/MSDM.aml
  SECTION RAW = Rennell/PPTT.aml
# SECTION RAW = Rennell/SPCR.aml
# SECTION RAW = Rennell/TPM2.aml
# SECTION RAW = Rennell/XSDT.aml
  SECTION UI = "AcpiTables"
}
