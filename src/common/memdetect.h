#pragma once

#include <stdint.h>

#define MAX_MEM_SHIT 64

enum {
    MEM_USABLE = 1,
    MEM_RESERVED = 2,
    MEM_ACPI_RECLAIMABLE = 3,
    MEM_ACPI_NVS = 4,
    MEM_BAD = 5
};

typedef struct {
    uint64_t Base;
    uint64_t Length;
    uint32_t Type;
    uint32_t ACPI;
} __attribute__((packed)) Mem_Map;

typedef struct {
    Mem_Map map[MAX_MEM_SHIT];
    uint32_t TotalMemoryBlocks;
} MemoryInfo;

int __attribute__((cdecl)) MemDetect(Mem_Map* map, uint32_t* ContId);
int16_t GetMemoryMap(MemoryInfo* memInfo);