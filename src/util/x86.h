#pragma once

void __attribute((cdecl)) x86_outb(uint16_t port, uint8_t data);
int __attribute((cdecl)) x86_inb(uint16_t port);