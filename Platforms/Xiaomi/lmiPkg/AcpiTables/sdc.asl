//
// Storage - SD card
//
Device (SDC2)
{
   Name (_DEP, Package(0x1) {
       \_SB_.GIO0
   })

   Name (_HID, "QCOM2466")
   Alias(\_SB.PSUB, _SUB)
   Name (_CID, "ACPI\QCOM2466")
   Name (_UID, 1)
   Name (_CCA, 0)

   Method (_CRS, 0x0, NotSerialized) {
       Name (RBUF, ResourceTemplate ()
       {
           // SDCC2 register address space
           Memory32Fixed (ReadWrite, 0x4784000, 0x00001000)

           Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive, , , ) {382}

           // Card detect GPIO
           GpioInt(Edge, ActiveBoth, SharedAndWake, PullUp, 30000, "\\_SB.GIO0", ,) {58}
           Gpioio(Shared, PullUp, 0, 0, , "\\_SB.GIO0", ,) {58}
       })
       Return (RBUF)
   }

   Method(_DIS)
   {
      // Place holder to allow disable
   }
   Method (_STA) 
   {
       Return(0xF)
   }
}
