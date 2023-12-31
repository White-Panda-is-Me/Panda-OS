#pragma once

void __attribute__((cdecl)) x86_outb(uint16_t port, uint8_t data);
uint8_t __attribute__((cdecl)) x86_inb(uint16_t port);
uint16_t __attribute__((cdecl)) x86_inw(uint16_t port);
uint16_t __attribute__((cdecl)) x86_insw(void* bffer, uint16_t port, uint32_t size);
void __attribute__((cdecl)) panic();
int __attribute__((cdecl)) check_long_mode();