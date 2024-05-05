//
// PMIC (Power Management Integrated Circuit) Core
//
Device (PMIC)
{
    Alias (\_SB.PSUB, _SUB)                                                                                                             // Subsystem ID

    Name (_HID, "QCOM0063")                                                                                                             // Hardware ID
    Name (_DEP, Package () { \_SB.SPMI })                                                                                               // Operation Region Dependencies

    Name (PMCF, Package ()                                                                                                              // PMIC Configuration
    {
        3,                                                                                                                              // Number of PMIC Packages
        Package ()
        {
            0,                                                                                                                          // PMIC Nr. 1
            1                                                                                                                           // PMIC Nr. 2
        },

        Package ()
        {
            2,                                                                                                                          // PMIC Nr. 3
            3                                                                                                                           // PMIC Nr. 4
        },

        Package ()
        {
            4,                                                                                                                          // PMIC Nr. 5
            5                                                                                                                           // PMIC Nr. 6
        },
    })

    Method (_STA, 0, NotSerialized) { Return (0x0F) }                                                                                   // Status
}

//
// PMIC PM8998 Nr. 1
//
Device (PM01)
{
    Alias (\_SB.PSUB, _SUB)                                                                                                             // Subsystem ID

    Name (_HID, "QCOM0066")                                                                                                             // Hardware ID
    Name (_UID, 1)                                                                                                                      // Unique ID

    Name (_DEP, Package () { \_SB.PMIC })                                                                                               // Operation Region Dependencies

    Name (_CRS, ResourceTemplate ()                                                                                                     // Current Resource Settings
    {
        Interrupt (ResourceConsumer, Level, ActiveHigh, Shared, ,, ) { 358 }                                                            // Interrupt
    })

    Name (PMIO, Package () { 0, 0, 4160, 64, 26, 192, 0, 160, 0, 0, 0 })

    Name (CONF, Buffer ()                                                                                                               // Configuration
    {
        0x00, 0x01, 0x01, 0x01, 0xFF, 0x00, 0x02, 0x00,
        0x07, 0x06, 0x04, 0x07, 0x01, 0xFF, 0x10, 0x01,
        0x00, 0x01, 0x08, 0x00, 0x00, 0x00, 0x02, 0x80,
        0x00, 0x00
    })

    Method (_STA, 0, NotSerialized) { Return (0x0F) }                                                                                   // Status

    Method (_DSM, 4, Serialized)                                                                                                        // Device-Specific Method
    {
        Switch (ToBuffer (Arg0))
        {
            Case (ToUUID ("4F248F40-D5E2-499F-834C-27758EA1CD3F"))                                                                      // GUID of GPIO
            {
                Switch (ToInteger (Arg2))
                {
                    Case (0)
                    {
                        Return (Buffer () { 3 })
                    }

                    Case (1)
                    {
                        Return (Package () { 0x40, 0x41 })
                    }

                    Default
                    {
                        Return (Buffer () { 0 })
                    }
                }
            }

            Default
            {
                Return (Buffer () { 0 })
            }
        }
    }
}

//
// PMIC PM8998 Nr. 2 (Disabled because it dosen't have GPIOs)
//
/*
Device (PM02)
{
    Alias (\_SB.PSUB, _SUB)                                                                                                             // Subsystem ID

    Name (_HID, "QCOM0066")                                                                                                             // Hardware ID
    Name (_UID, 2)                                                                                                                      // Unique ID

    Name (_DEP, Package () { \_SB.PMIC })                                                                                               // Operation Region Dependencies

    Name (_CRS, ResourceTemplate ()                                                                                                     // Current Resource Settings
    {
        Interrupt (ResourceConsumer, Level, ActiveHigh, Shared, ,, ) { 358 }                                                            // Interrupt
    })

    Name (PMIO, Package () { 1, 2, 4160, 64, 14, 192, 0, 160, 0, 0, 0 })

    Name (CONF, Buffer ()                                                                                                               // Configuration
    {
        0x00, 0x01, 0x01, 0x01, 0xFF, 0x00, 0x02, 0x00,
        0x07, 0x06, 0x04, 0x07, 0x01, 0xFF, 0x10, 0x01,
        0x00, 0x01, 0x08, 0x00, 0x00, 0x00, 0x02, 0x80,
        0x00, 0x00
    })

    Method (_STA, 0, NotSerialized) { Return (0x0F) }                                                                                   // Status

    Method (_DSM, 4, Serialized)                                                                                                        // Device-Specific Method
    {
        Switch (ToBuffer (Arg0))
        {
            Case (ToUUID ("4F248F40-D5E2-499F-834C-27758EA1CD3F"))                                                                      // GUID of GPIO
            {
                Switch (ToInteger (Arg2))
                {
                    Case (0)
                    {
                        Return (Buffer () { 3 })
                    }

                    Case (1)
                    {
                        Return (Package () { 0x1001, 0x1002, 0x8A })
                    }

                    Default
                    {
                        Return (Buffer () { 0 })
                    }
                }
            }

            Default
            {
                Return (Buffer () { 0 })
            }
        }
    }
}
*/
