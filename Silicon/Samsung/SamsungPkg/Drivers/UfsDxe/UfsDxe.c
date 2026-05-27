#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryMapHelperLib.h>
#include <Library/IoLib.h>
#include <Library/TimerLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DevicePathLib.h>
#include <Library/UfsHostBridge.h>

#include <Library/PlatformUfsLib.h>

#include <Protocol/EfiChipData.h>

#include "UfsDxe.h"

UINT8 gQueryParams[][5] = {
  /* [0] unused */
  {0,                    0,                              0,                           0, 0},
  /* FLAG_W_FDEVICEINIT */
  {UFS_STD_WRITE_REQ,    UPIU_QUERY_OPCODE_SET_FLAG,     UPIU_FLAG_ID_DEVICEINIT,     0, 0},
  /* FLAG_R_FDEVICEINIT */
  {UFS_STD_READ_REQ,     UPIU_QUERY_OPCODE_READ_FLAG,    UPIU_FLAG_ID_DEVICEINIT,     0, 0},
  /* DESC_R_DEVICE_DESC */
  {UFS_STD_READ_REQ,     UPIU_QUERY_OPCODE_READ_DESC,    UPIU_DESC_ID_DEVICE,         0, 0},
  /* DESC_W_CONFIG_DESC */
  {UFS_STD_WRITE_REQ,    UPIU_QUERY_OPCODE_WRITE_DESC,   UPIU_DESC_ID_CONFIGURATION,  0, 0},
  /* DESC_R_CONFIG_DESC */
  {UFS_STD_READ_REQ,     UPIU_QUERY_OPCODE_READ_DESC,    UPIU_DESC_ID_CONFIGURATION,  0, 0},
  /* DESC_R_UNIT_DESC */
  {UFS_STD_READ_REQ,     UPIU_QUERY_OPCODE_READ_DESC,    UPIU_DESC_ID_UNIT,           0, 0},
  /* DESC_R_GEOMETRY_DESC */
  {UFS_STD_READ_REQ,     UPIU_QUERY_OPCODE_READ_DESC,    UPIU_DESC_ID_GEOMETRY,       0, 0},
  /* ATTR_W_BOOTLUNEN */
  {UFS_STD_WRITE_REQ,    UPIU_QUERY_OPCODE_WRITE_ATTR,   UPIU_ATTR_ID_BOOTLUNEN,      0, 0},
  /* ATTR_R_BOOTLUNEN */
  {UFS_STD_READ_REQ,     UPIU_QUERY_OPCODE_READ_ATTR,    UPIU_ATTR_ID_BOOTLUNEN,      0, 0},
  /* ATTR_W_REFCLKFREQ */
  {UFS_STD_WRITE_REQ,    UPIU_QUERY_OPCODE_WRITE_ATTR,   UPIU_ATTR_ID_REFCLKFREQ,     0, 0},
  /* ATTR_R_REFCLKFREQ */
  {UFS_STD_READ_REQ,     UPIU_QUERY_OPCODE_READ_ATTR,    UPIU_ATTR_ID_REFCLKFREQ,     0, 0},
};

STATIC EFI_CHIP_DATA_PROTOCOL *mChipDataProtocol;

STATIC
VOID
UfsMapSg (struct UfsHost *Ufs)
{
  UINT32 i, len, sg_segs, remaining, seg_bytes;
  len = Ufs->ScsiCmd->DataLen;
  sg_segs = (len + UFS_SG_BLOCK_SIZE - 1) / UFS_SG_BLOCK_SIZE;

  for (i = 0; i < sg_segs; i++) {
    UINT64 phys = (UINT64)(UINTN)Ufs->ScsiCmd->Buf + (UINT64)i * UFS_SG_BLOCK_SIZE;
    remaining = len - i * UFS_SG_BLOCK_SIZE;
    seg_bytes = (remaining < UFS_SG_BLOCK_SIZE) ? remaining : UFS_SG_BLOCK_SIZE;
    Ufs->CmdDescAddr->PrdTable[i].Size = seg_bytes - 1;
    Ufs->CmdDescAddr->PrdTable[i].BaseAddr  = (UINT32)(phys & 0xFFFFFFFFULL);
    Ufs->CmdDescAddr->PrdTable[i].UpperAddr = (UINT32)(phys >> 32);
  }
}

STATIC
UINT32
UfsCmdGetDir (ScsiCommandMeta *Cmd)
{
  if (!Cmd->DataLen) return UTP_NO_DATA_TRANSFER;
  switch (Cmd->Cdb[0]) {
  case SCSI_OP_WRITE_10:
  case SCSI_OP_WRITE_BUFFER:
  case SCSI_OP_FORMAT_UNIT:
  case SCSI_OP_UNMAP:
  case SCSI_OP_SECU_PROT_OUT:
  case SCSI_OP_START_STOP_UNIT:
    return UTP_HOST_TO_DEVICE;
  default:
    return UTP_DEVICE_TO_HOST;
  }
}

STATIC
UINT32
UfsCmdGetFlags (ScsiCommandMeta *Cmd)
{
  if (!Cmd->DataLen) return UPIU_CMD_FLAGS_NONE;
  switch (Cmd->Cdb[0]) {
  case SCSI_OP_WRITE_10:
  case SCSI_OP_WRITE_BUFFER:
  case SCSI_OP_FORMAT_UNIT:
  case SCSI_OP_UNMAP:
  case SCSI_OP_SECU_PROT_OUT:
  case SCSI_MODE_SEL10:
    return UPIU_CMD_FLAGS_WRITE;
  case SCSI_OP_START_STOP_UNIT:
    return UPIU_CMD_FLAGS_NONE;
  default:
    return UPIU_CMD_FLAGS_READ;
  }
}

STATIC
INT32
UfsHandleUicInt (
  struct UfsHost *Ufs,
  UINT32 stat
)
{
  INT32 Ret = UFS_IN_PROGRESS;
  struct UfsUicCmd *Cmd = Ufs->UicCmd;

  if (stat & UIC_COMMAND_COMPL) {
    if (Cmd->uiccmdr == UIC_CMD_DME_LINK_STARTUP)
      Ret = UFS_NO_ERROR;
    else if ((Cmd->uiccmdr == UIC_CMD_DME_SET) && (Cmd->Arg1 == (0x1571U << 16))) {
      if (stat & UIC_POWER_MODE) Ret = UFS_NO_ERROR;
    }
    else
      Ret = UFS_NO_ERROR;
  }

  if ((stat & UIC_ERROR) && (Cmd->uiccmdr != UIC_CMD_DME_LINK_STARTUP)) {
    DEBUG ((EFI_D_ERROR, "UFS UIC ERROR 0x%08x\n", stat));
    Ret = UFS_ERROR;
  }
  return Ret;
}

STATIC
INT32
UfsHandleUtpInt (
  struct UfsHost *Ufs,
  UINT32 stat
)
{
  if (stat & UTP_TRANSFER_REQ_COMPL) {
    if (!(MmioRead32((UINTN)(Ufs->IoAddr + REG_UTP_TRANSFER_REQ_DOOR_BELL)) & 1))
      return UFS_NO_ERROR;
  }
  return UFS_IN_PROGRESS;
}

STATIC
INT32
UfsHandleInt (
  struct UfsHost *Ufs,
  BOOLEAN IsUic
)
{
  UINT32 Stat = MmioRead32((UINTN)(Ufs->IoAddr + REG_INTERRUPT_STATUS));
  INT32 Ret;

  Ret = IsUic ? UfsHandleUicInt(Ufs, Stat) : UfsHandleUtpInt (Ufs, Stat);

  if (Stat & INT_FATAL_ERRORS) {
    DEBUG ((EFI_D_ERROR, "UFS Fatal error 0x%08x\n", Stat));
    Ret = UFS_ERROR;
  }

  if (Ret == UFS_IN_PROGRESS) {
    if (Ufs->Timeout--)
      MicroSecondDelay(1);
    else {
      Ret = UFS_TIMEOUT;
      DEBUG ((EFI_D_ERROR, "UFS Timeout\n"));
    }
  }

  return Ret;
}

