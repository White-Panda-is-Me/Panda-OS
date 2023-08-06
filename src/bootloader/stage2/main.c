#include <stdint.h>
#include "x86.h"

int main() {
    uint8_t* vid_mem = (uint8_t*) 0xB8002;
    *vid_mem = 'A';
    vid_mem++;
    *vid_mem = 0x48;

    while(1) {}
}