//
// TLMM controller.
//
Device (GIO0)
{
	Name (_HID, "QCOM050D")
	Alias(\_SB.PSUB, _SUB)
	Name (_UID, 0)

	Method (_CRS, 0x0, NotSerialized) {
		Name (RBUF, ResourceTemplate ()
		{
		   // TLMM register address space
			Memory32Fixed (ReadWrite, 0x00400000, 0x00C00000)

			// Summary Interrupt shared by all banks
			Interrupt(ResourceConsumer, Level, ActiveHigh, Shared, , , ) {259}
			Interrupt(ResourceConsumer, Level, ActiveHigh, Shared, , , ) {259}
			Interrupt(ResourceConsumer, Level, ActiveHigh, Shared, , , ) {259}
		})
		Return (RBUF)
	}
	// ACPI method to return Num pins
	Method(OFNI, 0x0, NotSerialized) {
		Name(RBUF, Buffer()
		{
			0x86,  // 0: TOTAL_GPIO_PINS
			0x00   // 1: TOTAL_GPIO_PINS
		})
		Return (RBUF)
	}
	
	Name(GABL, Zero)
	Method(_REG, 0x2, NotSerialized)
	{
		If(LEqual(Arg0, 0x8))
		{
			Store(Arg1, GABL)
		}
	} 
}
