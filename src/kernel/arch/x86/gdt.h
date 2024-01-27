#pragma once
#include <stdint.h>

typedef enum {
    GDT_ACCESS_PRESENT                            = 0x80,
    GDT_FLAG_PVLG_RING0                           = 0x00,
    GDT_FLAG_PVLG_RING1                           = 0x20,
    GDT_FLAG_PVLG_RING2                           = 0x40,
    GDT_FLAG_PVLG_RING3                           = 0x60,
    GDT_ACCESS_DES_TYPE_BIT_TSS                   = 0x00,
    GDT_ACCESS_CODE                               = 0x18,
    GDT_ACCESS_DATA                               = 0x10,
    GDT_ACCESS_CODE_READABLE                      = 0x02,
    GDT_ACCESS_DATA_WRITABLE                      = 0x02,
    GDT_ACCESS_CODE_CONFORMING                    = 0x04,
    GDT_ACCESS_DATA_DIRECTION_NORMAL              = 0x00,
    GDT_ACCESS_DATA_DIRECTION_DOWN                = 0x04
} GTD_Access;

typedef enum {
    GDT_FLAGS_GRANULARITY_1B                      = 0x00,
    GDT_FLAGS_GRANULARITY_4K                      = 0x80,
    GDT_FLAGS_16BIT_PMODE                         = 0x00,
    GDT_FLAGS_32BIT_PMODE                         = 0x40
} GTD_Flags;

typedef struct {
    uint16_t LimitLow;
    uint16_t BaseLow;
    uint8_t BaseMiddle;
    uint8_t AccessBytes;
    uint8_t LimitHi_Flags;
    uint8_t BaseHi;
	uint32_t BaseHier;
	uint32_t Reserved;
} __attribute__((packed)) GDTEntry;

typedef struct {
    uint16_t limit;
    uint64_t* Base;
} __attribute__((packed)) GDT_Descriptor;

void GDT_Init();
