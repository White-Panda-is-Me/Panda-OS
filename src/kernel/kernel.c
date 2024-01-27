#include "stdio.h"
#include "drivers/disk/ata/ata.h"
//#include "arch/hal/hal.h"

device g_dev;

void _start(bootparam_t *bootpar) {
	init_GUI(bootpar);

    printf("hello from kernel\n");
	printf("pitch: %d\n", bootpar->pitch);
	printf("address of boot parameters: %x\n", bootpar);
	ata_init(&g_dev);
//	HAL_Init();
    while(1);
}

