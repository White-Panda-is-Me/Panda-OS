#include <stdint.h>
#include "stdio.h"

void __attribute__((section(".entry"))) kmain() {
    printf("Kernel loaded successfully!\n");

    while(1);
}
