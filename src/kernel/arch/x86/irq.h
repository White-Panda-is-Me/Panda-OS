#include "isr.h"

typedef void (*IRQHandler) (Pushed_Regs* regs);
void Common_Handler(Pushed_Regs* regs);
void IRQ_Init();
void IRQ_Set_Handler(int irq, IRQHandler handler);