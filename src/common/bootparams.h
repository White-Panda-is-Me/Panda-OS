#include <memdetect.h>
#include <disk/device.h>

typedef struct {
    MemoryInfo* mem_info;
    device* dev;
} BootParams;
