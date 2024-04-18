//
// NOTE: The 3rd parameter (i.e. ComplianceRevision) must be >=2 for 64-bit integer support.
//
DefinitionBlock("DSDT.AML", "DSDT", 0x02, "QCOMM ", "SM6225 ", 3)
{
    Scope(\_SB_) {

        Include("addSub.asl")
        
        Include("dsdt_common.asl")

   }

}
