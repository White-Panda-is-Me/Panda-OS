#pragma once
#include <stdint.h>

typedef struct {
    uint64_t rdi, rsi, rbp, rbx, rdx, rcx, rax;    // I push general porpuse registers
    uint64_t interrupt;                                  // I push the error code
    uint64_t error_code, rip, cs, rflags, esp, ss;       // Cpu pushes these registers
} __attribute__((packed)) Pushed_Regs;

typedef void (*ISRHandler)(Pushed_Regs* regs);
void x64_ISR_SetGates();
void ISR_Init();
void x64_Set_Interrupt_Handler(int interrupt, ISRHandler handler);
