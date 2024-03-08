//
// CPU (Central Processing Unit) Cores & Clusters
//
Device (SYSM)
{
    Name (_HID, "ACPI0010")                                                                                                             // Hardware ID
    Name (_UID, 0x00100000)                                                                                                             // Unique ID

    Device (CLU0)
    {
        Name (_HID, "ACPI0010")                                                                                                         // Hardware ID
        Name (_UID, 0x10)                                                                                                               // Unique ID

        Name (_LPI, Package ()                                                                                                          // Low Power Idle States
        {
            0,                                                                                                                          // Version
            0x1000000,                                                                                                                  // Level ID
            2,                                                                                                                          // Count

            Package ()
            {
                504,                                                                                                                    // Minimal Residency (us)
                134,                                                                                                                    // Wake Latency      (us)
                0,                                                                                                                      // Enable State
                0,                                                                                                                      // Arch Context Flag
                0,                                                                                                                      // Residency Counter Frequency
                0,                                                                                                                      // Parent Enable State
                0x20,                                                                                                                   // Integer Entry Method

                ResourceTemplate () { Register (SystemMemory, 0, 0, 0, 0) },                                                            // Residency Counter Register
                ResourceTemplate () { Register (SystemMemory, 0, 0, 0, 0) },                                                            // Usage Counter Register

                "L2Active.D2"                                                                                                           // Name
            }, 

            Package ()
            {
                9183,                                                                                                                   // Minimal Residency (us)
                4562,                                                                                                                   // Wake Latency      (us)
                1,                                                                                                                      // Enable State
                0,                                                                                                                      // Arch Context Flag
                0,                                                                                                                      // Residency Counter Frequency
                1,                                                                                                                      // Parent Enable State
                0x40,                                                                                                                   // Integer Entry Method

                ResourceTemplate () { Register (SystemMemory, 0, 0, 0, 0) },                                                            // Residency Counter Register
                ResourceTemplate () { Register (SystemMemory, 0, 0, 0, 0) },                                                            // Usage Counter Register

                "L2PowerCollapse.D4"                                                                                                    // Name
            }
        })

        Device (CPU0)
        {
            Name   (_HID, "ACPI0007")                                                                                                   // Hardware ID
            Name   (_UID, 0)                                                                                                            // Unique ID
            Method (_STA, 0, NotSerialized) { Return (0x0F) }                                                                           // Status

            Name (_LPI, Package ()                                                                                                      // Low Power Idle States
            {
                0,                                                                                                                      // Version
                0,                                                                                                                      // Level ID
                3,                                                                                                                      // Count

                Package ()
                {
                    0,                                                                                                                  // Minimal Residency (us)
                    0,                                                                                                                  // Wake Latency      (us)
                    1,                                                                                                                  // Enable State
                    0,                                                                                                                  // Arch Context Flag
                    0,                                                                                                                  // Residency Counter Frequency
                    0,                                                                                                                  // Parent Enable State

                    ResourceTemplate () { Register (FFixedHW,     0x20, 0, 0xFFFFFFFF, 3) },                                            // Register Entry Method
                    ResourceTemplate () { Register (SystemMemory, 0,    0, 0,  0) },                                                    // Residency Counter Register
                    ResourceTemplate () { Register (SystemMemory, 0,    0, 0,  0) },                                                    // Usage Counter Register

                    "KryoSilver0.C1"                                                                                                    // Name
                }, 

                Package ()
                {
                    504,                                                                                                                // Minimal Residency (us)
                    86,                                                                                                                 // Wake Latency      (us)
                    0,                                                                                                                  // Enable State
                    0,                                                                                                                  // Arch Context Flag
                    0,                                                                                                                  // Residency Counter Frequency
                    1,                                                                                                                  // Parent Enable State

                    ResourceTemplate () { Register (FFixedHW,     0x20, 0, 2, 3) },                                                     // Register Entry Method
                    ResourceTemplate () { Register (SystemMemory, 0,    0, 0, 0) },                                                     // Residency Counter Register
                    ResourceTemplate () { Register (SystemMemory, 0,    0, 0, 0) },                                                     // Usage Counter Register

                    "KryoSilver0.C2"                                                                                                    // Name
                }, 

                Package ()
                {
                    9183,                                                                                                               // Minimal Residency (us)
                    4562,                                                                                                               // Wake Latency      (us)
                    1,                                                                                                                  // Enable State
                    1,                                                                                                                  // Arch Context Flag
                    0,                                                                                                                  // Residency Counter Frequency
                    1,                                                                                                                  // Parent Enable State

                    ResourceTemplate () { Register (FFixedHW,     0x20, 0, 0x40000003, 3) },                                            // Register Entry Method
                    ResourceTemplate () { Register (SystemMemory, 0,    0, 0,  0) },                                                    // Residency Counter Register
                    ResourceTemplate () { Register (SystemMemory, 0,    0, 0,  0) },                                                    // Usage Counter Register

                    "KryoSilver0.C3"                                                                                                    // Name
                }
            })
        }

        Device (CPU1)
        {
            Name   (_HID, "ACPI0007")                                                                                                   // Hardware ID
            Name   (_UID, 1)                                                                                                            // Unique ID
            Method (_STA, 0, NotSerialized) { Return (0x0F) }                                                                           // Status

            Name (_LPI, Package ()                                                                                                      // Low Power Idle States
            {
                0,                                                                                                                      // Version
                0,                                                                                                                      // Level ID
                3,                                                                                                                      // Count

                Package ()
                {
                    0,                                                                                                                  // Minimal Residency (us)
                    0,                                                                                                                  // Wake Latency      (us)
                    1,                                                                                                                  // Enable State
                    0,                                                                                                                  // Arch Context Flag
                    0,                                                                                                                  // Residency Counter Frequency
                    0,                                                                                                                  // Parent Enable State

                    ResourceTemplate () { Register (FFixedHW,     0x20, 0, 0xFFFFFFFF, 3) },                                            // Register Entry Method
                    ResourceTemplate () { Register (SystemMemory, 0,    0, 0,  0) },                                                    // Residency Counter Register
                    ResourceTemplate () { Register (SystemMemory, 0,    0, 0,  0) },                                                    // Usage Counter Register

                    "KryoSilver1.C1"                                                                                                    // Name
                }, 

                Package ()
                {
                    504,                                                                                                                // Minimal Residency (us)
                    86,                                                                                                                 // Wake Latency      (us)
                    0,                                                                                                                  // Enable State
                    0,                                                                                                                  // Arch Context Flag
                    0,                                                                                                                  // Residency Counter Frequency
                    1,                                                                                                                  // Parent Enable State

                    ResourceTemplate () { Register (FFixedHW,     0x20, 0, 2, 3) },                                                     // Register Entry Method
                    ResourceTemplate () { Register (SystemMemory, 0,    0, 0, 0) },                                                     // Residency Counter Register
                    ResourceTemplate () { Register (SystemMemory, 0,    0, 0, 0) },                                                     // Usage Counter Register

                    "KryoSilver1.C2"                                                                                                    // Name
                }, 

                Package ()
                {
                    9183,                                                                                                               // Minimal Residency (us)
                    4562,                                                                                                               // Wake Latency      (us)
                    1,                                                                                                                  // Enable State
                    1,                                                                                                                  // Arch Context Flag
                    0,                                                                                                                  // Residency Counter Frequency
                    1,                                                                                                                  // Parent Enable State

                    ResourceTemplate () { Register (FFixedHW,     0x20, 0, 0x40000003, 3) },                                            // Register Entry Method
                    ResourceTemplate () { Register (SystemMemory, 0,    0, 0,  0) },                                                    // Residency Counter Register
                    ResourceTemplate () { Register (SystemMemory, 0,    0, 0,  0) },                                                    // Usage Counter Register

                    "KryoSilver1.C3"                                                                                                    // Name
                }
            })
        }

        Device (CPU2)
        {
            Name   (_HID, "ACPI0007")                                                                                                   // Hardware ID
            Name   (_UID, 2)                                                                                                            // Unique ID
            Method (_STA, 0, NotSerialized) { Return (0x0F) }                                                                           // Status

            Name (_LPI, Package ()                                                                                                      // Low Power Idle States
            {
                0,                                                                                                                      // Version
                0,                                                                                                                      // Level ID
                3,                                                                                                                      // Count

                Package ()
                {
                    0,                                                                                                                  // Minimal Residency (us)
                    0,                                                                                                                  // Wake Latency      (us)
                    1,                                                                                                                  // Enable State
                    0,                                                                                                                  // Arch Context Flag
                    0,                                                                                                                  // Residency Counter Frequency
                    0,                                                                                                                  // Parent Enable State

                    ResourceTemplate () { Register (FFixedHW,     0x20, 0, 0xFFFFFFFF, 3) },                                            // Register Entry Method
                    ResourceTemplate () { Register (SystemMemory, 0,    0, 0,  0) },                                                    // Residency Counter Register
                    ResourceTemplate () { Register (SystemMemory, 0,    0, 0,  0) },                                                    // Usage Counter Register

                    "KryoSilver2.C1"                                                                                                    // Name
                }, 

                Package ()
                {
                    504,                                                                                                                // Minimal Residency (us)
                    86,                                                                                                                 // Wake Latency      (us)
                    0,                                                                                                                  // Enable State
                    0,                                                                                                                  // Arch Context Flag
                    0,                                                                                                                  // Residency Counter Frequency
                    1,                                                                                                                  // Parent Enable State

                    ResourceTemplate () { Register (FFixedHW,     0x20, 0, 2, 3) },                                                     // Register Entry Method
                    ResourceTemplate () { Register (SystemMemory, 0,    0, 0, 0) },                                                     // Residency Counter Register
                    ResourceTemplate () { Register (SystemMemory, 0,    0, 0, 0) },                                                     // Usage Counter Register

                    "KryoSilver2.C2"                                                                                                    // Name
                }, 

                Package ()
                {
                    9183,                                                                                                               // Minimal Residency (us)
                    4562,                                                                                                               // Wake Latency      (us)
                    1,                                                                                                                  // Enable State
                    1,                                                                                                                  // Arch Context Flag
                    0,                                                                                                                  // Residency Counter Frequency
                    1,                                                                                                                  // Parent Enable State

                    ResourceTemplate () { Register (FFixedHW,     0x20, 0, 0x40000003, 3) },                                            // Register Entry Method
                    ResourceTemplate () { Register (SystemMemory, 0,    0, 0,  0) },                                                    // Residency Counter Register
                    ResourceTemplate () { Register (SystemMemory, 0,    0, 0,  0) },                                                    // Usage Counter Register

                    "KryoSilver2.C3"                                                                                                    // Name
                }
            })
        }

        Device (CPU3)
        {
            Name   (_HID, "ACPI0007")                                                                                                   // Hardware ID
            Name   (_UID, 3)                                                                                                            // Unique ID
            Method (_STA, 0, NotSerialized) { Return (0x0F) }                                                                           // Status

            Name (_LPI, Package ()                                                                                                      // Low Power Idle States
            {
                0,                                                                                                                      // Version
                0,                                                                                                                      // Level ID
                3,                                                                                                                      // Count

                Package ()
                {
                    0,                                                                                                                  // Minimal Residency (us)
                    0,                                                                                                                  // Wake Latency      (us)
                    1,                                                                                                                  // Enable State
                    0,                                                                                                                  // Arch Context Flag
                    0,                                                                                                                  // Residency Counter Frequency
                    0,                                                                                                                  // Parent Enable State

                    ResourceTemplate () { Register (FFixedHW,     0x20, 0, 0xFFFFFFFF, 3) },                                            // Register Entry Method
                    ResourceTemplate () { Register (SystemMemory, 0,    0, 0,  0) },                                                    // Residency Counter Register
                    ResourceTemplate () { Register (SystemMemory, 0,    0, 0,  0) },                                                    // Usage Counter Register

                    "KryoSilver3.C1"                                                                                                    // Name
                }, 

                Package ()
                {
                    504,                                                                                                                // Minimal Residency (us)
                    86,                                                                                                                 // Wake Latency      (us)
                    0,                                                                                                                  // Enable State
                    0,                                                                                                                  // Arch Context Flag
                    0,                                                                                                                  // Residency Counter Frequency
                    1,                                                                                                                  // Parent Enable State

                    ResourceTemplate () { Register (FFixedHW,     0x20, 0, 2, 3) },                                                     // Register Entry Method
                    ResourceTemplate () { Register (SystemMemory, 0,    0, 0, 0) },                                                     // Residency Counter Register
                    ResourceTemplate () { Register (SystemMemory, 0,    0, 0, 0) },                                                     // Usage Counter Register

                    "KryoSilver3.C2"                                                                                                    // Name
                }, 

                Package ()
                {
                    9183,                                                                                                               // Minimal Residency (us)
                    4562,                                                                                                               // Wake Latency      (us)
                    1,                                                                                                                  // Enable State
                    1,                                                                                                                  // Arch Context Flag
                    0,                                                                                                                  // Residency Counter Frequency
                    1,                                                                                                                  // Parent Enable State

                    ResourceTemplate () { Register (FFixedHW,     0x20, 0, 0x40000003, 3) },                                            // Register Entry Method
                    ResourceTemplate () { Register (SystemMemory, 0,    0, 0,  0) },                                                    // Residency Counter Register
                    ResourceTemplate () { Register (SystemMemory, 0,    0, 0,  0) },                                                    // Usage Counter Register

                    "KryoSilver3.C3"                                                                                                    // Name
                }
            })
        }
    }

    Device (CLU1)
    {
        Name (_HID, "ACPI0010")                                                                                                         // Hardware ID
        Name (_UID, 0x20)                                                                                                               // Unique ID

        Name (_LPI, Package ()                                                                                                          // Low Power Idle States
        {
            0,                                                                                                                          // Version
            0x1000000,                                                                                                                  // Level ID
            2,                                                                                                                          // Count

            Package ()
            {
                1302,                                                                                                                   // Minimal Residency (us)
                82,                                                                                                                     // Wake Latency      (us)
                0,                                                                                                                      // Enable State
                0,                                                                                                                      // Arch Context Flag
                0,                                                                                                                      // Residency Counter Frequency
                0,                                                                                                                      // Parent Enable State
                0x20,                                                                                                                   // Integer Entry Method

                ResourceTemplate () { Register (SystemMemory, 0, 0, 0, 0) },                                                            // Residency Counter Register
                ResourceTemplate () { Register (SystemMemory, 0, 0, 0, 0) },                                                            // Usage Counter Register

                "L2Active.D2"                                                                                                           // Name
            }, 

            Package ()
            {
                9419,                                                                                                                   // Minimal Residency (us)
                2027,                                                                                                                   // Wake Latency      (us)
                1,                                                                                                                      // Enable State
                0,                                                                                                                      // Arch Context Flag
                0,                                                                                                                      // Residency Counter Frequency
                1,                                                                                                                      // Parent Enable State
                0x40,                                                                                                                   // Integer Entry Method

                ResourceTemplate () { Register (SystemMemory, 0, 0, 0, 0) },                                                            // Residency Counter Register
                ResourceTemplate () { Register (SystemMemory, 0, 0, 0, 0) },                                                            // Usage Counter Register

                "L2PowerCollapse.D4"                                                                                                    // Name
            }
        })

        Device (CPU4)
        {
            Name (_HID, "ACPI0007")                                                                                                     // Hardware ID
            Name (_UID, 4)                                                                                                              // Unique ID
            Method (_STA, 0, NotSerialized) { Return (0x0F) }                                                                           // Status

            Name (_LPI, Package ()                                                                                                      // Low Power Idle States
            {
                0,                                                                                                                      // Version
                0,                                                                                                                      // Level ID
                3,                                                                                                                      // Count

                Package ()
                {
                    0,                                                                                                                  // Minimal Residency (us)
                    0,                                                                                                                  // Wake Latency      (us)
                    1,                                                                                                                  // Enable State
                    0,                                                                                                                  // Arch Context Flag
                    0,                                                                                                                  // Residency Counter Frequency
                    0,                                                                                                                  // Parent Enable State

                    ResourceTemplate () { Register (FFixedHW,     0x20, 0, 0xFFFFFFFF, 3) },                                            // Register Entry Method
                    ResourceTemplate () { Register (SystemMemory, 0,    0, 0,  0) },                                                    // Residency Counter Register
                    ResourceTemplate () { Register (SystemMemory, 0,    0, 0,  0) },                                                    // Usage Counter Register

                    "KryoGold0.C1"                                                                                                      // Name
                }, 

                Package ()
                {
                    516,                                                                                                                // Minimal Residency (us)
                    82,                                                                                                                 // Wake Latency      (us)
                    0,                                                                                                                  // Enable State
                    0,                                                                                                                  // Arch Context Flag
                    0,                                                                                                                  // Residency Counter Frequency
                    1,                                                                                                                  // Parent Enable State

                    ResourceTemplate () { Register (FFixedHW,     0x20, 0, 2, 3) },                                                     // Register Entry Method
                    ResourceTemplate () { Register (SystemMemory, 0,    0, 0, 0) },                                                     // Residency Counter Register
                    ResourceTemplate () { Register (SystemMemory, 0,    0, 0, 0) },                                                     // Usage Counter Register

                    "KryoGold0.C2"                                                                                                      // Name
                }, 

                Package ()
                {
                    9419,                                                                                                               // Minimal Residency (us)
                    2027,                                                                                                               // Wake Latency      (us)
                    1,                                                                                                                  // Enable State
                    1,                                                                                                                  // Arch Context Flag
                    0,                                                                                                                  // Residency Counter Frequency
                    1,                                                                                                                  // Parent Enable State

                    ResourceTemplate () { Register (FFixedHW,     0x20, 0, 0x40000003, 3) },                                            // Register Entry Method
                    ResourceTemplate () { Register (SystemMemory, 0,    0, 0,  0) },                                                    // Residency Counter Register
                    ResourceTemplate () { Register (SystemMemory, 0,    0, 0,  0) },                                                    // Usage Counter Register

                    "KryoGold0.C3"                                                                                                      // Name
                }
            })
        }

        Device (CPU5)
        {
            Name (_HID, "ACPI0007")                                                                                                     // Hardware ID
            Name (_UID, 5)                                                                                                              // Unique ID
            Method (_STA, 0, NotSerialized) { Return (0x0F) }                                                                           // Status

            Name (_LPI, Package ()                                                                                                      // Low Power Idle States
            {
                0,                                                                                                                      // Version
                0,                                                                                                                      // Level ID
                3,                                                                                                                      // Count

                Package ()
                {
                    0,                                                                                                                  // Minimal Residency (us)
                    0,                                                                                                                  // Wake Latency      (us)
                    1,                                                                                                                  // Enable State
                    0,                                                                                                                  // Arch Context Flag
                    0,                                                                                                                  // Residency Counter Frequency
                    0,                                                                                                                  // Parent Enable State

                    ResourceTemplate () { Register (FFixedHW,     0x20, 0, 0xFFFFFFFF, 3) },                                            // Register Entry Method
                    ResourceTemplate () { Register (SystemMemory, 0,    0, 0,  0) },                                                    // Residency Counter Register
                    ResourceTemplate () { Register (SystemMemory, 0,    0, 0,  0) },                                                    // Usage Counter Register

                    "KryoGold1.C1"                                                                                                      // Name
                }, 

                Package ()
                {
                    516,                                                                                                                // Minimal Residency (us)
                    82,                                                                                                                 // Wake Latency      (us)
                    0,                                                                                                                  // Enable State
                    0,                                                                                                                  // Arch Context Flag
                    0,                                                                                                                  // Residency Counter Frequency
                    1,                                                                                                                  // Parent Enable State

                    ResourceTemplate () { Register (FFixedHW,     0x20, 0, 2, 3) },                                                     // Register Entry Method
                    ResourceTemplate () { Register (SystemMemory, 0,    0, 0, 0) },                                                     // Residency Counter Register
                    ResourceTemplate () { Register (SystemMemory, 0,    0, 0, 0) },                                                     // Usage Counter Register

                    "KryoGold1.C2"                                                                                                      // Name
                }, 

                Package ()
                {
                    9419,                                                                                                               // Minimal Residency (us)
                    2027,                                                                                                               // Wake Latency      (us)
                    1,                                                                                                                  // Enable State
                    1,                                                                                                                  // Arch Context Flag
                    0,                                                                                                                  // Residency Counter Frequency
                    1,                                                                                                                  // Parent Enable State

                    ResourceTemplate () { Register (FFixedHW,     0x20, 0, 0x40000003, 3) },                                            // Register Entry Method
                    ResourceTemplate () { Register (SystemMemory, 0,    0, 0,  0) },                                                    // Residency Counter Register
                    ResourceTemplate () { Register (SystemMemory, 0,    0, 0,  0) },                                                    // Usage Counter Register

                    "KryoGold1.C3"                                                                                                      // Name
                }
            })
        }

        Device (CPU6)
        {
            Name (_HID, "ACPI0007")                                                                                                     // Hardware ID
            Name (_UID, 6)                                                                                                              // Unique ID
            Method (_STA, 0, NotSerialized) { Return (0x0F) }                                                                           // Status

            Name (_LPI, Package ()                                                                                                      // Low Power Idle States
            {
                0,                                                                                                                      // Version
                0,                                                                                                                      // Level ID
                3,                                                                                                                      // Count

                Package ()
                {
                    0,                                                                                                                  // Minimal Residency (us)
                    0,                                                                                                                  // Wake Latency      (us)
                    1,                                                                                                                  // Enable State
                    0,                                                                                                                  // Arch Context Flag
                    0,                                                                                                                  // Residency Counter Frequency
                    0,                                                                                                                  // Parent Enable State

                    ResourceTemplate () { Register (FFixedHW,     0x20, 0, 0xFFFFFFFF, 3) },                                            // Register Entry Method
                    ResourceTemplate () { Register (SystemMemory, 0,    0, 0,  0) },                                                    // Residency Counter Register
                    ResourceTemplate () { Register (SystemMemory, 0,    0, 0,  0) },                                                    // Usage Counter Register

                    "KryoGold2.C1"                                                                                                      // Name
                }, 

                Package ()
                {
                    516,                                                                                                                // Minimal Residency (us)
                    82,                                                                                                                 // Wake Latency      (us)
                    0,                                                                                                                  // Enable State
                    0,                                                                                                                  // Arch Context Flag
                    0,                                                                                                                  // Residency Counter Frequency
                    1,                                                                                                                  // Parent Enable State

                    ResourceTemplate () { Register (FFixedHW,     0x20, 0, 2, 3) },                                                     // Register Entry Method
                    ResourceTemplate () { Register (SystemMemory, 0,    0, 0, 0) },                                                     // Residency Counter Register
                    ResourceTemplate () { Register (SystemMemory, 0,    0, 0, 0) },                                                     // Usage Counter Register

                    "KryoGold2.C2"                                                                                                      // Name
                }, 

                Package ()
                {
                    9419,                                                                                                               // Minimal Residency (us)
                    2027,                                                                                                               // Wake Latency      (us)
                    1,                                                                                                                  // Enable State
                    1,                                                                                                                  // Arch Context Flag
                    0,                                                                                                                  // Residency Counter Frequency
                    1,                                                                                                                  // Parent Enable State

                    ResourceTemplate () { Register (FFixedHW,     0x20, 0, 0x40000003, 3) },                                            // Register Entry Method
                    ResourceTemplate () { Register (SystemMemory, 0,    0, 0,  0) },                                                    // Residency Counter Register
                    ResourceTemplate () { Register (SystemMemory, 0,    0, 0,  0) },                                                    // Usage Counter Register

                    "KryoGold2.C3"                                                                                                      // Name
                }
            })
        }

        Device (CPU7)
        {
            Name (_HID, "ACPI0007")                                                                                                     // Hardware ID
            Name (_UID, 7)                                                                                                              // Unique ID
            Method (_STA, 0, NotSerialized) { Return (0x0F) }                                                                           // Status

            Name (_LPI, Package ()                                                                                                      // Low Power Idle States
            {
                0,                                                                                                                      // Version
                0,                                                                                                                      // Level ID
                3,                                                                                                                      // Count

                Package ()
                {
                    0,                                                                                                                  // Minimal Residency (us)
                    0,                                                                                                                  // Wake Latency      (us)
                    1,                                                                                                                  // Enable State
                    0,                                                                                                                  // Arch Context Flag
                    0,                                                                                                                  // Residency Counter Frequency
                    0,                                                                                                                  // Parent Enable State

                    ResourceTemplate () { Register (FFixedHW,     0x20, 0, 0xFFFFFFFF, 3) },                                            // Register Entry Method
                    ResourceTemplate () { Register (SystemMemory, 0,    0, 0,  0) },                                                    // Residency Counter Register
                    ResourceTemplate () { Register (SystemMemory, 0,    0, 0,  0) },                                                    // Usage Counter Register

                    "KryoGold3.C1"                                                                                                      // Name
                }, 

                Package ()
                {
                    516,                                                                                                                // Minimal Residency (us)
                    82,                                                                                                                 // Wake Latency      (us)
                    0,                                                                                                                  // Enable State
                    0,                                                                                                                  // Arch Context Flag
                    0,                                                                                                                  // Residency Counter Frequency
                    1,                                                                                                                  // Parent Enable State

                    ResourceTemplate () { Register (FFixedHW,     0x20, 0, 2, 3) },                                                     // Register Entry Method
                    ResourceTemplate () { Register (SystemMemory, 0,    0, 0, 0) },                                                     // Residency Counter Register
                    ResourceTemplate () { Register (SystemMemory, 0,    0, 0, 0) },                                                     // Usage Counter Register

                    "KryoGold3.C2"                                                                                                      // Name
                }, 

                Package ()
                {
                    9419,                                                                                                               // Minimal Residency (us)
                    2027,                                                                                                               // Wake Latency      (us)
                    1,                                                                                                                  // Enable State
                    1,                                                                                                                  // Arch Context Flag
                    0,                                                                                                                  // Residency Counter Frequency
                    1,                                                                                                                  // Parent Enable State

                    ResourceTemplate () { Register (FFixedHW,     0x20, 0, 0x40000003, 3) },                                            // Register Entry Method
                    ResourceTemplate () { Register (SystemMemory, 0,    0, 0,  0) },                                                    // Residency Counter Register
                    ResourceTemplate () { Register (SystemMemory, 0,    0, 0,  0) },                                                    // Usage Counter Register

                    "KryoGold3.C3"                                                                                                      // Name
                }
            })
        }
    }
}