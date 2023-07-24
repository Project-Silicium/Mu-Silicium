/* Copyright (c) 2015-2016, The Linux Foundation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * * Redistributions of source code must retain the above copyright
 *  notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided
 *  with the distribution.
 *   * Neither the name of The Linux Foundation nor the names of its
 * contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef __EFIPMICVERSION_H__
#define __EFIPMICVERSION_H__

/*===========================================================================
  MACRO DECLARATIONS
===========================================================================*/
/** @ingroup efi_pmicVersion_constants
  Protocol version.
*/
#define PMIC_VERSION_REVISION 0x0000000000010003
/** @} */ /* end_addtogroup efi_pmicVersion_constants */

/* Protocol GUID definition */
/** @ingroup efi_pmicVersion_protocol */
#define EFI_PMIC_VERSION_PROTOCOL_GUID                                         \
  {                                                                            \
    0x4684800a, 0x2755, 0x4edc,                                                \
    {                                                                          \
      0xb4, 0x43, 0x7f, 0x8c, 0xeb, 0x32, 0x39, 0xd3                           \
    }                                                                          \
  }

/** @cond */
/**
  External reference to the PMIC Version Protocol GUID.
*/
extern EFI_GUID gQcomPmicVersionProtocolGuid;

/*===========================================================================
  TYPE DEFINITIONS
===========================================================================*/
/**
  PMIC VERSION UEFI typedefs
*/
typedef struct _EFI_QCOM_PMIC_VERSION_PROTOCOL EFI_QCOM_PMIC_VERSION_PROTOCOL;
/** @endcond */

/** @addtogroup efi_pmicVersion_data_types
@{ */
/* Specifies which PMIC we are using */
/**
  PMIC model; version resource ID.
*/
typedef enum {
  EFI_PMIC_IS_UNKNOWN = 0,   /**<UNKNOWN */
  EFI_PMIC_IS_PM8941 = 1,    /**<PM8941  */
  EFI_PMIC_IS_PM8841 = 2,    /**<PM8841  */
  EFI_PMIC_IS_PM8019 = 3,    /**<PM8019  */
  EFI_PMIC_IS_PM8026 = 4,    /**<PM8026  */
  EFI_PMIC_IS_PM8110 = 5,    /**<PM8110  */
  EFI_PMIC_IS_PMA8084 = 6,   /**<PMA8084 */
  EFI_PMIC_IS_PMI8962 = 7,   /**<PMI8962 */
  EFI_PMIC_IS_PMD9635 = 8,   /**<PMD9635 */
  EFI_PMIC_IS_PM8994 = 9,    /**<PM8994  */
  EFI_PMIC_IS_PMI8994 = 0xA, /**<PMI8994 */
  EFI_PMIC_IS_PM8916 = 0xB,  /**<PM8916  */
  EFI_PMIC_IS_PM8004 = 0xC,  /**<PM8004  */
  EFI_PMIC_IS_PM8909 = 0xD,  /**<PM8909  */
  EFI_PMIC_IS_PM2433 = 0xE,  /**<PM2433  */
  EFI_PMIC_IS_PMD9655 = 0xF, /**<MDM9x55 */
  EFI_PMIC_IS_PMF2432 = EFI_PMIC_IS_PMA8084,
  EFI_PMIC_IS_PM8950 = 0x10,        /**<PM8950  */
  EFI_PMIC_IS_PMI8950 = 0x11,       /**<PMI8950 */
  EFI_PMIC_IS_PMI8937 = 0x11,       /**<PMI8937 */
  EFI_PMIC_IS_PMK8001 = 0x12,       /**<PMK8001 */
  EFI_PMIC_IS_PMI8996 = 0x13,       /**<PMI8996 */
  EFI_PMIC_IS_PM8998 = 0x14,        /**<PM8998  */
  EFI_PMIC_IS_PMI8998 = 0x15,       /**<PMI8998 */
  EFI_PMIC_IS_PM8953 = 0x16,        /**<PM8953  */
  EFI_PMIC_IS_PM8005 = 0x18,        /**<PM8005  */
  EFI_PMIC_IS_PM8937 = 0x19,        /**<PM8937  */
  EFI_PMIC_IS_PM660L = 0x1A,        /**<PM660L  */
  EFI_PMIC_IS_PM660 = 0x1B,         /**<PM660  */
  EFI_PMIC_IS_INVALID = 0x7FFFFFFF, /**<INVALID PMIC */
} EFI_PM_MODEL_TYPE;

