#ifndef _PLATFORM_REGISTERS_H_
#define _PLATFORM_REGISTERS_H_

//
// GIC Region
//
#define GICR_BASE        FixedPcdGet64 (PcdGicRedistributorsBase)
#define GICR_STRIDE_SIZE 0x20000

//
// GIC Registers
//
#define GICR_WAKER             0x14
#define GICR_WAKER_CURRENT_CPU GICR_BASE + GICR_WAKER
#define GICR_WAKER_CPU(x)      GICR_BASE + (x * GICR_STRIDE_SIZE)  + GICR_WAKER

//
// GIC Wake Up
//
#define GIC_WAKER_PROCESSORSLEEP 2

#endif /* _PLATFORM_REGISTERS_H_ */