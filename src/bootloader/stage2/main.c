#include <stdint.h>
#include <mlfs.h>
#include <stdio.h>

uint8_t* KernelLoadAddr = (uint8_t*) 0x200000;
typedef void (*Kernel)();
// static MemoryInfo mem_info;

void main() {
    clearScr();
    printf("Welcome to Panda OS!\n");
    printf("stage2 Loaded Successfully!\n");
    printf("Switching to 32 bit Pmode...\tDone!\n");
    printf("Switching language to C...\tDone!\n");

    // uint16_t memRegionCount = GetMemoryMap(&mem_info);
    // printf("Base\tLength\tType\tACPI\n");
    // for (int i = 0;i < memRegionCount;i++) {
    //     printf("%llp\t%llp\t%p\t%p\n", mem_info.map[i].Base, mem_info.map[i].Length, mem_info.map[i].Type, mem_info.map[i].ACPI);
    // }

    MLFS_init();
    Entry* entry = OpenFile("/kernel.bin");
    ReadFile(entry, KernelLoadAddr);
    Kernel Kmain = (Kernel) KernelLoadAddr;
    CloseFile(entry);
    // printf("KernelBase: %p\n", Kmain);
    // printf("KernelLength: %u\n", entry->Info.file.fileSize);
    Kmain();
}