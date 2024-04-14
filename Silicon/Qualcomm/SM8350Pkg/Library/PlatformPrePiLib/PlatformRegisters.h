#ifndef _PLATFORM_REGISTERS_H_
#define _PLATFORM_REGISTERS_H_

#define GICR_WAKER               0x0014
#define GICR_SIZE                0x20000
#define GICR_BASE                FixedPcdGet64(PcdGicRedistributorsBase)
#define GICR_WAKER_CURRENT_CPU   GICR_BASE + GICR_WAKER
#define GICR_WAKER_CPU(x)        GICR_BASE + (x * GICR_SIZE)  + GICR_WAKER

#define GIC_WAKER_PROCESSORSLEEP 2

#endif /* _PLATFORM_REGISTERS_H_ */