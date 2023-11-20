/*
 * Copyright (c) 2012, 2014-2015, The Linux Foundation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of The Linux Foundation nor
 *     the names of its contributors may be used to endorse or promote
 *     products derived from this software without specific prior written
 *     permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NON-INFRINGEMENT ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include <PiDxe.h>

#include <Library/IoLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Protocol/QcomClock.h>

static struct clk_list msm_clk_list;

EFI_STATUS
SetClockParent (struct clk *clk, struct clk *parent)
{
  if (!clk->ops->set_parent) {
    return EFI_INVALID_PARAMETER;
  }

  clk->ops->set_parent(clk, parent);
  return EFI_SUCCESS;
}

struct clk *GetClockParent (struct clk *clk)
{
  if (!clk->ops->set_parent) {
    return 0;
  }

  return clk->ops->get_parent(clk);
}

EFI_STATUS
ResetClock (struct clk *clk, enum clk_reset_action action)
{
  if (!clk || !clk->ops->reset) {
    return EFI_INVALID_PARAMETER;
  } 

  clk->ops->reset(clk, action);
  return EFI_SUCCESS;
}

EFI_STATUS
DisableClock (struct clk *clk)
{
  struct clk *parent;

  if (!clk) {
    return EFI_INVALID_PARAMETER;
  } else if (clk->count == 0) {
    return EFI_SUCCESS;
  } else if (clk->count == 1) {
    if (clk->ops->disable) {
      clk->ops->disable(clk);
    }

    parent = GetClockParent(clk);
    DisableClock (parent);
  }
  clk->count--;

  return EFI_SUCCESS;
}

EFI_STATUS
EnableClock (struct clk *clk)
{
  INT32 Ret = 0;

  struct clk *parent;

  if (!clk) {
    return EFI_INVALID_PARAMETER;
  }

  if (clk->count == 0) {
    parent = GetClockParent(clk);
    Ret = EnableClock(parent);

    if (Ret) {
      return EFI_DEVICE_ERROR;
    } else if (clk->ops->enable) {
      Ret = clk->ops->enable(clk);

      if (Ret) {
        DisableClock(parent);
        return EFI_DEVICE_ERROR;
      }
    }
  }
  clk->count++;

  return EFI_SUCCESS;
}

UINT32
GetClockRate (struct clk *clk)
{
  if (!clk->ops->get_rate) {
    return 0;
  }

  return clk->ops->get_rate(clk);
}

EFI_STATUS
SetClockRate (struct clk *clk, UINT32 ClockRate)
{
  if (!clk->ops->set_rate) {
    return EFI_INVALID_PARAMETER;
  }

  clk->ops->set_rate(clk, ClockRate);
  return EFI_SUCCESS;
}

struct clk *GetClock (CONST CHAR16 *cid)
{
  UINT32 i;
  UINT32 Num = msm_clk_list.num;

  struct clk_lookup *cl = msm_clk_list.clist;

  if (!cl || !Num) {
    return NULL;
  }

  for (i = 0; i < Num; i++, cl++) {
    if (!StrCmp(cl->con_id, cid)) {
      return cl->clk;
    }
  }

  return NULL;
}

EFI_STATUS
SetClock (CHAR16 *id, UINT32 ClockRate, BOOLEAN Enable)
{
  EFI_STATUS Status = EFI_SUCCESS;

  struct clk *cp;

  cp = GetClock(id);
  if (!cp) {
    return EFI_NOT_FOUND;
  }

  if (ClockRate) {
    Status = SetClockRate(cp, ClockRate);
    if (EFI_ERROR (Status)) {
      goto exit;
    }
  }

  if (Enable) {
    Status = EnableClock(cp);
    if (EFI_ERROR (Status)) {
      goto exit;
    }
  }

exit:
  return Status;
}

STATIC QCOM_CLOCK_PROTOCOL mInternalClock = {
  SetClockParent,
  //GetClockParent,
  ResetClock,
  DisableClock,
  EnableClock,
  GetClockRate,
  SetClockRate,
  //GetClock,
  SetClock
};

EFI_STATUS
EFIAPI
ClockInitialize (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status;

  Status = gBS->InstallMultipleProtocolInterfaces(&ImageHandle, &gQcomClockProtocolGuid, &mInternalClock, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Install Clock Protocol! Status = %r\n", Status));
    ASSERT_EFI_ERROR(Status);
  }

  return Status;
}