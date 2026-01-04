FILE FREEFORM = 7E374E25-8E01-4FEE-87F2-390C23C606CD {
# SECTION RAW = Kamorta/APIC.aml
  SECTION RAW = Kamorta/APIC.4Cores.aml
# SECTION RAW = Kamorta/BERT.aml
# SECTION RAW = Kamorta/CSRT.aml
!if $(TARGET) == DEBUG
# SECTION RAW = Kamorta/DBG2.aml
!endif
  SECTION RAW = Kamorta/DSDT.aml
  SECTION RAW = lime/SSDT.aml
# SECTION RAW = Kamorta/FADT.aml
  SECTION RAW = Kamorta/FACP.aml
  SECTION RAW = Kamorta/GTDT.aml
# SECTION RAW = Kamorta/IORT.aml
# SECTION RAW = Kamorta/MCFG.aml
# SECTION RAW = Kamorta/MSDM.aml
# SECTION RAW = Kamorta/PPTT.aml
# SECTION RAW = Kamorta/SPCR.aml
  SECTION UI = "AcpiTables"
}
