#ifndef _CLOCK_IMPL_LIB_H_
#define _CLOCK_IMPL_LIB_H_

typedef enum {
  ClkApMixed,
  ClkTopCkGen,
  ClkInfraCfgAo,
  ClkMax,
} MTK_CLOCK_CONTROLLER_TYPE;

typedef enum {
  ClockTypeFixed,
  ClockTypePll,
  ClockTypeMux,
  ClockTypeMuxGate,
  ClockTypeFactors,
  ClockTypeGate,
} MTK_CLOCK_TYPE;

typedef struct {
  UINT32 SetOffset;
  UINT32 ClearOffset;
  UINT32 StatusOffset;
  UINT8  GateShift;

  BOOLEAN Inverted;
  UINT32  Parent;
} MTK_GATE_DESC;

typedef struct {
  UINT32 MuxOffset;
  UINT32 SetOffset;
  UINT32 ClearOffset;
  UINT32 UpdateOffset;

  UINT8 MuxShift;
  UINT8 MuxWidth;
  UINT8 GateShift;
  UINT8 UpdateShift;

  CONST UINT32 *Parents;
  UINT32        ParentCount;
} MTK_MUX_GATE_DESC;

typedef struct {
  UINT32 BaseOffset;
  UINT32 PowerOffset;
  UINT32 EnMask;
  UINT32 ResetBarMask;

  UINT32 PostDivOffset;
  UINT8  PostDivShift;

  UINT64 FMax;
  UINT64 FMin;

  UINT32 PcwOffset;
  UINT8  PcwShift;
  UINT8  PcwBits;
  UINT8  PcwiBits;

  UINT32 Parent;
} MTK_PLL_DESC;

typedef struct {
  UINT32 Parent;
  UINT32 Mult;
  UINT32 Div;
} MTK_FACTOR_DESC;

typedef struct {
  UINT32                    Id;
  CONST CHAR8              *Name;
  MTK_CLOCK_CONTROLLER_TYPE Controller;

  MTK_CLOCK_TYPE Type;
  union {
    UINT64            Fixed;
    MTK_PLL_DESC      Pll;
    MTK_MUX_GATE_DESC MuxGate;
    MTK_FACTOR_DESC   Factor;
    MTK_GATE_DESC     Gate;
  };
} MTK_CLOCK_DESC;

extern MTK_CLOCK_DESC gClocks[];
extern UINTN          gClockCount;

#endif /* _CLOCK_IMPL_LIB_H_ */
