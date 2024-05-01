//
// ABD (?)
//
Device (ABD)
{
    Name (_HID, "QCOM003F")                                                                                                             // Hardware ID
    Name (_UID, 0)                                                                                                                      // Unique ID

    Name (AVBL, Zero)

    OperationRegion (ROP1, GenericSerialBus, Zero, 0x0100)

    Method (_REG, 2, NotSerialized)                                                                                                     // Region Availability
    {
        If ((Arg0 == 0x09))
        {
            AVBL = Arg1
        }
    }

    Method (_STA, 0, NotSerialized)                                                                                                     // Status
    {
        Return (0x0B)
    }
}