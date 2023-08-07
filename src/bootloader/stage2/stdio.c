#include "stdio.h"
#include "x86.h"

uint8_t* vid_mem = (uint8_t*) 0xB8000;
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define DEFAULT_COLOR 0x3F

// // // // // // // // // //
//                         //
//     cursor handling     //
//                         //
// // // // // // // // // //

uint16_t getCurPos() {
    uint16_t pos = 0;
    x86_outb(0x3D4, 0x0F);
    pos |= x86_inb(0x3D5);
    x86_outb(0x3D4, 0x0E);
    pos |= ((uint16_t)x86_inb(0x3D5)) << 8;
    return pos;
}

void setCurPos(uint8_t x, uint8_t y) {
	uint16_t pos = y * SCREEN_WIDTH + x;

	x86_outb(0x3D4, 0x0F);
	x86_outb(0x3D5, (uint8_t) (pos & 0xFF));
	x86_outb(0x3D4, 0x0E);
	x86_outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

// // // // // // // // // //
//                         //
//     screen handling     //
//                         //
// // // // // // // // // //

void cursor2VGA(uint8_t x, uint8_t y) {
    vid_mem = (uint8_t*) 0xB8000;
    vid_mem += ((x + y * 80)* 2);
}

void clearScr() {
    char clear = ' ';

    for (int i = 0; i < (80 * 50);i++) {
        *vid_mem = clear;
        vid_mem++;
        *vid_mem = DEFAULT_COLOR;
        vid_mem++;
    }

    setCurPos(0, 0);
    vid_mem = (uint8_t*) 0xB8000;
}

void putc(const char chr) {
    uint16_t pos = getCurPos();
    uint8_t x = pos % SCREEN_WIDTH;
    uint8_t y = (pos - x) / SCREEN_WIDTH;

    switch (chr) {
        case '\n':
            y++;
            x = 0;
            cursor2VGA(x, y);
            setCurPos(x, y);
            break;
        case '\t':
            x += 4;
            cursor2VGA(x, y);
            setCurPos(x, y);
            break;
        default:
            *vid_mem = chr;
            vid_mem++;
            *vid_mem = DEFAULT_COLOR;
            vid_mem++;
            x++;
            setCurPos(x, y);
            break;
    }
}

void puts(const char* str) {
    while (*str) {
        putc(*str);
        str++;
    }
}

void printf(const char* fmt, ...) {

}