FILE FREEFORM = 7E374E25-8E01-4FEE-87F2-390C23C606CD {
  SECTION RAW = Starlord/APIC.aml
# SECTION RAW = Starlord/BERT.aml
# SECTION RAW = Starlord/CSRT.aml
!if $(TARGET) == DEBUG
  SECTION RAW = Starlord/DBG2.aml
!endif
  SECTION RAW = Starlord/DSDT_minimal.aml
  SECTION RAW = Common/SSDT.aml
  SECTION RAW = Starlord/FACP.aml
# SECTION RAW = Starlord/FPDT.aml
  SECTION RAW = Starlord/GTDT.aml
# SECTION RAW = Starlord/IORT.aml
# SECTION RAW = Starlord/MCFG.aml
# SECTION RAW = Starlord/MSDM.aml
# SECTION RAW = Starlord/PPTT.aml
# SECTION RAW = Starlord/SPCR.aml
  SECTION UI = "AcpiTables"
}
