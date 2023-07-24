/* Copyright (c) 2018-2020, The Linux Foundation. All rights reserved.
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

#ifndef __EFIDDRGETINFO_H__
#define __EFIDDRGETINFO_H__

#include "DDRDetails.h"

/** @cond */
typedef struct _EFI_DDRGETINFO_PROTOCOL EFI_DDRGETINFO_PROTOCOL;
/** @endcond */

/** @addtogroup efi_ddrgetinfo_constants
@{ */
/**
  Protocol version.
*/
#define EFI_DDRGETINFO_PROTOCOL_REVISION 0x0000000000040000

/*Both the protocl version and the structure version should be same */
#if (EFI_DDRGETINFO_PROTOCOL_REVISION != DDR_DETAILS_STRUCT_VERSION)
ASSERT (FALSE);
#endif

/** @} */ /* end_addtogroup efi_ddrgetinfo_constants */

/*  Protocol GUID definition */
/** @ingroup efi_ddrgetinfo_protocol */
#define EFI_DDRGETINFO_PROTOCOL_GUID                                           \
  {                                                                            \
    0x1a7c0eb8, 0x5646, 0x45f7,                                                \
    {                                                                          \
      0xab, 0x20, 0xea, 0xe5, 0xda, 0x46, 0x40, 0xa2                           \
    }                                                                          \
  }

/** @cond */
/**
  External reference to the DDRGetInfo Protocol GUID defined
  in the .dec file.
*/
extern EFI_GUID gEfiDDRGetInfoProtocolGuid;
/** @endcond */

/** @} */ /* end_addtogroup efi_ddrgetinfo_data_types */

/*==============================================================================

                             API IMPLEMENTATION

==============================================================================*/

/* ============================================================================
**  Function : EFI_DDRGETINFO_GETDDRDETAILS
** ============================================================================
*/
/** @ingroup efi_ddrgetinfo_GETDDRDETAILS
  @par Summary
  Gets the DDR Details

  @param[in]   This            Pointer to the EFI_DDRGETINFO_PROTOCOL instance.
  @param[out]  DetailsEntry    Pointer to DDR Details

  @return
  EFI_SUCCESS        -- Function completed successfully. \n
  EFI_PROTOCOL_ERROR -- Error occurred during the operation.
*/
typedef EFI_STATUS (EFIAPI *EFI_DDRGETINFO_GETDDRDETAILS) (
    IN EFI_DDRGETINFO_PROTOCOL *This,
    OUT struct ddr_details_entry_info *DetailsEntry);

/* ============================================================================
**  Function : EFI_DDRGetInfo_GetDDRFreqTable
** ============================================================================
*/
/** @ingroup EFI_DDRGetInfo_GetDDRFreqTable
  @par Summary
  Gets the DDR Clock plan table

  @param[in]   This            Pointer to the EFI_DDRGETINFO_PROTOCOL instance.
  @param[out]  ClkPlanTable    Pointer to DDR clock plan Table

  @return
  EFI_SUCCESS        -- Function completed successfully. \n
  EFI_PROTOCOL_ERROR -- Error occurred during the operation.
*/
typedef
EFI_STATUS
(EFIAPI *EFI_DDRGETINFO_GETDDRFREQTABLE)(
   IN EFI_DDRGETINFO_PROTOCOL *This,
   OUT struct ddr_freq_plan_entry_info   *clk_plan_tbl
   );

/* ============================================================================
**  Function : EFI_DDRGetInfo_GetDDRFreq
** ============================================================================
*/
/** @ingroup EFI_DDRGetInfo_GetDDRFreq
  @par Summary
  Gets the Current DDR Freq

  @param[in]   This            Pointer to the EFI_DDRGETINFO_PROTOCOL instance.
  @param[out]  ddr_freq        Pointer to Current DDR clock frequency

  @return
  EFI_SUCCESS        -- Function completed successfully. \n
  EFI_PROTOCOL_ERROR -- Error occurred during the operation.
*/
typedef
EFI_STATUS
(EFIAPI *EFI_DDRGETINFO_GETDDRFREQ)(
   IN EFI_DDRGETINFO_PROTOCOL *This,
   OUT UINT32                 *ddr_freq
   );

/* ============================================================================
**  Function : EFI_DDRGetInfo_SetDDRFreq
** ============================================================================
*/
/** @ingroup EFI_DDRGETINFO_SETDDRFREQ
  @par Summary
  Gets the DDR Details

  @param[in]   This            Pointer to the EFI_DDRGETINFO_PROTOCOL instance.
  @param[in]   ddr_freq        DDR freq to be set in the system.

  @return
  EFI_SUCCESS        -- Function completed successfully. \n
  EFI_PROTOCOL_ERROR -- Error occurred during the operation.
*/
typedef
EFI_STATUS
(EFIAPI *EFI_DDRGETINFO_SETDDRFREQ)(
   IN EFI_DDRGETINFO_PROTOCOL *This,
   IN UINT32                   ddr_freq
   );

/*===========================================================================
  PROTOCOL INTERFACE
===========================================================================*/
/** @ingroup efi_ddrgetinfo_protocol
  @par Summary
  DDR Get Info Protocol interface.

  @par Parameters
*/
struct _EFI_DDRGETINFO_PROTOCOL {
   UINT64                                Revision;
   EFI_DDRGETINFO_GETDDRDETAILS          GetDDRDetails;
   EFI_DDRGETINFO_GETDDRFREQTABLE        GetDDRFreqTable;
   EFI_DDRGETINFO_GETDDRFREQ             GetDDRFreq;
   EFI_DDRGETINFO_SETDDRFREQ             SetDDRFreq;
};

#endif /* __EFIDDRGETINFO_H__ */
