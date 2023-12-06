#include <stdint.h>
#include <stdio.h>
#include <bootparams.h>
#include <arch/hal/hal.h>
#include <io.h>
void __attribute__((section(".entry"))) kmain(BootParams* boot_params) {
    printf("-> Kernel loaded successfully...\n");
    printf("-> Selected ATA device: %s\n", boot_params->dev->name);
    HAL_Init();
    printf("-> HAL Initialized successfully!\n");
    if(check_long_mode())
        printf("-> long mode is supported!\n");
    else {
        printf("-> long mode is NOT supported!\n");
        panic();
    }
    while(1);
}