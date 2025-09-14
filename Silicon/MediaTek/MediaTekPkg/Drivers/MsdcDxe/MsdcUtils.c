#include "MsdcDxe.h"

VOID MsdcWrite (
  IN MSDC_PRIVATE_DATA *Private,
  IN UINT32 Offset,
  IN UINT32 Value
  )
{
  MmioWrite32 (Private->MsdcMmioReg + Offset, Value);
}

VOID MsdcRead (
  IN MSDC_PRIVATE_DATA *Private,
  IN  UINT32  Offset,
  OUT UINT32 *Value
  )
{
  *Value = MmioRead32 (Private->MsdcMmioReg + Offset);
}

VOID MsdcTopWrite (
  IN MSDC_PRIVATE_DATA *Private,
  IN UINT32 Offset,
  IN UINT32 Value
  )
{
  MmioWrite32 (Private->TopMmioReg + Offset, Value);
}

VOID MsdcTopRead (
  IN MSDC_PRIVATE_DATA *Private,
  IN  UINT32  Offset,
  OUT UINT32 *Value
  )
{
  *Value = MmioRead32 (Private->TopMmioReg + Offset);
}

VOID MsdcSetBits (
  IN MSDC_PRIVATE_DATA *Private,
  IN UINT32 Offset,
  IN UINT32 BitMask
  )
{
  UINT32 Reg;
  MsdcRead (Private, Offset, &Reg);
  Reg |= BitMask;
  MsdcWrite (Private, Offset, Reg);
}

VOID MsdcClrSetBits (
  IN MSDC_PRIVATE_DATA *Private,
  IN UINT32 Offset,
  IN UINT32 BitMask,
  IN UINT32 BitMaskSet
  )
{
  UINT32 Reg;
  MsdcRead (Private, Offset, &Reg);
  Reg &= ~BitMask;
  Reg |= BitMaskSet;
  MsdcWrite (Private, Offset, Reg);
}

VOID MsdcClrBits (
  IN MSDC_PRIVATE_DATA *Private,
  IN UINT32 Offset,
  IN UINT32 BitMask
  )
{
  UINT32 Reg;
  MsdcRead (Private, Offset, &Reg);
  Reg &= ~BitMask;
  MsdcWrite (Private, Offset, Reg);
}

VOID MsdcTopSetBits (
  IN MSDC_PRIVATE_DATA *Private,
  IN UINT32 Offset,
  IN UINT32 BitMask
  )
{
  UINT32 Reg;
  MsdcTopRead (Private, Offset, &Reg);
  Reg |= BitMask;
  MsdcTopWrite (Private, Offset, Reg);
}

VOID MsdcTopClrSetBits (
  IN MSDC_PRIVATE_DATA *Private,
  IN UINT32 Offset,
  IN UINT32 BitMask,
  IN UINT32 BitMaskSet
  )
{
  UINT32 Reg;
  MsdcTopRead (Private, Offset, &Reg);
  Reg &= ~BitMask;
  Reg |= BitMaskSet;
  MsdcTopWrite (Private, Offset, Reg);
}

VOID MsdcTopClrBits (
  IN MSDC_PRIVATE_DATA *Private,
  IN UINT32 Offset,
  IN UINT32 BitMask
  )
{
  UINT32 Reg;
  MsdcTopRead (Private, Offset, &Reg);
  Reg &= ~BitMask;
  MsdcTopWrite (Private, Offset, Reg);
}