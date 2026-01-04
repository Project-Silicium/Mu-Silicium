#FILE FREEFORM = 7E374E25-8E01-4FEE-87F2-390C23C606CD {
#  SECTION RAW = Palima/APIC.aml
#  SECTION RAW = Palima/BERT.aml
#  SECTION RAW = Palima/CSRT.aml
!if $(TARGET) == DEBUG
#  SECTION RAW = Palima/DBG2.aml
!endif
!if ($(DEVICE_MODEL) == 0) || ($(DEVICE_MODEL) == 1)
#  SECTION RAW = senna_b/DSDT.aml
!else
#  SECTION RAW = senna_c/DSDT.aml
!endif
#  SECTION RAW = Palima/FACP.aml
#  SECTION RAW = Palima/FPDT.aml
#  SECTION RAW = Palima/GTDT.aml
#  SECTION RAW = Palima/IORT.aml
#  SECTION RAW = Palima/MCFG.aml
#  SECTION RAW = Palima/MSDM.aml
#  SECTION RAW = Palima/PPTT.aml
#  SECTION RAW = Palima/SPCR.aml
#  SECTION UI = "AcpiTables"
#}
