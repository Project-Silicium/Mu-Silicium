//
// USB (Universal Serial Bus) Role Switch
//
Device (URS0)
{
    Name (_HID, "QCOM0497")                                                                                                             // Hardware ID
    Name (_CID, "PNP0CA1")                                                                                                              // Compatible ID
    Name (_UID, 0)                                                                                                                      // Unique ID
    Name (_CCA, 0)                                                                                                                      // Cache Coherency Attribute

    Alias (\_SB.PSUB, _SUB)                                                                                                             // Subsystem ID

    Name (_DEP, Package ()                                                                                                              // Operation Region Dependencies
    {
        \_SB.PEP0
    })

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

        Name (_PLD, Package ()                                                                                                          // Physical Location of Device
        {
            ToPLD (
                PLD_Revision           = 2,                                                                                             // Revision
                PLD_IgnoreColor        = 1,                                                                                             // Ignore Color
                PLD_Red                = 0,                                                                                             // Color Red
                PLD_Green              = 0,                                                                                             // Color Green
                PLD_Blue               = 0,                                                                                             // Color Blue
                PLD_Width              = 0,                                                                                             // Width
                PLD_Height             = 0,                                                                                             // Height
                PLD_UserVisible        = 1,                                                                                             // User Visible
                PLD_Dock               = 0,                                                                                             // Dock
                PLD_Lid                = 0,                                                                                             // Lid
                PLD_Panel              = 6,                                                                                             // Panel
                PLD_VerticalPosition   = 0,                                                                                             // Vetrical Position
                PLD_HorizontalPosition = 0,                                                                                             // Horizontal Position
                PLD_Shape              = 7,                                                                                             // Shape
                PLD_GroupOrientation   = 0,                                                                                             // Group Orientation
                PLD_GroupToken         = 0,                                                                                             // Group Token
                PLD_GroupPosition      = 0,                                                                                             // Group Position
                PLD_Bay                = 0,                                                                                             // Bay
                PLD_Ejectable          = 0,                                                                                             // Ejectable
                PLD_EjectRequired      = 0,                                                                                             // OSPM Ejection Required
                PLD_CabinetNumber      = 0,                                                                                             // Cabinet Number
                PLD_CardCageNumber     = 0,                                                                                             // Card Cage Number
                PLD_Reference          = 0,                                                                                             // Reference
                PLD_Rotation           = 0,                                                                                             // Rotation
                PLD_Order              = 0,                                                                                             // Order
                PLD_VerticalOffset     = 0xFFFF,                                                                                        // Vertical Offset
                PLD_HorizontalOffset   = 0xFFFF                                                                                         // Horizontal Offset
            )
        })

        Name (_CRS, ResourceTemplate ()                                                                                                 // Current Resource Settings
        {
            Interrupt (ResourceConsumer, Level, ActiveHigh, Shared, ,, )        { 0xA3 }                                                // Interrupt
            Interrupt (ResourceConsumer, Level, ActiveHigh, SharedAndWake, ,, ) { 0x17B }                                               // Interrupt
            Interrupt (ResourceConsumer, Level, ActiveHigh, SharedAndWake, ,, ) { 0x113 }                                               // Interrupt
            Interrupt (ResourceConsumer, Edge,  ActiveHigh, SharedAndWake, ,, ) { 0xD4 }                                                // Interrupt (Might be wrong?)
        })

        Method (_STA, 0, NotSerialized) { Return (0x0F) }                                                                               // Status
        Method (CCVL, 0, NotSerialized) { Return (2) }

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
            Return (0)
        }
    }

    // TODO: Add Device Mode here.
}