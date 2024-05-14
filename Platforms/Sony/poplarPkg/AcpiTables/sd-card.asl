//
// SD Card
//
Device (SDC2)
{
    Alias (\_SB.PSUB, _SUB)                                                                                                             // Subsystem ID

    Name (_HID, "QCOM2466")                                                                                                             // Hardware ID
    Name (_CID, "ACPI\QCOM2466")                                                                                                        // Compatible ID
    Name (_UID, 0)                                                                                                                      // Unique ID
    Name (_CCA, 0)                                                                                                                      // Cache Coherency Attribute

    Name (_DEP, Package ()                                                                                                              // Operation Region Dependencies
    {
        \_SB.GPIO
    })

    Name (_CRS, ResourceTemplate ()                                                                                                     // Current Resource Settings
    {
        Memory32Fixed (ReadWrite, 0x0C0A4900, 0x00000314)                                                                               // SDHCI Base Address & Length

        Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive, ,, ) { 157 }                                                         // Interrupt
        //Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive, ,, ) { 253 }                                                       // Interrupt (Breaks Windows Boot)

        GpioInt (Edge, ActiveBoth, SharedAndWake, PullNone, 0x1388, "\\_SB.GPIO", 0, ResourceConsumer, ,) { 95 }                        // GPIO Pin
        GpioIo  (Shared, PullNone, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer,, RawDataBuffer () { 1 }) { 95 }          // GPIO IO
    })

    Method (_DIS, 0, NotSerialized) {}                                                                                                  // Disable Device
    Method (_STA, 0, NotSerialized) { Return (0x0F) }                                                                                   // Status
}