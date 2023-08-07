#include <stdint.h>

void clearScr();
void setCurPos(uint8_t x, uint8_t y);
uint16_t getCurPos();
void putc(const char chr);
void puts(const char* str);
void printf(const char* fmt, ...);