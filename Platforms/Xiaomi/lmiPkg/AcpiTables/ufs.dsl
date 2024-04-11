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
                    Memory32Fixed (ReadWrite,
                        0x04804000,         // Address Base
                        0x00014000,         // Address Length
                        )
                    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive, ,, )
                    {
                        0x00000184,
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
