DefinitionBlock ("", "DSDT", 2, "QCOMM ", "SM6225 ", 0x00000003)
{
    Scope (_SB)
    {
        Name (PSUB, "MTP06225")
        Name (SOID, 0xFFFFFFFF)
        Name (STOR, 0xABCABCAB)
        Name (SIDS, "899800000000000")
        Name (SIDV, 0xFFFFFFFF)
        Name (SVMJ, 0xFFFF)
        Name (SVMI, 0xFFFF)
        Name (SDFE, 0xFFFF)
        Name (SFES, "899800000000000")
        Name (SIDM, 0x0000000FFFFFFFFF)
        Name (SUFS, 0xFFFFFFFF)
        Name (PUS3, 0xFFFFFFFF)
        Name (SUS3, 0xFFFFFFFF)
        Name (SIDT, 0xFFFFFFFF)
        Name (SOSN, 0xAAAAAAAABBBBBBBB)
        Name (PLST, 0xFFFFFFFF)
        Name (EMUL, 0xFFFFFFFF)
        Name (SJTG, 0xFFFFFFFF)
        Name (RMTB, 0xAAAAAAAA)
        Name (RMTX, 0xBBBBBBBB)
        Name (RFMB, 0xCCCCCCCC)
        Name (RFMS, 0xDDDDDDDD)
        Name (RFAB, 0xEEEEEEEE)
        Name (RFAS, 0x77777777)
        Name (TCMA, 0xDEADBEEF)
        Name (TCML, 0xBEEFDEAD)
        Name (SOSI, 0xDEADBEEFFFFFFFFF)
        Name (PRP0, 0xFFFFFFFF)
        Name (PRP1, 0xFFFFFFFF)
        Name (PRP2, 0xFFFFFFFF)
        Name (PRP3, 0xFFFFFFFF)
        Name (PRP4, 0xFFFFFFFF)
        Name (PRP5, 0xFFFFFFFF)
        Name (PRP6, 0xFFFFFFFF)
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

        Device (SYSM)
        {
            Name (_HID, "ACPI0010" /* Processor Container Device */)  // _HID: Hardware ID
            Name (_UID, 0x00100000)  // _UID: Unique ID
            Device (APC0)
            {
                Name (_HID, "ACPI0010" /* Processor Container Device */)  // _HID: Hardware ID
                Name (_UID, 0x0100)  // _UID: Unique ID
                Name (_LPI, Package (0x06)  // _LPI: Low Power Idle States
                {
                    Zero,
                    0x02000000,
                    0x03,
                    Package (0x0A)
                    {
                        0x0BB8,
                        0x044C,
                        One,
                        Zero,
                        Zero,
                        Zero,
                        0x0400,
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

                        "CCI.E3"
                    },

                    Package (0x0A)
                    {
                        0x0DAC,
                        0x047E,
                        One,
                        Zero,
                        Zero,
                        Zero,
                        0x0500,
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

                        "CCI.E3+RPM"
                    },

                    Package (0x0A)
                    {
                        0x80E8,
                        0x01F4,
                        One,
                        0x20,
                        Zero,
                        Zero,
                        0x0300,
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

                        "platform.xo"
                    }
                })
                Device (CL0)
                {
                    Name (_HID, "ACPI0010" /* Processor Container Device */)  // _HID: Hardware ID
                    Name (_UID, 0x10)  // _UID: Unique ID
                    Name (_LPI, Package (0x06)  // _LPI: Low Power Idle States
                    {
                        Zero,
                        0x01000000,
                        0x03,
                        Package (0x0A)
                        {
                            0x012C,
                            0x82,
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

                            "L2_Silver.D2d"
                        },

                        Package (0x0A)
                        {
                            0x015E,
                            0x96,
                            Zero,
                            Zero,
                            Zero,
                            Zero,
                            0x30,
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

                            "L2_Silver.D2e"
                        },

                        Package (0x0A)
                        {
                            0x1900,
                            0x0384,
                            One,
                            Zero,
                            Zero,
                            0x03,
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

                            "L2_Silver.D4"
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
                                Zero,
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

                                "KryoSilver0.C2d"
                            },

                            Package (0x0A)
                            {
                                0x01C2,
                                0x012C,
                                One,
                                One,
                                Zero,
                                0x03,
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
                                0x01C2,
                                0x012C,
                                Zero,
                                One,
                                Zero,
                                0x03,
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

                                "KryoSilver0.C3_NI"
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
                                        0x04,               // Access Size
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
                                Zero,
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

                                "KryoSilver1.C2d"
                            },

                            Package (0x0A)
                            {
                                0x01C2,
                                0x012C,
                                One,
                                One,
                                Zero,
                                0x03,
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
                                0x01C2,
                                0x012C,
                                Zero,
                                One,
                                Zero,
                                0x03,
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

                                "KryoSilver1.C3_NI"
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
                                        0x04,               // Access Size
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
                                Zero,
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

                                "KryoSilver2.C2d"
                            },

                            Package (0x0A)
                            {
                                0x01C2,
                                0x012C,
                                One,
                                One,
                                Zero,
                                0x03,
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
                                0x01C2,
                                0x012C,
                                Zero,
                                One,
                                Zero,
                                0x03,
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

                                "KryoSilver2.C3_NI"
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
                                        0x04,               // Access Size
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
                                Zero,
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

                                "KryoSilver3.C2d"
                            },

                            Package (0x0A)
                            {
                                0x01C2,
                                0x012C,
                                One,
                                One,
                                Zero,
                                0x03,
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
                                0x01C2,
                                0x012C,
                                Zero,
                                One,
                                Zero,
                                0x03,
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

                                "KryoSilver3.C3_NI"
                            }
                        })
                    }
                }

                Device (CL1)
                {
                    Name (_HID, "ACPI0010" /* Processor Container Device */)  // _HID: Hardware ID
                    Name (_UID, 0x20)  // _UID: Unique ID
                    Name (_LPI, Package (0x06)  // _LPI: Low Power Idle States
                    {
                        Zero,
                        0x01000000,
                        0x03,
                        Package (0x0A)
                        {
                            0x0384,
                            0xC8,
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

                            "L2_Gold.D2d"
                        },

                        Package (0x0A)
                        {
                            0x03E8,
                            0x0190,
                            Zero,
                            Zero,
                            Zero,
                            Zero,
                            0x30,
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

                            "L2_Gold.D2e"
                        },

                        Package (0x0A)
                        {
                            0x1770,
                            0x04B0,
                            One,
                            Zero,
                            Zero,
                            0x03,
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

                            "L2_Gold.D4"
                        }
                    })
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
                                        0x04,               // Access Size
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
                                0x0258,
                                0x50,
                                Zero,
                                Zero,
                                Zero,
                                Zero,
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

                                "KryoGold0.C2d"
                            },

                            Package (0x0A)
                            {
                                0x1B58,
                                0x5A,
                                One,
                                One,
                                Zero,
                                0x03,
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
                                0x1B58,
                                0x5A,
                                Zero,
                                One,
                                Zero,
                                0x03,
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

                                "KryoGold0.C3_NI"
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
                                        0x04,               // Access Size
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
                                0x0258,
                                0x50,
                                Zero,
                                Zero,
                                Zero,
                                Zero,
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

                                "KryoGold1.C2d"
                            },

                            Package (0x0A)
                            {
                                0x1B58,
                                0x5A,
                                One,
                                One,
                                Zero,
                                0x03,
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
                                0x1B58,
                                0x5A,
                                Zero,
                                One,
                                Zero,
                                0x03,
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

                                "KryoGold1.C3_NI"
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
                                        0x04,               // Access Size
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

                                "KryoGold2.C1"
                            },

                            Package (0x0A)
                            {
                                0x0258,
                                0x50,
                                Zero,
                                Zero,
                                Zero,
                                Zero,
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

                                "KryoGold2.C2d"
                            },

                            Package (0x0A)
                            {
                                0x1B58,
                                0x5A,
                                One,
                                One,
                                Zero,
                                0x03,
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

                                "KryoGold2.C3"
                            },

                            Package (0x0A)
                            {
                                0x1B58,
                                0x5A,
                                Zero,
                                One,
                                Zero,
                                0x03,
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

                                "KryoGold2.C3_NI"
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
                                        0x04,               // Access Size
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

                                "KryoGold3.C1"
                            },

                            Package (0x0A)
                            {
                                0x0258,
                                0x50,
                                Zero,
                                Zero,
                                Zero,
                                Zero,
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

                                "KryoGold3.C2d"
                            },

                            Package (0x0A)
                            {
                                0x1B58,
                                0x5A,
                                One,
                                One,
                                Zero,
                                0x03,
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

                                "KryoGold3.C3"
                            },

                            Package (0x0A)
                            {
                                0x1B58,
                                0x5A,
                                Zero,
                                One,
                                Zero,
                                0x03,
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

                                "KryoGold3.C3_NI"
                            }
                        })
                    }
                }
            }
        }

        Include("usb.asl")

    }
}

