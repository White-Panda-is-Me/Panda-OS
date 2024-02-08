#include "hal.h"

// extern void x86_GDT_Load();

void HAL_Init() {
   GDT_Init();
   // IDT_Init();
   // ISR_Init();
   // IRQ_Init();
   // Handlers_Init();
}
