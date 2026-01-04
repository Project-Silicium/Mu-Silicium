FILE FREEFORM = 7E374E25-8E01-4FEE-87F2-390C23C606CD {
  SECTION RAW = Divar/APIC.aml
# SECTION RAW = Divar/BERT.aml
# SECTION RAW = Divar/CSRT.aml
!if $(TARGET) == DEBUG
# SECTION RAW = Divar/DBG2.aml
!endif
  SECTION RAW = tapas/DSDT.aml
  SECTION RAW = Common/SSDT.aml
# SECTION RAW = Divar/FADT.aml
  SECTION RAW = Divar/FACP.aml
  SECTION RAW = Divar/GTDT.aml
# SECTION RAW = Divar/IORT.aml
# SECTION RAW = Divar/MCFG.aml
# SECTION RAW = Divar/MSDM.aml
  SECTION RAW = Divar/PPTT.aml
# SECTION RAW = Divar/SPCR.aml
  SECTION UI = "AcpiTables"
}
