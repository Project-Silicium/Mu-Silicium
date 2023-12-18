/**
@file  EFIUsbPwrCtrl.h
@brief Platform Usb Power Control Protocol for UEFI.
*/
/*=============================================================================
Copyright (c) 2017 Qualcomm Technologies, Incorporated.
All rights reserved.
Qualcomm Technologies, Confidential and Proprietary.
=============================================================================*/

/*=============================================================================
EDIT HISTORY


when       who     what, where, why
--------   ---     -----------------------------------------------------------
05/23/17   dc      Initial Release
=============================================================================*/

#ifndef __EFIUSBPWRCTRL_H__
#define __EFIUSBPWRCTRL_H__

#define MAX_USB_PORT_NUMBER 2

/**
  Protocol version.
*/
#define USB_PWR_CTRL_REVISION 0x0000000000010001
/** @} */ /* end_addtogroup efi_pmicfg_constants */

/*  Protocol GUID definition */
/** @ingroup efi_qcomcharger_protocol */
#define EFI_USB_PWR_CTRL_PROTOCOL_GUID \
  { 0xe07df17e, 0xe79e, 0x4150, { 0x93, 0x78, 0x50, 0x62, 0x3a, 0x14, 0x99, 0x4a } }

/*===========================================================================
EXTERNAL VARIABLES
===========================================================================*/
/**
External reference to the USB Power Control Protocol GUID.
*/

extern EFI_GUID gQcomUsbPwrCtrlProtocolGuid;


/*===========================================================================
TYPE DEFINITIONS
===========================================================================*/
/**
Protocol declaration.
*/
typedef struct _EFI_USB_PWR_CTRL_PROTOCOL  EFI_USB_PWR_CTRL_PROTOCOL;

/*
Data Sturcuture for HW information and capability
*/
typedef enum _EFI_USB_PWR_CTRL_PORTHW_CONNECTOR_TYPE
{
  EFI_USB_PWR_CTRL_PORTHW_CONNECTOR_NONE,
  EFI_USB_PWR_CTRL_PORTHW_CONNECTOR_uUSB,
  EFI_USB_PWR_CTRL_PORTHW_CONNECTOR_TYPEC,
  EFI_USB_PWR_CTRL_PORTHW_CONNECTOR_TYPEA,
  EFI_USB_PWR_CTRL_PORTHW_CONNECTOR_INVALID
}EFI_USB_PWR_CTRL_PORTHW_CONNECTOR_TYPE;

typedef enum _EFI_USB_PWR_CTRL_PORTHW_BUS_TYPE
{
  EFI_USB_PWR_CTRL_PORTHW_BUS_NONE,                   /*HW resource not accessible, not supported or controled by HW*/
  EFI_USB_PWR_CTRL_PORTHW_BUS_INTERNAL,               /*HW resource access through MSM directly*/
  EFI_USB_PWR_CTRL_PORTHW_BUS_SPMI,                   /*HW resource access through SPMI bus*/
  EFI_USB_PWR_CTRL_PORTHW_BUS_I2C,                    /*HW resource access through I2C*/
  EFI_USB_PWR_CTRL_PORTHW_BUS_INVALID,
}EFI_USB_PWR_CTRL_PORTHW_BUS_TYPE;

typedef enum _EFI_USB_PWR_CTRL_PORTHW_HW_TYPE
{
  EFI_USB_PWR_CTRL_PORTHW_HW_NONE,
  EFI_USB_PWR_CTRL_PORTHW_HW_AUTO,
  EFI_USB_PWR_CTRL_PORTHW_HW_PMSCHG,
  EFI_USB_PWR_CTRL_PORTHW_HW_PMGPIO,
  EFI_USB_PWR_CTRL_PORTHW_HW_ROHM,
  EFI_USB_PWR_CTRL_PORTHW_HW_ANX74XX,
  EFI_USB_PWR_CTRL_PORTHW_HW_INVALID,
}EFI_USB_PWR_CTRL_PORTHW_HW_TYPE;

typedef enum _EFI_USB_PWR_CTRL_TYPEC_ROLE
{
  EFI_USB_PWR_CTRL_TYPEC_ROLE_NONE,
  EFI_USB_PWR_CTRL_TYPEC_ROLE_DRP,
  EFI_USB_PWR_CTRL_TYPEC_ROLE_UFP,
  EFI_USB_PWR_CTRL_TYPEC_ROLE_DFP,
  EFI_USB_PWR_CTRL_TYPEC_ROLE_INVALID    
}EFI_USB_PWR_CTRL_TYPEC_ROLE;

