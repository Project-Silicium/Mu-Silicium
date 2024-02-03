        Device (SYSM)
        {
            Name (_HID, "ACPI0010" /* Processor Container Device */)  // _HID: Hardware ID
            Name (_UID, 0x00100000)  // _UID: Unique ID
            Device (CLU0)
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
                        0x030E,
                        0x01A5,
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

                        "GDHS.D2"
                    },

                    Package (0x0A)
                    {
                        0x1CD0,
                        0x0846,
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

                        "PowerCollapse.D4"
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

                    Name (_LPI, Package (0x05)  // _LPI: Low Power Idle States
                    {
                        Zero,
                        Zero,
                        0x02,
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
                            0x049E,
                            0x0178,
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

                    Name (_LPI, Package (0x05)  // _LPI: Low Power Idle States
                    {
                        Zero,
                        Zero,
                        0x02,
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
                            0x049E,
                            0x0178,
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

                    Name (_LPI, Package (0x05)  // _LPI: Low Power Idle States
                    {
                        Zero,
                        Zero,
                        0x02,
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
                            0x049E,
                            0x0178,
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

                    Name (_LPI, Package (0x05)  // _LPI: Low Power Idle States
                    {
                        Zero,
                        Zero,
                        0x02,
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
                            0x049E,
                            0x0178,
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
                        }
                    })
                }
            }

            Device (CLU1)
            {
                Name (_HID, "ACPI0010" /* Processor Container Device */)  // _HID: Hardware ID
                Name (_UID, 0x20)  // _UID: Unique ID
                Name (_LPI, Package (0x05)  // _LPI: Low Power Idle States
                {
                    Zero,
                    0x01000000,
                    0x02,
                    Package (0x0A)
                    {
                        0x0294,
                        0x0159,
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

                        "GDHS.D2"
                    },

                    Package (0x0A)
                    {
                        0x1F9E,
                        0x0676,
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

                        "PowerCollapse.D4"
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

                    Name (_LPI, Package (0x05)  // _LPI: Low Power Idle States
                    {
                        Zero,
                        Zero,
                        0x02,
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
                            0x0456,
                            0x0144,
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

                    Name (_LPI, Package (0x05)  // _LPI: Low Power Idle States
                    {
                        Zero,
                        Zero,
                        0x02,
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
                            0x0456,
                            0x0144,
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

                    Name (_LPI, Package (0x05)  // _LPI: Low Power Idle States
                    {
                        Zero,
                        Zero,
                        0x02,
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

                            "KryoGold2.C1"
                        },

                        Package (0x0A)
                        {
                            0x0456,
                            0x0144,
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

                            "KryoGold2.C2"
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

                    Name (_LPI, Package (0x05)  // _LPI: Low Power Idle States
                    {
                        Zero,
                        Zero,
                        0x02,
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

                            "KryoGold3.C1"
                        },

                        Package (0x0A)
                        {
                            0x0456,
                            0x0144,
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

                            "KryoGold3.C2"
                        }
                    })
                }
            }
        }
