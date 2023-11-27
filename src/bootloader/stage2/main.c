#include <stdint.h>
#include <mlfs.h>
#include <stdio.h>
#include <memory.h>
#include <mlfs.h>
#include <memdetect.h>
#include <bootparams.h>

uint8_t* KernelLoadAddr = (uint8_t*) 0x200000;
typedef void (*Kernel)(BootParams* boot_params);
BootParams boot_params;
MemoryInfo g_mem_info;
device g_dev;

void main() {
    clearScr();
    uint16_t memRegionCount = GetMemoryMap(&g_mem_info);

    MLFS_init(&g_dev);
    Entry* entry = OpenFile("/kernel.bin", &g_dev);
    Kernel Kmain = (Kernel) KernelLoadAddr;
    ReadFile(entry, KernelLoadAddr, &g_dev);
    CloseFile(entry);

    boot_params.dev = &g_dev;
    boot_params.mem_info = &g_mem_info;
    Kmain(&boot_params);
}