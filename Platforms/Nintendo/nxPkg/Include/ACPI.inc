#FILE FREEFORM = 7E374E25-8E01-4FEE-87F2-390C23C606CD {
!if $(DEVICE_MODEL) == 0
#  SECTION RAW = Erista/APIC.aml
#  SECTION RAW = Erista/BERT.aml
#  SECTION RAW = Erista/CSRT.aml
!if $(TARGET) == DEBUG
#  SECTION RAW = Erista/DBG2.aml
!endif
#  SECTION RAW = icosa/DSDT.aml
#  SECTION RAW = Common/SSDT.aml
#  SECTION RAW = Erista/FACP.aml
#  SECTION RAW = Erista/FPDT.aml
#  SECTION RAW = Erista/GTDT.aml
#  SECTION RAW = Erista/IORT.aml
#  SECTION RAW = Erista/MCFG.aml
#  SECTION RAW = Erista/MSDM.aml
#  SECTION RAW = Erista/PPTT.aml
#  SECTION RAW = Erista/SPCR.aml
!else
#  SECTION RAW = Mariko/APIC.aml
#  SECTION RAW = Mariko/BERT.aml
#  SECTION RAW = Mariko/CSRT.aml
!if $(TARGET) == DEBUG
#  SECTION RAW = Mariko/DBG2.aml
!endif
!if $(DEVICE_MODEL) == 1
#  SECTION RAW = iowa/DSDT.aml
!endif
#  SECTION RAW = Common/SSDT.aml
#  SECTION RAW = Mariko/FACP.aml
#  SECTION RAW = Mariko/FPDT.aml
#  SECTION RAW = Mariko/GTDT.aml
#  SECTION RAW = Mariko/IORT.aml
#  SECTION RAW = Mariko/MCFG.aml
#  SECTION RAW = Mariko/MSDM.aml
#  SECTION RAW = Mariko/PPTT.aml
#  SECTION RAW = Mariko/SPCR.aml
!endif
#  SECTION UI = "AcpiTables"
#}
