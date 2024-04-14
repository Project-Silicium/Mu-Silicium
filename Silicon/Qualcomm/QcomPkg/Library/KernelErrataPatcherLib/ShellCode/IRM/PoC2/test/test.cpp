#include <iostream>
#include <Windows.h>

#define SGIR_TGT_MASK     ((UINT64)0xffff)
#define SGIR_AFF1_SHIFT   16
#define SGIR_INTID_SHIFT  24
#define SGIR_INTID_MASK   ((UINT64)0xf)
#define SGIR_AFF2_SHIFT   32
#define SGIR_IRM_SHIFT    40
#define SGIR_IRM_MASK     ((UINT64)0x1)
#define SGIR_AFF3_SHIFT   48
#define SGIR_AFF_MASK     ((UINT64)0xff)

#define SGIR_IRM_TO_AFF     0
#define SGIR_IRM_TO_OTHERS  1

#define GICV3_SGIR_VALUE(_aff3, _aff2, _aff1, _intid, _irm, _tgt) \
  ((((UINT64) (_aff3) & SGIR_AFF_MASK) << SGIR_AFF3_SHIFT) | \
   (((UINT64) (_irm) & SGIR_IRM_MASK) << SGIR_IRM_SHIFT) | \
   (((UINT64) (_aff2) & SGIR_AFF_MASK) << SGIR_AFF2_SHIFT) | \
   (((_intid) & SGIR_INTID_MASK) << SGIR_INTID_SHIFT) | \
   (((_aff1) & SGIR_AFF_MASK) << SGIR_AFF1_SHIFT) | \
   ((_tgt) & SGIR_TGT_MASK))

void ArmGicV3SendNsG1Sgi(UINT64 SgiVal)
{
    printf_s("SGIValue=0x%p\n", SgiVal);
}

void Send(UINT64 SGI, UINT64 CurrentMpidr)
{
  if ((SGI & 0x10000000000LL) != 0) {
    UINT64 v1 = CurrentMpidr & 0xF00;
    UINT64 v2 = 0LL;
    UINT32 v3 = 8;
    do {
      if (v2 != (v1 >> 8)) {
        ArmGicV3SendNsG1Sgi((SGI & 0xFFFFFEFFFFFFFFFELL | 1) | (v2 << 16));
      }
      ++v2;
      --v3;
    } while (v3);
  }
  else {
    ArmGicV3SendNsG1Sgi(SGI);
  }
}

int main()
{
    Send(0x1000F000000, 0x81000200);
}