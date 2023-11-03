/* Copyright (c) 2012, 2014-2015, The Linux Foundation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above
 *      copyright notice, this list of conditions and the following
 *      disclaimer in the documentation and/or other materials provided
 *      with the distribution.
 *    * Neither the name of The Linux Foundation nor the names of its
 *      contributors may be used to endorse or promote products derived
 *      from this software without specific prior written permission.
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
#include <Library/IoLib.h>
#include <Library/MallocLib.h>

#include <Protocol/QcomSpmi.h>

#define SPMI_BASE            0x02000000
#define PMIC_ARB_V2          0x20010000
#define CHANNEL_INDEX(SID, PID) ((SID << 8) | PID)

STATIC UINT32 PmicArbChannelNumber;
STATIC UINT32 PmicArbVersion;

STATIC UINT8 *ChannelTbl;

STATIC
VOID
WriteWDataFromArray(
  UINT8 *Array,
  UINT8  RegNumber,
  UINT8  ArraySize,
  UINT8 *BytesWritten)
{
  UINT32 ShiftValue[] = { 0, 8, 16, 24 };
  UINT32 Value        = 0;
  INT32  i;

  // Write to WDATA
  for (i = 0; (*BytesWritten < ArraySize) && (i < 4); i++) {
    Value |= (UINT32)(Array[*BytesWritten]) << ShiftValue[i];
    (*BytesWritten)++;
  }

  MmioWrite32 (SPMI_BASE + 0x400000 + 0x10 + PmicArbChannelNumber * 0x8000 + RegNumber * 4, Value);
}

EFI_STATUS
WritePmicArbCmd(
  struct pmic_arb_cmd   *cmd,
  struct pmic_arb_param *param)
{
  UINT32 BytesWritten = 0;
  UINT32 Error        = 0;
  UINT32 Value        = 0;

  //
  // Look up for pmic channel only for V2 hardware
  // For V1-HW we dont care for channel number & always
  // use '0'
  //
  if (PmicArbVersion >= PMIC_ARB_V2) {
    PmicArbChannelNumber = ChannelTbl[CHANNEL_INDEX(cmd->slave_id, cmd->address)];
  }

  // Disable IRQ Mode for the Current Channel
  MmioWrite32 (SPMI_BASE + 0x00400000 + 0x4 + PmicArbChannelNumber * 0x8000, 0x0);

  // Write Parameters for the CMD
  if (cmd == NULL) {
    DEBUG ((EFI_D_WARN, "No Command Provided\n"));
    return EFI_INVALID_PARAMETER;
  }

  // Write first 4 Bytes to WDATA0
  WriteWDataFromArray(param->buffer, 0, param->size, (UINT8 *)&BytesWritten);

  if (BytesWritten < param->size) {
    // Write next 4 Bytes to WDATA1
    WriteWDataFromArray(param->buffer, 1, param->size, (UINT8 *)&BytesWritten);
  }

  //
  // Fill in the Byte Count for the Command
  // NOTE: Byte Count is one less than the Number of Bytes Transferred
  //
  cmd->byte_cnt = param->size - 1;

  // Fill in the Write CMD opcode
  cmd->opcode = SPMI_CMD_EXT_REG_WRITE_LONG;

  // Write the Command
  Value |= ((UINT32)(cmd->opcode)   << PMIC_ARB_CMD_OPCODE_SHIFT);
  Value |= ((UINT32)(cmd->priority) << PMIC_ARB_CMD_PRIORITY_SHIFT);
  Value |= ((UINT32)(cmd->slave_id) << PMIC_ARB_CMD_SLAVE_ID_SHIFT);
  Value |= ((UINT32)(cmd->address)  << PMIC_ARB_CMD_ADDR_SHIFT);
  Value |= ((UINT32)(cmd->offset)   << PMIC_ARB_CMD_ADDR_OFFSET_SHIFT);
  Value |= ((UINT32)(cmd->byte_cnt));

  MmioWrite32 (SPMI_BASE + 0x00400000 + PmicArbChannelNumber * 0x8000, Value);

  // Wait till CMD DONE Status
  while (!(Value = MmioRead32(SPMI_BASE + 0x00400000 + 0x8 + PmicArbChannelNumber * 0x8000)));

  // Check for Errors
  Error = Value ^ (1 << PMIC_ARB_CMD_DONE);

  if (Error) {
    DEBUG ((EFI_D_ERROR, "SPMI Write Command Failure: cmd_id = %u, error = 0x%x\n", cmd->opcode, Error));
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

STATIC
VOID
ReadRDataIntoArray(
  UINT8 *Array,
  UINT8  RegNumber,
  UINT8  ArraySize,
  UINT8 *BytesRead)
{
  UINT32 Value        = 0;
  UINT32 MaskValue[]  = { 0xFF, 0xFF00, 0xFF0000, 0xFF000000 };
  UINT8  ShiftValue[] = { 0, 8, 16, 24 };
  INT32  i;

  Value = MmioRead32 (SPMI_BASE + 0xC00000 + 0x18 + PmicArbChannelNumber * 0x8000 + RegNumber * 4);

  // Read at most 4 Bytes
  for (i = 0; (i < 4) && (*BytesRead < ArraySize); i++) {
    Array[*BytesRead] = (Value & MaskValue[i]) >> ShiftValue[i];
    (*BytesRead)++;
  }
}

EFI_STATUS
ReadPmicArbCmd(
  struct pmic_arb_cmd   *cmd,
  struct pmic_arb_param *param)
{
  UINT32 Value     = 0;
  UINT32 Error     = 0;
  UINT8  BytesRead = 0;

  //
  // Look up for pmic channel only for V2 hardware
  // For V1-HW we dont care for channel number & always
  // use '0'
  //
  if (PmicArbVersion >= PMIC_ARB_V2) {
    PmicArbChannelNumber = ChannelTbl[CHANNEL_INDEX(cmd->slave_id, cmd->address)];
  }

  // Disable IRQ Mode for the Current Channel
  MmioWrite32 (SPMI_BASE + 0xC00000 + 0x4 + PmicArbChannelNumber * 0x8000, 0x0);

  //
  // Fill in the Byte Count for the Command
  // NOTE: Byte Count is one less than the Number of Bytes transferred.
  //
  cmd->byte_cnt = param->size - 1;

  // Fill in the Write cmd opcode
  cmd->opcode = SPMI_CMD_EXT_REG_READ_LONG;

  Value |= ((UINT32)(cmd->opcode)   << PMIC_ARB_CMD_OPCODE_SHIFT);
  Value |= ((UINT32)(cmd->priority) << PMIC_ARB_CMD_PRIORITY_SHIFT);
  Value |= ((UINT32)(cmd->slave_id) << PMIC_ARB_CMD_SLAVE_ID_SHIFT);
  Value |= ((UINT32)(cmd->address)  << PMIC_ARB_CMD_ADDR_SHIFT);
  Value |= ((UINT32)(cmd->offset)   << PMIC_ARB_CMD_ADDR_OFFSET_SHIFT);
	Value |= ((UINT32)(cmd->byte_cnt));

  MmioWrite32 (SPMI_BASE + 0xC00000 + PmicArbChannelNumber * 0x8000, Value);

  // Wait till CMD DONE Status
  while (!(Value = MmioRead32(SPMI_BASE + 0xC00000 + 0x8 + PmicArbChannelNumber * 0x8000)));

  // Check for Errors
  Error = Value ^ (1 << PMIC_ARB_CMD_DONE);

  if (Error) {
    DEBUG ((EFI_D_ERROR, "SPMI Read Command Failure: cmd_id = %u, error = 0x%x\n", cmd->opcode, Error));
    return EFI_DEVICE_ERROR;
  }

  // Read the RDATA0
  ReadRDataIntoArray(param->buffer, 0, param->size, &BytesRead);

  if (BytesRead < param->size) {
    // Read the RDATA1
    ReadRDataIntoArray(param->buffer, 1, param->size, &BytesRead);

    if (BytesRead < param->size) {
      // Read the RDATA2
      ReadRDataIntoArray(param->buffer, 2, param->size, &BytesRead);
    }
  }

  return EFI_SUCCESS;
}

STATIC QCOM_SPMI_PROTOCOL mInternalSpmi = {
  WritePmicArbCmd,
  ReadPmicArbCmd
};

STATIC
VOID
LookupSpmiChannelNumber()
{
  UINT32 i;
  UINT8  SlaveID     = 0;
  UINT8  PPIDAddress = 0;
  UINT32 ChannelTblSZ = (1 << 12) * sizeof(UINT8);

  // Allocate the Channel Table
  ChannelTbl = (UINT8 *) malloc(ChannelTblSZ);
  ASSERT(ChannelTbl);

  for (i = 0; i < 256; i++) {
    SlaveID     = (MmioRead32(0x200F000 + 0x800 + 0x4 * i) & 0xF0000) >> 16;
    PPIDAddress = (MmioRead32(0x200F000 + 0x800 + 0x4 * i) & 0xFF00) >> 8;

    ChannelTbl[CHANNEL_INDEX(SlaveID, PPIDAddress)] = i;
  }
}

EFI_STATUS
EFIAPI
SpmiDxeInitialize (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status;

  // Read the Version Number
  PmicArbVersion = MmioRead32(SPMI_BASE + 0xF000);

  if (PmicArbVersion < PMIC_ARB_V2) {
    //
    // Initialize PMIC Arbiter Channel Number to
    // 0 by default of V1 HW
    //
    PmicArbChannelNumber = 0;
  } else {
    LookupSpmiChannelNumber();
  }

  // Install Pmic Protocol
  Status = gBS->InstallMultipleProtocolInterfaces(&ImageHandle, &gQcomSpmiProtocolGuid, &mInternalSpmi, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Install Qcom SPMI Protocol! Status = %r\n", Status));
    ASSERT_EFI_ERROR(Status);
  }

  return Status;
}
