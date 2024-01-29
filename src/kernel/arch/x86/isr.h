#pragma once
#include <stdint.h>

typedef struct {
    uint64_t rdi, rsi, rbp, shit, rbx, rdx, rcx, rax;        // getting those shit regs from stack
    uint64_t interrupt, error;                                  // no need for comment, It's obvious
    uint32_t eip, cs, eflags, esp, ss;                          // It's pushed to the stack by stupid CPU
} __attribute__((packed)) Pushed_Regs;

typedef void (*ISRHandler)(Pushed_Regs* regs);
void x86_ISR_SetGates();
void ISR_Init();
void x86_Set_Interrupt_Handler(int interrupt, ISRHandler handler);
