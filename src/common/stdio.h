#pragma once

#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>

void setCurPos(uint8_t x, uint8_t y);
void clearScr();
void putc(const char chr);
void print_buffer(const char* msg, const void* buffer, uint32_t count);
void printf(const char* fmt, ...);