typedef struct _EFI_USB_PWR_CTRL_PORTHW_PWR_CAP
{
  UINT32  MaxCurrentMA;
  UINT32  MaxVoltageMV;
}EFI_USB_PWR_CTRL_PORTHW_PWR_CAP;

///////////////////////////////////////////////////////////////////////////////

//General HW information
typedef struct _EFI_USB_PWR_CTRL_PORTHW_HW_INFO
{
  UINT32  ResourceIndex;
  UINT32  ResourceIDorAddr;
}EFI_USB_PWR_CTRL_PORTHW_HW_INFO;


typedef struct _EFI_USB_PWR_CTRL_PORTHW_GENERAL_HW_INFO
{
  EFI_USB_PWR_CTRL_PORTHW_HW_TYPE                     HwType;
  EFI_USB_PWR_CTRL_PORTHW_HW_INFO                     HwInfo;
}EFI_USB_PWR_CTRL_PORTHW_GENERAL_HW_INFO;

//USB3 Hardware Inforamtion
typedef struct _EFI_USB_PWR_CTRL_PORTHW_USB3_HW_INFO
{
  BOOLEAN                                             bSSSupported;       /**< Flag indicate if SuperSpeed USB (USB3.0) supported */    
}EFI_USB_PWR_CTRL_PORTHW_USB3_HW_INFO;

//TypeC Hardware information
typedef struct _EFI_USB_PWR_CTRL_PORTHW_TYPEC_INFO
{
  EFI_USB_PWR_CTRL_PORTHW_HW_TYPE                     HwType;
  EFI_USB_PWR_CTRL_PORTHW_HW_INFO                     HwInfo;        
  BOOLEAN                                             bPDSupported;
}EFI_USB_PWR_CTRL_PORTHW_TYPEC_INFO;

//Power Source or Sink Hardware information
typedef struct _EFI_USB_PWR_CTRL_PORTHW_PWR_SRC_SNK_INFO
{
  EFI_USB_PWR_CTRL_PORTHW_HW_TYPE                     HwType;
  EFI_USB_PWR_CTRL_PORTHW_HW_INFO                     HwInfo;
  EFI_USB_PWR_CTRL_PORTHW_PWR_CAP                     PwrCapability;
}EFI_USB_PWR_CTRL_PORTHW_PWR_SRC_SNK_INFO;

typedef struct _EFI_USB_PWR_CTRL_PORTHW_INFO
{
  EFI_USB_PWR_CTRL_PORTHW_CONNECTOR_TYPE              ConnectorType;      /**< Describe connector type for this port */
  /*USB Hardware*/        
  EFI_USB_PWR_CTRL_PORTHW_GENERAL_HW_INFO             Usb2ChgDetect;      /**< Describe How USB2.0 is supported for this port */
  EFI_USB_PWR_CTRL_PORTHW_USB3_HW_INFO                Usb3;               /**< Describe How USB3.0 is supported for this port */            
  /*TYPEC & PD Hardware*/     
  EFI_USB_PWR_CTRL_PORTHW_TYPEC_INFO                  TypeC;              /**< Describe How Type-C Port is supported for this port */
  /*Power Control Hardware*/      
  EFI_USB_PWR_CTRL_PORTHW_GENERAL_HW_INFO             VbusCtrl;           /**< Describe how Vbus Control is supported for this port */
  EFI_USB_PWR_CTRL_PORTHW_GENERAL_HW_INFO             VconnCtrl;          /**< Describe how Vconn Control is supported for this port */
  /*Power Detect Hardware*/
  EFI_USB_PWR_CTRL_PORTHW_GENERAL_HW_INFO             VbusDetect;         /**< Describe how Vbus Detection is supported for this port */
  EFI_USB_PWR_CTRL_PORTHW_GENERAL_HW_INFO             VbusSrcOkDetect;    /**< Describe how Vbus SourceOK Detection is done for this port */
  EFI_USB_PWR_CTRL_PORTHW_GENERAL_HW_INFO             UsbIdDetect;        /**< Describe how USB ID detection is supported for this port */
  /* Power Source and Sink Hardware*/             
  EFI_USB_PWR_CTRL_PORTHW_PWR_SRC_SNK_INFO            PowerSource;        /**< Describe How Power Source is supported for this port */
  EFI_USB_PWR_CTRL_PORTHW_PWR_SRC_SNK_INFO            PowerSink;          /**< Describe How Power Sink is supported for this port */
  /*Power Error Detect Hardware*/    
  //EFI_USB_PWR_CTRL_PORTHW_GENERAL_HW_INFO             VbusOV;             /**< Describe how Vbus Over-Voltage detection is supported for this port */
  //EFI_USB_PWR_CTRL_PORTHW_GENERAL_HW_INFO             VbusOC;             /**< Describe how Vbus Over-Current detection is supported for this port */
  //EFI_USB_PWR_CTRL_PORTHW_GENERAL_HW_INFO             VconnOC;            /**< Describe how Vconn Over-Current detection is supported for this port */    
}EFI_USB_PWR_CTRL_PORTHW_INFO;

