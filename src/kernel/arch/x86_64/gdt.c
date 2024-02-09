#include "gdt.h"

GDTEntry g_gdts[3];

void set_gdt_gate(int index, uint64_t base, uint32_t limit, uint8_t access, uint8_t flags) {
    g_gdts[index].LimitLow = (limit & 0xFFFF);
    g_gdts[index].BaseLow = (base & 0xFFFF);
    g_gdts[index].BaseMiddle = ((base >> 16) & 0xFF);
    g_gdts[index].AccessBytes = access;
    g_gdts[index].LimitHi_Flags = (((limit >> 16) & 0xF) | (flags & 0xF0));
    g_gdts[index].BaseHi = ((base >> 24) & 0xFF);
    g_gdts[index].BaseHier = ((base >> 32) & 0xFFFFFFFF);
    g_gdts[index].Reserved = 0;
}

GDT_Descriptor g_gdt_desc = { (sizeof(g_gdts) - 1), g_gdts};
void x64_GDT_Load(GDT_Descriptor* descriptor, uint8_t CS, uint8_t DS);

void GDT_Init() {
    set_gdt_gate(0, 0, 0, 0, 0);
    set_gdt_gate(1, 0, 0xFFFFF, 0x9A, 0xAF);
    set_gdt_gate(2, 0, 0xFFFFF, 0x92, 0xAF);
    x64_GDT_Load(&g_gdt_desc, 0x10, 0x20);
}
