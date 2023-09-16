#include "ata.h"

void chs_ata_read(uint8_t* buffer) {
    x86_outb(0x01f6, 0b10100000);
    x86_outb(0x01f2, 1);
    x86_outb(0x01f3, 15);
    x86_outb(0x01f4, 0);
    x86_outb(0x01f5, 0);
    x86_outb(0x01f7, 0x20);


    uint8_t al = x86_inb(0x01f7);
    while (al & 8)
        al = x86_inb(0x01f7);
    for(int i = 0;i < 512;i++) {
        buffer[i] = x86_inb(0x01f0);
    }

}