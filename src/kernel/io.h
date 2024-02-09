#pragma once

void x64_outb(uint16_t port, uint8_t data);
void x64_outl(uint16_t port, uint32_t data);
uint8_t x64_inb(uint16_t port);
uint16_t x64_inw(uint16_t port);
uint16_t x64_insw(void* buffer, uint16_t port, uint32_t size);
uint32_t x64_inl(uint16_t port);
void panic();