typedef struct _EFI_USB_PWR_CTRL_HW_INFO
{
  UINT8                                               NoOfPorts;                      /**< No of USB port in this platform */
  EFI_USB_PWR_CTRL_PORTHW_INFO                        Ports[MAX_USB_PORT_NUMBER];     /**< Data Array contains port information */
}EFI_USB_PWR_CTRL_HW_INFO;


/*
Data Sturcuture for HW status
*/
typedef enum _EFI_USB_PWR_CTRL_HSUSB_CHGPORT_TYPE
{
  EFI_USB_PWR_CTRL_HSUSB_CHGPORT_UNKNOWN,
  EFI_USB_PWR_CTRL_HSUSB_CHGPORT_SDP,               /**< Standard Downstream Port */
  EFI_USB_PWR_CTRL_HSUSB_CHGPORT_OCP,               /**< Other Charging Port      */
  EFI_USB_PWR_CTRL_HSUSB_CHGPORT_CDP,               /**< Charging Downstream Port */
  EFI_USB_PWR_CTRL_HSUSB_CHGPORT_DCP,               /**< Dedicated Charging Port  */
  EFI_USB_PWR_CTRL_HSUSB_CHGPORT_FLOAT_CHARGER,
  EFI_USB_PWR_CTRL_HSUSB_CHGPORT_QC_2P0,
  EFI_USB_PWR_CTRL_HSUSB_CHGPORT_QC_3P0,
  EFI_USB_PWR_CTRL_HSUSB_CHGPORT_INVALID = 0xFF,
}EFI_USB_PWR_CTRL_HSUSB_CHGPORT_TYPE;

typedef enum _EFI_USB_PWR_CTRL_TYPEC_POWERROLE_TYPE
{
  EFI_USB_PWR_CTRL_TYPEC_POWERROLE_UNKNOWN,
  EFI_USB_PWR_CTRL_TYPEC_POWERROLE_SINK,
  EFI_USB_PWR_CTRL_TYPEC_POWERROLE_SOURCE,
  EFI_USB_PWR_CTRL_TYPEC_POWERROLE_INVALID,
}EFI_USB_PWR_CTRL_TYPEC_POWERROLE_TYPE;

typedef enum _EFI_USB_PWR_CTRL_TYPEC_DATAROLE_TYPE
{
  EFI_USB_PWR_CTRL_TYPEC_DATAROLE_UNKNOWN,
  EFI_USB_PWR_CTRL_TYPEC_DATAROLE_UFP,
  EFI_USB_PWR_CTRL_TYPEC_DATAROLE_DFP,
  EFI_USB_PWR_CTRL_TYPEC_DATAROLE_INVALID
}EFI_USB_PWR_CTRL_TYPEC_DATAROLE_TYPE;


typedef enum _EFI_USB_PWR_CTRL_TYPEC_ORIENTATION_TYPE
{
    EFI_USB_PWR_CTRL_TYPEC_ORIENTATION_OPEN,
    EFI_USB_PWR_CTRL_TYPEC_ORIENTATION_CC1,
    EFI_USB_PWR_CTRL_TYPEC_ORIENTATION_CC2,
    EFI_USB_PWR_CTRL_TYPEC_ORIENTATION_UNORIENTED,
    EFI_USB_PWR_CTRL_TYPEC_ORIENTATION_INVALID,
}EFI_USB_PWR_CTRL_TYPEC_ORIENTATION_TYPE;

