// This file contains the Bus Access Modules (BAM)
// ACPI device definitions and pipe configurations
//

//
//  Device Map:
//    0x2401 - BAM
//
//  List of Devices
//    BAM1  - CRYPTO1
//    BAM5  - SLIMBUS1
//    BAM6  - SLIMBUS
//    BAM7  - TSIF
//    BAMD  - USB3.0 secondary
//    BAME  - QDSS 
//    BAMF  - USB3.0 primary
Device (BAM1)
{
    Name (_HID, "QCOM050A")
	Alias(\_SB.PSUB, _SUB)
    Name (_UID, 1)
    Name (_CCA, 0)
    Method (_CRS, 0x0, NotSerialized) {
        Name (RBUF, ResourceTemplate ()
        {
            // CRYPTO0 register address space
            Memory32Fixed (ReadWrite, 0x1B04000, 0x00024000)

            Interrupt(ResourceConsumer, Edge, ActiveHigh, Exclusive, , , ) {279}
        })
        Return (RBUF)
    }
}

Device (BAM5)
{
    Name (_HID, "QCOM050A")
	Alias(\_SB.PSUB, _SUB)
    Name (_UID, 5)
    Name (_CCA, 0)
    Method (_CRS, 0x0, NotSerialized) {
        Name (RBUF, ResourceTemplate ()
        {
            // AUD_SLIMBUS register address space
            Memory32Fixed (ReadWrite, 0x0A584000, 0x00020000)

            Interrupt(ResourceConsumer, Edge, ActiveHigh, Exclusive, , , ) {316}
        })
        Return (RBUF)
    }
}

Device (BAMD)
{
    Name (_HID, "QCOM050A")
	Alias(\_SB.PSUB, _SUB)
    Name (_UID, 13)
    Name (_CCA, 0)
    Method (_CRS, 0x0, NotSerialized) {
        Name (RBUF, ResourceTemplate ()
        {
            // USB30 sec register address space
            Memory32Fixed (ReadWrite, 0x04904000, 0x00017000)

            Interrupt(ResourceConsumer, Edge, ActiveHigh, Exclusive, , , ) {169}
        })
        Return (RBUF)
    }
}

//Device (BAME)                                   //I cant find it
//{
//    Name (_HID, "QCOM050A")
//	Alias(\_SB.PSUB, _SUB)
//    Name (_UID, 14)
//    Name (_CCA, 0)
//    Method (_CRS, 0x0, NotSerialized) {
//        Name (RBUF, ResourceTemplate ()
//        {
//            // QDSS register address space
//            Memory32Fixed (ReadWrite, 0x8064000, 0x00015000)
//
//            Interrupt(ResourceConsumer, Edge, ActiveHigh, Exclusive, , , ) {199}
//        })
//        Return (RBUF)
//    }
//}

Device (BAMF)
{
    Name (_HID, "QCOM050A")
	Alias(\_SB.PSUB, _SUB)
    Name (_UID, 15)
    Name (_CCA, 0)
    Method (_CRS, 0x0, NotSerialized) {
        Name (RBUF, ResourceTemplate ()
        {
            // USB30 PRI register address space
            Memory32Fixed (ReadWrite, 0x04F04000, 0x00017000)

            Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive, , , ) {285}
        })
        Return (RBUF)
    }
}
