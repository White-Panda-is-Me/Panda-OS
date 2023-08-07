#include <stdint.h>
#include "x86.h"
#include "stdio.h"

void main() {
    int x = 10;
    clearScr();
    printf("Welcome to IDK OS!\n");
    printf("stage2 Loaded Successfully!\n");
    printf("Switching to 32 bit Pmode...\tDone!\n");
    printf("Switching language to C%p...\tDone!\n", &x);
    while(1) {}
}