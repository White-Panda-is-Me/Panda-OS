#include <stdint.h>
#include <stdio.h>
#include <bootparams.h>

void kmain(BootParams* boot_params) {
    printf("-> Kernel loaded successfully\n");
    printf("-> Selected ATA device: %s\n", boot_params->dev->name);
    while(1);
}
