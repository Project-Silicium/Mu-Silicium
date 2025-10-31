FILE FREEFORM = 7E374E25-8E01-4FEE-87F2-390C23C606CD {
  SECTION RAW = Waipio/APIC.UniCore.aml
# SECTION RAW = Waipio/BERT.aml
# SECTION RAW = Waipio/BGRT.aml
# SECTION RAW = Waipio/CSRT.aml
!if $(TARGET) == DEBUG
  SECTION RAW = Waipio/DBG2.aml
!endif
  SECTION RAW = r0q/DSDT.aml
  SECTION RAW = Waipio/FACP.aml
# SECTION RAW = Waipio/FPDT.aml
  SECTION RAW = Waipio/GTDT.aml
# SECTION RAW = Waipio/IORT.aml
# SECTION RAW = Waipio/MCFG.aml
# SECTION RAW = Waipio/MSDM.aml
  SECTION RAW = Waipio/PPTT.aml
# SECTION RAW = Waipio/SPCR.aml
# SECTION RAW = Waipio/TPM2.aml
# SECTION RAW = Waipio/XSDT.aml
  SECTION UI = "AcpiTables"
}
