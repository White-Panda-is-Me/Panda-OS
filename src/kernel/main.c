#include <stdint.h>
#include <stdio.h>
#include <x86.h>
#include "cpu/cpu.h"

void kmain(uint16_t ax) {
    // setting the video memory to put the things after bootloader
    set_vid_mem((uint8_t*) (0xB8000 + 684));
    printf("Done!\n");

    int edx = edx_get_features();
    if(edx & CPUID_FEAT_EDX_FPU)
        printf("yeah!!!");

    while(1);
}
