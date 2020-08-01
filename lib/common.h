#ifndef STM32_COMMON_H
#define STM32_COMMON_H

#define __RMM(addr)  (*(volatile uint32_t *)((uintptr_t)addr))
#define __RMMB(addr) (*(volatile uint8_t *)((uintptr_t)addr))

#endif
