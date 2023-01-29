// ACPI Tables
#FILE FREEFORM = 7E374E25-8E01-4FEE-87F2-390C23C606CD {
#  SECTION RAW = SDM845Pkg/AcpiTables/DBG2.aml
#  SECTION RAW = SDM845Pkg/AcpiTables/beryllium/DSDT.aml
#  SECTION RAW = SDM845Pkg/AcpiTables/MADT.aml
#  SECTION RAW = SDM845Pkg/AcpiTables/FADT.aml
#  SECTION RAW = SDM845Pkg/AcpiTables/GTDT.aml
#  SECTION RAW = SDM845Pkg/AcpiTables/IORT.aml
#  SECTION RAW = SDM845Pkg/AcpiTables/PPTT.aml
#  SECTION UI = "AcpiTables"
#}

// Mainline device tree blob
!if $(DISPLAY_PANEL) == ebbg
FILE FREEFORM = 25462CDA-221F-47DF-AC1D-259CFAA4E326 {
  SECTION RAW = SDM845Pkg/Devices/beryllium/FdtBlob/sdm845-xiaomi-poco-f1-ebbg.dtb
}
!else
!if $(DISPLAY_PANEL) == tianma
FILE FREEFORM = 25462CDA-221F-47DF-AC1D-259CFAA4E326 {
  SECTION RAW = SDM845Pkg/Devices/beryllium/FdtBlob/sdm845-xiaomi-poco-f1-tianma.dtb
}
!else
!error "Invaild Display Panel! Use ebbg or tianma."
!endif
!endif