typedef enum _EFI_USB_PWR_CTRL_TYPEC_PORT_SNK_TYPE
{
    EFI_USB_PWR_CTRL_TYPEC_PORT_SNK_NONE,
    EFI_USB_PWR_CTRL_TYPEC_PORT_SNK_DEFAULT,
    EFI_USB_PWR_CTRL_TYPEC_PORT_SNK_1P5,
    EFI_USB_PWR_CTRL_TYPEC_PORT_SNK_3P0,
    EFI_USB_PWR_CTRL_TYPEC_PORT_SNK_DEBUG,
    EFI_USB_PWR_CTRL_TYPEC_PORT_SNK_INVALID
}EFI_USB_PWR_CTRL_TYPEC_PORT_SNK_TYPE;

typedef enum _EFI_USB_PWR_CTRL_TYPEC_PORT_SRC_TYPE
{
    EFI_USB_PWR_CTRL_TYPEC_PORT_SRC_NONE,
    EFI_USB_PWR_CTRL_TYPEC_PORT_SRC_UNORIENTED_RD_RD,
    EFI_USB_PWR_CTRL_TYPEC_PORT_SRC_RD_OPEN,
    EFI_USB_PWR_CTRL_TYPEC_PORT_SRC_RD_RA_VCONN,
    EFI_USB_PWR_CTRL_TYPEC_PORT_SRC_RA_OPEN,
    EFI_USB_PWR_CTRL_TYPEC_PORT_SRC_RA_RA,
    EFI_USB_PWR_CTRL_TYPEC_PORT_SRC_INVALID,
}EFI_USB_PWR_CTRL_TYPEC_PORT_SRC_TYPE;

typedef enum _EFI_USB_PWR_CTRL_TYPEC_CC_TYPE
{
    EFI_USB_PWR_CTRL_TYPEC_CC_OPEN,
    EFI_USB_PWR_CTRL_TYPEC_CC_RA,
    EFI_USB_PWR_CTRL_TYPEC_CC_RD,
    EFI_USB_PWR_CTRL_TYPEC_CC_RP,
    EFI_USB_PWR_CTRL_TYPEC_CC_INVALID,
}EFI_USB_PWR_CTRL_TYPEC_CC_TYPE;

typedef struct _EFI_USB_PWR_CTRL_TYPEC_CC_STATUS_TYPE
{
    EFI_USB_PWR_CTRL_TYPEC_CC_TYPE     CC1;
    EFI_USB_PWR_CTRL_TYPEC_CC_TYPE     CC2;
}EFI_USB_PWR_CTRL_TYPEC_CC_STATUS;

typedef struct _EFI_USB_PWR_CTRL_TYPEC_PORT_STATUS
{
  BOOLEAN                                                 ConnectSts;
  EFI_USB_PWR_CTRL_TYPEC_POWERROLE_TYPE                   PowerRole;              /* Power Sink or Power Source */
  EFI_USB_PWR_CTRL_TYPEC_DATAROLE_TYPE                    DataRole;               /* DFP (Host) or UFP (Device)  */
  EFI_USB_PWR_CTRL_TYPEC_PORT_SNK_TYPE                    DetectedSinkType;       /* Detected Sink Port Type */
  EFI_USB_PWR_CTRL_TYPEC_PORT_SRC_TYPE                    DetectedSourceType;     /* Detected Source Port Type */ 
  EFI_USB_PWR_CTRL_TYPEC_ORIENTATION_TYPE                 CCOrientation;
}EFI_USB_PWR_CTRL_TYPEC_PORT_STATUS;

typedef struct _EFI_USB_PWR_CTRL_PD_STATUS
{
  BOOLEAN                                                 bPDConsumer;            /* Indicate if a PD Contract as Consumer is established */
  EFI_USB_PWR_CTRL_PORTHW_PWR_CAP                         PDConsumerState;
}EFI_USB_PWR_CTRL_PD_STATUS;

/*
Data Sturcuture for HW Configuration
*/
typedef struct _EFI_USB_PWR_CTRL_INOUT_POWER_CONFIG
{
  UINT32                                                  MaxCurrentMA;
}EFI_USB_PWR_CTRL_INOUT_POWER_CONFIG;

