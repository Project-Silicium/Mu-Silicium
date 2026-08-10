#FILE FREEFORM = 7E374E25-8E01-4FEE-87F2-390C23C606CD {
!if $(DEVICE_MODEL) == 0
#  SECTION RAW = Erista/APIC.aml
#  SECTION RAW = Erista/BERT.aml
#  SECTION RAW = Erista/CSRT.aml
!if $(ENABLE_SECUREBOOT) == 0
#  SECTION RAW = Erista/DBG2.aml
!endif
#  SECTION RAW = Erista/DSDT.aml
#  SECTION RAW = nx/SSDT_icosa.aml
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
!if $(ENABLE_SECUREBOOT) == 0
#  SECTION RAW = Mariko/DBG2.aml
!endif
#  SECTION RAW = Mariko/DSDT.aml
!if $(DEVICE_MODEL) == 1
#  SECTION RAW = nx/SSDT_iowa.aml
!elseif $(DEVICE_MODEL) == 2
#  SECTION RAW = nx/SSDT_hoag.aml
!elseif $(DEVICE_MODEL) == 3
#  SECTION RAW = nx/SSDT_aula.aml
!endif
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
