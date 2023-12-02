#include <stdint.h>
#include <stdio.h>
#include <bootparams.h>
#include <arch/x86/gdt.h>
#include <arch/x86/idt.h>
#include <arch/x86/isr.h>
#include <cpuid.h>

void __attribute__((section(".entry"))) kmain(BootParams* boot_params) {
    printf("-> Kernel loaded successfully\n");
    printf("-> Selected ATA device: %s\n", boot_params->dev->name);
    GDT_Init();
    IDT_Init();
    ISR_Init();
    printf("-> GDT & IDT Initialized successfully!!!\n");
    printf("-> ISR Initialized successfully!!!\n");
    unsigned int a, b, c, d;
    __get_cpuid(0, &a, &b, &c, &d);
    if(d & (1 << 9))
        printf("-> APIC is Supported\n");
    else
        printf("-> APIC isn't Supported\n");

    while(1);
}
