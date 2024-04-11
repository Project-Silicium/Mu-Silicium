// This file contains common Power Management IC (PMIC) ACPI device definitions
//

//
//
//PMIC KMDF
//
Device (PMIC)
{
    Name (_DEP, Package(0x1)
    {
        \_SB_.SPMI
    })
    Name (_HID, "PMIC6225")
    Name (_CID, "PNP0CA3")

        Method (PMCF) {
        Name (CFG0,
        Package()
                {
                        // PMIC Info
                        1,                          // Number of PMICs, must match the number of info packages
                        Package()
                        {
                                0,
                                1,
                        },
                })
        Return (CFG0)
    }
}

//
// PMIC GPIO PM6125
//
Device (PM01)
{
    Name (_HID, "PM016225")
    Alias(\_SB.PSUB, _SUB)
    Name (_UID, 1)
    Name (_DEP,
        Package(0x1) {
            \_SB_.PMIC
        }
    )

    Method (_CRS, 0x0, NotSerialized) {
        Name (RBUF,
            ResourceTemplate() {
                // QGIC Interrupt Resource
                // Register for SPMI Interrupt 215
                Interrupt(ResourceConsumer, Level, ActiveHigh, Shared, , ,) {215}
            }
        )
        Return (RBUF)
    }

    // _DSM method to mark PM01's ActiveBoth interrupts
    Method(_DSM, 0x4, NotSerialized) {
        // DSM UUID
        switch(ToBuffer(Arg0))
        {
            // ACPI DSM UUID for GPIO
            case(ToUUID("4F248F40-D5E2-499F-834C-27758EA1CD3F"))
            {
                // DSM Function
                switch(ToInteger(Arg2))
                {
                    // Function 0: Return supported functions, based on revision
                    case(0)
                    {
                        // revision 0: function 0 & 1 are supported.
                        return (Buffer() {0x3})
                    }

                    // Function 1: For emulated ActiveBoth controllers, returns
                    //             a package of controller-relative pin numbers.
                    //             Each corresponding pin will have an initial
                    //             polarity of ActiveHigh.
                    case(1)
                    {
                        // Marks pins KPDPWR_ON, RESIN_ON to be ActiveHigh.
                        Return (Package() {0, 1})
                    }

                    default
                    {
                        // Functions 2+: not supported
                    }
                }
            }

            default
            {
                // No other GUIDs supported
                Return(Buffer(One) { 0x00 })
            }
        }
    }
}

