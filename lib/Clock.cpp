#include "Clock.h"
#include "common.h"

#define RCC_ADDR (0x40021000u)
#define RCC_CFGR  __RMM(RCC_ADDR + 0x4u)

#define HSI_CLOCK (8u)
#define HSE_CLOCK (8u)

uint8_t Clock::getSystemClockMHz() {
    //Read the SWS field of RCC_CFGR
    switch ((RCC_CFGR >> 2u) & 0b11u) {
        case 0b00u: //HSI
            return HSI_CLOCK;
        case 0b01u: //HSE
            return HSE_CLOCK;
    }

    //If we reach here, the PLL is used
    uint8_t pllMul = ((RCC_CFGR >> 18u) & 0xFu) + 2u; //PLLMUL

    if (((RCC_CFGR >> 16u) & 1u)) { //PLLSRC
        //HSE is used                       v-- PLLXTPRE
        return (HSE_CLOCK >> ((RCC_CFGR >> 17u) & 1u)) * pllMul;
    } else {
        //HSI is used
        return (HSI_CLOCK >> 1u) * pllMul;
    }
}

uint8_t Clock::getAHBClockMHz() {
    uint8_t clock = Clock::getSystemClockMHz();

    //Read the HPRE (AHB prescaler)
    uint8_t prescaler = (RCC_CFGR >> 4u) & 0xFu;

    if (prescaler & 0x8u)
        prescaler = (prescaler & 0x7u) + 1u;
    else
        prescaler = 0u;

    return clock >> prescaler;
}
