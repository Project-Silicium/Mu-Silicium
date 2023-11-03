/* Copyright (c) 2012-2015, 2017-2018, The Linux Foundation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of The Linux Foundation, nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
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
#include <PiDxe.h>

#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Protocol/QcomSpmi.h>
#include <Protocol/QcomPmic.h>

QCOM_SPMI_PROTOCOL *mSpmiProtocol;

UINT8
ReadReg(UINT32 Address)
{
  UINT8 Value = 0;

  struct pmic_arb_cmd   cmd;
  struct pmic_arb_param param;

  cmd.address  = (Address & 0xFF00) >> 8;
  cmd.offset   = Address & 0xFF;
  cmd.slave_id = Address >> 16;
  cmd.priority = 0;

  param.buffer = &Value;
  param.size   = 1;

  mSpmiProtocol->ReadPmicArbCmd(&cmd, &param);

  return Value;
}

EFI_STATUS
WriteReg(UINT32 Address, UINT8 Value)
{
  EFI_STATUS Status;

  struct pmic_arb_cmd   cmd;
  struct pmic_arb_param param;

  cmd.address  = (Address & 0xFF00) >> 8;
  cmd.offset   = Address & 0xFF;
  cmd.slave_id = Address >> 16;
  cmd.priority = 0;

  param.buffer = &Value;
  param.size   = 1;

  Status = mSpmiProtocol->WritePmicArbCmd(&cmd, &param);

  return Status;
}

STATIC QCOM_PMIC_PROTOCOL mInternalPm8x41 = {
  ReadReg,
  WriteReg
};

EFI_STATUS
EFIAPI
Pm8x41DxeInitialize (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status;

  // Locate SPMI Protocol
  Status = gBS->LocateProtocol(&gQcomSpmiProtocolGuid, NULL, (VOID *)&mSpmiProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate SPMI Protocol! Status = %r\n", Status));
    goto exit;
  }

  // Install Pmic Protocol
  Status = gBS->InstallMultipleProtocolInterfaces(&ImageHandle, &gQcomPmicProtocolGuid, &mInternalPm8x41, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Install Qcom Pm8x41 Protocol! Status = %r\n", Status));
    ASSERT_EFI_ERROR(Status);
  }

exit:
  return Status;
}