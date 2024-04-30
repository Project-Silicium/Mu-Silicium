//
// SD Card
//
Device (SDC2)
{
    Name (_HID, "QCOM2466")                                                                                                             // Hardware ID
    Name (_CID, "ACPIQCOM2466")                                                                                                         // Compatible ID
    Name (_UID, 0)                                                                                                                      // Unique ID
    Name (_CCA, 0)                                                                                                                      // Cache Coherency Attribute

    Alias (\_SB.PSUB, _SUB)                                                                                                             // Subsystem ID

    Name (_DEP, Package ()                                                                                                              // Operation Region Dependencies
    {
        \_SB.PEP0,
        \_SB.GPIO
    })

    Name (_CRS, ResourceTemplate ()                                                                                                     // Current Resource Settings
    {
        Memory32Fixed (ReadWrite, 0x0C0A4900, 0x00000314)                                                                               // SDHCI Base Address & Length

        Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive, ,, ) { 0x9D }                                                        // Interrupt
        //Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive, ,, ) { 0xFD }                                                      // Interrupt (Breaks Windows Boot)

        GpioInt (Edge, ActiveBoth, SharedAndWake, PullNone, 0x1388, "\\_SB.GPIO", 0, ResourceConsumer, ,) { 0x5F }                      // GPIO Pin

        GpioIo  (Shared, PullNone, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer,, RawDataBuffer () { 1 }) { 0x5F }        // GPIO IO
    })

    Method (_DIS, 0, NotSerialized) {}                                                                                                  // Disable Device
    Method (_STA, 0, NotSerialized) { Return (0x0F) }                                                                                   // Status
}