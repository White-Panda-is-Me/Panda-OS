#include "handlers.h"

void Handlers_Init() {
    IRQ_Set_Handler(0, timer);      // IRQ 0
}