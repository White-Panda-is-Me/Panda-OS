#include <stdint.h>
#include <stdio.h>
#include <bootparams.h>
#include <arch/hal/hal.h>
#include <arch/x86/irq_handlers/handlers.h>

void __attribute__((section(".entry"))) kmain(BootParams* boot_params) {
    printf("-> Kernel loaded successfully...\n");
    printf("-> Selected ATA device: %s\n", boot_params->dev->name);
    HAL_Init();
    printf("-> HAL Initialized successfully!\n");
    Handlers_Init();
    while(1);
}