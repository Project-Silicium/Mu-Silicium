Device (SYSM)
{
    Name (_HID, "ACPI0010")
    Name (_UID, 0x100000)
    Name (_LPI, Package ()
    {
        0, 
        0x1000000, 
        1, 
        Package () 
        {
            0x251C, 
            0x1770, 
            1, 
            0x20, 
            0, 
            0, 
            0xC300, 
            ResourceTemplate(){Register(SystemMemory,0,0,0,0)},
            ResourceTemplate(){Register(SystemMemory,0,0,0,0)},
            "platform.DRIPS"
        }
    })

    Device (CLUS)
    {
        Name (_HID, "ACPI0010")
        Name (_UID, 0x10)
        Name (_LPI, Package ()
        {
            0, 
            0x1000000, 
            2, 
            Package ()
            {
                0x170C, 
                0x0BB8, 
                0, 
                0, 
                0, 
                0, 
                0x20, 
                ResourceTemplate(){Register(SystemMemory,0,0,0,0)},
                ResourceTemplate(){Register(SystemMemory,0,0,0,0)},
                "L3Cluster.D2"
            }, 
            Package ()
            {
                0x1770, 
                0x0CE4, 
                1, 
                0, 
                0, 
                1, 
                0x40, 
                ResourceTemplate(){Register(SystemMemory,0,0,0,0)},
                ResourceTemplate(){Register(SystemMemory,0,0,0,0)},
                "L3Cluster.D4"
            }
        })

        Device (CPU0)
        {
            Name (_HID, "ACPI0007")
            Name (_UID, 0x0)
            Method (_STA){ Return (0x0F) }
            Name (_LPI, Package()
            {
                0, 
                0, 
                4, 
                Package ()
                {
                    0, 
                    0, 
                    1, 
                    0, 
                    0, 
                    0, 
                    ResourceTemplate(){Register(FFixedHW, 0x20, 0, 0x00000000FFFFFFFF, 3)}, 
                    ResourceTemplate(){Register(SystemMemory,0,0,0,0)},
                    ResourceTemplate(){Register(SystemMemory,0,0,0,0)},
                    "KryoGold0.C1"
                },
                Package ()
                {
                    0x0190, 
                    0x64, 
                    0, 
                    0, 
                    0, 
                    1, 
                    ResourceTemplate(){Register(FFixedHW, 0x20, 0, 0x0000000000000002, 3)},
                    ResourceTemplate(){Register(SystemMemory,0,0,0,0)},
                    ResourceTemplate(){Register(SystemMemory,0,0,0,0)},
                    "KryoGold0.C2"
                },
                Package ()
                {
                    0x0F0A, 
                    0x035C, 
                    1, 
                    1, 
                    0, 
                    1, 
                    ResourceTemplate(){Register(FFixedHW, 0x20, 0, 0x0000000040000003, 3)},
                    ResourceTemplate(){Register(SystemMemory,0,0,0,0)},
                    ResourceTemplate(){Register(SystemMemory,0,0,0,0)},
                    "KryoGold0.C3"
                },
                Package ()
                {
                    0x0F6E, 
                    0x038E, 
                    1, 
                    1, 
                    0, 
                    2, 
                    ResourceTemplate(){Register(FFixedHW, 0x20, 0, 0x0000000040000004, 3)},
                    ResourceTemplate(){Register(SystemMemory,0,0,0,0)}, 
                    ResourceTemplate(){Register(SystemMemory,0,0,0,0)},
                    "KryoGold0.C4"
                }
            })
        }

        Device (CPU1)
        {
            Name (_HID, "ACPI0007")
            Name (_UID, 1)
            Method (_STA){ Return (0x0F) }
            Name (_LPI, Package ()
            {
                0, 
                0, 
                4, 
                Package ()
                {
                    0, 
                    0, 
                    1, 
                    0, 
                    0, 
                    0, 
                    ResourceTemplate(){Register(FFixedHW, 0x20, 0, 0x00000000FFFFFFFF, 3)}, 
                    ResourceTemplate(){Register(SystemMemory,0,0,0,0)},
                    ResourceTemplate(){Register(SystemMemory,0,0,0,0)},
                    "KryoGold1.C1"
                }, 
                Package ()
                {
                    0x0190, 
                    0x64, 
                    0, 
                    0, 
                    0, 
                    1, 
                    ResourceTemplate(){Register(FFixedHW, 0x20, 0, 0x0000000000000002, 3)},
                    ResourceTemplate(){Register(SystemMemory,0,0,0,0)},
                    ResourceTemplate(){Register(SystemMemory,0,0,0,0)},
                    "KryoGold1.C2"
                }, 
                Package ()
                {
                    0x0F0A, 
                    0x035C, 
                    1, 
                    1, 
                    0, 
                    1, 
                    ResourceTemplate(){Register(FFixedHW, 0x20, 0, 0x0000000040000003, 3)},
                    ResourceTemplate(){Register(SystemMemory,0,0,0,0)},
                    ResourceTemplate(){Register(SystemMemory,0,0,0,0)},
                    "KryoGold1.C3"
                }, 
                Package ()
                {
                    0x0F6E, 
                    0x038E, 
                    1, 
                    1, 
                    0, 
                    2, 
                    ResourceTemplate(){Register(FFixedHW, 0x20, 0, 0x0000000040000004, 3)},
                    ResourceTemplate(){Register(SystemMemory,0,0,0,0)},
                    ResourceTemplate(){Register(SystemMemory,0,0,0,0)},
                    "KryoGold1.C4"
                }
            })
        }

        Device (CPU2)
        {
            Name (_HID, "ACPI0007")
            Name (_UID, 2)
            Method (_STA){ Return (0x0F) }
            Name (_LPI, Package ()
            {
                0, 
                0, 
                4, 
                Package ()
                {
                    0, 
                    0, 
                    1, 
                    0, 
                    0, 
                    0, 
                    ResourceTemplate(){Register(FFixedHW, 0x20, 0, 0x00000000FFFFFFFF, 3)}, 
                    ResourceTemplate(){Register(SystemMemory,0,0,0,0)},
                    ResourceTemplate(){Register(SystemMemory,0,0,0,0)},
                    "KryoGold2.C1"
                }, 
                Package ()
                {
                    0x0190, 
                    0x64, 
                    0, 
                    0, 
                    0, 
                    1, 
                    ResourceTemplate(){Register(FFixedHW, 0x20, 0, 0x0000000000000002, 3)}, 
                    ResourceTemplate(){Register(SystemMemory,0,0,0,0)},
                    ResourceTemplate(){Register(SystemMemory,0,0,0,0)},
                    "KryoGold2.C2"
                },
                Package ()
                {
                    0x0F0A, 
                    0x035C, 
                    1, 
                    1, 
                    0, 
                    1, 
                    ResourceTemplate(){Register(FFixedHW, 0x20, 0, 0x0000000040000003, 3)},
                    ResourceTemplate(){Register(SystemMemory,0,0,0,0)},
                    ResourceTemplate(){Register(SystemMemory,0,0,0,0)},
                    "KryoGold2.C3"
                },
                Package ()
                {
                    0x0F6E, 
                    0x038E, 
                    1, 
                    1, 
                    0, 
                    2, 
                    ResourceTemplate(){Register(FFixedHW, 0x20, 0, 0x0000000040000004, 3)},
                    ResourceTemplate(){Register(SystemMemory,0,0,0,0)},
                    ResourceTemplate(){Register(SystemMemory,0,0,0,0)},
                    "KryoGold2.C4"
                }
            })
        }

        Device (CPU3)
        {
            Name (_HID, "ACPI0007")
            Name (_UID, 3)
            Method (_STA){ Return (0x0F) }
            Name (_LPI, Package ()
            {
                0, 
                0, 
                4, 
                Package ()
                {
                    0, 
                    0, 
                    1, 
                    0, 
                    0, 
                    0, 
                    ResourceTemplate(){Register(FFixedHW, 0x20, 0, 0x00000000FFFFFFFF, 3)}, 
                    ResourceTemplate(){Register(SystemMemory,0,0,0,0)},
                    ResourceTemplate(){Register(SystemMemory,0,0,0,0)},
                    "KryoGold3.C1"
                },
                Package ()
                {
                    0x0190, 
                    0x64, 
                    0, 
                    0, 
                    0, 
                    1, 
                    ResourceTemplate(){Register(FFixedHW, 0x20, 0, 0x0000000000000002, 3)}, 
                    ResourceTemplate(){Register(SystemMemory,0,0,0,0)},
                    ResourceTemplate(){Register(SystemMemory,0,0,0,0)},
                    "KryoGold3.C2"
                }, 
                Package ()
                {
                    0x0F0A, 
                    0x035C, 
                    1, 
                    1, 
                    0, 
                    1, 
                    ResourceTemplate(){Register(FFixedHW, 0x20, 0, 0x0000000040000003, 3)}, 
                    ResourceTemplate(){Register(SystemMemory,0,0,0,0)},
                    ResourceTemplate(){Register(SystemMemory,0,0,0,0)},
                    "KryoGold3.C3"
                }, 
                Package ()
                {
                    0x0F6E, 
                    0x038E, 
                    1, 
                    1, 
                    0, 
                    2, 
                    ResourceTemplate(){Register(FFixedHW, 0x20, 0, 0x0000000040000004, 3)}, 
                    ResourceTemplate(){Register(SystemMemory,0,0,0,0)},
                    ResourceTemplate(){Register(SystemMemory,0,0,0,0)},
                    "KryoGold3.C4"
                }
            })
        }

        Device (CPU4)
        {
            Name (_HID, "ACPI0007")
            Name (_UID, 4)
            Method (_STA){ Return (0x0F) }
            Name (_LPI, Package ()
            {
                0, 
                0, 
                4, 
                Package ()
                {
                    0, 
                    0, 
                    1, 
                    0, 
                    0, 
                    0, 
                    ResourceTemplate(){Register(FFixedHW, 0x20, 0, 0x00000000FFFFFFFF, 3)},
                    ResourceTemplate(){Register(SystemMemory,0,0,0,0)},
                    ResourceTemplate(){Register(SystemMemory,0,0,0,0)},
                    "KryoPrime0.C1"
                },
                Package ()
                {
                    0x0190, 
                    0x64, 
                    0, 
                    0, 
                    0, 
                    1, 
                    ResourceTemplate(){Register(FFixedHW, 0x20, 0, 0x0000000000000002, 3)}, 
                    ResourceTemplate(){Register(SystemMemory,0,0,0,0)},
                    ResourceTemplate(){Register(SystemMemory,0,0,0,0)},
                    "KryoPrime0.C2"
                }, 
                Package ()
                {
                    0x0F96, 
                    0x03E8, 
                    1, 
                    1, 
                    0, 
                    1, 
                    ResourceTemplate(){Register (FFixedHW, 0x20, 0, 0x0000000040000003, 3)},
                    ResourceTemplate(){Register(SystemMemory,0,0,0,0)},
                    ResourceTemplate(){Register(SystemMemory,0,0,0,0)},
                    "KryoPrime0.C3"
                }, 
                Package ()
                {
                    0x118A, 
                    0x05DC, 
                    1, 
                    1, 
                    0, 
                    2, 
                    ResourceTemplate(){Register(FFixedHW, 0x20, 0, 0x0000000040000004, 3)}, 
                    ResourceTemplate(){Register(SystemMemory,0,0,0,0)},
                    ResourceTemplate(){Register(SystemMemory,0,0,0,0)},
                    "KryoPrime0.C4"
                }
            })
        }

        Device (CPU5)
        {
            Name (_HID, "ACPI0007")
            Name (_UID, 5)  //
            Method (_STA){ Return (0x0F) }
            Name (_LPI, Package ()
            {
                0, 
                0, 
                4, 
                Package ()
                {
                    0, 
                    0, 
                    1, 
                    0, 
                    0, 
                    0, 
                    ResourceTemplate(){Register(FFixedHW, 0x20, 0, 0x00000000FFFFFFFF, 3)}, 
                    ResourceTemplate(){Register(SystemMemory,0,0,0,0)},
                    ResourceTemplate(){Register(SystemMemory,0,0,0,0)},
                    "KryoPrime1.C1"
                }, 
                Package ()
                {
                    0x0190, 
                    0x64, 
                    0, 
                    0, 
                    0, 
                    1, 
                    ResourceTemplate(){Register(FFixedHW, 0x20, 0, 0x0000000000000002, 3)}, 
                    ResourceTemplate(){Register(SystemMemory,0,0,0,0)},
                    ResourceTemplate(){Register(SystemMemory,0,0,0,0)},
                    "KryoPrime1.C2"
                }, 
                Package ()
                {
                    0x0F96, 
                    0x03E8, 
                    1, 
                    1, 
                    0, 
                    1, 
                    ResourceTemplate(){Register(FFixedHW, 0x20, 0, 0x0000000040000003, 3)}, 
                    ResourceTemplate(){Register(SystemMemory,0,0,0,0)},
                    ResourceTemplate(){Register(SystemMemory,0,0,0,0)},
                    "KryoPrime1.C3"
                }, 
                Package ()
                {
                    0x118A, 
                    0x05DC, 
                    1, 
                    1, 
                    0, 
                    2, 
                    ResourceTemplate(){Register(FFixedHW, 0x20, 0, 0x0000000040000004, 3)}, 
                    ResourceTemplate(){Register(SystemMemory,0,0,0,0)},
                    ResourceTemplate(){Register(SystemMemory,0,0,0,0)},
                    "KryoPrime1.C4"
                }
            })
        }

        Device (CPU6)
        {
            Name (_HID, "ACPI0007")
            Name (_UID, 6)
            Method (_STA){ Return (0x0F) }
            Name (_LPI, Package ()
            {
                0, 
                0, 
                4, 
                Package ()
                {
                    0, 
                    0, 
                    1, 
                    0, 
                    0, 
                    0, 
                    ResourceTemplate(){Register(FFixedHW, 0x20, 0, 0x00000000FFFFFFFF, 3)}, 
                    ResourceTemplate(){Register(SystemMemory,0,0,0,0)},
                    ResourceTemplate(){Register(SystemMemory,0,0,0,0)},
                    "KryoPrime2.C1"
                }, 
                Package ()
                {
                    0x0190, 
                    0x64, 
                    0, 
                    0, 
                    0, 
                    1, 
                    ResourceTemplate(){Register(FFixedHW, 0x20, 0, 0x0000000000000002, 3)}, 
                    ResourceTemplate(){Register(SystemMemory,0,0,0,0)},
                    ResourceTemplate(){Register(SystemMemory,0,0,0,0)},
                    "KryoPrime2.C2"
                }, 
                Package ()
                {
                    0x0F96, 
                    0x03E8, 
                    1, 
                    1, 
                    0, 
                    1, 
                    ResourceTemplate(){Register(FFixedHW, 0x20, 0, 0x0000000040000003, 3)}, 
                    ResourceTemplate(){Register(SystemMemory,0,0,0,0)},
                    ResourceTemplate(){Register(SystemMemory,0,0,0,0)},
                    "KryoPrime2.C3"
                }, 
                Package ()
                {
                    0x118A, 
                    0x05DC, 
                    1, 
                    1, 
                    0, 
                    2, 
                    ResourceTemplate(){Register(FFixedHW, 0x20, 0, 0x0000000040000004, 3)}, 
                    ResourceTemplate(){Register(SystemMemory,0,0,0,0)},
                    ResourceTemplate(){Register(SystemMemory,0,0,0,0)},
                    "KryoPrime2.C4"
                }
            })
        }

        Device (CPU7)
        {
            Name (_HID, "ACPI0007")
            Name (_UID, 7)
            Method (_STA){ Return (0x0F) }
            Name (_LPI, Package ()
            {
                0, 
                0, 
                4, 
                Package ()
                {
                    0, 
                    0, 
                    1, 
                    0, 
                    0, 
                    0, 
                    ResourceTemplate(){Register(FFixedHW, 0x20, 0, 0x00000000FFFFFFFF, 3)}, 
                    ResourceTemplate(){Register(SystemMemory,0,0,0,0)},
                    ResourceTemplate(){Register(SystemMemory,0,0,0,0)},
                    "KryoPrime3.C1"
                }, 
                Package ()
                {
                    0x0190, 
                    0x64, 
                    0, 
                    0, 
                    0, 
                    1, 
                    ResourceTemplate(){Register(FFixedHW, 0x20, 0, 0x0000000000000002, 3)}, 
                    ResourceTemplate(){Register(SystemMemory,0,0,0,0)},
                    ResourceTemplate(){Register(SystemMemory,0,0,0,0)},
                    "KryoPrime3.C2"
                }, 
                Package ()
                {
                    0x0F96, 
                    0x03E8, 
                    1, 
                    1, 
                    0, 
                    1, 
                    ResourceTemplate(){Register(FFixedHW, 0x20, 0, 0x0000000040000003, 3)}, 
                    ResourceTemplate(){Register(SystemMemory,0,0,0,0)},
                    ResourceTemplate(){Register(SystemMemory,0,0,0,0)},
                    "KryoPrime3.C3"
                }, 
                Package ()
                {
                    0x118A, 
                    0x05DC, 
                    1, 
                    1, 
                    0, 
                    2, 
                    ResourceTemplate(){Register(FFixedHW, 0x20, 0, 0x0000000040000004, 3)}, 
                    ResourceTemplate(){Register(SystemMemory,0,0,0,0)},
                    ResourceTemplate(){Register(SystemMemory,0,0,0,0)},
                    "KryoPrime3.C4"
                }
            })
        }
    }
}
