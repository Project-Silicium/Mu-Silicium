//
// GPIO (General-Purpose Input/Output) Controller
//
Device (GPIO)
{
    Alias (\_SB.PSUB, _SUB)                                                                                                             // Subsystem ID

    Name (_HID, "QCOM0016")                                                                                                             // Hardware ID
    Name (_UID, 0)                                                                                                                      // Unique ID

    Name (_CRS, ResourceTemplate ()                                                                                                     // Current Resource Settings
    {
        Memory32Fixed (ReadWrite, 0x03400000, 0x00C00000)                                                                               // Pinctrl Base Address & Length

        Interrupt (ResourceConsumer, Level, ActiveHigh, Shared, ,, ) { 0xF0 }                                                           // Interrupt
        Interrupt (ResourceConsumer, Level, ActiveHigh, Shared, ,, ) { 0xF0 }                                                           // Interrupt
        Interrupt (ResourceConsumer, Level, ActiveHigh, Shared, ,, ) { 0xF0 }                                                           // Interrupt
    })

    Name (OFNI, Buffer () { 0x96, 0x00 }) 
    Name (GABL, 0)

    Method (_STA, 0, NotSerialized) { Return (0x0F) }                                                                                   // Status
    Method (_REG, 2, NotSerialized)                                                                                                     // Region Availability
    {
        if ((Arg0 == 8))
        {
            GABL = Arg1
        }
    }
}
