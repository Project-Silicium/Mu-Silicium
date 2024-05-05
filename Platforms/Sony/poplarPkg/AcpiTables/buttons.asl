//
// Buttons
//
Device (BTNS)
{
    Alias (\_SB.PSUB, _SUB)                                                                                                             // Subsystem ID

    Name (_HID, "ACPI0011")                                                                                                             // Hardware ID
    Name (_UID, 0)                                                                                                                      // Unique ID

    Name (_CRS, ResourceTemplate ()                                                                                                     // Current Resource Settings
    {
        GpioInt (Edge, ActiveLow, ExclusiveAndWake, PullUp, 0, "\\_SB.PM01", ,) { 64 }                                                  // Power Button       (Index: 0)
        GpioInt (Edge, ActiveLow, Exclusive,        PullUp, 0, "\\_SB.PM01", ,) { 65 }                                                  // Volume Up Button   (Index: 1)
    })

    Name (_DSD, Package ()                                                                                                              // Device-Specific Data
    {
        ToUUID ("FA6BD625-9CE8-470D-A2C7-B3CA36C4282E"),                                                                                // HID Button Descriptor GUID

        Package ()                                                                                                                      // Data Structure for HID Button Descriptor
        {
            Package ()
            {
                0,                                                                                                                      // Type                (Collection)
                1,                                                                                                                      // Unique ID
                0,                                                                                                                      // Level Type          (Top-Level)
                1,                                                                                                                      // Page Type           (Generic Desktop Page)
                13                                                                                                                      // Control Type        (Portable Device Control)
            }, 

            Package ()
            {
                1,                                                                                                                      // Type                (Control)
                0,                                                                                                                      // Button Index
                1,                                                                                                                      // Unique ID of Parent
                1,                                                                                                                      // Page Type           (Generic Desktop Page)
                129                                                                                                                     // Button Function     (System Power Down)
            }, 

            Package ()
            {
                1,                                                                                                                      // Type                (Control)
                1,                                                                                                                      // Button Index
                1,                                                                                                                      // Unique ID of Parent
                12,                                                                                                                     // Page Type           (Consumer Page)
                233                                                                                                                     // Button Function     (Volume Increment)
            }, 

            /*
            Package ()
            {
                1,                                                                                                                      // Type                (Control)
                2,                                                                                                                      // Button Index
                1,                                                                                                                      // Unique ID of Parent
                12,                                                                                                                     // Page Type           (Consumer Page)
                234                                                                                                                     // Button Function     (Volume Decrement)
            }
            */
        }
    })
}