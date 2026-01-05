/** @file
  Differentiated System Description Table Fields (DSDT)

  Copyright (c) 2014-2018, ARM Ltd. All rights reserved.<BR>
  Copyright (c) 2019-2020, Bingxing Wang. All rights reserved.<BR>
  Portion retrieved from Surface RT 2 DSDT dump

  This program and the accompanying materials are licensed and made available under 
  the terms and conditions of the BSD License which accompanies this distribution.  
  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "Platform.h"

DefinitionBlock("DsdtTable.aml", "DSDT", 1, "NVIDIA", "TEGRA210", EFI_ACPI_ARM_OEM_REVISION) 
{
    Scope(_SB) 
    {

        Device(CPU0) 
        {
            Name(_HID, "ACPI0007")
            Name(_UID, 0)
        }

        Device(CPU1) 
        {
            Name(_HID, "ACPI0007")
            Name(_UID, 1)
        }

        Device(CPU2) 
        {
            Name(_HID, "ACPI0007")
            Name(_UID, 2)
        }

        Device(CPU3) 
        {
            Name(_HID, "ACPI0007")
            Name(_UID, 3)
        }

        // Helper functions
        Method (GCRS, 3, NotSerialized)
        {
            Name (RSRC, ResourceTemplate ()
            {
                Memory32Fixed (ReadWrite,
                    0x00000000,         // Address Base
                    0x00000000,         // Address Length
                    _Y00)
                Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive, ,, _Y01)
                {
                    0x00000000,
                }
            })
            CreateDWordField (RSRC, \_SB.GCRS._Y00._BAS, MBAS)  // _BAS: Base Address
            CreateDWordField (RSRC, \_SB.GCRS._Y00._LEN, MBLE)  // _LEN: Length
            CreateWordField (RSRC, \_SB.GCRS._Y01._INT, INTN)  // _INT: Interrupts
            MBAS = Arg0
            MBLE = Arg1
            INTN = Arg2
            Return (RSRC)
        }

        Method (CRSD, 2, NotSerialized)
        {
            Name (RSRC, ResourceTemplate ()
            {
                FixedDMA (0x0000, 0x0000, Width32bit, _Y07)
                FixedDMA (0x0001, 0x0001, Width32bit, _Y08)
            })
            CreateWordField (RSRC, One, REQ1)
            CreateWordField (RSRC, \_SB.CRSD._Y07._TYP, CHN1)  // _TYP: Type
            CreateWordField (RSRC, \_SB.CRSD._Y08._DMA, REQ2)  // _DMA: Direct Memory Access
            CreateWordField (RSRC, \_SB.CRSD._Y08._TYP, CHN2)  // _TYP: Type
            REQ1 = Arg0
            REQ2 = (Arg0 | 0x20)
            CHN1 = Arg1
            CHN2 = Arg1++
            Return (RSRC)
        }

        Method (CRS3, 5, NotSerialized)
        {
            Local0 = GCRS (Arg0, Arg1, Arg2)
            Local1 = CRSD (Arg3, Arg4)
            Return (ConcatenateResTemplate (Local0, Local1))
        }

        // UART 2: Right Joy-Con
        Device (UAR2)
        {
            Method (_HID, 0, NotSerialized)
            {
                Return ("NVDA0100")
            }

            Name (_UID, One)
            Method (_CRS, 0, NotSerialized)
            {
                Return (CRS3 (0x70006040, 0x40, 0x45, 0x09, 0x02))
            }

            Method (_STA, 0, NotSerialized)
            {
                Return (0x0F)
            }
        }

        // STM FingerTip S Touchscreen (HID over I2C)
        Device (TCHP)
        {
            Name (_HID, "STMF0001")  // STM FingerTip S
            Name (_CID, "PNP0C50")   // HID over I2C
            Name (_UID, 0)

            Method (_CRS, 0, NotSerialized)
            {
                Name (RBUF, ResourceTemplate ()
                {
                    // I2C Connection to Touch Controller
                    I2cSerialBusV2 (0x49,           // I2C Address
                        ControllerInitiated,
                        400000,                     // 400 kHz
                        AddressingMode7Bit,
                        "\\_SB.I2C3",               // I2C Controller
                        0,
                        ResourceConsumer,
                        ,
                        )
                    // Touch Interrupt (not used in polled mode, placeholder)
                    // GpioInt (Level, ActiveLow, Shared, PullUp, 0, "\\_SB.GPIO0") {7}
                })
                Return (RBUF)
            }

            // HID Descriptor Address for Windows
            Method (_DSM, 4, Serialized)
            {
                If (LEqual (Arg0, ToUUID ("3cdff6f7-4267-4555-ad05-b30a3d8938de")))
                {
                    // HID over I2C DSM
                    Switch (ToInteger (Arg2))
                    {
                        Case (0)
                        {
                            // Query supported functions
                            Return (Buffer () {0x03})  // Functions 0 and 1
                        }
                        Case (1)
                        {
                            // HID Descriptor Address (STM uses 0x0001)
                            Return (0x0001)
                        }
                        Default
                        {
                            Return (Buffer () {0})
                        }
                    }
                }
                Return (Buffer () {0})
            }

            Method (_STA, 0, NotSerialized)
            {
                Return (0x00)  // DISABLED - references non-existent I2C3
            }
        }

        // USB Device Controller (Legacy ChipIdea)
        Device (USBD)
        {
            Name (_HID, "NVDA0203")  // Tegra210 USBD
            Name (_UID, 0)

            Method (_CRS, 0, NotSerialized)
            {
                Name (RBUF, ResourceTemplate ()
                {
                    // Legacy USBD Base (0x7D000000)
                    Memory32Fixed (ReadWrite, 0x7D000000, 0x4000)
                    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) {22} // Interrupt 20 + 32? (Check Tegra TRM, usually high)
                })
                Return (RBUF)
            }

            Method (_STA, 0, NotSerialized)
            {
                Return (0x0F)  // Device present and enabled
            }
        }

    } // Scope(_SB)
}