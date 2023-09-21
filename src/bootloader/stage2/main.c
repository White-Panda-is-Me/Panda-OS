#include <stdint.h>
#include <mlfs.h>
#include <stdio.h>
#include "memdetect.h"

uint8_t* KernelLoadAddr = (uint8_t*) 0x100000;
typedef void (*Kernel)();
// MemoryInfo mem_info;

void main() {
    clearScr();
    printf("Welcome to panda OS!\n");
    printf("stage2 Loaded Successfully!\n");
    printf("Switching to 32 bit Pmode...\tDone!\n");
    printf("Switching language to C...\tDone!\n");

    // uint16_t memRegionCount = GetMemoryMap(&mem_info);

    MLFS_init();
    Entry* entry = OpenFile("/kernel.bin");
    ReadFile(entry, KernelLoadAddr);
    Kernel Kmain = (Kernel) KernelLoadAddr;
    CloseFile(entry);
    printf("KernelBase: %p\n", Kmain);
    printf("KernelLength: %u\n", entry->Info.file.fileSize);
    Kmain();
    while(1);
}