#include "stdio.h"
#include "x86.h"

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define DEFAULT_COLOR 0x70

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

uint8_t* cursor2VGA(uint8_t x, uint8_t y) {
    return (uint8_t*) (0xB8000 + ((x + y * 80)* 2));
}

void clearScr() {
    uint8_t* vid = (uint8_t*) 0xb8000;

    for (int i = 0; i < (80 * 25);i++) {
        *vid = ' ';
        vid++;
        *vid = DEFAULT_COLOR;
        vid++;
    }

    setCurPos(0, 0);
}

void putc(const char chr) {
    uint16_t pos = getCurPos();
    uint8_t x = pos % SCREEN_WIDTH;
    uint8_t y = (pos - x) / SCREEN_WIDTH;
    uint8_t* vid_mem = cursor2VGA(x, y);

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

//
// The main function goes here which is printf!
//

#define PRINTF_STATE_NORMAL         0
#define PRINTF_STATE_LENGTH         1
#define PRINTF_STATE_LENGTH_SHORT   2
#define PRINTF_STATE_LENGTH_LONG    3
#define PRINTF_STATE_SPEC           4

#define PRINTF_LENGTH_DEFAULT       0
#define PRINTF_LENGTH_SHORT_SHORT   1
#define PRINTF_LENGTH_SHORT         2
#define PRINTF_LENGTH_LONG          3
#define PRINTF_LENGTH_LONG_LONG     4


const char g_HexChars[] = "0123456789ABCDEF";

void printf_unsigned(unsigned long long number, int radix) {
    char buffer[32];
    int pos = 0;

    do {
        unsigned long long rem = number % radix;
        number /= radix;
        buffer[pos++] = g_HexChars[rem];
    } while (number > 0);

    // print number in reverse order
    while (--pos >= 0)
        putc(buffer[pos]);
}

void printf_signed(long long number, int radix) {
    if (number < 0) {
        putc('-');
        printf_unsigned((unsigned long long)(-number), radix);
    }
    else {
        printf_unsigned((unsigned long long)number, radix);
    }
}


void print_buffer(const char* msg, const void* buffer, uint32_t count) {
    const uint8_t* u8Buffer = (const uint8_t*)buffer;
    
    puts(msg);
    for (uint16_t i = 0; i < count; i++)
    {
        putc(g_HexChars[u8Buffer[i] >> 4]);
        putc(g_HexChars[u8Buffer[i] & 0xF]);
    }
    puts("\n");
}

void printf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    int state = PRINTF_STATE_NORMAL;
    int length = PRINTF_LENGTH_DEFAULT;
    bool number = false;
    bool sign = false;
    int radix = 10;

    while (*fmt) {
        switch (state) {
            case PRINTF_STATE_NORMAL:
                switch (*fmt) {
                    case '%':   state = PRINTF_STATE_LENGTH;
                                break;
                    default:    putc(*fmt);
                                break;
                }
                break;

            case PRINTF_STATE_LENGTH:
                switch (*fmt) {
                    case 'h':   length = PRINTF_LENGTH_SHORT;
                                state = PRINTF_STATE_LENGTH_SHORT;
                                break;
                    case 'l':   length = PRINTF_LENGTH_LONG;
                                state = PRINTF_STATE_LENGTH_LONG;
                                break;
                    default:    goto PRINTF_STATE_SPEC_;
                }
                break;

            case PRINTF_STATE_LENGTH_SHORT:
                if (*fmt == 'h') {
                    length = PRINTF_LENGTH_SHORT_SHORT;
                    state = PRINTF_STATE_SPEC;
                }
                else goto PRINTF_STATE_SPEC_;
                break;

            case PRINTF_STATE_LENGTH_LONG:
                if (*fmt == 'l') {
                    length = PRINTF_LENGTH_LONG_LONG;
                    state = PRINTF_STATE_SPEC;
                }
                else goto PRINTF_STATE_SPEC_;
                break;

            case PRINTF_STATE_SPEC:
            PRINTF_STATE_SPEC_:
                switch (*fmt) {
                    case 'c':   putc((char)va_arg(args, int));
                                break;

                    case 's':   puts(va_arg(args, const char*));
                                break;

                    case '%':   putc('%');
                                break;

                    case 'd':
                    case 'i':   number = true; radix = 10; sign = true;
                                break;

                    case 'u':   number = true; radix = 10; sign = false;
                                break;

                    case 'X':
                    case 'x':
                    case 'p':   number = true; radix = 16; sign = false;
                                puts("0x");
                                break;

                    case 'o':   number = true; radix = 8; sign = false;
                                puts("0o");
                                break;

                    // ignore invalid spec
                    default:    break;
                }

                // handle numbers
                if (number) {
                    if (sign) {
                        switch (length) {
                            case PRINTF_LENGTH_SHORT_SHORT:
                            case PRINTF_LENGTH_SHORT:
                            case PRINTF_LENGTH_DEFAULT:     printf_signed(va_arg(args, int), radix);
                                                            break;

                            case PRINTF_LENGTH_LONG:        printf_signed(va_arg(args, long), radix);
                                                            break;

                            case PRINTF_LENGTH_LONG_LONG:   printf_signed(va_arg(args, long long), radix);
                                                            break;
                        }
                    }
                    else {
                        switch (length) {
                            case PRINTF_LENGTH_SHORT_SHORT:
                            case PRINTF_LENGTH_SHORT:
                            case PRINTF_LENGTH_DEFAULT:     printf_unsigned(va_arg(args, unsigned int), radix);
                                                            break;

                            case PRINTF_LENGTH_LONG:        printf_unsigned(va_arg(args, unsigned long), radix);
                                                            break;

                            case PRINTF_LENGTH_LONG_LONG:   printf_unsigned(va_arg(args, unsigned long long), radix);
                                                            break;
                        }
                    }
                }

                // reset state
                state = PRINTF_STATE_NORMAL;
                length = PRINTF_LENGTH_DEFAULT;
                radix = 10;
                sign = false;
                number = false;
                break;
        }

        fmt++;
    }

    va_end(args);
}
