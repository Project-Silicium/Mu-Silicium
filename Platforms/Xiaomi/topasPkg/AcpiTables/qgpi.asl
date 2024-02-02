//
// This file contains the QUPv3 ACPI device definitions.
// GPI is the interface used by buses drivers for different peripherals.
//

//
//  Device Map:
//    QGPI
//
//  List of Devices

// sc8180x gpi
Device (QGP0)
{
  // Indicates dependency on PEP
  //Name (_DEP, Package () { \_SB_.PEP0 })

  Name (_HID, "QCOM02F4")
  Alias(\_SB.PSUB, _SUB)
  Name (_UID, 0)
  Name (_CCA, 0)
   
   Method (_CRS, 0x0, Serialized) 
  {
    Name (RBUF, ResourceTemplate ()
    {
      // QUPV3_0 address space
      Memory32Fixed (ReadWrite, 0x04A04000, 0x50000)
      
      //Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive, , , ) {367} // GPII-ID 0x0
      Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive, , , ) {368} // GPII-ID 0x1
      //Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive, , , ) {369} // GPII-ID 0x2
      //Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive, , , ) {370} // GPII-ID 0x3
      //Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive, , , ) {371} // GPII-ID 0x4
      Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive, , , ) {372} // GPII-ID 0x5
      //Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive, , , ) {373} // GPII-ID 0x6
      //Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive, , , ) {374} // GPII-ID 0x7
      //Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive, , , ) {375} // GPII-ID 0x8
      //Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive, , , ) {376} // GPII-ID 0x9
      //Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive, , , ) {377} // GPII-ID 0xA
      //Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive, , , ) {378} // GPII-ID 0xB
      //Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive, , , ) {379} // GPII-ID 0xC
    })      
    Return (RBUF)
  }
}