static
EFI_STATUS
UfsInitCal (struct UfsHost *Ufs)
{
  UINT32 ChipRevision[2];

  mChipDataProtocol->GetRevision(ChipRevision);

  Ufs->CalParam->Host = Ufs;
  Ufs->CalParam->Board = UfsCalGetTargetBoard();
  Ufs->CalParam->EvtVer = ChipRevision[0] & 0xF;

  DEBUG((EFI_D_INFO, "UFS EVT version %d\n", Ufs->CalParam->EvtVer));

  if (UfsCalInit(Ufs->CalParam) != UFS_CAL_NO_ERROR) {
    DEBUG ((EFI_D_ERROR, "UFS Calibration Init failed\n"));
    return EFI_DEVICE_ERROR;
  }
  return EFI_SUCCESS;
}

STATIC
VOID
UfsDeviceReset (struct UfsHost *Ufs)
{
  MmioWrite32((UINTN)(Ufs->VsAddr + VS_GPIO_OUT), 0);
  MicroSecondDelay(5);
  MmioWrite32((UINTN)(Ufs->VsAddr + VS_GPIO_OUT), 1);
}

STATIC
VOID
UfsDevicePower (
  struct UfsHost *Ufs,
  UINT8 On
)
{
  UINT32 Register;
  UINT32 Mask;

  Mask = 1 << (Ufs->DevPwrShift);

  Register = MmioRead32((UINTN)Ufs->DevPwrAddr);
  Register &= ~Mask;
  Register |= (On << Ufs->DevPwrShift);

  MmioWrite32((UINTN)Ufs->DevPwrAddr, Register);
}

INT32
UfsSendUicCmd (struct UfsHost *Ufs)
{
  INT32 Error, ErrorCode;

  MmioWrite32((UINTN)(Ufs->IoAddr + REG_UIC_COMMAND_ARG_1), Ufs->UicCmd->Arg1);
  MmioWrite32((UINTN)(Ufs->IoAddr + REG_UIC_COMMAND_ARG_2), Ufs->UicCmd->Arg2);
  MmioWrite32((UINTN)(Ufs->IoAddr + REG_UIC_COMMAND_ARG_3), Ufs->UicCmd->Arg3);
  MmioWrite32((UINTN)(Ufs->IoAddr + REG_UIC_COMMAND), Ufs->UicCmd->uiccmdr);

  Ufs->Timeout = Ufs->UicCmdTimeout;
  while (UFS_IN_PROGRESS == (Error = UfsHandleInt(Ufs, TRUE)));

  MmioWrite32((UINTN)(Ufs->IoAddr + REG_INTERRUPT_STATUS), MmioRead32((UINTN)(Ufs->IoAddr + REG_INTERRUPT_STATUS)));

  ErrorCode = MmioRead32((UINTN)(Ufs->IoAddr + REG_UIC_COMMAND_ARG_2));

  if (Ufs->UicCmd->uiccmdr == UIC_CMD_DME_GET ||
      Ufs->UicCmd->uiccmdr == UIC_CMD_DME_PEER_GET) {
    Ufs->UicCmd->Arg3 = MmioRead32((UINTN)(Ufs->IoAddr + REG_UIC_COMMAND_ARG_3));
  }

  return ErrorCode | Error;
}