/*===========================================================================
  FUNCTION DEFINITIONS
===========================================================================*/
/* EFI_USB_PWR_CTRL_GET_HWINFO */
/** @ingroup efi_usbpwrctrlGetHwInfo
* @par Summary
* Return Hardware Information
*
* @param[out] pHwInfo:      Return Hardware Information
*
*@return 
* EFI_SUCCESS           -- Function completed successfully. \n
* EFI_INVALID_PARAMETER -- Parameter is invalid. \n
* EFI_DEVICE_ERROR      -- Physical device reported an error. \n
* EFI_NOT_READY         -- Physical device is busy or not ready to
*                          process this request.
*/
typedef
EFI_STATUS(EFIAPI *EFI_USB_PWR_CTRL_GET_HWINFO)( 
  OUT EFI_USB_PWR_CTRL_HW_INFO *pHwInfo
);

/* EFI_USB_PWR_CTRL_GET_POWER_STATUS */
/** @ingroup efi_usbpwrctrlGetPowerStatus
* @par Summary
* Return Power Status, such as Vbus Detection result or Vbus Power SourceOK result
*
* @param[IN]  PortIndex     Specify which port the request is for
* @param[out] bPowerSt:     Return Requested Power Status
*
*@return 
* EFI_SUCCESS           -- Function completed successfully. \n
* EFI_INVALID_PARAMETER -- Parameter is invalid. \n
* EFI_DEVICE_ERROR      -- Physical device reported an error. \n
* EFI_NOT_READY         -- Physical device is busy or not ready to
*                          process this request.
*/
typedef
EFI_STATUS(EFIAPI *EFI_USB_PWR_CTRL_GET_POWER_STATUS)(
  IN  UINT8   PortIndex,
  OUT BOOLEAN *bPowerSt
);

/* EFI_USB_PWR_CTRL_GET_HSUSB_CHGPORT_TYPE */
/** @ingroup efi_usbpwrctrlGetHsUsbChgPortType
* @par Summary
* Return BC1.2 Detection result
*
* @param[IN]  PortIndex     Specify which port the request is for
* @param[out] ChgPortType:  Return BC1.2 Detection result
*
*@return 
* EFI_SUCCESS           -- Function completed successfully. \n
* EFI_INVALID_PARAMETER -- Parameter is invalid. \n
* EFI_DEVICE_ERROR      -- Physical device reported an error. \n
* EFI_NOT_READY         -- Physical device is busy or not ready to
*                          process this request.
*/
typedef
EFI_STATUS(EFIAPI *EFI_USB_PWR_CTRL_GET_HSUSB_CHGPORT_TYPE)(
  IN  UINT8   PortIndex,
  OUT EFI_USB_PWR_CTRL_HSUSB_CHGPORT_TYPE *ChgPortType
);

/* EFI_USB_PWR_CTRL_GET_TYPEC_PORT_STATUS */
/** @ingroup efi_usbpwrctrlGetTypeCPortStatus
* @par Summary
* Return Type-C Port status 
*
* @param[IN]  PortIndex     Specify which port the request is for
* @param[out] TypeCPortStatus  Type-C Port Status
*
*@return 
* EFI_SUCCESS           -- Function completed successfully. \n
* EFI_INVALID_PARAMETER -- Parameter is invalid. \n
* EFI_DEVICE_ERROR      -- Physical device reported an error. \n
* EFI_NOT_READY         -- Physical device is busy or not ready to
*                          process this request.
*/
typedef
EFI_STATUS(EFIAPI *EFI_USB_PWR_CTRL_GET_TYPEC_PORT_STATUS)(
  IN  UINT8   PortIndex,
  OUT EFI_USB_PWR_CTRL_TYPEC_PORT_STATUS *TypeCPortStatus
);

/* EFI_USB_PWR_CTRL_SET_TYPEC_PD_STATUS */
/** @ingroup efi_usbpwrctrlGetTypeCPDStatus
* @par Summary
* Return Type-C Port Power Delivery status 
*
* @param[IN]  PortIndex     Specify which port the request is for
* @param[out] PDStatus      USB Power Delivery Status
*
*@return 
* EFI_SUCCESS           -- Function completed successfully. \n
* EFI_INVALID_PARAMETER -- Parameter is invalid. \n
* EFI_DEVICE_ERROR      -- Physical device reported an error. \n
* EFI_NOT_READY         -- Physical device is busy or not ready to
*                          process this request.
*/
typedef
EFI_STATUS(EFIAPI *EFI_USB_PWR_CTRL_GET_TYPEC_PD_STATUS)(
  IN  UINT8   PortIndex,
  OUT EFI_USB_PWR_CTRL_PD_STATUS  *PDStatus
);

