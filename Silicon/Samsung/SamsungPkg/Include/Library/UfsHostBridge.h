#ifndef __UFS_HOST_BRIDGE_H__
#define __UFS_HOST_BRIDGE_H__

#define ALIGNED_UPIU_SIZE 1024
#define UPIU_DATA_SIZE (ALIGNED_UPIU_SIZE - 20 - sizeof(struct UfsUpiuHeader))
#define DW_NUM_OF_TSF 20
#define SCSI_MAX_SG_SEGMENTS 128

#define UIC_ARG_MIB_SEL(attr, sel) ((((attr) & 0xFFFF) << 16) | ((sel) & 0xFFFF))
#define UIC_ARG_MIB(attr)           UIC_ARG_MIB_SEL(attr, 0)

typedef struct
{
  UINT8 Cdb[16];
  UINT8 *Buf;
  UINT32 DataLen;
  UINT8 Status;
  UINT8 Sense_buf[64];
  UINT32 Lun;
} ScsiCommandMeta;

struct UfsUpiuHeader
{
  UINT8 Type;
  UINT8 Flags;
  UINT8 Lun;
  UINT8 Tag;
  UINT8 CmdType;
  UINT8 Function;
  UINT8 Response;
  UINT8 Status;
  UINT8 EhsLength;
  UINT8 DeviceInfo;
  UINT16 DataLength;
};

struct UfsUpiu
{
  struct UfsUpiuHeader Header;
  UINT8 Tsf[DW_NUM_OF_TSF];
  UINT8 Data[UPIU_DATA_SIZE];
};

struct UfsPrdt
{
  UINT32 BaseAddr;
  UINT32 UpperAddr;
  UINT32 Reserved;
  UINT32 Size;
};

struct UfsCmdDesc
{
  struct UfsUpiu CommandUpiu;
  struct UfsUpiu ResponseUpiu;
  struct UfsPrdt PrdTable[SCSI_MAX_SG_SEGMENTS];
};

struct UfsUtrd
{
  UINT32 dw[4];
  UINT32 cmd_desc_addr_l;
  UINT32 cmd_desc_addr_h;
  UINT16 rsp_upiu_len;
  UINT16 rsp_upiu_off;
  UINT16 prdt_len;
  UINT16 prdt_off;
};

struct UfsUtmrd
{
  UINT32 dw[8];
};

struct UfsFlagBit
{
  UINT8 Reserved0;
  UINT8 fDeviceInit;
  UINT8 fPermanentWPEn;
  UINT8 fPowerOnWPEn;
  UINT8 fBackgroundOpsEn;
  UINT8 Reserved5;
  UINT8 fPurgeEnable;
  UINT8 Reserved7[25];
};

union UfsFlags
{
  UINT8 Array[32];
  struct UfsFlagBit flag;
};

union UfsAttributes
{
  UINT32 Array[18];
};

struct UfsConfigDescHeader
{
  UINT8 bLength;
  UINT8 bDescriptorType;
  UINT8 bConfDescContinue;
  UINT8 bBootEnable;
  UINT8 bDescrAccessEn;
  UINT8 bInitPowerMode;
  UINT8 bHighPriorityLUN;
  UINT8 bSecureRemovalType;
  UINT8 bInitActiveICCLevel;
  UINT16 wPeriodicRTCUpdate;
  UINT8 reserved[5];
  UINT8 bTurboWriteBufferNoUserSpaceReductionEn;
  UINT8 bTurboWriteBufferType;
};

struct UfsUnitDescParam
{
  UINT8 bLUEnable;
  UINT8 bBootLunID;
  UINT8 bLUWriteProtect;
  UINT8 bMemoryType;
  UINT32 dNumAllocUnits;
  UINT8 bDataReliability;
  UINT8 bLogicalBlockSize;
  UINT8 bProvisioningType;
  UINT16 wContextCapabilities;
  UINT8 Reserved[3];
  UINT8 Reserved1[6];
  UINT32 dLUNumTurboWriteBufferAllocUnits;
};

struct UfsConfigDesc
{
  struct UfsConfigDescHeader Header;
  struct UfsUnitDescParam Unit[128];
};

struct UfsDeviceDesc
{
  UINT8 bLength;
  UINT8 bDescriptorType;
  UINT8 bDevice;
  UINT8 bDeviceClass;
  UINT8 bDeviceSubClass;
  UINT8 bProtocol;
  UINT8 bNumberLU;
  UINT8 iNumberWLU;
  UINT8 bBootEnable;
  UINT8 bDescrAccessEn;
  UINT8 bInitPowerMode;
  UINT8 bHighPriorityLUN;
  UINT8 bSecureRemovalType;
  UINT8 bSecurityLU;
  UINT8 reserved;
  UINT8 bInitActiveICCLevel;
  UINT16 wSpecVersion;
  UINT16 wManufactureData;
  UINT8 iManufacturerName;
  UINT8 iProductName;
  UINT8 iSerialNumber;
  UINT8 iOemID;
  UINT16 wManufacturerID;
  UINT8 bUD0BaseOffset;
  UINT8 bUDConfigPlength;
  UINT8 bDeviceRTTCap;
  UINT16 wPeriodicRTCUpdate;
  UINT8 Reserved1[48];
  UINT32 dExtendedUFSFeaturesSupport;
  UINT8 Reserved2[4];
};

