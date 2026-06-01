#ifndef __UFS_CAL_ADAPTER_LIB_H__
#define __UFS_CAL_ADAPTER_LIB_H__

typedef enum {
  UFS_CAL_NO_ERROR = 0,
  UFS_CAL_TIMEOUT,
  UFS_CAL_ERROR,
  UFS_CAL_INV_ARG,
} UfsCalError;

enum {
  TX_LANE_0 = 0,
  TX_LANE_1,
  TX_LANE_2,
  TX_LANE_3,
  RX_LANE_0,
  RX_LANE_1,
  RX_LANE_2,
  RX_LANE_3,
};

enum
{
  HOST_EMBD = 0,
  HOST_CARD = 1
};
enum
{
  GEAR_1 = 1,
  GEAR_2,
  GEAR_3,
  GEAR_4
};

struct UfsCalParam
{
  void *Host;
  UINT8 AvailableLane;
  UINT8 ConnectedTxLane;
  UINT8 ConnectedRxLane;
  UINT8 ActiveTxLane;
  UINT8 ActiveRxLane;
  UINT32 MclkRate;
  UINT8 Table;
  UINT8 Board;
  UINT8 EvtVer;
  UINT8 MaxGear;
  struct UicPwrMode *Pmd;
};

VOID   ufs_lld_dme_set (VOID *h, UINT32 addr, UINT32 val);
VOID   ufs_lld_dme_get (VOID *h, UINT32 addr, UINT32 *val);
VOID   ufs_lld_dme_peer_set (VOID *h, UINT32 addr, UINT32 val);
VOID   ufs_lld_pma_write (VOID *h, UINT32 val, UINT32 addr);
UINT32 ufs_lld_pma_read (VOID *h, UINT32 addr);
VOID   ufs_lld_unipro_write (VOID *h, UINT32 val, UINT32 addr);

#endif /* __UFS_CAL_ADAPTER_LIB_H__ */
