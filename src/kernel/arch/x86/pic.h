#pragma once
#include <stdint.h>

void pic_init();
void pic_set_irq_line(uint8_t irq, int enable);
void send_eoi(uint8_t irq);