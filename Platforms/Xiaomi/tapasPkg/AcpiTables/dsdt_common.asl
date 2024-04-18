// To enable SOC revision based run time differentiation, uncomment following line
// and uncomment SSID method in ABD device. The original string is artificailly set as 
// 16 characters, so there is enough room to hold SOC revision string. 
// To adjust the number, the MAX_SOCID_LEN macro as defined in ABD device.h should be
// adjusted at the same time. 

Name (SOID, 0xffffffff)          // Holds the Chip Id
Name (SIDS, "SM6225")            // Holds the Chip ID translated to a string
Name (SIDV, 0xffffffff)          // Holds the Chip Version as (major<<16)|(minor&0xffff)
Name (SVMJ, 0xffff)              // Holds the major Chip Version
Name (SVMI, 0xffff)              // Holds the minor Chip Version
Name (SDFE, 0xffff)              // Holds the Chip Family enum
Name (SFES, "899800000000000")   // Holds the Chip Family translated to a string
Name (SIDM, 0xfffffffff)         // Holds the Modem Support bit field
Name (SOSN, 0xaaaaaaaabbbbbbbb)  // Holds the Chip Serial Number
Name (PLST, 0xffffffff)  		 // Holds the Device platform subtype
Name (RMTB, 0xaaaaaaaa)
Name (RMTX, 0xbbbbbbbb)
Name (RFMB, 0xcccccccc)
Name (RFMS, 0xdddddddd)
Name (RFAB, 0xeeeeeeee)
Name (RFAS, 0x77777777)
Name (TPMA, 0x11111111)         // Holds whether TPM is seperate app or combined with Winsecapp
Name (TDTV, 0x6654504D)         // Holds TPM type, by default it set to fTPM type
Name (TCMA, 0xDEADBEEF)         // Holds TrEE Carveout Memory Address
Name (TCML, 0xBEEFDEAD)         // Holds TrEE Carveout Memory Length
// Name (SOSI, 0xdeadbeefffffffff) // Holds the base address of the SoCInfo shared memory region used by ChipInfoLib
		
        //
        // Storage - UFS/SD 
        //
        Include("ufs.asl")
        //Include("sdc.asl") 

        //
        //
        //
        Include("pmic_core.asl")

		//
		// SPMI driver 
        //
        Include("spmi.asl")

        //
        // Clusters & cores
        //
        Include("pep_lpi.dsl")

        //
        // USB support
        //
        Include("usb.dsl")

