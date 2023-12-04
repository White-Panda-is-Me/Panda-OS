#pragma once
#include "../isr.h"
#include "../irq.h"

void timer(Pushed_Regs* regs);
void Handlers_Init();