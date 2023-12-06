#include "hal.h"

void HAL_Init() {
    GDT_Init();
    IDT_Init();
    ISR_Init();
    IRQ_Init();
    Handlers_Init();
}