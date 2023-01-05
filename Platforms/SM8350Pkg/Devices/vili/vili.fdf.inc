// ACPI Tables
FILE FREEFORM = 7E374E25-8E01-4FEE-87F2-390C23C606CD {
  SECTION RAW = SM8350Pkg/Devices/vili/AcpiTables/DSDT.aml
  SECTION RAW = SM8350Pkg/AcpiTables/APIC.aml
  SECTION RAW = SM8350Pkg/AcpiTables/FACP.aml
  SECTION RAW = SM8350Pkg/AcpiTables/GTDT.aml
  SECTION RAW = SM8350Pkg/AcpiTables/BGRT.aml
  SECTION UI = "AcpiTables" 
}

// Mainline device tree blob
# FILE FREEFORM = 25462CDA-221F-47DF-AC1D-259CFAA4E326 {
#   SECTION RAW = SM8350Pkg/Devices/$(TARGET_DEVICE)/FdtBlob/sm8350-xiaomi-11t-pro.dtb
# }
