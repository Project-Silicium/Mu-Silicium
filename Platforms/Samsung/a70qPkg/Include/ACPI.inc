FILE FREEFORM = FF000D60-DD35-4601-9C25-B5EF72AACAE9 {
    SECTION RAW = Talos/APIC.aml
    # SECTION RAW = Talos/BERT.aml
    # SECTION RAW = Talos/CSRT.aml
    !if $(ENABLE_SECUREBOOT) == 0
    # SECTION RAW = Talos/DBG2.aml
    !endif
    SECTION RAW = Talos/DSDT_minimal.aml
    SECTION RAW = Common/SSDT.aml
    # SECTION RAW = Talos/FADT.aml
    SECTION RAW = Talos/FACP.aml
    SECTION RAW = Talos/GTDT.aml
    # SECTION RAW = Talos/IORT.aml
    # SECTION RAW = Talos/MCFG.aml
    # SECTION RAW = Talos/MSDM.aml
    # SECTION RAW = Talos/PPTT.aml
    # SECTION RAW = Talos/SPCR.aml
    SECTION UI = "AcpiTables"
}
