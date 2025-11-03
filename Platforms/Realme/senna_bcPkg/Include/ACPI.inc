#FILE FREEFORM = 7E374E25-8E01-4FEE-87F2-390C23C606CD {
#  SECTION RAW = Pakala/APIC.aml
#  SECTION RAW = Pakala/BERT.aml
#  SECTION RAW = Pakala/BGRT.aml
#  SECTION RAW = Pakala/CSRT.aml
!if $(TARGET) == DEBUG
#  SECTION RAW = Pakala/DBG2.aml
!endif
!if ($(DEVICE_MODEL) == 0) || ($(DEVICE_MODEL) == 1)
#  SECTION RAW = senna_b/DSDT.aml
!else
#  SECTION RAW = senna_c/DSDT.aml
!endif
#  SECTION RAW = Pakala/FACP.aml
#  SECTION RAW = Pakala/FPDT.aml
#  SECTION RAW = Pakala/GTDT.aml
#  SECTION RAW = Pakala/IORT.aml
#  SECTION RAW = Pakala/MCFG.aml
#  SECTION RAW = Pakala/MSDM.aml
#  SECTION RAW = Pakala/PPTT.aml
#  SECTION RAW = Pakala/SPCR.aml
#  SECTION RAW = Pakala/TPM2.aml
#  SECTION RAW = Pakala/XSDT.aml
#  SECTION UI = "AcpiTables"
#}
