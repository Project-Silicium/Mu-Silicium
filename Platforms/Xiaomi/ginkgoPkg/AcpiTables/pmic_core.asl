Device (PMIC)
{
    Name (_DEP, Package (0x01)  // _DEP: Dependencies
    {
        SPMI
    })
    Name (_HID, "QCOM0063")  // _HID: Hardware ID
    Name (_CID, "PNP0CA3")  // _CID: Compatible ID
    Method (PMCF, 0, NotSerialized)
    {
        Name (CFG0, Package (0x04)
        {
            0x03, 
            Package (0x02)
            {
                Zero, 
                One
            }, 
            Package (0x02)
            {
                0x02, 
                0x03
            }, 
            Package (0x02)
            {
                0x04, 
                0x05
            }
        })
        Return (CFG0) /* \_SB_.PMIC.PMCF.CFG0 */
    }
}
Device (PM01)
{
    Name (_HID, "QCOM0530")  // _HID: Hardware ID
    Name (_UID, One)  // _UID: Unique ID
    Name (_DEP, Package (One)  // _DEP: Dependencies
    {
        PMIC
    })
    Method (_CRS, 0, NotSerialized)  // _CRS: Current Resource Settings
    {
        Name (RBUF, ResourceTemplate ()
        {
            Interrupt (ResourceConsumer, Level, ActiveHigh, Shared, ,, )
            {
                0x000000D7,
            }
        })
        Return (RBUF) /* \_SB_.PM01._CRS.RBUF */
    }
    Method (_DSM, 4, NotSerialized)  // _DSM: Device-Specific Method
    {
        While (One)
        {
            Name (_T_0, Buffer (0x01)  // _T_x: Emitted by ASL Compiler, x=0-9, A-Z
            {
                    0x00                                             // .
            })
            CopyObject (ToBuffer (Arg0), _T_0) /* \_SB_.PM01._DSM._T_0 */
            If ((_T_0 == ToUUID ("4f248f40-d5e2-499f-834c-27758ea1cd3f") /* GPIO Controller */))
            {
                While (One)
                {
                    Name (_T_1, 0x00)  // _T_x: Emitted by ASL Compiler, x=0-9, A-Z
                    _T_1 = ToInteger (Arg2)
                    If ((_T_1 == Zero))
                    {
                        Return (Buffer (One)
                        {
                                0x03                                             // .
                        })
                    }
                    ElseIf ((_T_1 == One))
                    {
                        Return (Package (0x02)
                        {
                            Zero, 
                            One
                        })
                    }
                    Else
                    {
                    }
                    Break
                }
            }
            Else
            {
                Return (Buffer (One)
                {
                        0x00                                             // .
                })
            }
            Break
        }
    }
}