/**
  PMIC device information.
*/
typedef struct {
  EFI_PM_MODEL_TYPE PmicModel;
  /**< Model type; see #EFI_PM_MODEL_TYPE for details. */
  UINT32 PmicAllLayerRevision;
  /**< All layer revision number. */
  UINT32 PmicMetalRevision;
  /**< Metal revision number. */
} EFI_PM_DEVICE_INFO_TYPE;
/** @} */ /* end_addtogroup efi_pmicVersion_data_types */

/**
  Extended PMIC device information with PMIC Variant info.
*/
typedef struct {
  EFI_PM_MODEL_TYPE PmicModel;
  /**< Model type; see #EFI_PM_MODEL_TYPE for details. */
  UINT32 PmicAllLayerRevision;
  /**< All layer revision number. */
  UINT32 PmicMetalRevision;
  /**< Metal revision number. */
  UINT32 PmicVariantRevision;
  /**< Variant revision number. */
} EFI_PM_DEVICE_INFO_EXT_TYPE;
/** @} */ /* end_addtogroup efi_pmicVersion_data_types */

/*===========================================================================
  FUNCTION DEFINITIONS
===========================================================================*/

/* EFI_PM_GET_PMIC_INFO */
/** @ingroup efi_pmicVersion_get_pmic_info
  @par Summary
  Retrieves information about the PMIC device for a specific device index.

  @param[in]  PmicDeviceIndex  Primary: 0. Secondary: 1.
  @param[out] PmicDeviceInfo   Variable with PMIC device information to return
                               to the caller; see #EFI_PM_DEVICE_INFO_TYPE for
                               details.

  @return
  EFI_SUCCESS        -- Function completed successfully. \n
  EFI_PROTOCOL_ERROR -- Error occurred during the operation.
*/
typedef EFI_STATUS (EFIAPI *EFI_PM_GET_PMIC_INFO) (
    IN UINT32 PmicDeviceIndex,
    OUT EFI_PM_DEVICE_INFO_TYPE *PmicDeviceInfo);

/* EFI_PM_GET_PMIC_INFO_EXT */
/** @ingroup efi_pmicVersion_get_pmic_info
  @par Summary
  Retrieves information about the PMIC device for a specific device index.

  @param[in]  PmicDeviceIndex  Primary: 0. Secondary: 1.
  @param[out] PmicDeviceInfo   Variable with PMIC device information to return
                               to the caller; see #EFI_PM_DEVICE_INFO_EXT_TYPE
  for
                               details.

  @return
  EFI_SUCCESS        -- Function completed successfully. \n
  EFI_PROTOCOL_ERROR -- Error occurred during the operation.
*/
typedef EFI_STATUS (EFIAPI *EFI_PM_GET_PMIC_INFO_EXT) (
    IN UINT32 PmicDeviceIndex,
    OUT EFI_PM_DEVICE_INFO_EXT_TYPE *PmicDeviceInfo);

/*===========================================================================
  PROTOCOL INTERFACE
===========================================================================*/
/** @ingroup efi_pmicVersion_protocol
  @par Summary
  Qualcomm PMIC Version Protocol interface.

  @par Parameters
  @inputprotoparams{pmic_version_proto_params.tex}
*/
struct _EFI_QCOM_PMIC_VERSION_PROTOCOL {
  UINT64 Revision;
  EFI_PM_GET_PMIC_INFO GetPmicInfo;
  EFI_PM_GET_PMIC_INFO_EXT GetPmicInfoExt;
};

#endif /* __EFIPMICVERSION_H__ */
