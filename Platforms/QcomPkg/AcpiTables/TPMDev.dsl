//  Copyright (c), Microsoft Corporation. All rights reserved.
//
//  This program and the accompanying materials
//  are licensed and made available under the terms and conditions of the BSD License
//  which accompanies this distribution.  The full text of the license may be found at
//  http://opensource.org/licenses/bsd-license.php
//
//  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
//  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
//

//
// SSDT definitions for the software TPM2 device and Microsoft FTPM simulator
//

DefinitionBlock ("TPMDev.dat", "SSDT", 2, "MSFT  ", "fTPM    ", 0x00000002)
{
    Scope (\_SB)
    {
        //
        // Description: Software TPM2.0
        //
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
                Name (RBUF, ResourceTemplate ()
                {
                })
                Return (RBUF) /* \_SB_.TPM2._CRS.RBUF */
            }

            Method (_STA, 0, NotSerialized)  // _STA: Status
            {
                Return (0x0F)
            }
        }

        //
        // Description: Microsoft FTPM Simulator
        //
        Device (FSIM)
        {
            Name (_ADR, Zero)  // _ADR: Address
            Name (_HID, "MPTF8888")  // _HID: Hardware ID
            Name (_UID, One)  // _UID: Unique ID
            Name (_DDN, "Microsoft fTPM Simulator")  // _DDN: DOS Device Name
            Name (_STR, Unicode ("Microsoft fTPM Simulator"))  // _STR: Description String
            Method (_CRS, 0, NotSerialized)  // _CRS: Current Resource Settings
            {
                Name (RBUF, ResourceTemplate ()
                {
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
