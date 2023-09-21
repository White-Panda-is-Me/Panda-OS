#pragma once
#include <stdint.h>

int memcmp(const void* ptr1, const void* ptr2, uint32_t size);
void* memcpy(void* dst, const void* src, uint32_t size);
void* memset(void* dst, int val, uint32_t size);