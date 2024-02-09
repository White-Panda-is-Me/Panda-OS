#pragma once

#include "../x86_64/gdt.h"
#include "../x86_64/idt.h"
#include "../x86_64/isr.h"
#include "../x86_64/irq.h"
#include "../x86_64/irq_handlers/handlers.h"

void HAL_Init();
