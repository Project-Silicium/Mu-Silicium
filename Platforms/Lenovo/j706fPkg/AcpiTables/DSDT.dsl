DefinitionBlock ("", "DSDT", 2, "QCOMM ", "SDM7150 ", 0x00000003)
{
    Scope (_SB)
    {
        Name (PSUB, "IDP07150")
        Name (SOID, 0xFFFFFFFF)
        Name (STOR, 0xABCABCAB)
        Name (SIDS, "899800000000000")
        Name (SIDV, 0xFFFFFFFF)
        Name (SVMJ, 0xFFFF)
        Name (SVMI, 0xFFFF)
        Name (SDFE, 0xFFFF)
        Name (SFES, "899800000000000")
        Name (SIDM, 0x0000000FFFFFFFFF)
        Name (PUS3, 0xFFFFFFFF)
        Name (SIDT, 0xFFFFFFFF)
        Name (SOSN, 0xAAAAAAAABBBBBBBB)
        Name (PLST, 0xFFFFFFFF)
        Name (RMTB, 0xAAAAAAAA)
        Name (RMTX, 0xBBBBBBBB)
        Name (RFMB, 0xCCCCCCCC)
        Name (RFMS, 0xDDDDDDDD)
        Name (RFAB, 0xEEEEEEEE)
        Name (RFAS, 0x77777777)
        Name (TPMA, 0x11111111)
        Name (TDTV, 0x6654504D)
        Name (TCMA, 0xDEADBEEF)
        Name (TCML, 0xBEEFDEAD)
        Name (SOSI, 0xDEADBEEFFFFFFFFF)
        Name (PRP0, 0xFFFFFFFF)
        Name (PRP1, 0xFFFFFFFF)
        Name (PRP2, 0xFFFFFFFF)
        Name (PRP3, 0xFFFFFFFF)
        Device (UFS0)
        {
            Method (_STA, 0, NotSerialized)  // _STA: Status
            {
                Return (0x0F)
            }

            Name (_HID, "QCOM24A5")  // _HID: Hardware ID
            Alias (PSUB, _SUB)
            Name (_UID, Zero)  // _UID: Unique ID
            Name (_CCA, Zero)  // _CCA: Cache Coherency Attribute
            Method (_CRS, 0, NotSerialized)  // _CRS: Current Resource Settings
            {
                Name (RBUF, ResourceTemplate ()
                {
                    Memory32Fixed (ReadWrite,
                        0x01D84000,         // Address Base
                        0x00014000,         // Address Length
                        )
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

        Device (SYSM)
        {
            Name (_HID, "ACPI0010" /* Processor Container Device */)  // _HID: Hardware ID
            Name (_UID, 0x00100000)  // _UID: Unique ID
            Name (_LPI, Package (0x04)  // _LPI: Low Power Idle States
            {
                Zero, 
                0x01000000, 
                One, 
                Package (0x0A)
                {
                    0x2662, 
                    0x19A2, 
                    One, 
                    0x20, 
                    Zero, 
                    Zero, 
                    0xB300, 
                    ResourceTemplate ()
                    {
                        Register (SystemMemory, 
                            0x00,               // Bit Width
                            0x00,               // Bit Offset
                            0x0000000000000000, // Address
                            ,)
                    }, 

                    ResourceTemplate ()
                    {
                        Register (SystemMemory, 
                            0x00,               // Bit Width
                            0x00,               // Bit Offset
                            0x0000000000000000, // Address
                            ,)
                    }, 

                    "platform.DRIPS"
                }
            })
            Device (CLUS)
            {
                Name (_HID, "ACPI0010" /* Processor Container Device */)  // _HID: Hardware ID
                Name (_UID, 0x10)  // _UID: Unique ID
                Name (_LPI, Package (0x05)  // _LPI: Low Power Idle States
                {
                    Zero, 
                    0x01000000, 
                    0x02, 
                    Package (0x0A)
                    {
                        0x170C, 
                        0x0BB8, 
                        Zero, 
                        Zero, 
                        Zero, 
                        Zero, 
                        0x20, 
                        ResourceTemplate ()
                        {
                            Register (SystemMemory, 
                                0x00,               // Bit Width
                                0x00,               // Bit Offset
                                0x0000000000000000, // Address
                                ,)
                        }, 

                        ResourceTemplate ()
                        {
                            Register (SystemMemory, 
                                0x00,               // Bit Width
                                0x00,               // Bit Offset
                                0x0000000000000000, // Address
                                ,)
                        }, 

                        "L3Cluster.D2"
                    }, 

                    Package (0x0A)
                    {
                        0x17E6, 
                        0x0BE8, 
                        One, 
                        Zero, 
                        Zero, 
                        One, 
                        0x40, 
                        ResourceTemplate ()
                        {
                            Register (SystemMemory, 
                                0x00,               // Bit Width
                                0x00,               // Bit Offset
                                0x0000000000000000, // Address
                                ,)
                        }, 

                        ResourceTemplate ()
                        {
                            Register (SystemMemory, 
                                0x00,               // Bit Width
                                0x00,               // Bit Offset
                                0x0000000000000000, // Address
                                ,)
                        }, 

                        "L3Cluster.D4"
                    }
                })
                Device (CPU0)
                {
                    Name (_HID, "ACPI0007" /* Processor Device */)  // _HID: Hardware ID
                    Name (_UID, Zero)  // _UID: Unique ID
                    Method (_STA, 0, NotSerialized)  // _STA: Status
                    {
                        Return (0x0F)
                    }

                    Name (_LPI, Package (0x07)  // _LPI: Low Power Idle States
                    {
                        Zero, 
                        Zero, 
                        0x04, 
                        Package (0x0A)
                        {
                            Zero, 
                            Zero, 
                            One, 
                            Zero, 
                            Zero, 
                            Zero, 
                            ResourceTemplate ()
                            {
                                Register (FFixedHW, 
                                    0x20,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x00000000FFFFFFFF, // Address
                                    0x03,               // Access Size
                                    )
                            }, 

                            ResourceTemplate ()
                            {
                                Register (SystemMemory, 
                                    0x00,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000000000000, // Address
                                    ,)
                            }, 

                            ResourceTemplate ()
                            {
                                Register (SystemMemory, 
                                    0x00,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000000000000, // Address
                                    ,)
                            }, 

                            "KryoSilver0.C1"
                        }, 

                        Package (0x0A)
                        {
                            0x0190, 
                            0x64, 
                            Zero, 
                            Zero, 
                            Zero, 
                            One, 
                            ResourceTemplate ()
                            {
                                Register (FFixedHW, 
                                    0x20,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000000000002, // Address
                                    0x03,               // Access Size
                                    )
                            }, 

                            ResourceTemplate ()
                            {
                                Register (SystemMemory, 
                                    0x00,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000000000000, // Address
                                    ,)
                            }, 

                            ResourceTemplate ()
                            {
                                Register (SystemMemory, 
                                    0x00,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000000000000, // Address
                                    ,)
                            }, 

                            "KryoSilver0.C2"
                        }, 

                        Package (0x0A)
                        {
                            0x06EE, 
                            0x0385, 
                            One, 
                            One, 
                            Zero, 
                            One, 
                            ResourceTemplate ()
                            {
                                Register (FFixedHW, 
                                    0x20,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000040000003, // Address
                                    0x03,               // Access Size
                                    )
                            }, 

                            ResourceTemplate ()
                            {
                                Register (SystemMemory, 
                                    0x00,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000000000000, // Address
                                    ,)
                            }, 

                            ResourceTemplate ()
                            {
                                Register (SystemMemory, 
                                    0x00,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000000000000, // Address
                                    ,)
                            }, 

                            "KryoSilver0.C3"
                        }, 

                        Package (0x0A)
                        {
                            0x0FA1, 
                            0x0393, 
                            One, 
                            One, 
                            Zero, 
                            0x02, 
                            ResourceTemplate ()
                            {
                                Register (FFixedHW, 
                                    0x20,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000040000004, // Address
                                    0x03,               // Access Size
                                    )
                            }, 

                            ResourceTemplate ()
                            {
                                Register (SystemMemory, 
                                    0x00,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000000000000, // Address
                                    ,)
                            }, 

                            ResourceTemplate ()
                            {
                                Register (SystemMemory, 
                                    0x00,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000000000000, // Address
                                    ,)
                            }, 

                            "KryoSilver0.C4"
                        }
                    })
                }

                Device (CPU1)
                {
                    Name (_HID, "ACPI0007" /* Processor Device */)  // _HID: Hardware ID
                    Name (_UID, One)  // _UID: Unique ID
                    Method (_STA, 0, NotSerialized)  // _STA: Status
                    {
                        Return (0x0F)
                    }

                    Name (_LPI, Package (0x07)  // _LPI: Low Power Idle States
                    {
                        Zero, 
                        Zero, 
                        0x04, 
                        Package (0x0A)
                        {
                            Zero, 
                            Zero, 
                            One, 
                            Zero, 
                            Zero, 
                            Zero, 
                            ResourceTemplate ()
                            {
                                Register (FFixedHW, 
                                    0x20,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x00000000FFFFFFFF, // Address
                                    0x03,               // Access Size
                                    )
                            }, 

                            ResourceTemplate ()
                            {
                                Register (SystemMemory, 
                                    0x00,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000000000000, // Address
                                    ,)
                            }, 

                            ResourceTemplate ()
                            {
                                Register (SystemMemory, 
                                    0x00,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000000000000, // Address
                                    ,)
                            }, 

                            "KryoSilver1.C1"
                        }, 

                        Package (0x0A)
                        {
                            0x0190, 
                            0x64, 
                            Zero, 
                            Zero, 
                            Zero, 
                            One, 
                            ResourceTemplate ()
                            {
                                Register (FFixedHW, 
                                    0x20,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000000000002, // Address
                                    0x03,               // Access Size
                                    )
                            }, 

                            ResourceTemplate ()
                            {
                                Register (SystemMemory, 
                                    0x00,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000000000000, // Address
                                    ,)
                            }, 

                            ResourceTemplate ()
                            {
                                Register (SystemMemory, 
                                    0x00,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000000000000, // Address
                                    ,)
                            }, 

                            "KryoSilver1.C2"
                        }, 

                        Package (0x0A)
                        {
                            0x06EE, 
                            0x0385, 
                            One, 
                            One, 
                            Zero, 
                            One, 
                            ResourceTemplate ()
                            {
                                Register (FFixedHW, 
                                    0x20,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000040000003, // Address
                                    0x03,               // Access Size
                                    )
                            }, 

                            ResourceTemplate ()
                            {
                                Register (SystemMemory, 
                                    0x00,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000000000000, // Address
                                    ,)
                            }, 

                            ResourceTemplate ()
                            {
                                Register (SystemMemory, 
                                    0x00,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000000000000, // Address
                                    ,)
                            }, 

                            "KryoSilver1.C3"
                        }, 

                        Package (0x0A)
                        {
                            0x0FA1, 
                            0x0393, 
                            One, 
                            One, 
                            Zero, 
                            0x02, 
                            ResourceTemplate ()
                            {
                                Register (FFixedHW, 
                                    0x20,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000040000004, // Address
                                    0x03,               // Access Size
                                    )
                            }, 

                            ResourceTemplate ()
                            {
                                Register (SystemMemory, 
                                    0x00,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000000000000, // Address
                                    ,)
                            }, 

                            ResourceTemplate ()
                            {
                                Register (SystemMemory, 
                                    0x00,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000000000000, // Address
                                    ,)
                            }, 

                            "KryoSilver1.C4"
                        }
                    })
                }

                Device (CPU2)
                {
                    Name (_HID, "ACPI0007" /* Processor Device */)  // _HID: Hardware ID
                    Name (_UID, 0x02)  // _UID: Unique ID
                    Method (_STA, 0, NotSerialized)  // _STA: Status
                    {
                        Return (0x0F)
                    }

                    Name (_LPI, Package (0x07)  // _LPI: Low Power Idle States
                    {
                        Zero, 
                        Zero, 
                        0x04, 
                        Package (0x0A)
                        {
                            Zero, 
                            Zero, 
                            One, 
                            Zero, 
                            Zero, 
                            Zero, 
                            ResourceTemplate ()
                            {
                                Register (FFixedHW, 
                                    0x20,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x00000000FFFFFFFF, // Address
                                    0x03,               // Access Size
                                    )
                            }, 

                            ResourceTemplate ()
                            {
                                Register (SystemMemory, 
                                    0x00,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000000000000, // Address
                                    ,)
                            }, 

                            ResourceTemplate ()
                            {
                                Register (SystemMemory, 
                                    0x00,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000000000000, // Address
                                    ,)
                            }, 

                            "KryoSilver2.C1"
                        }, 

                        Package (0x0A)
                        {
                            0x0190, 
                            0x64, 
                            Zero, 
                            Zero, 
                            Zero, 
                            One, 
                            ResourceTemplate ()
                            {
                                Register (FFixedHW, 
                                    0x20,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000000000002, // Address
                                    0x03,               // Access Size
                                    )
                            }, 

                            ResourceTemplate ()
                            {
                                Register (SystemMemory, 
                                    0x00,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000000000000, // Address
                                    ,)
                            }, 

                            ResourceTemplate ()
                            {
                                Register (SystemMemory, 
                                    0x00,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000000000000, // Address
                                    ,)
                            }, 

                            "KryoSilver2.C2"
                        }, 

                        Package (0x0A)
                        {
                            0x06EE, 
                            0x0385, 
                            One, 
                            One, 
                            Zero, 
                            One, 
                            ResourceTemplate ()
                            {
                                Register (FFixedHW, 
                                    0x20,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000040000003, // Address
                                    0x03,               // Access Size
                                    )
                            }, 

                            ResourceTemplate ()
                            {
                                Register (SystemMemory, 
                                    0x00,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000000000000, // Address
                                    ,)
                            }, 

                            ResourceTemplate ()
                            {
                                Register (SystemMemory, 
                                    0x00,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000000000000, // Address
                                    ,)
                            }, 

                            "KryoSilver2.C3"
                        }, 

                        Package (0x0A)
                        {
                            0x0FA1, 
                            0x0393, 
                            One, 
                            One, 
                            Zero, 
                            0x02, 
                            ResourceTemplate ()
                            {
                                Register (FFixedHW, 
                                    0x20,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000040000004, // Address
                                    0x03,               // Access Size
                                    )
                            }, 

                            ResourceTemplate ()
                            {
                                Register (SystemMemory, 
                                    0x00,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000000000000, // Address
                                    ,)
                            }, 

                            ResourceTemplate ()
                            {
                                Register (SystemMemory, 
                                    0x00,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000000000000, // Address
                                    ,)
                            }, 

                            "KryoSilver2.C4"
                        }
                    })
                }

                Device (CPU3)
                {
                    Name (_HID, "ACPI0007" /* Processor Device */)  // _HID: Hardware ID
                    Name (_UID, 0x03)  // _UID: Unique ID
                    Method (_STA, 0, NotSerialized)  // _STA: Status
                    {
                        Return (0x0F)
                    }

                    Name (_LPI, Package (0x07)  // _LPI: Low Power Idle States
                    {
                        Zero, 
                        Zero, 
                        0x04, 
                        Package (0x0A)
                        {
                            Zero, 
                            Zero, 
                            One, 
                            Zero, 
                            Zero, 
                            Zero, 
                            ResourceTemplate ()
                            {
                                Register (FFixedHW, 
                                    0x20,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x00000000FFFFFFFF, // Address
                                    0x03,               // Access Size
                                    )
                            }, 

                            ResourceTemplate ()
                            {
                                Register (SystemMemory, 
                                    0x00,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000000000000, // Address
                                    ,)
                            }, 

                            ResourceTemplate ()
                            {
                                Register (SystemMemory, 
                                    0x00,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000000000000, // Address
                                    ,)
                            }, 

                            "KryoSilver3.C1"
                        }, 

                        Package (0x0A)
                        {
                            0x0190, 
                            0x64, 
                            Zero, 
                            Zero, 
                            Zero, 
                            One, 
                            ResourceTemplate ()
                            {
                                Register (FFixedHW, 
                                    0x20,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000000000002, // Address
                                    0x03,               // Access Size
                                    )
                            }, 

                            ResourceTemplate ()
                            {
                                Register (SystemMemory, 
                                    0x00,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000000000000, // Address
                                    ,)
                            }, 

                            ResourceTemplate ()
                            {
                                Register (SystemMemory, 
                                    0x00,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000000000000, // Address
                                    ,)
                            }, 

                            "KryoSilver3.C2"
                        }, 

                        Package (0x0A)
                        {
                            0x06EE, 
                            0x0385, 
                            One, 
                            One, 
                            Zero, 
                            One, 
                            ResourceTemplate ()
                            {
                                Register (FFixedHW, 
                                    0x20,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000040000003, // Address
                                    0x03,               // Access Size
                                    )
                            }, 

                            ResourceTemplate ()
                            {
                                Register (SystemMemory, 
                                    0x00,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000000000000, // Address
                                    ,)
                            }, 

                            ResourceTemplate ()
                            {
                                Register (SystemMemory, 
                                    0x00,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000000000000, // Address
                                    ,)
                            }, 

                            "KryoSilver3.C3"
                        }, 

                        Package (0x0A)
                        {
                            0x0FA1, 
                            0x0393, 
                            One, 
                            One, 
                            Zero, 
                            0x02, 
                            ResourceTemplate ()
                            {
                                Register (FFixedHW, 
                                    0x20,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000040000004, // Address
                                    0x03,               // Access Size
                                    )
                            }, 

                            ResourceTemplate ()
                            {
                                Register (SystemMemory, 
                                    0x00,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000000000000, // Address
                                    ,)
                            }, 

                            ResourceTemplate ()
                            {
                                Register (SystemMemory, 
                                    0x00,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000000000000, // Address
                                    ,)
                            }, 

                            "KryoSilver3.C4"
                        }
                    })
                }

                Device (CPU4)
                {
                    Name (_HID, "ACPI0007" /* Processor Device */)  // _HID: Hardware ID
                    Name (_UID, 0x04)  // _UID: Unique ID
                    Method (_STA, 0, NotSerialized)  // _STA: Status
                    {
                        Return (0x0F)
                    }

                    Name (_LPI, Package (0x07)  // _LPI: Low Power Idle States
                    {
                        Zero, 
                        Zero, 
                        0x04, 
                        Package (0x0A)
                        {
                            Zero, 
                            Zero, 
                            One, 
                            Zero, 
                            Zero, 
                            Zero, 
                            ResourceTemplate ()
                            {
                                Register (FFixedHW, 
                                    0x20,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x00000000FFFFFFFF, // Address
                                    0x03,               // Access Size
                                    )
                            }, 

                            ResourceTemplate ()
                            {
                                Register (SystemMemory, 
                                    0x00,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000000000000, // Address
                                    ,)
                            }, 

                            ResourceTemplate ()
                            {
                                Register (SystemMemory, 
                                    0x00,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000000000000, // Address
                                    ,)
                            }, 

                            "KryoSilver4.C1"
                        }, 

                        Package (0x0A)
                        {
                            0x0190, 
                            0x64, 
                            Zero, 
                            Zero, 
                            Zero, 
                            One, 
                            ResourceTemplate ()
                            {
                                Register (FFixedHW, 
                                    0x20,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000000000002, // Address
                                    0x03,               // Access Size
                                    )
                            }, 

                            ResourceTemplate ()
                            {
                                Register (SystemMemory, 
                                    0x00,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000000000000, // Address
                                    ,)
                            }, 

                            ResourceTemplate ()
                            {
                                Register (SystemMemory, 
                                    0x00,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000000000000, // Address
                                    ,)
                            }, 

                            "KryoSilver4.C2"
                        }, 

                        Package (0x0A)
                        {
                            0x06EE, 
                            0x0385, 
                            One, 
                            One, 
                            Zero, 
                            One, 
                            ResourceTemplate ()
                            {
                                Register (FFixedHW, 
                                    0x20,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000040000003, // Address
                                    0x03,               // Access Size
                                    )
                            }, 

                            ResourceTemplate ()
                            {
                                Register (SystemMemory, 
                                    0x00,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000000000000, // Address
                                    ,)
                            }, 

                            ResourceTemplate ()
                            {
                                Register (SystemMemory, 
                                    0x00,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000000000000, // Address
                                    ,)
                            }, 

                            "KryoSilver4.C3"
                        }, 

                        Package (0x0A)
                        {
                            0x0FA1, 
                            0x0393, 
                            One, 
                            One, 
                            Zero, 
                            0x02, 
                            ResourceTemplate ()
                            {
                                Register (FFixedHW, 
                                    0x20,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000040000004, // Address
                                    0x03,               // Access Size
                                    )
                            }, 

                            ResourceTemplate ()
                            {
                                Register (SystemMemory, 
                                    0x00,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000000000000, // Address
                                    ,)
                            }, 

                            ResourceTemplate ()
                            {
                                Register (SystemMemory, 
                                    0x00,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000000000000, // Address
                                    ,)
                            }, 

                            "KryoSilver4.C4"
                        }
                    })
                }

                Device (CPU5)
                {
                    Name (_HID, "ACPI0007" /* Processor Device */)  // _HID: Hardware ID
                    Name (_UID, 0x05)  // _UID: Unique ID
                    Method (_STA, 0, NotSerialized)  // _STA: Status
                    {
                        Return (0x0F)
                    }

                    Name (_LPI, Package (0x07)  // _LPI: Low Power Idle States
                    {
                        Zero, 
                        Zero, 
                        0x04, 
                        Package (0x0A)
                        {
                            Zero, 
                            Zero, 
                            One, 
                            Zero, 
                            Zero, 
                            Zero, 
                            ResourceTemplate ()
                            {
                                Register (FFixedHW, 
                                    0x20,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x00000000FFFFFFFF, // Address
                                    0x03,               // Access Size
                                    )
                            }, 

                            ResourceTemplate ()
                            {
                                Register (SystemMemory, 
                                    0x00,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000000000000, // Address
                                    ,)
                            }, 

                            ResourceTemplate ()
                            {
                                Register (SystemMemory, 
                                    0x00,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000000000000, // Address
                                    ,)
                            }, 

                            "KryoSilver5.C1"
                        }, 

                        Package (0x0A)
                        {
                            0x0190, 
                            0x64, 
                            Zero, 
                            Zero, 
                            Zero, 
                            One, 
                            ResourceTemplate ()
                            {
                                Register (FFixedHW, 
                                    0x20,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000000000002, // Address
                                    0x03,               // Access Size
                                    )
                            }, 

                            ResourceTemplate ()
                            {
                                Register (SystemMemory, 
                                    0x00,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000000000000, // Address
                                    ,)
                            }, 

                            ResourceTemplate ()
                            {
                                Register (SystemMemory, 
                                    0x00,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000000000000, // Address
                                    ,)
                            }, 

                            "KryoSilver5.C2"
                        }, 

                        Package (0x0A)
                        {
                            0x06EE, 
                            0x0385, 
                            One, 
                            One, 
                            Zero, 
                            One, 
                            ResourceTemplate ()
                            {
                                Register (FFixedHW, 
                                    0x20,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000040000003, // Address
                                    0x03,               // Access Size
                                    )
                            }, 

                            ResourceTemplate ()
                            {
                                Register (SystemMemory, 
                                    0x00,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000000000000, // Address
                                    ,)
                            }, 

                            ResourceTemplate ()
                            {
                                Register (SystemMemory, 
                                    0x00,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000000000000, // Address
                                    ,)
                            }, 

                            "KryoSilver5.C3"
                        }, 

                        Package (0x0A)
                        {
                            0x0FA1, 
                            0x0393, 
                            One, 
                            One, 
                            Zero, 
                            0x02, 
                            ResourceTemplate ()
                            {
                                Register (FFixedHW, 
                                    0x20,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000040000004, // Address
                                    0x03,               // Access Size
                                    )
                            }, 

                            ResourceTemplate ()
                            {
                                Register (SystemMemory, 
                                    0x00,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000000000000, // Address
                                    ,)
                            }, 

                            ResourceTemplate ()
                            {
                                Register (SystemMemory, 
                                    0x00,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000000000000, // Address
                                    ,)
                            }, 

                            "KryoSilver5.C4"
                        }
                    })
                }

                Device (CPU6)
                {
                    Name (_HID, "ACPI0007" /* Processor Device */)  // _HID: Hardware ID
                    Name (_UID, 0x06)  // _UID: Unique ID
                    Method (_STA, 0, NotSerialized)  // _STA: Status
                    {
                        Return (0x0F)
                    }

                    Name (_LPI, Package (0x07)  // _LPI: Low Power Idle States
                    {
                        Zero, 
                        Zero, 
                        0x04, 
                        Package (0x0A)
                        {
                            Zero, 
                            Zero, 
                            One, 
                            Zero, 
                            Zero, 
                            Zero, 
                            ResourceTemplate ()
                            {
                                Register (FFixedHW, 
                                    0x20,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x00000000FFFFFFFF, // Address
                                    0x03,               // Access Size
                                    )
                            }, 

                            ResourceTemplate ()
                            {
                                Register (SystemMemory, 
                                    0x00,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000000000000, // Address
                                    ,)
                            }, 

                            ResourceTemplate ()
                            {
                                Register (SystemMemory, 
                                    0x00,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000000000000, // Address
                                    ,)
                            }, 

                            "KryoGold0.C1"
                        }, 

                        Package (0x0A)
                        {
                            0x0190, 
                            0x64, 
                            Zero, 
                            Zero, 
                            Zero, 
                            One, 
                            ResourceTemplate ()
                            {
                                Register (FFixedHW, 
                                    0x20,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000000000002, // Address
                                    0x03,               // Access Size
                                    )
                            }, 

                            ResourceTemplate ()
                            {
                                Register (SystemMemory, 
                                    0x00,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000000000000, // Address
                                    ,)
                            }, 

                            ResourceTemplate ()
                            {
                                Register (SystemMemory, 
                                    0x00,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000000000000, // Address
                                    ,)
                            }, 

                            "KryoGold0.C2"
                        }, 

                        Package (0x0A)
                        {
                            0x089F, 
                            0x04DC, 
                            One, 
                            One, 
                            Zero, 
                            One, 
                            ResourceTemplate ()
                            {
                                Register (FFixedHW, 
                                    0x20,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000040000003, // Address
                                    0x03,               // Access Size
                                    )
                            }, 

                            ResourceTemplate ()
                            {
                                Register (SystemMemory, 
                                    0x00,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000000000000, // Address
                                    ,)
                            }, 

                            ResourceTemplate ()
                            {
                                Register (SystemMemory, 
                                    0x00,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000000000000, // Address
                                    ,)
                            }, 

                            "KryoGold0.C3"
                        }, 

                        Package (0x0A)
                        {
                            0x15B3, 
                            0x073E, 
                            One, 
                            One, 
                            Zero, 
                            0x02, 
                            ResourceTemplate ()
                            {
                                Register (FFixedHW, 
                                    0x20,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000040000004, // Address
                                    0x03,               // Access Size
                                    )
                            }, 

                            ResourceTemplate ()
                            {
                                Register (SystemMemory, 
                                    0x00,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000000000000, // Address
                                    ,)
                            }, 

                            ResourceTemplate ()
                            {
                                Register (SystemMemory, 
                                    0x00,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000000000000, // Address
                                    ,)
                            }, 

                            "KryoGold0.C4"
                        }
                    })
                }

                Device (CPU7)
                {
                    Name (_HID, "ACPI0007" /* Processor Device */)  // _HID: Hardware ID
                    Name (_UID, 0x07)  // _UID: Unique ID
                    Method (_STA, 0, NotSerialized)  // _STA: Status
                    {
                        Return (0x0F)
                    }

                    Name (_LPI, Package (0x07)  // _LPI: Low Power Idle States
                    {
                        Zero, 
                        Zero, 
                        0x04, 
                        Package (0x0A)
                        {
                            Zero, 
                            Zero, 
                            One, 
                            Zero, 
                            Zero, 
                            Zero, 
                            ResourceTemplate ()
                            {
                                Register (FFixedHW, 
                                    0x20,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x00000000FFFFFFFF, // Address
                                    0x03,               // Access Size
                                    )
                            }, 

                            ResourceTemplate ()
                            {
                                Register (SystemMemory, 
                                    0x00,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000000000000, // Address
                                    ,)
                            }, 

                            ResourceTemplate ()
                            {
                                Register (SystemMemory, 
                                    0x00,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000000000000, // Address
                                    ,)
                            }, 

                            "KryoGold1.C1"
                        }, 

                        Package (0x0A)
                        {
                            0x0190, 
                            0x64, 
                            Zero, 
                            Zero, 
                            Zero, 
                            One, 
                            ResourceTemplate ()
                            {
                                Register (FFixedHW, 
                                    0x20,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000000000002, // Address
                                    0x03,               // Access Size
                                    )
                            }, 

                            ResourceTemplate ()
                            {
                                Register (SystemMemory, 
                                    0x00,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000000000000, // Address
                                    ,)
                            }, 

                            ResourceTemplate ()
                            {
                                Register (SystemMemory, 
                                    0x00,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000000000000, // Address
                                    ,)
                            }, 

                            "KryoGold1.C2"
                        }, 

                        Package (0x0A)
                        {
                            0x089F, 
                            0x04DC, 
                            One, 
                            One, 
                            Zero, 
                            One, 
                            ResourceTemplate ()
                            {
                                Register (FFixedHW, 
                                    0x20,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000040000003, // Address
                                    0x03,               // Access Size
                                    )
                            }, 

                            ResourceTemplate ()
                            {
                                Register (SystemMemory, 
                                    0x00,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000000000000, // Address
                                    ,)
                            }, 

                            ResourceTemplate ()
                            {
                                Register (SystemMemory, 
                                    0x00,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000000000000, // Address
                                    ,)
                            }, 

                            "KryoGold1.C3"
                        }, 

                        Package (0x0A)
                        {
                            0x15B3, 
                            0x073E, 
                            One, 
                            One, 
                            Zero, 
                            0x02, 
                            ResourceTemplate ()
                            {
                                Register (FFixedHW, 
                                    0x20,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000040000004, // Address
                                    0x03,               // Access Size
                                    )
                            }, 

                            ResourceTemplate ()
                            {
                                Register (SystemMemory, 
                                    0x00,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000000000000, // Address
                                    ,)
                            }, 

                            ResourceTemplate ()
                            {
                                Register (SystemMemory, 
                                    0x00,               // Bit Width
                                    0x00,               // Bit Offset
                                    0x0000000000000000, // Address
                                    ,)
                            }, 

                            "KryoGold1.C4"
                        }
                    })
                }
            }
        }
    }
}

