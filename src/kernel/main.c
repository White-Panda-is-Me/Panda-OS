#include <stdint.h>
#include <stdio.h>
#include <x86.h>
#include "x86.h"
void kmain(uint16_t ax) {
    // setting the video memory to put the things after bootloader
    set_vid_mem((uint8_t*) (0xB8000 + 684));
    printf("Done!\n");

    uint8_t buffer[512];
    chs_ata_read(buffer);
    printf("%x\n", buffer);

    while(1);
}
