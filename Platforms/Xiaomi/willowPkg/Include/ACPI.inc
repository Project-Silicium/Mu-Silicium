FILE FREEFORM = 7E374E25-8E01-4FEE-87F2-390C23C606CD {
  SECTION RAW = Nicobar/APIC.aml
# SECTION RAW = Nicobar/BERT.aml
# SECTION RAW = Nicobar/BGRT.aml
# SECTION RAW = Nicobar/CSRT.aml
!if $(TARGET) == DEBUG
# SECTION RAW = Nicobar/DBG2.aml
!endif
  SECTION RAW = willow/DSDT.aml
# SECTION RAW = Nicobar/FADT.aml
  SECTION RAW = Nicobar/FACP.aml
  SECTION RAW = Nicobar/GTDT.aml
# SECTION RAW = Nicobar/IORT.aml
# SECTION RAW = Nicobar/MCFG.aml
# SECTION RAW = Nicobar/MSDM.aml
# SECTION RAW = Nicobar/PPTT.aml
# SECTION RAW = Nicobar/SPCR.aml
# SECTION RAW = Nicobar/TPM2.aml
# SECTION RAW = Nicobar/XSDT.aml
  SECTION UI = "AcpiTables"
}
