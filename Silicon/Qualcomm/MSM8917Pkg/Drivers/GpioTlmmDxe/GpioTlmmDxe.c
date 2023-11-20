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
 *  * Neither the name of The Linux Foundation nor the names of its
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
#include <PiDxe.h>

#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Protocol/QcomGpioTlmm.h>

VOID
SetGpioConfig(
  UINT32  TlmmBaseAddr,
  UINT32  Gpio,
  UINT8   Func,
  UINT8   Pull,
  UINT8   Drvstr,
  BOOLEAN Enable)
{
  UINT32 Value = 0;

  Value |= Pull;
  Value |= Func << 2;
  Value |= Drvstr << 6;
  Value |= Enable << 9;

  MmioWrite32 (TlmmBaseAddr + 0x1004 + Gpio * 0x10, Value);
}

VOID
SetGpio(
  UINT32  TlmmBaseAddr,
  UINT32  Gpio,
  UINT32  Value)
{
  MmioWrite32 (TlmmBaseAddr + 0x4 + Gpio * 0x1000, Value);
}

UINT32
GetGpio(
  UINT32  TlmmBaseAddr,
  UINT32  Gpio)
{
  return MmioRead32 (TlmmBaseAddr + 0x4 + Gpio * 0x1000) & (1 << 0);
}

VOID
SetTlmmPins(struct tlmm_cfgs *cfg)
{
  UINT32 Value;

  if (!cfg->reg) {
    cfg->reg = 0x110A000;
  }

  Value = MmioRead32 (cfg->reg);

  Value &= ~(cfg->mask << cfg->off);
  Value |= (cfg->val << cfg->off);

  MmioWrite32 (cfg->reg, Value);
}

VOID
SetTlmmHDriveCtrl(
  struct tlmm_cfgs *hdrv_cfgs,
  UINT8             Sz)
{
  UINT8 i;

  for (i = 0; i < Sz; i++) {
    SetTlmmPins (&hdrv_cfgs[i]);
  }
}

VOID
SetTlmmPullCtrl(
  struct tlmm_cfgs *pull_cfgs,
  UINT8             Sz)
{
  UINT8 i;

  for (i = 0; i < Sz; i++) {
    SetTlmmPins (&pull_cfgs[i]);
  }
}

STATIC QCOM_GPIO_TLMM_PROTOCOL mInternalGpioTlmm = {
  SetGpioConfig,
  SetGpio,
  GetGpio,

  SetTlmmPins,
  SetTlmmHDriveCtrl,
  SetTlmmPullCtrl
};

EFI_STATUS
EFIAPI
GpioTlmmDxeInitialize (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status;

  Status = gBS->InstallMultipleProtocolInterfaces(&ImageHandle, &gQcomGpioTlmmProtocolGuid, &mInternalGpioTlmm, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Install Qcom GPIO Protocol! Status = %r\n", Status));
    ASSERT_EFI_ERROR(Status);
  }

  return Status;
}