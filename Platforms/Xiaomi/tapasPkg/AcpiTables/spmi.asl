//
//SPMI driver.
//
Device(SPMI)
{
   Name(_HID, "SPMI6225")
   Alias(\_SB.PSUB, _SUB)
   Name (_CID, "PNP0CA2")
   Name(_UID, One)
   Name(_CCA, 0)

   Method(_CRS, 0x0, NotSerialized)
   {
      Name(RBUF, ResourceTemplate ()
      {
         Memory32Fixed(ReadWrite, 0x01C00000, 0x02800000)
      })
      Return(RBUF)
   }

   Include("spmi_conf.asl")
}
