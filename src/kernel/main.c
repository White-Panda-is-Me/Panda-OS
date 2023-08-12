#include <stdint.h>
#include <stdio.h>
#include <x86.h>

void kmain() {
    set_vid_mem((uint8_t*) (0xB8000 + 684));
    printf("Done!\n");
    while(1);
}
