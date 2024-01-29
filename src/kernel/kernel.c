#include "stdio.h"
#include "drivers/disk/ata/ata.h"
#include "arch/hal/hal.h"
//#include "drivers/disk/ahci/ahci.h"
device g_dev;

void _start(bootparam_t *bootpar) {
	init_GUI(bootpar);

    printf("hello from kernel\n");
	printf("address of boot parameters: %x\n", bootpar);
	printf("sizeof a pointer: %d\n", sizeof(GDTEntry*));
	HAL_Init();
	//ata_init(&g_dev);
	//HBA_MEM_REGS* hba_mem = find_ahci_controllers();
    while(1);
}

void kmain() {
    printf("hello after that piece of shit\n");
}

