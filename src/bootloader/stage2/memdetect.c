#include "../../common/stdio.h"
#include "memdetect.h"

int16_t GetMemoryMap(MemoryInfo* memInfo) {
    Mem_Map mem;
    int16_t readSize, RegionCount = 0;
    uint32_t contID = 0;

    do {
        readSize = MemDetect(&mem, &contID);

        if (readSize < 20) {
            printf("Memory map read failure");
            return 0;
        }

        memInfo->map[RegionCount].ACPI = mem.ACPI;
        memInfo->map[RegionCount].Base = mem.Base;
        memInfo->map[RegionCount].Length = mem.Length;
        memInfo->map[RegionCount].Type = mem.Type;

        RegionCount++;
    }
    while (readSize > 0 && contID != 0);

    memInfo->TotalMemoryBlocks = RegionCount;
    return RegionCount;
}
