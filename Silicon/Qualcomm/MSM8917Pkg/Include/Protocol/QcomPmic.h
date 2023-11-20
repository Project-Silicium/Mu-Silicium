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
#ifndef _QCOM_PMIC_H_
#define _QCOM_PMIC_H_

//
// Global ID for the Gpa Protocol
//
#define QCOM_PMIC_PROTOCOL_GUID { 0x2511a4ee, 0x3372, 0x44fb, { 0x9a, 0x15, 0x7f, 0x9a, 0x42, 0xa8, 0x83, 0x47 } }

//
// Declare forward reference to the Pmic Protocol
//
typedef struct _QCOM_PMIC_PROTOCOL QCOM_PMIC_PROTOCOL;

/**
  This Function Reads a Register from SPMI.

  @param Adress       The Register.
**/
typedef 
UINT8
(EFIAPI *EFI_READ_REG)(
  UINT32 Address
  );

/**
  This Function Writes a Register to SPMI.

  @param Adress       The Register.
  @param Value        The Value to Write.

  @return EFI_SUCCESS The Register has been written Successfully.
**/
typedef 
EFI_STATUS
(EFIAPI *EFI_WRITE_REG)(
  UINT32 Address,
  UINT8  Value
  );

//
// Define Protocol Functions
//
struct _QCOM_PMIC_PROTOCOL {
  EFI_READ_REG        ReadReg;
  EFI_WRITE_REG       WriteReg;
};

extern EFI_GUID gQcomPmicProtocolGuid;

#endif /* _QCOM_PMIC_H_ */