#include "irq.h"
#include "pic.h"
#include "isr.h"
#include <stdio.h>
#include <stddef.h>

IRQHandler Handlers[16];

void Common_Handler(Pushed_Regs* regs) {
    uint8_t irq = regs->interrupt - 0x20;
    if(Handlers[irq] != NULL)
        Handlers[irq](regs);
    else
        printf("Unhandled IRQ %d\n", irq);

    send_eoi(irq);
}

void IRQ_Init() {
    pic_init();
    for (int i = 0;i < 16;i++)
        x64_Set_Interrupt_Handler(0x20 + i, Common_Handler);

    __asm__ volatile ("sti");
}

void IRQ_Set_Handler(int irq, IRQHandler handler) {
    Handlers[irq] = handler;
}
