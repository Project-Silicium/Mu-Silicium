//
// PMIC (Power Management Integrated Circuit) Core
//
Device (PMIC)
{
    Name (_HID, "QCOM0063")                                                                                                             // Hardware ID

    Alias (\_SB.PSUB, _SUB)                                                                                                             // Subsystem ID

    Name (_DEP, Package ()                                                                                                              // Operation Region Dependencies
    {
        \_SB.SPMI
    })

    Method (_STA, 0, NotSerialized) { Return (0x0F) }                                                                                   // Status

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
}

//
// PMIC PM8998 Nr. 1
//
Device (PM01)
{
    Name (_HID, "QCOM0066")                                                                                                             // Hardware ID
    Name (_UID, 1)                                                                                                                      // Unique ID

    Alias (\_SB.PSUB, _SUB)                                                                                                             // Subsystem ID

    Name (_DEP, Package ()                                                                                                              // Operation Region Dependencies
    {
        \_SB.PMIC
    })

    Method (_STA, 0, NotSerialized) { Return (0x0B) }                                                                                   // Status

    Name (_CRS, ResourceTemplate ()                                                                                                     // Current Resource Settings
    {
        Interrupt (ResourceConsumer, Level, ActiveHigh, Shared, ,, ) { 0x166 }                                                          // Interrupt
    })

    Name (CONF, Buffer ()                                                                                                               // Configuration
    {
        0x00, 0x01, 0x01, 0x01, 0xFF, 0x00, 0x02, 0x00,
        0x07, 0x06, 0x04, 0x07, 0x01, 0xFF, 0x10, 0x01,
        0x00, 0x01, 0x08, 0x00, 0x00, 0x00, 0x02, 0x80,
        0x00, 0x00
    })

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
// PMIC PM8998 Nr. 2
//
Device (PM02)
{
    Name (_HID, "QCOM0066")                                                                                                             // Hardware ID
    Name (_UID, 2)                                                                                                                      // Unique ID

    Alias (\_SB.PSUB, _SUB)                                                                                                             // Subsystem ID

    Name (_DEP, Package ()                                                                                                              // Operation Region Dependencies
    {
        \_SB.PMIC
    })

    Method (_STA, 0, NotSerialized) { Return (0x0B) }                                                                                   // Status

    Name (_CRS, ResourceTemplate ()                                                                                                     // Current Resource Settings
    {
        Interrupt (ResourceConsumer, Level, ActiveHigh, Shared, ,, ) { 0x166 }                                                          // Interrupt
    })

    Name (CONF, Buffer ()                                                                                                               // Configuration
    {
        0x00, 0x01, 0x01, 0x01, 0xFF, 0x00, 0x02, 0x00,
        0x07, 0x06, 0x04, 0x07, 0x01, 0xFF, 0x10, 0x01,
        0x00, 0x01, 0x08, 0x00, 0x00, 0x00, 0x02, 0x80,
        0x00, 0x00
    })

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
// PMIC Apps
//
Device (PMAP)
{
    Name (_HID, "QCOM0065")                                                                                                             // Hardware ID
    Name (BUFF, Buffer (4) {})                                                                                                          // Buffer

    Alias (\_SB.PSUB, _SUB)                                                                                                             // Subsystem ID

    Name (_DEP, Package ()                                                                                                              // Dependencies
    {
        \_SB.PMIC, 
        \_SB.ABD, 
        \_SB.SCM0
    })

    Method (_STA, 0, NotSerialized) { Return (0x0B) }                                                                                   // Status

    Method (GEPT, 0, Serialized)
    {
        //CreateByteField (BUFF, 0, STAT)
        CreateWordField (BUFF, 2, DATA)

        DATA = 2

        Return (DATA)
    }

    Name (_CRS, Buffer () { 0x79, 0x00 })                                                                                               // Current Resource Settings
}
