/**
  @file  EFITsens.h
  @brief TSENS Protocol for UEFI.
*/
/*=============================================================================
  Copyright (c) 2012, 2014, 2016 Qualcomm Technologies, Inc.
  All rights reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
=============================================================================*/

/*=============================================================================
                              EDIT HISTORY

  when       who     what, where, why
  --------   ---     -----------------------------------------------------------
  12/20/16   jjo     Clarified temperature units.
  11/06/14   jjo     Added max temp API.
  07/21/14   jjo     Updated for XBL.
  10/24/12   llg     (Tech Pubs) Edited/added Doxygen comments and markup.
  10/08/12   jjo     Created.
=============================================================================*/
#ifndef __EFITSENS_H__
#define __EFITSENS_H__

/*===========================================================================
  INCLUDE FILES
===========================================================================*/

/*===========================================================================
  MACRO DECLARATIONS
===========================================================================*/

/** @ingroup efi_tsens_constants
  Protocol version. */
#define EFI_TSENS_PROTOCOL_REVISION 0x0000000000020001

/* Protocol GUID definition */
/** @ingroup efi_tsens_protocol */
#define EFI_TSENS_PROTOCOL_GUID \
   { 0xE8D644E6, 0x47AE, 0xDA4E, { 0x9D, 0x3C, 0xDA, 0xB8, 0x24, 0x30, 0xEB, 0xF8 } }

/*===========================================================================
  EXTERNAL VARIABLES
===========================================================================*/
/** @cond */
/* External reference to the TSENS Protocol GUID. */
extern EFI_GUID gEfiTsensProtocolGuid;

/*===========================================================================
  TYPE DEFINITIONS
===========================================================================*/
/* Protocol declaration. */
typedef struct _EFI_TSENS_PROTOCOL EFI_TSENS_PROTOCOL;
/** @endcond */

/*===========================================================================
  FUNCTION DEFINITIONS
===========================================================================*/
/* EFI_TSENS_GET_NUM_SENSORS */
/** @ingroup efi_tsens_get_num_sensors
  @par Summary
  Gets the number of TSENS sensors.

  @param[out]  pnNumSensors  The number of temperature sensors supported.

  @return
  EFI_SUCCESS           -- Function completed successfully. \n
  EFI_INVALID_PARAMETER -- Parameter is invalid. \n
  EFI_UNSUPPORTED       -- Function is not supported in this context,
                           e.g., the device is not calibrated.
*/
typedef
EFI_STATUS
(EFIAPI *EFI_TSENS_GET_NUM_SENSORS)(
  OUT UINT32 *pnNumSensors
);

/* EFI_TSENS_GET_TEMP */
/** @ingroup efi_tsens_get_temp
  @par Summary
  Gets the temperature in tenths of a degree Celsius of the requested sensor. The number
  of sensors can be obtained by first calling GetNumSensors. The sensor index is
  zero-based from 0 to NumSensors - 1.

  @param[in]  uSensor  Temperature sensor index.
  @param[out]  pnTempTenthDegC  Temperature of the sensor in tenths of a degree Celsius.

  @return
  EFI_SUCCESS           -- Function completed successfully. \n
  EFI_INVALID_PARAMETER -- Parameter is invalid. \n
  EFI_TIMEOUT           -- Timeout occurred when reading the sensor. \n
  EFI_UNSUPPORTED       -- Function is not supported in this context,
                           e.g., the sensor is dead.
*/
typedef
EFI_STATUS
(EFIAPI *EFI_TSENS_GET_TEMP)(
  IN UINT32 uSensor,
  OUT INT32 *pnTempTenthDegC
);

/* EFI_TSENS_GET_MAX_TEMP */
/** @ingroup efi_tsens_get_max_temp
  @par Summary
  Gets the maximum temperature of all the sensors, in tenths of a degree Celsius, during
  the last measurement period.

  @param[out]  pnMaxTempTenthDegC  Maximum temperature in tenths of a degree Celsius.

  @return
  EFI_SUCCESS           -- Function completed successfully. \n
  EFI_INVALID_PARAMETER -- Parameter is invalid. \n
  EFI_TIMEOUT           -- Timeout occurred when reading the sensor. \n
  EFI_UNSUPPORTED       -- Function is not supported in this context,
                           e.g., the sensor is dead.
*/
typedef
EFI_STATUS
(EFIAPI *EFI_TSENS_GET_MAX_TEMP)(
  OUT INT32 *pnMaxTempTenthDegC
);

/*===========================================================================
  PROTOCOL INTERFACE
===========================================================================*/
/** @ingroup efi_tsens_protocol
  @par Summary
  Temperature Sensor Protocol interface.

  @par Parameters
  @inputprotoparams{tsens_proto_params.tex}
*/
struct _EFI_TSENS_PROTOCOL {
  UINT64 Revision;
  EFI_TSENS_GET_NUM_SENSORS GetNumSensors;
  EFI_TSENS_GET_TEMP GetTemp;
  EFI_TSENS_GET_MAX_TEMP GetMaxTemp;
};

#endif /* __EFITSENS_H__ */
