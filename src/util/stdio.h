#pragma once
#ifndef __STDIO_H
#define __STDIO_H


#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>

void clearScr();
void printf(const char* fmt, ...);
void print_buffer(const char* msg, const void* buffer, uint32_t count);
void set_vid_mem(uint8_t* mem);

#endif