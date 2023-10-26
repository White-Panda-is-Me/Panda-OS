#include <stdint.h>

void smth() {
    uint8_t* vid = (uint8_t*) 0xb8000;
    *vid = 'A';
}