#include "stdio.h"
#include "drivers/disk/ata/ata.h"
#include "drivers/disk/ahci/ahci.h"
#include "arch/hal/hal.h"
device g_dev;

extern void fuck();

void _start(bootparam_t *bootpar) {
	init_GUI(bootpar);

	printf("Hello from kernel!!!\n");
	HAL_Init();
	printf("HAL Initialized\n");
	ata_init(&g_dev);
	// HBA_MEM_REGS* hba_mem = find_ahci_controllers();
	// printf("AHCI MEMORY BASE ADDRESS: %llx\n", hba_mem);
	while(1);
}
