//
// USB (Universal Serial Bus) Role Switch
//
Device (URS0)
{
    Alias (\_SB.PSUB, _SUB)                                                                                                             // Subsystem ID

    Name (_HID, "QCOM0497")                                                                                                             // Hardware ID
    Name (_CID, "PNP0CA1")                                                                                                              // Compatible ID
    Name (_UID, 0)                                                                                                                      // Unique ID
    Name (_CCA, 0)                                                                                                                      // Cache Coherency Attribute

    Name (_CRS, ResourceTemplate ()                                                                                                     // Current Resource Settings
    {
        Memory32Fixed (ReadWrite, 0x0A800000, 0x00100000)                                                                               // USB Base Address & Length
    })

    Method (_STA, 0, NotSerialized) { Return (0x0F) }                                                                                   // Status

    //
    // USB Host Mode
    //
    Device (USB0)
    {
        Name (_ADR, 0)                                                                                                                  // Address
        Name (_S0W, 0)                                                                                                                  // S0 Device Wake State

        Name (_UPC, Package ()                                                                                                          // USB Port Capabilities
        {
            1,                                                                                                                          // Connectable Port
            9,                                                                                                                          // Connector Type
            0,                                                                                                                          // Reserved
            0                                                                                                                           // Reserved
        })

        Name(_PLD, Package()
        {
            Buffer()
            {
                0x82,                                                                                                                   // Revision 2, Ignore Color
                0x00, 0x00, 0x00,                                                                                                       // Color (Ignored)
                0x00, 0x00, 0x00, 0x00,                                                                                                 // Width and Height
                0x69,                                                                                                                   // User Visible; Back Panel; VerticalPos: Center
                0x0C,                                                                                                                   // HorizontalPos: 0 Shape: Vertical Rectangle; GroupOrientation: 0
                0x00, 0x00,                                                                                                             // Group Token: 0 Group Position: 0 So Connector ID is 0
                0x00, 0x00, 0x00, 0x00,                                                                                                 // Not Ejectable
                0xFF, 0xFF, 0xFF, 0xFF                                                                                                  // Vert. and Horiz. Offsets not Supplied
            }
        })

        Name (_CRS, ResourceTemplate ()                                                                                                 // Current Resource Settings
        {
            Interrupt (ResourceConsumer, Level, ActiveHigh, Shared, ,, )        { 0xA3 }                                                // Interrupt
            Interrupt (ResourceConsumer, Level, ActiveHigh, SharedAndWake, ,, ) { 0x17B }                                               // Interrupt
            Interrupt (ResourceConsumer, Level, ActiveHigh, SharedAndWake, ,, ) { 0x113 }                                               // Interrupt
            Interrupt (ResourceConsumer, Edge,  ActiveHigh, SharedAndWake, ,, ) { 0xD4 }                                                // Interrupt (Might be wrong?)
        })

        Method (_STA, 0, NotSerialized) { Return (0x0F) }                                                                               // Status
        Method (CCVL, 0, NotSerialized) { Return (Buffer () { 2 }) }
        Method (HSEN, 0, NotSerialized) { Return (Buffer () { 0 }) }

        Method (_DSM, 4, Serialized)                                                                                                    // Device-Specific Method
        {
            Switch (ToBuffer (Arg0))
            {
                Case (ToUUID ("CE2EE385-00E6-48CB-9F05-2EDB927C4899"))                                                                  // USB Controller GUID
                {
                    Switch (ToInteger (Arg2))
                    {
                        Case (0)
                        {
                            Switch (ToInteger (Arg1))
                            {
                                Case (0)
                                {
                                    Return (Buffer () { 0x1D })
                                }

                                Default
                                {
                                    Return (Buffer () { 1 })
                                }
                            }

                            Return (Buffer () { 0 })
                        }

                        Case (2)
                        {
                            Return (0)
                        }

                        Case (3)
                        {
                            Return (0)
                        }

                        Case (4)
                        {
                            Return (2)
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

        Method (PHYC, 0, Serialized)
        {
            // TODO: Add Proper PHYC here.
            Return ( Package () {} )
        }
    }

    //
    // USB Device Mode
    //
    Device (UFN0)
    {
        Name (_ADR, 0)                                                                                                                  // Address
        Name (_S0W, 0)                                                                                                                  // S0 Device Wake State

        Name (_UPC, Package ()                                                                                                          // USB Port Capabilities
        {
            1,                                                                                                                          // Connectable Port
            9,                                                                                                                          // Connector Type
            0,                                                                                                                          // Reserved
            0                                                                                                                           // Reserved
        })

        Name(_PLD, Package()
        {
            Buffer()
            {
                0x82,                                                                                                                   // Revision 2, Ignore Color
                0x00, 0x00, 0x00,                                                                                                       // Color (Ignored)
                0x00, 0x00, 0x00, 0x00,                                                                                                 // Width and Height
                0x69,                                                                                                                   // User Visible; Back Panel; VerticalPos: Center
                0x0C,                                                                                                                   // HorizontalPos: 0 Shape: Vertical Rectangle; GroupOrientation: 0
                0x00, 0x00,                                                                                                             // Group Token: 0 Group Position: 0 So Connector ID is 0
                0x00, 0x00, 0x00, 0x00,                                                                                                 // Not Ejectable
                0xFF, 0xFF, 0xFF, 0xFF                                                                                                  // Vert. and Horiz. Offsets not Supplied
            }
        })

        Name (_CRS, ResourceTemplate ()                                                                                                 // Current Resource Settings
        {
            Interrupt (ResourceConsumer, Level, ActiveHigh, Shared, ,, )        { 0xA3 }                                                // Interrupt
            Interrupt (ResourceConsumer, Level, ActiveHigh, SharedAndWake, ,, ) { 0x17B }                                               // Interrupt
        })

        Method (_STA, 0, NotSerialized) { Return (0x0F) }                                                                               // Status
        Method (CCVL, 0, NotSerialized) { Return (Buffer () { 2 }) }

        Method (_DSM, 4, Serialized)                                                                                                    // Device-Specific Method
        {
            Switch (ToBuffer (Arg0))
            {
                Case (ToUUID ("FE56CFEB-49D5-4378-A8A2-2978DBE54AD2"))
                {
                    Switch (ToInteger (Arg2))
                    {
                        Case (0)
                        {
                            Switch (ToInteger (Arg1))
                            {
                                Case (0)
                                {
                                    Return (Buffer () { 3 })
                                }

                                Default
                                {
                                    Return (Buffer () { 1 })
                                }
                            }

                            Return (Buffer () { 0 })
                        }

                        Case (1)
                        {
                            Return ( 0x20 )
                        }

                        Default
                        {
                            Return (Buffer () { 0 })
                        }
                    }
                }

                Case (ToUUID ("18DE299F-9476-4FC9-B43B-8AEB713ED751"))
                {
                    Switch (ToInteger (Arg2))
                    {
                        Case (0)
                        {
                            Switch (ToInteger (Arg1))
                            {
                                Case (0)
                                {
                                    Return (Buffer () { 3 })
                                }

                                Default
                                {
                                    Return (Buffer () { 1 })
                                }
                            }

                            Return (Buffer () { 0 })
                        }

                        Case (1)
                        {
                            Return ( 0x39 )
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

        Method (PHYC, 0, Serialized)
        {
            // TODO: Add Proper PHYC here.
            Return ( Package () {} )
        }
    }
}