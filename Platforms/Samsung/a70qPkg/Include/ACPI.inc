FILE FREEFORM = 7E374E25-8E01-4FEE-87F2-390C23C606CD {
    SECTION RAW = Talos/APIC.aml
    # SECTION RAW = Talos/BERT.aml
    # SECTION RAW = Talos/CSRT.aml
    !if $(ENABLE_SECUREBOOT) == 0
    SECTION RAW = Talos/DBG2.aml
    !endif
    SECTION RAW = a70q/DSDT.aml
    SECTION RAW = Common/SSDT.aml
    SECTION RAW = Talos/FACP.aml
    # SECTION RAW = Talos/FPDT.aml
    SECTION RAW = Talos/GTDT.aml
    SECTION RAW = Talos/IORT.aml
    SECTION RAW = Talos/MCFG.aml
    # SECTION RAW = Talos/MSDM.aml
    SECTION RAW = Talos/PPTT.aml
    SECTION RAW = Talos/SPCR.aml
    SECTION UI = "AcpiTables"
}
