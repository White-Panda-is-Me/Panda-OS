#include "memory.h"

int memcmp(const void* ptr1, const void* ptr2, uint32_t size) {
    uint8_t* u8Ptr1 = (uint8_t*) ptr1;
    uint8_t* u8Ptr2 = (uint8_t*) ptr2;

    for (uint32_t i = 0;i < size;i++) {
        if(u8Ptr1[i] != u8Ptr2[i])
            return u8Ptr2[i] - u8Ptr1[i];
    }
    return 0;
}

void* memcpy(void* dst, const void* src, uint32_t size) {
    uint8_t* u8Src = (uint8_t*) src;
    uint8_t* u8Dst = (uint8_t*) dst;

    for (uint32_t i = 0;i < size;i++) {
        u8Dst[i] = u8Src[i];
    }

    return (void*) dst;
}

void* memset(void* dst, int val, uint32_t size) {
    uint8_t* u8Dst = (uint8_t*) dst;

    for (uint32_t i = 0;i < size;i++) {
        u8Dst[i] = (uint8_t) val;
    }

    return u8Dst;
}