struct UfsGeometryDesc
{
  UINT8 bLength;
  UINT8 bDescriptorType;
  UINT8 bMediaTechnology;
  UINT8 Reserved03;
  UINT32 qTotalRawDeviceCapacity_h;
  UINT32 qTotalRawDeviceCapacity_l;
  UINT8 Reserved0c;
  UINT32 dSegmentSize;
  UINT8 bAllocationUnitSize;
  UINT8 bMinAddrBlockSize;
  UINT8 bOptimalReadBlockSize;
  UINT8 bOptimalWriteBlockSize;
  UINT8 bMaxInBufferSize;
  UINT8 bMaxOutBufferSize;
  UINT8 bRPMB_ReadWriteSize;
  UINT8 Reserved18;
  UINT8 bDataOrdering;
  UINT8 bMaxContexIDNumber;
  UINT8 bSysDataTagUnitSize;
  UINT8 bSysDataTagResSize;
  UINT8 bSupportedSecRTypes;
  UINT16 wSupportedMemoryTypes;
  UINT32 dSystemCodeMaxNAllocU;
  UINT16 wSystemCodeCapAdjFac;
  UINT32 dNonPersistMaxNAllocU;
  UINT16 wNonPersistCapAdjFac;
  UINT32 dEnhanced1MaxNAllocU;
  UINT16 wEnhanced1CapAdjFac;
  UINT32 dEnhanced2MaxNAllocU;
  UINT16 wEnhanced2CapAdjFac;
  UINT32 dEnhanced3MaxNAllocU;
  UINT16 wEnhanced3CapAdjFac;
  UINT32 dEnhanced4MaxNAllocU;
  UINT16 wEnhanced4CapAdjFac;
  UINT32 Reserved44;
  UINT8 Reserved[7];
  UINT32 dTurboWriteBufferMaxNAllocUnits;
  UINT8 Reserved1[2];
  UINT8 bSupportedTurboWriteBufferUserSpaceReductionTypes;
  UINT8 bSupportedTurboWriteBufferTypes;
  UINT8 Reserved2;
};

struct UfsUnitDesc
{
  UINT8 bLength;
  UINT8 bDescriptorType;
  UINT8 bUnitIndex;
  UINT8 bLUEnable;
  UINT8 bBootLunID;
  UINT8 bLUWriteProtect;
  UINT8 bLUQueueDepth;
  UINT8 Reserved;
  UINT8 bMemoryType;
  UINT8 bDataReliability;
  UINT8 bLogicalBlockSize;
  UINT32 qLogicalBlockCount_h;
  UINT32 qLogicalBlockCount_l;
  UINT32 dEraseBlockSize;
  UINT8 bProvisioningType;
  UINT32 qPhyMemResourceCount_h;
  UINT32 qPhyMemResourceCount_l;
  UINT16 wContextCapabilities;
  UINT8 bLargeUnitSize_M1;
};

/* UIC Commands */
enum
{
	UIC_CMD_DME_GET = 0x01,
	UIC_CMD_DME_SET = 0x02,
	UIC_CMD_DME_PEER_GET = 0x03,
	UIC_CMD_DME_PEER_SET = 0x04,
	UIC_CMD_DME_POWERON = 0x10,
	UIC_CMD_DME_POWEROFF = 0x11,
	UIC_CMD_DME_ENABLE = 0x12,
	UIC_CMD_DME_RESET = 0x14,
	UIC_CMD_DME_END_PT_RST = 0x15,
	UIC_CMD_DME_LINK_STARTUP = 0x16,
	UIC_CMD_DME_HIBER_ENTER = 0x17,
	UIC_CMD_DME_HIBER_EXIT = 0x18,
	UIC_CMD_DME_TEST_MODE = 0x1A,
	UIC_CMD_WAIT = 0x80,
	UIC_CMD_WAIT_ISR = 0x90,
	PHY_PMA_COMN_SET = 0xf0,
	PHY_PMA_TRSV_SET = 0xf1,
	PHY_PMA_COMN_WAIT = 0xf2,
	PHY_PMA_TRSV_WAIT = 0xf3,
	UIC_CMD_REGISTER_SET = 0xff,
};


struct UicPwrMode
{
  UINT8 Lane;
  UINT8 Gear;
  UINT8 Mode;
  UINT8 HsSeries;
};

struct UfsUicCmd
{
  UINT32 uiccmdr;
  UINT32 Arg1;
  UINT32 Arg2;
  UINT32 Arg3;
};

struct UfsHost
{
  VOID *IoAddr;
  VOID *VsAddr;
  VOID *UniProAddr;
  VOID *UfsPaddr;
  VOID *PhyPma;
  VOID *DevPwrAddr;
  VOID *PhyIsoAddr;
  
  ScsiCommandMeta *ScsiCmd;
  UINT32 Lun;
  
  struct UfsCmdDesc *CmdDescAddr;
  struct UfsUtrd *UtrdAddr;
  struct UfsUtmrd *UtmrdAddr;
  struct UfsUicCmd *UicCmd;
  
  UINT32 Quirks;
  
  UINT32 UfsCmdTimeout;
  UINT32 UicCmdTimeout;
  UINT32 Timeout;
  
  union UfsFlags Flags;
  union UfsAttributes Attributes;
  
  struct UfsConfigDesc ConfigDesc;
  struct UfsDeviceDesc DeviceDesc;
  struct UfsGeometryDesc GeometryDesc;
  struct UfsUnitDesc UnitDesc[8];
  UINT16 DataSegLen;
  
  struct UfsCalParam *CalParam;
  UINT32 MclkRate;
  struct UicPwrMode PmdCxt;
  UINT32 DevPwrShift;
  UINT32 GearMode;
};

INT32
UfsSendUicCmd (struct UfsHost *Ufs);

#endif /* __UFS_HOST_BRIDGE_H__ */
