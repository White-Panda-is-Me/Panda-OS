#include <stdint.h>
#include "x86.h"

void main() {
    // uint8_t* vid_mem = (uint8_t*) 0xB8002;
    // *vid_mem = 'A';
    // vid_mem++;
    // *vid_mem = 0x48;

    __asm__("cli");
    __asm__("hlt");

    while(1) {}
}