FILE FREEFORM = 7E374E25-8E01-4FEE-87F2-390C23C606CD {
  SECTION RAW = Lahaina/APIC.aml
# SECTION RAW = Lahaina/BERT.aml
# SECTION RAW = Lahaina/CSRT.aml
!if $(TARGET) == DEBUG
  SECTION RAW = Lahaina/DBG2.aml
!endif
  SECTION RAW = Lahaina/DSDT_minimal.aml
  SECTION RAW = Common/SSDT.aml
  SECTION RAW = Lahaina/FACP.aml
# SECTION RAW = Lahaina/FPDT.aml
  SECTION RAW = Lahaina/GTDT.aml
# SECTION RAW = Lahaina/IORT.aml
# SECTION RAW = Lahaina/MCFG.aml
# SECTION RAW = Lahaina/MSDM.aml
# SECTION RAW = Lahaina/PPTT.aml
# SECTION RAW = Lahaina/SPCR.aml
  SECTION UI = "AcpiTables"
}
