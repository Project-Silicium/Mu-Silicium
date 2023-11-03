/* Copyright (c) 2013-2014, The Linux Foundation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer in the documentation and/or other materials provided
 *    with the distribution.
 *  * Neither the name of The Linux Foundation, Inc. nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
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
#ifndef _QCOM_SPMI_H_
#define _QCOM_SPMI_H_

#include <Device/SPMI.h>

//
// Global ID for the Gpa Protocol
//
#define QCOM_SPMI_PROTOCOL_GUID { 0x22859cf6, 0x4904, 0x4acb, { 0x8a, 0x8d, 0x51, 0x21, 0x56, 0x6b, 0x67, 0xc8 } }

//
// Declare forward reference to the Spmi Protocol
//
typedef struct _QCOM_SPMI_PROTOCOL QCOM_SPMI_PROTOCOL;

/**
  This Function Writes a Register to SPMI.

  @param cmd          The CMD Register.
  @param param        The Optimal Params.

  @param EFI_SUCCESS  The Register has been Written Successfully.
**/
typedef 
EFI_STATUS
(EFIAPI *EFI_WRITE_PMIC_ARB_CMD)(
  struct pmic_arb_cmd   *cmd,
  struct pmic_arb_param *param
  );

/**
  This Function Reads a Register from SPMI.

  @param cmd          The CMD Register.
  @param param        The Optimal Params.

  @param EFI_SUCCESS  The Register has been Read Successfully.
**/
typedef 
EFI_STATUS
(EFIAPI *EFI_READ_PMIC_ARB_CMD)(
  struct pmic_arb_cmd   *cmd,
  struct pmic_arb_param *param
  );

//
// Define Protocol Functions
//
struct _QCOM_SPMI_PROTOCOL {
  EFI_WRITE_PMIC_ARB_CMD       WritePmicArbCmd;
  EFI_READ_PMIC_ARB_CMD        ReadPmicArbCmd;
};

extern EFI_GUID gQcomSpmiProtocolGuid;

#endif /* _QCOM_SPMI_H_ */