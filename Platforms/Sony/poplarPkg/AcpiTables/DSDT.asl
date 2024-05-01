DefinitionBlock ("", "DSDT", 2, "QCOM  ", "MSM8998 ", 0x00000003)
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
        Include ("sd-card.asl")

        //
        // ABD
        //
        Include ("abd.asl")

        //
        // PMIC (Maybe Finisched?)
        //
        Include ("pmic.asl")

        //
        // PEP (Incomplete)
        //
        Include ("pep.asl")

        //
        // SCM
        //
        Include ("scm.asl")

        //
        // SPMI
        //
        Include ("spmi.asl")

        //
        // GPIO Controller
        //
        Include ("gpio.asl")

        //
        // CPU Cores & Clusters
        //
        Include ("cpu.asl")

        //
        // USB Port (Incomplete)
        //
        Include ("usb.asl")
    }
}