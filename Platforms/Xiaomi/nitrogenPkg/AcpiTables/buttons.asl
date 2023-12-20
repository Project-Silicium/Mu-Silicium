Device (BTNS)
{
    Name (_HID, "ACPI0011") // _HID: Hardware ID
    Alias (PSUB, _SUB)
    Name (_UID, Zero)  // _UID: Unique ID
    Name (_CRS, ResourceTemplate()  // _CRS: Current Resource Settings
    {
        GpioInt (Edge, ActiveBoth, ExclusiveAndWake, PullDown, 0x0000, "\\_SB.PM01", , )
            {   // Pin list
                0x0
            }
        GpioInt (Edge, ActiveBoth, Exclusive, PullUp, 0x0000, "\\_SB.PM01", , )
            {   // Pin list
                0x85
            }
        GpioInt (Edge, ActiveBoth, Exclusive, PullDown, 0x0000, "\\_SB.PM01", , )
            {   // Pin list 
                0x1
            }
    }
    )
    Name(_DSD, Package(2) {
        //UUID for HID Button Descriptors:
        ToUUID("FA6BD625-9CE8-470D-A2C7-B3CA36C4282E"),
        //Data structure for this UUID:
        Package() {
            Package(5) {
                0,    // This is a Collection
                1,    // Unique ID for this Collection
                0,    // This is a Top-Level Collection
                0x01, // Usage Page ("Generic Desktop Page")
                0x0D  // Usage ("Portable Device Control")
            },
            Package(5) {
                1,    // This is a Control
                0,    // Interrupt index in _CRS for Power Button
                1,    // Unique ID of Parent Collection
                0x01, // Usage Page ("Generic Desktop Page")
                0x81  // Usage ("System Power Down")
            },
            Package(5) {
                1,    // This is a Control
                1,    // Interrupt index in _CRS for Volume Up Button
                1,    // Unique ID of Parent Collection
                0x0C, // Usage Page ("Consumer Page")
                0xE9  // Usage ("Volume Increment")
            },
            Package(5) {
                1,    // This is a Control
                2,    // Interrupt index in _CRS for Volume Down Button
                1,    // Unique ID of Parent Collection
                0x0C, // Usage Page ("Consumer Page")
                0xEA  // Usage ("Volume Decrement")
            }
        }
    })
}
