#include "idt.h"

IDTEntry g_IDTs[256];
IDTDescriptor g_IDT_desc = { sizeof(g_IDTs) - 1, g_IDTs };

void x86_IDT_SetGate(int interrupt, void* base, uint16_t segmentDescriptor, uint8_t attr) {
    g_IDTs[interrupt].BaseLow = ((uint32_t)base) & 0xFFFF;
    g_IDTs[interrupt].SegmentSelector = segmentDescriptor;
    g_IDTs[interrupt].Reserved = 0;
    g_IDTs[interrupt].TypeAttributes = attr;
    g_IDTs[interrupt].BaseHi = ((uint32_t)base >> 16) & 0xFFFF;
}

void x86_IDT_EnableGate(int interrupt) {
    g_IDTs[interrupt].TypeAttributes |= IDT_FLAG_PRESENT;
}

void x86_IDT_DisableGate(int interrupt) {
    g_IDTs[interrupt].TypeAttributes &= ~IDT_FLAG_PRESENT;
}

void IDT_Init() {
    x86_IDT_Load(&g_IDT_desc);
}