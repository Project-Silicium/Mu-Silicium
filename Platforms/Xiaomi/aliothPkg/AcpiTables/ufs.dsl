Device (UFS0)
{
    Method (_STA, 0, NotSerialized)  // _STA: Status
    {
        Return (0x0F)
    }

    Name (_HID, "QCOM24A5")  // _HID: Hardware ID
    Alias (^EMUL, EMUL)
    Name (_UID, Zero)  // _UID: Unique ID
    Name (_CCA, Zero)  // _CCA: Cache Coherency Attribute
    Method (_CRS, 0, NotSerialized)  // _CRS: Current Resource Settings
    {
        Name (RBUF, ResourceTemplate ()
        {
		
			/* Device UFS controller is at 0x1D8400: run "getprop | grep ufs" = [ro.boot.bootdevice]: [1d8400.ufshc]
			On device tree ufs device with 1D8400 address is: soc { > ufshc@1d84000 {
			It's registry address is
				reg = <0x1d84000 0x3000 0x1d90000 0x8000>;
			starting at 1d84000 and ending at 0x1d90000+0x8000=0x1d98000
			so it's length is 0x1d98000-1d84000=0x14000 */
		
            Memory32Fixed (ReadWrite,
                0x01D84000,         // Address Base
                0x00014000,         // Address Length
            )
			
			/* interrupts = <0x00 0x109 0x04>;
			0x109 in dec is 265, 265 +32 GIC shift = 297, 297 in hex is 0x129 */
			
            Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive, ,, )
            {
                0x00000129,
            }
        })
        Return (RBUF) /* \_SB_.UFS0._CRS.RBUF */
    }

    Device (DEV0)
    {
        Method (_ADR, 0, NotSerialized)  // _ADR: Address
        {
            Return (0x08)
        }

        Method (_RMV, 0, NotSerialized)  // _RMV: Removal Status
        {
            Return (Zero)
        }
    }
}