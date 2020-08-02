#include "Interrupts.h"
#include "common.h"

#define NVIC_BASE 0xE000E100u //Programming manual, pag 128
#define NVIC_ISER0_ADDR (NVIC_BASE + 0x000u)
#define NVIC_IPR0_ADDR  (NVIC_BASE + 0x300u)

void Interrupts::setPriority(Interrupts::Interrupt interrupt, Interrupts::Priority priority) {
    //Set the interrupt priority (Programming manual rev6, pag 125)
    //Find the IPRx register
    if (priority == Priority::Default)
        return;

    __RMMB(NVIC_IPR0_ADDR + static_cast<uint8_t>(interrupt)) = (uint8_t) (static_cast<uint8_t>(priority) << 4u);
}

void Interrupts::enable(Interrupts::Interrupt interrupt) {
    auto irNumber = static_cast<uint8_t>(interrupt);

    //Enable the interrupt
    uint8_t iserOffset = (uint8_t) (irNumber >> 3u) & ~0b11u;
    uint8_t iserBitOffset = irNumber & 0b11111u;

    __RMM(NVIC_ISER0_ADDR + iserOffset) |= (uint32_t) (1u << iserBitOffset);
}
