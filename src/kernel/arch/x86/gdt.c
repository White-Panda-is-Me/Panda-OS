#include "gdt.h"
GDTEntry g_gdts[5];

void set_gate(int index, uint64_t base, uint32_t limit, uint8_t flags, uint8_t access) {
    g_gdts[index].LimitLow = (limit & 0xFFFF);
    g_gdts[index].BaseLow = (base & 0xFFFF);
    g_gdts[index].BaseMiddle = ((base >> 16) & 0xFF);
    g_gdts[index].AccessBytes = access;
    g_gdts[index].LimitHi_Flags = (((limit >> 16) & 0xF) | (flags & 0xF0));
    g_gdts[index].BaseHi = ((base >> 24) & 0xFF);
    g_gdts[index].BaseHier = ((base >> 32) & 0xFFFFFFFF);
    g_gdts[index].Reserved = 0ULL;
}

GDT_Descriptor g_gdt_desc;
void x86_GDT_Load(GDT_Descriptor* descriptor, uint16_t codeSegment, uint16_t dataSegment);

void GDT_Init() {
    set_gate(0, 0, 0, 0, 0);
    set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xAF);
    set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xAF);
    //set_gate(3, 0, 0xFFFFFFFF, 0xF2, 0xAF);
    //set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xAF);
	g_gdt_desc.limit = (sizeof(g_gdts) - 1);
    g_gdt_desc.Base = (uint64_t*) g_gdts;
    x86_GDT_Load(&g_gdt_desc, 0x08, 0x10);
}

