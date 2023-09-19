#include <stdint.h>
#include "memory.h"
#include "x86.h"
#include "ata.h"
#include "mlfs.h"
#include "stdio.h"

uint8_t* KernelLoadBuffer = (uint8_t*) 0x2000;
uint8_t* KernelAddress = (uint8_t*) 0x7E00;
typedef void (*Kernel)();

void main() {
    clearScr();
    printf("Welcome to panda OS!\n");
    printf("stage2 Loaded Successfully!\n");
    printf("Switching to 32 bit Pmode...\tDone!\n");
    printf("Switching language to C...\tDone!\n");
    MLFS_init();
    Entry entry;
    uint8_t* KenrelLoad = KernelAddress;
    OpenFile(&entry, "/kernel.bin");
    ReadFile(&entry, KernelLoadBuffer);
    memcpy(KenrelLoad, KernelLoadBuffer, entry.Info.file.fileSize);
    CloseFile(&entry);
    Kernel Kmain = (Kernel) KernelAddress;
    // Kmain -= 588;
    Kmain();
    printf("couldn't fucking execute kernel!\n");
    printf("KernelAddress: %p\n", Kmain);
    print_buffer("", KernelLoadBuffer, 128);
}