/* EFI_USB_PWR_CTRL_SET_INOUT_POWER */
/** @ingroup efi_usbpwrctrlSetInOutPower
* @par Summary
* Configure Input or Output Power
*
* @param[IN]  PortIndex     Specify which port the request is for
* @param[out] PowerCfg      Power Configuration to apply
*
*@return 
* EFI_SUCCESS           -- Function completed successfully. \n
* EFI_INVALID_PARAMETER -- Parameter is invalid. \n
* EFI_DEVICE_ERROR      -- Physical device reported an error. \n
* EFI_NOT_READY         -- Physical device is busy or not ready to
*                          process this request.
*/
typedef
EFI_STATUS(EFIAPI *EFI_USB_PWR_CTRL_SET_INOUT_POWER)(
  IN  UINT8   PortIndex,
  IN  EFI_USB_PWR_CTRL_INOUT_POWER_CONFIG  PowerCfg
);

/* EFI_USB_PWR_CTRL_SET_POWER_SOURCE */
/** @ingroup efi_usbpwrctrlSetPowerSource
* @par Summary
* Enable/Disable Power source, such as Vbus, Vconn
*
* @param[IN]  PortIndex     Specify which port the request is for
* @param[IN]  bPowerEn      Power Configuration to apply
*
*@return 
* EFI_SUCCESS           -- Function completed successfully. \n
* EFI_INVALID_PARAMETER -- Parameter is invalid. \n
* EFI_DEVICE_ERROR      -- Physical device reported an error. \n
* EFI_NOT_READY         -- Physical device is busy or not ready to
*                          process this request.
*/
typedef
EFI_STATUS(EFIAPI *EFI_USB_PWR_CTRL_SET_POWER_SOURCE)(
  IN  UINT8   PortIndex,
  IN  BOOLEAN  bPowerEn
);

/*===========================================================================
PROTOCOL INTERFACE
===========================================================================*/
/** @ingroup efi_PlatformUsbPwrCtrl_protocol
@par Summary
USB Power control Protocol interface.

@par Parameters
@inputprotoparams{usbpwrctrl_proto_params.tex}
*/
struct _EFI_USB_PWR_CTRL_PROTOCOL
{
    UINT64                                      Revision;
    EFI_USB_PWR_CTRL_GET_HWINFO                 GetHwInfo;              /* Return Hardware Info*/
    EFI_USB_PWR_CTRL_GET_POWER_STATUS           GetVbusDetectStatus;    /* Return Vbus Detect result */
    EFI_USB_PWR_CTRL_GET_POWER_STATUS           GetVbusSrcOkStatus;     /* Return Vbus SrcOK result */
    EFI_USB_PWR_CTRL_GET_HSUSB_CHGPORT_TYPE     GetHsUsbChgPortType;    /* BC1.2 Detection Result */
    EFI_USB_PWR_CTRL_GET_POWER_STATUS           GetUsbIDStatus;         /* Return USBID result */
    EFI_USB_PWR_CTRL_GET_TYPEC_PORT_STATUS      GetTypeCPortStatus;     /* Return TypeC Port Type, Orientation */
    EFI_USB_PWR_CTRL_GET_TYPEC_PD_STATUS        GetTypeCPortPDStatus;   /* Return PD Status */
    EFI_USB_PWR_CTRL_SET_INOUT_POWER            SetSinkPower;           /* Configure Sink (Input) power */
    EFI_USB_PWR_CTRL_SET_INOUT_POWER            SetSourcePower;         /* Configure Source (Output) power */
    EFI_USB_PWR_CTRL_SET_POWER_SOURCE           SetVbusSourceEn;        /* Enable/Disable Power Source Vbus Source*/
    EFI_USB_PWR_CTRL_SET_POWER_SOURCE           SetVconnEn;             /* Enable/Disable Power Source Vconn */
};

#endif /* __EFIUSBPWRCTRL_H__ */
