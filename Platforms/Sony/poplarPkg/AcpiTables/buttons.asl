//
// Buttons
//
Device (BTNS)
{
    Name (_HID, "ACPI0011")                                                                                                             // Hardware ID
    Name (_UID, 0)                                                                                                                      // Unique ID

    Alias (\_SB.PSUB, _SUB)                                                                                                             // Subsystem ID

    Name (_CRS, ResourceTemplate ()                                                                                                     // Current Resource Settings
    {
        GpioInt (Edge, ActiveBoth, ExclusiveAndWake, PullDown, 0x0000, "\\_SB.PM01", 0x00, ResourceConsumer, ,) { 0 }                   // GPIO Pin (Power Button)
        GpioInt (Edge, ActiveBoth, Exclusive,        PullUp,   0x0000, "\\_SB.PM01", 0x00, ResourceConsumer, ,) { 0x85 }                // GPIO Pin (Volume Up)
        GpioInt (Edge, ActiveBoth, Exclusive,        PullDown, 0x0000, "\\_SB.PM01", 0x84, ResourceConsumer, ,) { 1 }                   // GPIO Pin (Volume Down)
    })

    Name (_DSD, Package ()                                                                                                              // Device-Specific Data
    {
        ToUUID ("FA6BD625-9CE8-470D-A2C7-B3CA36C4282E"),
        Package ()
        {
            Package ()
            {
                0,
                1,
                0,
                1,
                13
            },

            Package ()
            {
                1,
                0,
                1,
                1,
                129
            },

            Package ()
            {
                1,
                1,
                1,
                12,
                233
            },

            Package ()
            {
                1,
                2,
                1,
                12,
                234
            },
        }
    })
}