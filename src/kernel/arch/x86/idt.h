#include <stdint.h>

typedef enum {
    IDT_FLAG_PRESENT                = 0x80,
    IDT_FLAG_GATE_TASK              = 0x5,
    IDT_FLAG_GATE_16BIT_INT         = 0x6,
    IDT_FLAG_GATE_16BIT_TRAP        = 0x7,
    IDT_FLAG_GATE_32BIT_INT         = 0xE,
    IDT_FLAG_GATE_32BIT_TRAP        = 0xF,
    IDT_FLAG_PVLG_RING0             = (0 << 5),
    IDT_FLAG_PVLG_RING1             = (1 << 5),
    IDT_FLAG_PVLG_RING2             = (2 << 5),
    IDT_FLAG_PVLG_RING3             = (3 << 5)
} IDT_Attributes;

typedef struct {
    uint16_t BaseLow;
    uint16_t SegmentSelector;
    uint8_t Reserved1;
    uint8_t TypeAttributes;
    uint16_t BaseHi;
    uint32_t BaseHier;
    uint32_t Reserved2;
} __attribute__((packed)) IDTEntry;

typedef struct {
    uint16_t Limit;
    uint64_t* IDTs;
} __attribute__((packed)) IDTDescriptor;

void x86_IDT_Load(IDTDescriptor* IDT_Desc);

void x86_IDT_SetGate(int interrupt, void* base, uint16_t segmentDescriptor, uint8_t attr);
void x86_IDT_EnableGate(int interrupt);
void x86_IDT_DisableGate(int interrupt);
void IDT_Init();
