#include <stddef.h>
#include <stdio.h>
#include <x86.h>
#include "isr.h"
#include "idt.h"

ISRHandler g_ISRHandlers[256];              // 256 Fucking handlers (aka Memory wasting)

static const char* const g_Exceptions[] = {
    "Division by zero error",
    "Debug",
    "Non-maskable Interrupt",
    "Breakpoint",
    "Overflow",
    "Bound Range Exceeded",
    "Invalid Opcode",
    "Device Not Available",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Invalid TSS",
    "Segment Not Present",
    "Stack-Segment Fault",
    "General Protection Fault",
    "Page Fault",
    "",
    "x87 Floating-Point Exception",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating-Point Exception",
    "Virtualization Exception",
    "Control Protection Exception ",
    "",
    "",
    "",
    "",
    "",
    "",
    "Hypervisor Injection Exception",
    "VMM Communication Exception",
    "Security Exception",
    ""
};

void ISR_Init() {
    x86_ISR_SetGates();
    for (int i = 0;i < 256;i++) {
        x86_IDT_EnableGate(i);
    }
}

void __attribute__((cdecl)) x86_ISR_Handler(Pushed_Regs* regs) {
    if(g_ISRHandlers[regs->interrupt] != NULL)
        g_ISRHandlers[regs->interrupt](regs);

    else if(regs->interrupt >= 32)
        printf("Fuck this Interrupt: %d, Not handled!", regs->interrupt);

    else {
        printf("Fuck this Exeption: %d Which means: %s\n", regs->interrupt, g_Exceptions[regs->interrupt]);

        printf("  eax=%x  ebx=%x  ecx=%x  edx=%x  esi=%x  edi=%x\n",
               regs->eax, regs->ebx, regs->ecx, regs->edx, regs->esi, regs->edi);

        printf("  esp=%x  ebp=%x  eip=%x  eflags=%x  cs=%x  ss=%x\n",
               regs->esp, regs->ebp, regs->eip, regs->eflags, regs->cs, regs->ss);

        printf("  interrupt=%x  errorcode=%x\n", regs->interrupt, regs->error);

        printf("KERNEL PANIC!!!\n");
        panic();
    }
}

void x86_Set_Interrupt_Handler(int interrupt, ISRHandler handler) {
    g_ISRHandlers[interrupt] = handler;
    x86_IDT_EnableGate(interrupt);
}