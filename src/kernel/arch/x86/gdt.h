#pragma once
#include <stdint.h>

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
    GDTEntry* Base;
} __attribute__((packed)) GDT_Descriptor;

void GDT_Init();
