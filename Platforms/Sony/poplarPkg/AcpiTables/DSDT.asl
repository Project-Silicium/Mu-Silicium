DefinitionBlock ("", "DSDT", 2, "QCOMM ", "MSM8998 ", 0x00000003)
{
    Scope (\_SB)
    {
        //
        // Global Variables
        //
        Name (PSUB, "MTP08998")

        //
        // SD Card
        //
        //Include ("sdcard.asl")

        //
        // PMIC
        //
        //Include ("pmic.asl")

        //
        // PEP (Incomplete)
        //
        Include ("pep.asl")

        //
        // SPMI
        //
        //Include ("spmi.asl")

        //
        // GPIO Controller
        //
        //Include ("gpio.asl")

        //
        // CPU Cores & Clusters
        //
        Include ("cpu.asl")

        //
        // USB Port (Incomplete)
        //
        Include ("usb.asl")

        //
        // Buttons (Incomplete)
        //
        //Include ("buttons.asl")
    }
}