STATIC
EFI_STATUS
UfsPreLink (
  struct UfsHost *Ufs,
  UINT8 lane
)
{
  struct UfsCalParam *p = Ufs->CalParam;

  p->MclkRate = Ufs->MclkRate;
  p->AvailableLane = lane;
  p->Table = HOST_EMBD;

  if (UfsCalPreLink(Ufs->CalParam) != UFS_CAL_NO_ERROR) {
    DEBUG ((EFI_D_ERROR, "UFS pre-link calibration failed\n"));
    return EFI_DEVICE_ERROR;
  }

  DEBUG ((EFI_D_INFO, "UFS pre-link calibration passed\n"));
  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
UfsUpdateMaxGear (
  struct UfsHost *Ufs)
{
  /* PA_MaxRxHSGear = 0x1587 */
  struct UfsUicCmd cmd = {UIC_CMD_DME_GET, UIC_ARG_MIB (0x1587), 0, 0};

  Ufs->UicCmd = &cmd;

  if (UfsSendUicCmd (Ufs))
    return EFI_DEVICE_ERROR;

  Ufs->CalParam->MaxGear = (UINT8)MIN(cmd.Arg3, (UINT32)Ufs->GearMode);

  DEBUG ((EFI_D_INFO, "UFS Max Gear: %d\n", Ufs->CalParam->MaxGear));
  return 0;
}

STATIC
EFI_STATUS
UfsUpdateActiveLane (struct UfsHost *Ufs)
{
  struct UfsUicCmd tx = {UIC_CMD_DME_GET, UIC_ARG_MIB (0x1560), 0, 0}; /* PA_ActiveTxDataLanes */
  struct UfsUicCmd rx = {UIC_CMD_DME_GET, UIC_ARG_MIB (0x1580), 0, 0}; /* PA_ActiveRxDataLanes */

  Ufs->UicCmd = &tx;
  if (UfsSendUicCmd (Ufs)) return EFI_DEVICE_ERROR;
  Ufs->CalParam->ActiveTxLane = (UINT8)tx.Arg3;

  Ufs->UicCmd = &rx;
  if (UfsSendUicCmd (Ufs)) return EFI_DEVICE_ERROR;
  Ufs->CalParam->ActiveRxLane = (UINT8)rx.Arg3;

  DEBUG ((EFI_D_INFO, "UFS active TX=%d RX=%d\n", Ufs->CalParam->ActiveTxLane, Ufs->CalParam->ActiveRxLane));
  return 0;
}

STATIC
VOID
UfsUtpInit (
  struct UfsHost *Ufs,
  UINT32 Lun
)
{
  Ufs->Lun = Lun;
  Ufs->ScsiCmd = NULL;

  ZeroMem(Ufs->CmdDescAddr, sizeof (struct UfsCmdDesc));
  ZeroMem(Ufs->UtrdAddr, sizeof (struct UfsUtrd));

  Ufs->UtrdAddr->cmd_desc_addr_l = (UINT32)(UINTN)Ufs->CmdDescAddr;
  Ufs->UtrdAddr->cmd_desc_addr_h = (UINT32)((UINTN)Ufs->CmdDescAddr >> 32);
  Ufs->UtrdAddr->rsp_upiu_off = OFFSET_OF(struct UfsCmdDesc, ResponseUpiu);
  Ufs->UtrdAddr->rsp_upiu_len = ALIGNED_UPIU_SIZE;
}

STATIC
VOID
UfsUtpSend (
  struct UfsHost *Ufs,
  UINT32 Type
)
{
  switch (Type) {
  case UPIU_TRANSACTION_NOP_OUT:
  case UPIU_TRANSACTION_QUERY_REQ:
    MmioWrite32((UINTN)(Ufs->VsAddr + VS_UTRL_NEXUS_TYPE), 0x0);
    break;
  case UPIU_TRANSACTION_COMMAND:
    MmioWrite32((UINTN)(Ufs->VsAddr + VS_UTRL_NEXUS_TYPE), 0xFFFFFFFF);
    break;
  default:
    break;
  }

  switch (Type & 0xFF) {
  case UPIU_TRANSACTION_NOP_OUT:
  case UPIU_TRANSACTION_COMMAND:
  case UPIU_TRANSACTION_QUERY_REQ:
    MmioWrite32((UINTN)(Ufs->IoAddr + REG_UTP_TRANSFER_REQ_DOOR_BELL), 1);
    break;
  default:
    break;
  }
}

STATIC
INT32
UfsUtpWaitResponse (
  struct UfsHost *Ufs,
  UINT32 Type
)
{
  int err;

  if (Type == UPIU_TRANSACTION_COMMAND && Ufs->ScsiCmd->Cdb[0] == SCSI_OP_FORMAT_UNIT)
    Ufs->Timeout = FORMAT_CMD_TIMEOUT;
  else if (Type == UPIU_TRANSACTION_NOP_OUT)
    Ufs->Timeout = NOP_OUT_TIMEOUT;
  else
    Ufs->Timeout = Ufs->UfsCmdTimeout;

  while (UFS_IN_PROGRESS == (err = UfsHandleInt (Ufs, 0)));

  MmioWrite32((UINTN)(Ufs->IoAddr + REG_INTERRUPT_STATUS), MmioRead32((UINTN)(Ufs->IoAddr + REG_INTERRUPT_STATUS)));

  /* Nexus clear for NOP / Query */
  if (Type == UPIU_TRANSACTION_NOP_OUT || Type == UPIU_TRANSACTION_QUERY_REQ)
    MmioWrite32((UINTN)(Ufs->IoAddr + 0x140), (MmioRead32((UINTN)(Ufs->IoAddr + 0x140)) | 0x01));

  return err;
}

STATIC
EFI_STATUS
UfsUtpCheckResult (struct UfsHost *Ufs)
{
  struct UfsUtrd *utrd = Ufs->UtrdAddr;
  struct UfsUpiu *resp = &Ufs->CmdDescAddr->ResponseUpiu;
  struct UfsUpiuHeader *hdr = &resp->Header;

  if (Ufs->ScsiCmd)
    Ufs->ScsiCmd->Status = hdr->Status;

  if (utrd->dw[2] != OCS_SUCCESS)
  {
    // Only warn even if it's technically not SUCCESS, NOP throws OCS 01 but UFS works as normal.
    DEBUG ((EFI_D_WARN, "UFS OCS=0x%02x response=0x%02x type=0x%02x\n", utrd->dw[2], hdr->Response, hdr->Type));
  
    return (hdr->Response != 0) ? EFI_DEVICE_ERROR : EFI_SUCCESS;
  }

  /* OCS is good, but also check SCSI status for COMMAND UPIUs */
  if (Ufs->ScsiCmd && hdr->Status != SCSI_STATUS_GOOD)
  {
    DEBUG ((EFI_D_ERROR, "UFS SCSI CHECK CONDITION status=0x%02x cdb0=0x%02x\n", hdr->Status, Ufs->ScsiCmd->Cdb[0]));
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

STATIC
INT32
UfsWriteUtrd (
  struct UfsHost *Ufs,
  UINT32 type
)
{
  struct UfsUtrd *utrd = Ufs->UtrdAddr;
  UINT32 Length, SgSegs;

  switch (type) {
  case UPIU_TRANSACTION_COMMAND:
    Length = Ufs->ScsiCmd->DataLen;
    SgSegs = (Length + UFS_SG_BLOCK_SIZE - 1) / UFS_SG_BLOCK_SIZE;

    utrd->dw[0] = UfsCmdGetDir (Ufs->ScsiCmd) | UTP_SCSI_COMMAND | UTP_REQ_DESC_INT_CMD;
    utrd->dw[2] = OCS_INVALID_COMMAND_STATUS;
    if (Length) {
      utrd->prdt_len = (UINT16)(SgSegs * sizeof (struct UfsPrdt));
      utrd->prdt_off = ALIGNED_UPIU_SIZE * 2;
    } else {
      utrd->prdt_len = 0;
      utrd->prdt_off = 0;
    }
    break;
  case UPIU_TRANSACTION_QUERY_REQ:
  case UPIU_TRANSACTION_NOP_OUT:
    utrd->dw[0] = UTP_REQ_DESC_INT_CMD;
    utrd->dw[2] = OCS_INVALID_COMMAND_STATUS;
    break;
  default:
    return -1;
  }
  return 0;
}

STATIC
VOID
UfsWriteCmdUcd (struct UfsHost *Ufs)
{
  struct UfsUpiu *cmd = &Ufs->CmdDescAddr->CommandUpiu;
  struct UfsUpiuHeader *hdr = &cmd->Header;
  UINT8 *tsf  = cmd->Tsf;
  UINT32 dlen = SwapBytes32(Ufs->ScsiCmd->DataLen);

  hdr->Type = UPIU_TRANSACTION_COMMAND;
  hdr->Flags = (UINT8)UfsCmdGetFlags (Ufs->ScsiCmd);
  hdr->Lun = (UINT8)Ufs->Lun;
  hdr->Tag = 0;

  CopyMem (&tsf[0], &dlen, sizeof (UINT32));
  CopyMem (&tsf[4], Ufs->ScsiCmd->Cdb, MAX_CDB_SIZE);
}

STATIC
INT32
UfsWriteQueryUcd (
  struct UfsHost *Ufs,
  QueryIndex qry
)
{
  struct UfsUpiu *cmd = &Ufs->CmdDescAddr->CommandUpiu;
  struct UfsUpiuHeader*Header = &cmd->Header;
  UINT8 *Tsf = cmd->Tsf;
  UINT16 dlen;
  UINT32 info;
  UINT32 lun;

  Header->Type = UPIU_TRANSACTION_QUERY_REQ;
  Header->Flags = UPIU_CMD_FLAGS_NONE;
  Header->Tag = 0;
  Header->Function = gQueryParams[qry][0];

  Tsf[0] = gQueryParams[qry][1]; /* OPCODE */
  Tsf[1] = gQueryParams[qry][2]; /* IDN */
  Tsf[2] = gQueryParams[qry][3]; /* INDEX */
  Tsf[3] = gQueryParams[qry][4]; /* SELECTOR */

  if (Header->Function == UFS_STD_WRITE_REQ && Tsf[0] == UPIU_QUERY_OPCODE_WRITE_DESC && Tsf[1] == UPIU_DESC_ID_CONFIGURATION)
  {
    Ufs->DataSegLen = Ufs->DeviceDesc.bUD0BaseOffset + (8 * Ufs->DeviceDesc.bUDConfigPlength);
    dlen = Ufs->DataSegLen;
    if (dlen > UPIU_DATA_SIZE) return -1;
    Header->DataLength = SwapBytes16(dlen);
    info = SwapBytes16(dlen);
    CopyMem(&Tsf[6], &info, sizeof (UINT16));
  } else if (Header->Function == UFS_STD_READ_REQ) {
    info = SwapBytes16(UPIU_DATA_SIZE);
    CopyMem(&Tsf[6], &info, sizeof (UINT16));
  } else if (Header->Function == UFS_STD_WRITE_REQ) {
    Header->DataLength = 0;
  }

  if (Tsf[0] == UPIU_QUERY_OPCODE_WRITE_ATTR) {
    info = SwapBytes32(Ufs->Attributes.Array[Tsf[1]]);
    CopyMem(&Tsf[8], &info, sizeof(UINT32));
  } else if (Tsf[0] == UPIU_QUERY_OPCODE_SET_FLAG)
    Tsf[11] = Ufs->Flags.Array[Tsf[1]];

  if (Tsf[0] == UPIU_QUERY_OPCODE_WRITE_DESC && Tsf[1] == UPIU_DESC_ID_CONFIGURATION) {
    CopyMem(cmd->Data, &Ufs->ConfigDesc.Header, Ufs->DeviceDesc.bUD0BaseOffset);
    for (lun = 0; lun < 8; lun++)
      CopyMem(cmd->Data + Ufs->DeviceDesc.bUD0BaseOffset + lun * Ufs->DeviceDesc.bUDConfigPlength, &Ufs->ConfigDesc.Unit[lun], Ufs->DeviceDesc.bUDConfigPlength);
  }
  return 0;
}

static void UfsQueryReadInfo (struct UfsHost *Ufs, UINT8 idn)
{
  struct UfsUpiu *resp = &Ufs->CmdDescAddr->ResponseUpiu;
  UINT8 *data = resp->Data;
  VOID *dst = NULL;
  UINTN len = 0;
  UINT32 lun;

  switch (idn) {
  case UPIU_DESC_ID_UNIT:
    if (data[2] >= 8) {
      DEBUG((EFI_D_ERROR, "UFS unit desc response INDEX %d out of range\n", data[2]));
      return;
    }
    dst = &Ufs->UnitDesc[data[2]];
    len = MIN(data[0], sizeof (struct UfsUnitDesc));
    break;
  case UPIU_DESC_ID_DEVICE:
    dst = &Ufs->DeviceDesc;
    len = MIN(data[0], sizeof (struct UfsDeviceDesc));
    break;
  case UPIU_DESC_ID_CONFIGURATION:
    Ufs->DataSegLen = Ufs->DeviceDesc.bUD0BaseOffset + 8 * Ufs->DeviceDesc.bUDConfigPlength;
    len = MIN(data[0], (UINT8)Ufs->DataSegLen);

    CopyMem (&Ufs->ConfigDesc.Header, resp->Data, Ufs->DeviceDesc.bUD0BaseOffset);

    for (lun = 0; lun < 8; lun++)
      CopyMem (&Ufs->ConfigDesc.Unit[lun], resp->Data + Ufs->DeviceDesc.bUD0BaseOffset + lun * Ufs->DeviceDesc.bUDConfigPlength, Ufs->DeviceDesc.bUDConfigPlength);
    return;
  case UPIU_DESC_ID_GEOMETRY:
    dst = &Ufs->GeometryDesc;
    len = MIN (data[0], sizeof (struct UfsGeometryDesc));
    break;
  default:
    return;
  }

  if (dst)
    CopyMem (dst, resp->Data, len);
}

static void UfsQueryGetData (struct UfsHost *Ufs, QueryIndex qry)
{
  struct UfsUpiu *resp = &Ufs->CmdDescAddr->ResponseUpiu;
  UINT8 opcode = gQueryParams[qry][1];
  UINT8 *tsf = resp->Tsf;
  UINT32 val;

  switch (opcode) {
  case UPIU_QUERY_OPCODE_READ_DESC:
    UfsQueryReadInfo(Ufs, gQueryParams[qry][2]);
    break;
  case UPIU_QUERY_OPCODE_READ_ATTR:
    val = (UINT32)(((UINT32)tsf[8] << 24) | ((UINT32)tsf[9] << 16) | ((UINT32)tsf[10] << 8) | ((UINT32)tsf[11]));
    Ufs->Attributes.Array[gQueryParams[qry][2]] = val;
    break;
  case UPIU_QUERY_OPCODE_READ_FLAG:
    Ufs->Flags.Array[gQueryParams[qry][2]] = (UINT32)tsf[11];
    break;
  default:
    break;
  }
}

STATIC
EFI_STATUS
UfsUtpQueryProcess (struct UfsHost *Ufs, QueryIndex qry, UINT32 lun)
{
  UfsUtpInit(Ufs, lun);

  if (UfsWriteQueryUcd(Ufs, qry))
    return EFI_DEVICE_ERROR;

  if (UfsWriteUtrd(Ufs, UPIU_TRANSACTION_QUERY_REQ))
    return EFI_DEVICE_ERROR;

  MemoryFence();

  UfsUtpSend(Ufs, UPIU_TRANSACTION_QUERY_REQ);

  if (UfsUtpWaitResponse(Ufs, UPIU_TRANSACTION_QUERY_REQ))
    return EFI_TIMEOUT;

  if (EFI_ERROR(UfsUtpCheckResult(Ufs)))
    return EFI_DEVICE_ERROR;

  UfsQueryGetData(Ufs, qry);

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
UfsUtpQueryRetry (struct UfsHost *Ufs, QueryIndex qry, UINT32 lun)
{
  EFI_STATUS Status;
  int retries;
  for (retries = QUERY_REQ_RETRIES; retries > 0; retries--)
  {
    Status = UfsUtpQueryProcess (Ufs, qry, lun);
    if (Status == EFI_SUCCESS) break;
    DEBUG ((DEBUG_WARN, "UFS Query retry (left=%d)\n", retries - 1));
  }
  return Status;
}

STATIC
EFI_STATUS
UfsUtpCmdProcess (
  struct UfsHost *Ufs,
  ScsiCommandMeta *Cmd)
{
  UfsUtpInit(Ufs, 0);
  Ufs->ScsiCmd = Cmd;
  Ufs->Lun = Cmd->Lun;

  UfsWriteCmdUcd(Ufs);
  UfsMapSg(Ufs);
  if (UfsWriteUtrd(Ufs, UPIU_TRANSACTION_COMMAND))
    return EFI_DEVICE_ERROR;

  MemoryFence();

  UfsUtpSend(Ufs, UPIU_TRANSACTION_COMMAND);

  if (UfsUtpWaitResponse(Ufs, UPIU_TRANSACTION_COMMAND))
    return EFI_TIMEOUT;

  return UfsUtpCheckResult(Ufs);
}

STATIC
EFI_STATUS
UfsUtpNopProcess (struct UfsHost *Ufs)
{
  UfsUtpInit(Ufs, 0);
  MemoryFence();

  UfsUtpSend(Ufs, UPIU_TRANSACTION_NOP_OUT);

  if (UfsUtpWaitResponse(Ufs, UPIU_TRANSACTION_NOP_OUT))
    return EFI_TIMEOUT;

  return UfsUtpCheckResult(Ufs);
}

STATIC
EFI_STATUS 
UfsEndBootMode (struct UfsHost *Ufs)
{
  int Retry = 100;
  UINT8 Flag;
  EFI_STATUS Status;

  for (int i = 0; i < NOP_OUT_RETRY; i++) {
    Status = UfsUtpNopProcess (Ufs);
    if (Status == EFI_SUCCESS) break;
  }
  if (Status != EFI_SUCCESS) {
    DEBUG ((EFI_D_ERROR, "UFS NOP OUT failed\n"));
    return Status;
  }

  Ufs->Flags.Array[UPIU_FLAG_ID_DEVICEINIT] = 1;
  Status = UfsUtpQueryRetry (Ufs, FLAG_W_FDEVICEINIT, 0);
  if (Status != EFI_SUCCESS)
    return Status;

  do {
    Status = UfsUtpQueryRetry (Ufs, FLAG_R_FDEVICEINIT, 0);
    if (Status != EFI_SUCCESS)
      return Status;
  
    Flag = Ufs->Flags.Array[UPIU_FLAG_ID_DEVICEINIT];
    if (!Flag)
      break;

    MicroSecondDelay (1000);
  } while (Retry-- > 0);

  if (Flag)
  {
    DEBUG ((DEBUG_INFO, "UFS fDeviceInit timeout\n"));
    return EFI_DEVICE_ERROR;
  }
  return EFI_SUCCESS;
}

STATIC 
EFI_STATUS
UfsCheck2Lane (struct UfsHost *Ufs)
{
  struct UfsUicCmd TxGet = {UIC_CMD_DME_GET, UIC_ARG_MIB (0x1561), 0, 0}; /* PA_ConnectedTxDataLanes */
  struct UfsUicCmd RxGet = {UIC_CMD_DME_GET, UIC_ARG_MIB (0x1581), 0, 0}; /* PA_ConnectedRxDataLanes */
  struct UfsUicCmd TxSet, RxSet;
  INT32 tx, rx;

  Ufs->UicCmd = &TxGet;
  if (UfsSendUicCmd (Ufs)) return -1;
  tx = (int)TxGet.Arg3;

  Ufs->UicCmd = &RxGet;
  if (UfsSendUicCmd (Ufs)) return -1;
  rx = (int)RxGet.Arg3;

  Ufs->CalParam->ConnectedTxLane = (UINT8)tx;
  Ufs->CalParam->ConnectedRxLane = (UINT8)rx;

  DEBUG ((EFI_D_INFO, "UFS connected TX=%d RX=%d\n", tx, rx));

  /* DME_SET PA_ActiveTxDataLanes / PA_ActiveRxDataLanes */
  TxSet.uiccmdr = UIC_CMD_DME_SET;
  TxSet.Arg1 = UIC_ARG_MIB (0x1560);
  TxSet.Arg2 = 0;
  TxSet.Arg3 = (UINT32)tx;

  RxSet.uiccmdr = UIC_CMD_DME_SET;
  RxSet.Arg1 = UIC_ARG_MIB (0x1580);
  RxSet.Arg2 = 0;
  RxSet.Arg3 = (UINT32)rx;

  Ufs->UicCmd = &TxSet;
  if (UfsSendUicCmd(Ufs)) return EFI_DEVICE_ERROR;

  Ufs->UicCmd = &RxSet;
  if (UfsSendUicCmd(Ufs)) return EFI_DEVICE_ERROR;

  Ufs->PmdCxt.Lane = (UINT8)tx;
  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
UfsRefClkSetup (struct UfsHost *Ufs)
{
  EFI_STATUS Status;

  Status = UfsUtpQueryRetry(Ufs, ATTR_R_REFCLKFREQ, 0);
  if (EFI_ERROR(Status)) return Status;

  if (Ufs->Attributes.Array[UPIU_ATTR_ID_REFCLKFREQ] != 0x1)
  {
    Ufs->Attributes.Array[UPIU_ATTR_ID_REFCLKFREQ] = 0x01;
    Status = UfsUtpQueryRetry(Ufs, ATTR_W_REFCLKFREQ, 0);
  }

  return Status;
}

STATIC
EFI_STATUS
UfsPreGearChange (
  struct UfsHost *Ufs,
  struct UicPwrMode *Pmd
)
{
  Ufs->CalParam->Pmd = Pmd;
  if (UfsCalPrePmc (Ufs->CalParam) != UFS_CAL_NO_ERROR)
    return EFI_DEVICE_ERROR;
  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
UfsPostGearChange (struct UfsHost *Ufs)
{
  if (UfsCalPostPmc (Ufs->CalParam) != UFS_CAL_NO_ERROR)
    return EFI_DEVICE_ERROR;
  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
UfsPreSetup (
  struct UfsHost *Ufs
)
{
  UINT32 Register;
  DEBUG((EFI_D_INFO, "UFS pre-setup\n"));
  
  /* UFS_PHY_CONTROL : 1 = Isolation bypassed, PMU MPHY ON */
  Register = MmioRead32((UINTN)Ufs->PhyIsoAddr);
  if (!(Register & 1))
  {
    Register |= 1;
    MmioWrite32((UINTN)Ufs->PhyIsoAddr, Register);
  }

  /* VS_SW_RST */
  if((MmioRead32((UINTN)(Ufs->VsAddr + VS_FORCE_HCS)) >> 4) & 0xF)
    MmioWrite32((UINTN)(Ufs->VsAddr + VS_FORCE_HCS), 0);

  MmioWrite32((UINTN)(Ufs->VsAddr + VS_SW_RST), 3);
  while (MmioRead32((UINTN)(Ufs->VsAddr + VS_SW_RST))); // Wait for reset

  /* VENDOR_SPECIFIC_IS[20] : clear UFS_IDLE_Indicator bit (if UFS_LINK is reset, this bit is asserted) */
  Register = MmioRead32((UINTN)(Ufs->VsAddr + VS_IS));
  if ((Register >> 20) & 1)
    MmioWrite32((UINTN)(Ufs->VsAddr + VS_IS), Register);

  UfsDevicePower(Ufs, 1);
  MicroSecondDelay(1000);
  UfsDeviceReset(Ufs);

  // TODO: Handle broken HCE for boards that have it
  // Send reset host command and enable command when that quirk is present.

  MmioWrite32((UINTN)(Ufs->IoAddr + REG_CONTROLLER_ENABLE), 1);
  while(!(MmioRead32((UINTN)(Ufs->IoAddr + REG_CONTROLLER_ENABLE)) & 1))
    MicroSecondDelay(1);

  /* ctrl refclkout */
  MmioWrite32((UINTN)(Ufs->VsAddr + VS_CLKSTOP_CTRL), MmioRead32((UINTN)(Ufs->VsAddr + VS_CLKSTOP_CTRL)) & ~(1 << 4));

  /* Set clock gating */
  MmioWrite32((UINTN)(Ufs->VsAddr + VS_FORCE_HCS), 0x0DE0);
  MmioWrite32((UINTN)(Ufs->VsAddr + VS_UFS_ACG_DISABLE), MmioRead32((UINTN)(Ufs->VsAddr + VS_UFS_ACG_DISABLE)) | 1);

  ZeroMem (Ufs->CmdDescAddr, UFS_NUTRS * sizeof (struct UfsCmdDesc));
  ZeroMem (Ufs->UtrdAddr, UFS_NUTRS * sizeof (struct UfsUtrd));

  Ufs->UtrdAddr->cmd_desc_addr_l = (UINT32)(UINTN)Ufs->CmdDescAddr;
  Ufs->UtrdAddr->cmd_desc_addr_h = (UINT32)((UINTN)Ufs->CmdDescAddr >> 32);
  Ufs->UtrdAddr->rsp_upiu_off = OFFSET_OF (struct UfsCmdDesc, ResponseUpiu);
  Ufs->UtrdAddr->rsp_upiu_len = ALIGNED_UPIU_SIZE;

  MmioWrite32((UINTN)(Ufs->IoAddr + REG_UTP_TASK_REQ_LIST_BASE_L), (UINT32)(UINTN)Ufs->UtmrdAddr);
  MmioWrite32((UINTN)(Ufs->IoAddr + REG_UTP_TASK_REQ_LIST_BASE_H), (UINT32)((UINTN)Ufs->UtmrdAddr >> 32));

  MmioWrite32((UINTN)(Ufs->IoAddr + REG_UTP_TRANSFER_REQ_LIST_BASE_L), (UINT32)(UINTN)Ufs->UtrdAddr);
  MmioWrite32((UINTN)(Ufs->IoAddr + REG_UTP_TRANSFER_REQ_LIST_BASE_H), (UINT32)((UINTN)Ufs->UtrdAddr >> 32));

  // TODO: cport
	MmioWrite32((UINTN)(Ufs->VsAddr + 0x0114), 0x22);
	MmioWrite32((UINTN)(Ufs->VsAddr + 0x0110), 1);

  return EFI_SUCCESS;
}

static void UfsVendorSetup (struct UfsHost *Ufs)
{
  MmioWrite32((UINTN)(Ufs->VsAddr + VS_DATA_REORDER), 0xA);
  MmioWrite32((UINTN)(Ufs->IoAddr + REG_UTP_TASK_REQ_LIST_RUN_STOP), 1);
  MmioWrite32((UINTN)(Ufs->IoAddr + REG_UTP_TRANSFER_REQ_LIST_RUN_STOP), 1);
  MmioWrite32((UINTN)(Ufs->VsAddr + VS_TXPRDT_ENTRY_SIZE), UFS_SG_BLOCK_SIZE_BIT);
  MmioWrite32((UINTN)(Ufs->VsAddr + VS_RXPRDT_ENTRY_SIZE), UFS_SG_BLOCK_SIZE_BIT);
  MmioWrite32((UINTN)(Ufs->VsAddr + VS_UTRL_NEXUS_TYPE), 0xFFFFFFFF);
  MmioWrite32((UINTN)(Ufs->VsAddr + VS_UMTRL_NEXUS_TYPE), 0xFFFFFFFF);
}

STATIC
EFI_STATUS
UfsPmcCommon (
  struct UfsHost *Ufs,
  struct UicPwrMode *pmd)
{
  UINT32 Register;
  struct UfsUicCmd pmc = {UIC_CMD_DME_SET, (0x1571U << 16), 0, UFS_RXTX_POWER_MODE};

  struct UfsUicCmd cmd[] =
  {
    {UIC_CMD_DME_SET, (0x1583U << 16), 0, Ufs->GearMode}, /* PA_RxGear       */
    {UIC_CMD_DME_SET, (0x1568U << 16), 0, Ufs->GearMode}, /* PA_TxGear       */
    {UIC_CMD_DME_SET, (0x1580U << 16), 0, pmd->Lane}, /* PA_ActiveRxLanes*/
    {UIC_CMD_DME_SET, (0x1560U << 16), 0, pmd->Lane}, /* PA_ActiveTxLanes*/
    {UIC_CMD_DME_SET, (0x1584U << 16), 0, 1}, /* PA_RxTermination*/
    {UIC_CMD_DME_SET, (0x1569U << 16), 0, 1}, /* PA_TxTermination*/
    {UIC_CMD_DME_SET, (0x156AU << 16), 0, UFS_RATE},       /* PA_HSSeries     */
    {0, 0, 0, 0},
  };

  for (INTN i = 0; cmd[i].uiccmdr; i++)
  {
    Ufs->UicCmd = &cmd[i];
    if (UfsSendUicCmd (Ufs)) return EFI_DEVICE_ERROR;
  }

  Ufs->UicCmd = &pmc;
  if (UfsSendUicCmd (Ufs)) return EFI_DEVICE_ERROR;

  Register = MmioRead32((UINTN)(Ufs->IoAddr + REG_CONTROLLER_STATUS));
  if (UPMCRS (Register) != PWR_LOCAL) {
    DEBUG ((DEBUG_ERROR, "UFS Gear change failed, UPMCRS=0x%x\n", UPMCRS (Register)));
    return EFI_DEVICE_ERROR;
  }
  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
UfsInitInterface (
  struct UfsHost *Ufs
)
{
  struct UfsUicCmd LinkCmd = {UIC_CMD_DME_LINK_STARTUP, 0, 0, 0};
  struct UfsUicCmd LaneCmd = {UIC_CMD_DME_GET, (0x1540U << 16), 0, 0};
  struct UicPwrMode *Pmd = &Ufs->PmdCxt;
  EFI_STATUS Status;

  DEBUG((EFI_D_INFO, "UFS Host interface init\n"));

  Status = UfsPreSetup(Ufs);
  if (EFI_ERROR(Status))
  {
    DEBUG((EFI_D_ERROR, "UFS pre-setup failed\n"));
    return Status;
  }

  DEBUG((EFI_D_INFO, "UFS pre-setup pass\n"));

  // UfsPreVendorSetup stubbed out cause its empty on all plats

  DEBUG((EFI_D_INFO, "Getting UFS Lanes\n"));

  Ufs->UicCmd = &LaneCmd;
  if(UfsSendUicCmd(Ufs))
  {
    DEBUG((EFI_D_ERROR, "UFS get lane count failed\n"));
    return EFI_DEVICE_ERROR;
  }

  DEBUG((EFI_D_INFO, "UFS get lane count pass\n"));
  DEBUG((EFI_D_INFO, "UFS lane count: %d\n", LaneCmd.Arg3));

  Ufs->CalParam->AvailableLane = (UINT8)LaneCmd.Arg3;

  Status = UfsPreLink(Ufs, Ufs->CalParam->AvailableLane);
  if (EFI_ERROR(Status))
  {
    DEBUG((EFI_D_ERROR, "UFS pre-link failed\n"));
    return Status;
  }

  DEBUG((EFI_D_INFO, "UFS pre-link pass\n"));

  Ufs->UicCmd = &LinkCmd;
  if(UfsSendUicCmd(Ufs))
  {
    DEBUG((EFI_D_ERROR, "UFS link startup failed\n"));
    return EFI_DEVICE_ERROR;
  }

  DEBUG ((EFI_D_INFO, "UFS link is up\n"));

  Status = UfsUpdateMaxGear(Ufs);
  if (EFI_ERROR(Status))
  {
    DEBUG((EFI_D_ERROR, "UFS update max gear failed\n"));
    return Status;
  }

  DEBUG((EFI_D_INFO, "UFS update max gear pass\n"));

  if(UfsCalPostLink(Ufs->CalParam) != UFS_CAL_NO_ERROR)
  {
    DEBUG((EFI_D_ERROR, "UFS post-link calibration failed\n"));
    return EFI_DEVICE_ERROR;
  }

  DEBUG((EFI_D_INFO, "UFS post-link calibration pass\n"));

  Status = UfsUpdateActiveLane(Ufs);
  if (EFI_ERROR(Status))
  {
    DEBUG((EFI_D_ERROR, "UFS update active lane failed\n"));
    return Status;
  }
  
  DEBUG((EFI_D_INFO, "UFS Active lanes updated\n"));

  UfsVendorSetup(Ufs);

  DEBUG((EFI_D_INFO, "UFS vendor setup done\n"));

  Status = UfsEndBootMode(Ufs);
  if (EFI_ERROR(Status))
  {
    DEBUG((EFI_D_ERROR, "UFS end boot mode failed\n"));
    return Status;
  }

  DEBUG((EFI_D_INFO, "UFS end boot mode done\n"));
  DEBUG((EFI_D_INFO, "UFS device initialised\n"));

  Status = UfsCheck2Lane(Ufs);
  if (EFI_ERROR(Status))
  {
    DEBUG((EFI_D_ERROR, "UFS 2 lane check failed\n"));
    return Status;
  } 

  DEBUG((EFI_D_INFO, "UFS 2 lane check pass\n"));

  Status = UfsRefClkSetup(Ufs);
  if (EFI_ERROR(Status))
  {
    DEBUG((EFI_D_ERROR, "UFS RefClk setup failed\n"));
    return Status;
  }

  DEBUG((EFI_D_INFO, "UFS RefClk setup pass\n"));

  Status = UfsUtpQueryRetry (Ufs, DESC_R_DEVICE_DESC, 0);
  if (EFI_ERROR(Status))
  {
    DEBUG((EFI_D_ERROR, "UFS read device descriptor failed\n"));
    return Status;
  }

  DEBUG((EFI_D_INFO, "UFS device descriptor read pass\n"));

  Status = UfsUtpQueryRetry (Ufs, DESC_R_GEOMETRY_DESC, 0);
  if (EFI_ERROR(Status))
  {
    DEBUG((EFI_D_ERROR, "UFS read geometry descriptor failed\n"));
    return Status;
  }

  DEBUG((EFI_D_INFO, "UFS geometry descriptor read pass\n"));

  Pmd->Gear = Ufs->GearMode;
  Pmd->Mode = UFS_POWER_MODE;
  Pmd->HsSeries = UFS_RATE;

  Status = UfsPreGearChange (Ufs, Pmd);
  if (EFI_ERROR (Status))
  {
    DEBUG((EFI_D_ERROR, "UFS pre-gear change failed\n"));
    return Status;
  }

  Status = UfsPmcCommon (Ufs, Pmd);
  if (EFI_ERROR (Status))
  {
    DEBUG((EFI_D_ERROR, "UFS power mode change failed\n"));
    return Status;
  }

  DEBUG((EFI_D_INFO, "UFS power mode change pass\n"));

  Status = UfsUpdateActiveLane(Ufs);
  if (EFI_ERROR(Status))
  {
    DEBUG((EFI_D_ERROR, "UFS update active lane failed\n"));
    return Status;
  }
  
  DEBUG((EFI_D_INFO, "UFS Active lanes updated\n"));

  Status = UfsPostGearChange (Ufs);
  if (EFI_ERROR (Status))
  {
    DEBUG((EFI_D_ERROR, "UFS post-gear change failed\n"));
    return Status;
  }

  DEBUG((EFI_D_INFO, "UFS post-gear change pass\n"));
  DEBUG((EFI_D_INFO, "UFS Power mode G%d M%d L%d Series%d\n", Pmd->Gear, Pmd->Mode, Pmd->Lane, Pmd->HsSeries));
  DEBUG((EFI_D_INFO, "UFS initialization fully complete, good night!\n"));

  return EFI_SUCCESS;
}

struct UfsHost *UfsAllocHost (VOID)
{
  struct UfsHost *Ufs;
  struct UfsCmdDesc *CmdDesc = NULL;
  struct UfsUtrd *Utrd = NULL;
  struct UfsUtmrd *Utmrd = NULL;
  struct UfsCalParam *Cal = NULL;

  Ufs = AllocateZeroPool (sizeof (struct UfsHost));
  if (!Ufs) return NULL;

  Cal = AllocateZeroPool (sizeof (struct UfsCalParam));
  if (!Cal) goto Error;

  Ufs->CalParam = Cal;

  CmdDesc = AllocateAlignedPages(EFI_SIZE_TO_PAGES (UFS_NUTRS * sizeof (struct UfsCmdDesc)), SIZE_4KB);
  if (!CmdDesc) goto Error;

  ZeroMem (CmdDesc, UFS_NUTRS * sizeof (struct UfsCmdDesc));
  Ufs->CmdDescAddr = CmdDesc;

  Utrd = AllocateAlignedPages(EFI_SIZE_TO_PAGES (UFS_NUTRS * sizeof (struct UfsUtrd)), SIZE_4KB);
  if (!Utrd) goto Error;

  ZeroMem (Utrd, UFS_NUTRS * sizeof (struct UfsUtrd));
  Ufs->UtrdAddr = Utrd;

  Utmrd = AllocateAlignedPages(EFI_SIZE_TO_PAGES (sizeof (struct UfsUtmrd)), SIZE_4KB);
  if (!Utmrd) goto Error;

  ZeroMem (Utmrd, sizeof (struct UfsUtmrd));
  Ufs->UtmrdAddr = Utmrd;

  return Ufs;

Error:
  FreePool (Ufs);
  if (Utmrd) FreeAlignedPages (Utmrd, EFI_SIZE_TO_PAGES (sizeof (struct UfsUtmrd)));
  if (Utrd) FreeAlignedPages (Utrd, EFI_SIZE_TO_PAGES (UFS_NUTRS * sizeof (struct UfsUtrd)));
  if (CmdDesc) FreeAlignedPages (CmdDesc, EFI_SIZE_TO_PAGES (UFS_NUTRS * sizeof (struct UfsCmdDesc)));
  if (Cal) FreePool (Cal);
  return NULL;
}

STATIC
EFI_STATUS
UfsInitHost (
  struct UfsHost *Ufs
)
{
  EFI_STATUS Status;
  
  DEBUG((DEBUG_INFO, "UFS Host init\n"));

	Ufs->UfsCmdTimeout = UTP_CMD_TIMEOUT;
	Ufs->UicCmdTimeout = UIC_CMD_TIMEOUT;

  Status = UfsBoardInit(Ufs);
  if (EFI_ERROR(Status))
  {
    DEBUG((EFI_D_ERROR, "UFS board init failed\n"));
    return Status;
  }
  return UfsInitCal(Ufs);
}

EFI_STATUS
UfsRequestSense (
  struct UfsHost *Ufs,
  UINT32 Lun
)
{
  ScsiCommandMeta Cmd;
  UINT8 *Buf;

  Buf = AllocateAlignedPages (1, SIZE_4KB);
  if (!Buf)
    return EFI_OUT_OF_RESOURCES;

  ZeroMem (Buf, SIZE_4KB);

  ZeroMem (&Cmd, sizeof (Cmd));
  Cmd.Cdb[0] = SCSI_OP_REQUEST_SENSE;
  Cmd.Cdb[4] = 18;
  Cmd.Buf = Buf;
  Cmd.DataLen = 18;
  Cmd.Lun = Lun;

  UfsUtpCmdProcess(Ufs, &Cmd);

  DEBUG ((EFI_D_INFO, "UFS: LUN%d REQUEST SENSE sense_key=0x%02x ASC=0x%02x ASCQ=0x%02x\n", Lun, Buf[2] & 0x0F, Buf[12], Buf[13]));

  FreeAlignedPages(Buf, 1);
  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
UfsReadCapacity (
  struct UfsHost *Ufs,
  UINT32 Lun,
  UINT64 *BlkCnt,
  UINT32 *BlkSize
)
{
  ScsiCommandMeta Cmd;
  UINT8 *Buf;

  Buf = AllocateAlignedPages (1, SIZE_4KB);
  if (!Buf)
    return EFI_OUT_OF_RESOURCES;

  ZeroMem (Buf, SIZE_4KB);
  ZeroMem (&Cmd, sizeof (Cmd));

  Cmd.Cdb[0] = SCSI_OP_READ_CAPACITY;
  Cmd.Buf = Buf;
  Cmd.DataLen = 8;
  Cmd.Lun = Lun;

  if (UfsUtpCmdProcess (Ufs, &Cmd))
  {
    FreeAlignedPages (Buf, 1);
    return EFI_DEVICE_ERROR;
  }

  *BlkCnt  = (UINT64)(((UINT32)Buf[0] << 24) | ((UINT32)Buf[1] << 16) | ((UINT32)Buf[2] <<  8) |  (UINT32)Buf[3]) + 1;
  *BlkSize = ((UINT32)Buf[4] << 24) | ((UINT32)Buf[5] << 16) | ((UINT32)Buf[6] <<  8) |  (UINT32)Buf[7];

  FreeAlignedPages (Buf, 1);
  return EFI_SUCCESS;
}

EFI_STATUS
UfsRead (
  struct UfsHost *Ufs,
  UINT32 Lun,
  UINT32 Lba,
  UINTN BlkCnt,
  UINT32 BlkSize,
  VOID *Buf)
{
  EFI_STATUS Status;
  ScsiCommandMeta Cmd;
  UINTN MaxBlksPerChunk;
  UINTN ChunkBlks;
  UINT8 *BufPtr;

  MaxBlksPerChunk = UFS_MAX_XFER_LEN / BlkSize;
  BufPtr = (UINT8 *)Buf;

  while (BlkCnt > 0) {
    ChunkBlks = MIN (BlkCnt, MaxBlksPerChunk);

    ZeroMem (&Cmd, sizeof (Cmd));

    Cmd.Cdb[0] = SCSI_OP_READ_10;
    Cmd.Cdb[1] = 0;
    Cmd.Cdb[2] = (UINT8)((Lba >> 24) & 0xFF);
    Cmd.Cdb[3] = (UINT8)((Lba >> 16) & 0xFF);
    Cmd.Cdb[4] = (UINT8)((Lba >> 8) & 0xFF);
    Cmd.Cdb[5] = (UINT8)(Lba & 0xFF);
    Cmd.Cdb[6] = 0;
    Cmd.Cdb[7] = (UINT8)((ChunkBlks >> 8) & 0xFF);
    Cmd.Cdb[8] = (UINT8)(ChunkBlks & 0xFF);
    Cmd.Cdb[9] = 0;
    Cmd.Buf = BufPtr;
    Cmd.DataLen = (UINT32)(ChunkBlks * BlkSize);
    Cmd.Lun = Lun;

    Status = UfsUtpCmdProcess (Ufs, &Cmd);
    if (EFI_ERROR (Status))
    {
      return Status;
    }

    Lba += ChunkBlks;
    BufPtr += ChunkBlks * BlkSize;
    BlkCnt -= ChunkBlks;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
UfsWrite (
  struct UfsHost *Ufs,
  UINT32 Lun,
  UINT32 Lba,
  UINTN BlkCnt,
  UINT32 BlkSize,
  VOID *Buf)
{
  EFI_STATUS Status;
  ScsiCommandMeta Cmd;
  UINTN MaxBlksPerChunk;
  UINTN ChunkBlks;
  UINT8 *BufPtr;

  MaxBlksPerChunk = UFS_MAX_XFER_LEN / BlkSize;
  BufPtr = (UINT8 *)Buf;

  while (BlkCnt > 0) {
    ChunkBlks = MIN (BlkCnt, MaxBlksPerChunk);

    ZeroMem (&Cmd, sizeof (Cmd));

    Cmd.Cdb[0] = SCSI_OP_WRITE_10;
    Cmd.Cdb[1] = 0;
    Cmd.Cdb[2] = (UINT8)((Lba >> 24) & 0xFF);
    Cmd.Cdb[3] = (UINT8)((Lba >> 16) & 0xFF);
    Cmd.Cdb[4] = (UINT8)((Lba >> 8) & 0xFF);
    Cmd.Cdb[5] = (UINT8)(Lba & 0xFF);
    Cmd.Cdb[6] = 0;
    Cmd.Cdb[7] = (UINT8)((ChunkBlks >> 8) & 0xFF);
    Cmd.Cdb[8] = (UINT8)(ChunkBlks & 0xFF);
    Cmd.Cdb[9] = 0;
    Cmd.Buf = BufPtr;
    Cmd.DataLen = (UINT32)(ChunkBlks * BlkSize);
    Cmd.Lun = Lun;

    Status = UfsUtpCmdProcess(Ufs, &Cmd);
    if (EFI_ERROR (Status))
    {
      return Status;
    }

    Lba += ChunkBlks;
    BufPtr += ChunkBlks * BlkSize;
    BlkCnt -= ChunkBlks;
  }

  return EFI_SUCCESS;
}

STATIC EFI_STATUS EFIAPI
UfsReadBlocks (
  IN  EFI_BLOCK_IO_PROTOCOL *This,
  IN  UINT32                 MediaId,
  IN  EFI_LBA                Lba,
  IN  UINTN                  BufferSize,
  OUT VOID                  *Buffer)
{
  UFS_LUN_DEV *Dev = UFS_LUN_FROM_BLOCKIO(This);

  if (!Buffer)                       return EFI_INVALID_PARAMETER;
  if (MediaId != Dev->Media.MediaId) return EFI_MEDIA_CHANGED;
  if (BufferSize == 0)               return EFI_SUCCESS;
  if (BufferSize % Dev->Media.BlockSize) return EFI_BAD_BUFFER_SIZE;

  return UfsRead(
    Dev->Ufs,
    Dev->Lun,
    (UINT32)Lba,
    BufferSize / Dev->Media.BlockSize,
    Dev->Media.BlockSize,
    Buffer
  );
}

STATIC EFI_STATUS EFIAPI
UfsWriteBlocks (
  IN EFI_BLOCK_IO_PROTOCOL *This,
  IN UINT32                 MediaId,
  IN EFI_LBA                Lba,
  IN UINTN                  BufferSize,
  IN VOID                  *Buffer)
{
  UFS_LUN_DEV *Dev = UFS_LUN_FROM_BLOCKIO(This);

  if (!Buffer)                       return EFI_INVALID_PARAMETER;
  if (MediaId != Dev->Media.MediaId) return EFI_MEDIA_CHANGED;
  if (Dev->Media.ReadOnly)           return EFI_WRITE_PROTECTED;
  if (BufferSize == 0)               return EFI_SUCCESS;
  if (BufferSize % Dev->Media.BlockSize) return EFI_BAD_BUFFER_SIZE;

  return UfsWrite(
    Dev->Ufs,
    Dev->Lun,
    (UINT32)Lba,
    BufferSize / Dev->Media.BlockSize,
    Dev->Media.BlockSize,
    Buffer
  );
}

STATIC EFI_STATUS EFIAPI
UfsFlushBlocks (IN EFI_BLOCK_IO_PROTOCOL *This)
{
  return EFI_SUCCESS;
}

STATIC EFI_STATUS EFIAPI
UfsBlockReset (
  IN EFI_BLOCK_IO_PROTOCOL *This,
  IN BOOLEAN ExtendedVerification
)
{
  return EFI_SUCCESS;
}

STATIC
EXYNOS_UFS_DEVICE_PATH *
UfsBuildDevicePath (UINT8 Lun)
{
  EXYNOS_UFS_DEVICE_PATH *Dp = AllocateZeroPool (sizeof (EXYNOS_UFS_DEVICE_PATH));
  if (!Dp)
    return NULL;

  Dp->VendorDp.Header.Type    = HARDWARE_DEVICE_PATH;
  Dp->VendorDp.Header.SubType = HW_VENDOR_DP;
  SetDevicePathNodeLength (&Dp->VendorDp.Header, sizeof (VENDOR_DEVICE_PATH) + sizeof (UINT8));
  CopyMem (&Dp->VendorDp.Guid, &gExynosUfsGuid, sizeof (EFI_GUID));
  Dp->Lun = Lun;

  SetDevicePathEndNode (&Dp->End);
  return Dp;
}

EFI_STATUS
EFIAPI
InitUfsDriver (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status;
  struct UfsHost *Ufs = UfsAllocHost();

  Status = gBS->LocateProtocol (&gEfiChipDataProtocolGuid, NULL, (VOID *)&mChipDataProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate Chip Data Protocol! Status = %r\n", Status));
    return Status;
  }

  if (!Ufs) {
    DEBUG((EFI_D_ERROR, "Failed to allocate UFS host\n"));
    ASSERT(FALSE);
  }

  Status = UfsInitHost(Ufs);
  if (EFI_ERROR(Status)) {
    DEBUG((EFI_D_ERROR, "Failed to initialize UFS host\n"));
    return Status;
  }

  Status = UfsInitInterface(Ufs);
  if (EFI_ERROR(Status)) {
    DEBUG((EFI_D_ERROR, "Failed to initialize UFS interface\n"));
    return Status;
  }

  if (!UfsUtpQueryRetry (Ufs, ATTR_R_BOOTLUNEN, 0)) {
    DEBUG ((EFI_D_INFO, "UFS bBootLunEn=0x%x\n", Ufs->Attributes.Array[UPIU_ATTR_ID_BOOTLUNEN]));
  }

  for (UINT32 Lun = 0; Lun < 8; Lun++)
  {
    UINT64 BlkCnt;
    UINT32 BlkSize;
    UFS_LUN_DEV *Dev = AllocateZeroPool(sizeof(UFS_LUN_DEV));
    EXYNOS_UFS_DEVICE_PATH *Dp;
    EFI_HANDLE Handle = NULL;

    if (!Dev)
    {
      DEBUG((EFI_D_ERROR, "Failed to allocate UFS disk io struct\n"));
      return EFI_OUT_OF_RESOURCES;
    }

    gQueryParams[DESC_R_UNIT_DESC][3] = (UINT8)Lun;
    if (UfsUtpQueryRetry (Ufs, DESC_R_UNIT_DESC, Lun))
    {
      DEBUG((EFI_D_ERROR, "UFS LUN %d unit desc read failed\n", Lun));
    }

    if (!Ufs->UnitDesc[Lun].bLUEnable)
    {
      continue;
    }

    UfsRequestSense(Ufs, Lun);

    Status = UfsReadCapacity(Ufs, Lun, &BlkCnt, &BlkSize);
    if (EFI_ERROR(Status))
    {
      DEBUG((EFI_D_ERROR, "UFS LUN %d read capacity failed\n", Lun));
      continue;
    }
    
    Dev->Signature = UFS_LUN_SIGNATURE;
    Dev->Ufs = Ufs;
    Dev->Lun = Lun;
    Dev->Media.MediaId = 1;
    Dev->Media.RemovableMedia = FALSE;
    Dev->Media.MediaPresent = TRUE;
    Dev->Media.LogicalPartition = FALSE;
    Dev->Media.ReadOnly = FALSE;
    Dev->Media.WriteCaching = FALSE;
    Dev->Media.BlockSize = BlkSize;
    Dev->Media.IoAlign = 0;
    Dev->Media.LastBlock = (BlkCnt > 0) ? (BlkCnt - 1) : 0;

    Dev->BlockIo.Revision = EFI_BLOCK_IO_PROTOCOL_REVISION2;
    Dev->BlockIo.Media = &Dev->Media;
    Dev->BlockIo.Reset = UfsBlockReset;
    Dev->BlockIo.ReadBlocks = UfsReadBlocks;
    Dev->BlockIo.WriteBlocks = UfsWriteBlocks;
    Dev->BlockIo.FlushBlocks = UfsFlushBlocks;

    DEBUG ((EFI_D_INFO, "UFS Well known lun[%d]   %llu MB\n", Lun, (BlkCnt * BlkSize) / (1024 * 1024)));

    Dp = UfsBuildDevicePath((UINT8)Lun);
    if (!Dp)
    {
      FreePool(Dev);
      continue;
    }

    Status = gBS->InstallMultipleProtocolInterfaces(&Handle, &gEfiBlockIoProtocolGuid, &Dev->BlockIo, &gEfiDevicePathProtocolGuid, Dp, NULL);

    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "UFS LUN %d Failed to DiskIO Protocol!\n", Lun));
      ASSERT_EFI_ERROR (Status);
    }
    else
    {
      DEBUG ((EFI_D_ERROR, "UFS LUN %d DiskIO Protocol installed\n", Lun));
    }
  }

  return EFI_SUCCESS;
}