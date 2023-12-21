//
//SPMI driver.
//
Device (SPMI)
{
    Name (_HID, "QCOM6015")  // _HID: Hardware ID
    Alias (\_SB.PSUB, _SUB)
    Name (_UID, One)  // _UID: Unique ID
    Name (_CCA, Zero)  // _CCA: Cache Coherency Attribute
    Method (_CRS, 0, NotSerialized)  // _CRS: Current Resource Settings
    {
        Name (RBUF, ResourceTemplate ()
        {
            Memory32Fixed (ReadWrite,
                0x08000000,         // Address Base
                0x02800000,         // Address Length
                )
        })
        Return (RBUF) /* \_SB_.SPMI._CRS.RBUF */
    }

    Method (CONF, 0, NotSerialized)
    {
        Name(XBUF,
        Buffer () {
            0x00,                      // uThisOwnerNumber
            0x01,                      // polling mode
            0x01,                      // reserved channel enable
            0x01, 0xFF,                // reserved channel number (upper byte, lower byte)
            0x00,                      // dynamic channel mode enable
            0x02, 0x00,                // number of channels (upper byte, lower byte)
            0x0A,                      // number of port priorities
            0x07,                      // number of PVC ports
            0x04,                      // number of PVC port PPIDs
            0x07,                      // number of masters
            0x01, 0xFF,                // number of mapping table entries (upper byte, lower byte)
            0x10,                      // number of PIC accumulated status registers
            0x01, 0x00,                // number of Program RAM REGS (upper byte, lower byte)
            0x01,                      // number of SPMI bus controllers
            0x08, 0x00, 0x00, 0x00,    // physical address 0 (byte3, byte2, byte1, byte0)
            0x02, 0x80, 0x00, 0x00     // physical size 0    (byte3, byte2, byte1, byte0)
        }
    )
    Return(XBUF)
    }
}
