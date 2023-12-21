Device (SDC1)
{
    //Name (_DEP, Package (One)  // _DEP: Dependencies
    //{
    //    \_SB.PEP0
    //})
    Name (_HID, "QCOM24BF")  // _HID: Hardware ID
    Name (_CID, "ACPIQCOM24BF")  // _CID: Compatible ID
    Name (_UID, Zero)  // _UID: Unique ID
    Name (_CCA, Zero)  // _CCA: Cache Coherency Attribute
    Method (_CRS, 0, NotSerialized)  // _CRS: Current Resource Settings
    {
        Name (RBUF, ResourceTemplate ()
        {
            Memory32Fixed (ReadWrite,
                0x0C0C4000,         // Address Base
                0x00001000,         // Address Length
                )
            
            Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive, ,, ) { 142, }
            //hc_irq + GIC SPI = 112 + 32 = 142
            
            Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive, ,, ) { 144, } 
            //pwr_irq + GIC SPI = 110 + 32 = 144
        })
        Return (RBUF)
    }

    Device (EMMC)
    {
        Method (_ADR, 0, NotSerialized)  // _ADR: Address
        {
            Return (0x08)
        }

        Method (_RMV, 0, NotSerialized)  // _RMV: Removal Status
        {
            Return (0)
        }
    }

    Method (_DIS, 0, NotSerialized)  // _DIS: Disable Device
    {
    }

    Method (_STA, 0, NotSerialized)  // _STA: Status
    {
        Return (0x0F)
    }
}
