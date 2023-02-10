DefinitionBlock ("", "SSDT", 2, "MSFT  ", "fTPM    ", 0x00000002)
{
    Scope (\_SB)
    {
        Device (TPM2)
        {
            Name (_ADR, Zero)  // _ADR: Address
            Name (_HID, "FTPM0101")  // _HID: Hardware ID
            Name (_CID, "MSFT0101" /* TPM 2.0 Security Device */)  // _CID: Compatible ID
            Name (_UID, One)  // _UID: Unique ID
            Name (_DDN, "Microsoft TPM 2.0")  // _DDN: DOS Device Name
            Name (_STR, Unicode ("Microsoft TPM 2.0"))  // _STR: Description String
            Method (_CRS, 0, NotSerialized)  // _CRS: Current Resource Settings
            {
                Name (RBUF, Buffer (0x02)
                {
                     0x79, 0x00                                       // y.
                })
                Return (RBUF) /* \_SB_.TPM2._CRS.RBUF */
            }

            Method (_STA, 0, NotSerialized)  // _STA: Status
            {
                Return (0x0F)
            }
        }

        Device (FSIM)
        {
            Name (_ADR, Zero)  // _ADR: Address
            Name (_HID, "MPTF8888")  // _HID: Hardware ID
            Name (_UID, One)  // _UID: Unique ID
            Name (_DDN, "Microsoft fTPM Simulator")  // _DDN: DOS Device Name
            Name (_STR, Unicode ("Microsoft fTPM Simulator"))  // _STR: Description String
            Method (_CRS, 0, NotSerialized)  // _CRS: Current Resource Settings
            {
                Name (RBUF, Buffer (0x02)
                {
                     0x79, 0x00                                       // y.
                })
                Return (RBUF) /* \_SB_.FSIM._CRS.RBUF */
            }

            Method (_STA, 0, NotSerialized)  // _STA: Status
            {
                Return (0x0F)
            }
        }
    }
}

