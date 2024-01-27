#pragma once
#include "bootparam.h"

void init_GUI(bootparam_t* bootpar);
void printString(char* s);
void putc(char c);
void printf(const char* fmt, ...);
