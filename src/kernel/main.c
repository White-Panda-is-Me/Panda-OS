#include <stdint.h>
#include <stdio.h>
#include <bootparams.h>
#include <arch/x86/gdt.h>
#include <arch/x86/idt.h>

void __attribute__((section(".entry"))) kmain(BootParams* boot_params) {
    printf("-> Kernel loaded successfully\n");
    printf("-> Selected ATA device: %s\n", boot_params->dev->name);
    GDT_Init();
    IDT_Init();
    printf("-> GDT Initialized successfully!!!\n");
    printf("-> IDT Initialized successfully!!!\n");
    while(1);
}
