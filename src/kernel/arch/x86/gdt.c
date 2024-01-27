#include "gdt.h"

#define GDT_LIMIT_LOW(limit)                (limit & 0xFFFF)
#define GDT_BASE_LOW(base)                  (base & 0xFFFF)
#define GDT_BASE_MIDDLE(base)               ((base >> 16) & 0xFF)
#define GDT_FLAGS_LIMIT_HI(limit, flags)    (((limit >> 16) & 0xF) | (flags & 0xF0))
#define GDT_BASE_HIGH(base)                 ((base >> 24) & 0xFF)
#define GDT_BASE_HIGHER(base)				((base >> 32) & 0xFFFFFFFF)

#define GDT_ENTRY(base, limit, flags, access) {              \
    GDT_LIMIT_LOW(limit),                                    \
    GDT_BASE_LOW(base),                                      \
    GDT_BASE_MIDDLE(base),                                   \
    access,                                                  \
    GDT_FLAGS_LIMIT_HI(limit, flags),                        \
    GDT_BASE_HIGH(base),                                     \
	GDT_BASE_HIGHER(base),									 \
	0														 \
}

GDTEntry g_gdts[] = {
    GDT_ENTRY(0, 0, 0, 0),
    GDT_ENTRY(0, 0xFFFFF, (GDT_FLAGS_GRANULARITY_4K | GDT_FLAGS_32BIT_PMODE),
              (GDT_ACCESS_PRESENT | GDT_FLAG_PVLG_RING0 | GDT_ACCESS_CODE | GDT_ACCESS_CODE_READABLE)),
    GDT_ENTRY(0, 0xFFFFF, (GDT_FLAGS_GRANULARITY_4K | GDT_FLAGS_32BIT_PMODE),
              (GDT_ACCESS_PRESENT | GDT_FLAG_PVLG_RING0 | GDT_ACCESS_DATA | GDT_ACCESS_DATA_WRITABLE))
};

GDT_Descriptor g_gdt_desc = { (sizeof(g_gdts) - 1), g_gdts};
void x86_GDT_Load(GDT_Descriptor* descriptor, uint16_t codeSegment, uint16_t dataSegment);

void GDT_Init() {
    x86_GDT_Load(&g_gdt_desc, 0x08, 0x10);
}
