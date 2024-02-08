#include "ata.h"
#include <io.h>
#include <string.h>

#define ATA_PRIMARY_IO 0x1F0
#define ATA_SECONDARY_IO 0x170

#define ATA_CMD_MASTER 0xE0
#define ATA_CMD_SLAVE 0xF0

#define ATA_PRIMARY_DCR_AS 0x3F6
#define ATA_SECONDARY_DCR_AS 0x376
uint8_t* ata_buffer = (uint8_t*) 0x5500;
void ide_select_drive(uint8_t bus, uint8_t i) {
	if(bus == ATA_PRIMARY)
		if(i == ATA_MASTER)
			x86_outb(ATA_PRIMARY_IO + ATA_REG_HDDEVSEL, 0xA0);
		else x86_outb(ATA_PRIMARY_IO + ATA_REG_HDDEVSEL, 0xB0);
	else
		if(i == ATA_MASTER)
			x86_outb(ATA_SECONDARY_IO + ATA_REG_HDDEVSEL, 0xA0);
		else x86_outb(ATA_SECONDARY_IO + ATA_REG_HDDEVSEL, 0xB0);
}

int Identify(uint8_t bus, uint8_t drive) {
    uint16_t io = 0;
    ide_select_drive(bus, drive);
    if (bus == ATA_PRIMARY) io = ATA_PRIMARY_IO;
    else io = ATA_SECONDARY_IO;
    x86_outb(io + ATA_REG_SECCOUNT0, 0);
    x86_outb(io + ATA_REG_LBA0, 0);
    x86_outb(io + ATA_REG_LBA1, 0);
    x86_outb(io + ATA_REG_LBA2, 0);
    // send Identify
    x86_outb(io + ATA_REG_COMMAND, ATA_CMD_IDENTIFY);
    uint8_t status = x86_inb(io + ATA_REG_STATUS);
    if (status) {
        uint8_t bsy = x86_inb(io + ATA_REG_STATUS);
		int i = 0;
        while(bsy & ATA_SR_BSY && i < 1000) {
        	bsy = x86_inb(io + ATA_REG_STATUS);
			i++;
        } if (i == 999) return 0;
		i = 0;
        pm_read_start:
        status = x86_inb(io + ATA_REG_STATUS);
		i++;
        if(status & ATA_SR_ERR) {
            printf("ATA DEVICE: %s %s not found!\n", bus == ATA_PRIMARY ? "Primary" : "Secondary", drive == ATA_MASTER ? "Master" : "Slave");
            return 0;
        }
        while(!(status & ATA_SR_DRQ) && i < 1000) goto pm_read_start;
		if (i == 999) return 0;
        printf("    ATA DEVICE: %s %s is online!\n", bus == ATA_PRIMARY ? "Primary" : "Secondary", drive == ATA_MASTER ? "Master" : "Slave");
        for(i = 0; i < 256; i++) {
			*(uint16_t *)(ata_buffer + i * 2) = x86_inw(io + ATA_REG_DATA);
		}
        return 1;
    }
    printf("ATA DEVICE: %s %s not found!\n", bus == ATA_PRIMARY ? "Primary" : "Secondary", drive == ATA_MASTER ? "Master" : "Slave");
	return 0;
}

void ide_400ns_delay(uint16_t io) {
	for(int i = 0;i < 4; i++)
		x86_inb(io + ATA_REG_ALTSTATUS);
}

void ide_poll(uint16_t io) {
	ide_400ns_delay(io);
	uint8_t status;
    retry:

	status = x86_inb(io + ATA_REG_STATUS);
	if(status & ATA_SR_BSY) goto retry;
    retry2:

    status = x86_inb(io + ATA_REG_STATUS);
	if(status & ATA_SR_ERR)	{
		printf("ERR set, device failure!\n");
	}
	if(!(status & ATA_SR_DRQ)) goto retry2;
	return;
}

void ata_init(device* dev) {
    printf("ATA DEVICES:\n");
    if (Identify(ATA_PRIMARY, ATA_MASTER) == 1) {
	    char name[40];
    	for (int i = 0;i < 40;i += 2) {
        	name[i] = ata_buffer[ATA_IDENT_MODEL + i + 1];
	        name[i + 1] = ata_buffer[ATA_IDENT_MODEL + i];
    	}
    	strcpy(dev->name, name);
    	dev->drive = (ATA_PRIMARY << 1) | ATA_MASTER;
		return;
	}
	printf("    No ATA devices found!\n");
    // Identify(ATA_PRIMARY, ATA_SLAVE);
}

void ata_read_one(uint32_t lba, void* buffer, device* dev) {
    uint8_t drive = dev->drive;
    uint16_t io;
    switch (drive) {
        case (ATA_PRIMARY << 1) | ATA_MASTER:
            drive = ATA_MASTER;
            io = ATA_PRIMARY_IO;
            break;
        case (ATA_PRIMARY << 1) | ATA_SLAVE:
            drive = ATA_SLAVE;
            io = ATA_PRIMARY_IO;
            break;
        case (ATA_SECONDARY << 1) | ATA_MASTER:
            drive = ATA_MASTER;
            io = ATA_SECONDARY_IO;
            break;
        case (ATA_SECONDARY << 1) | ATA_SLAVE:
            drive = ATA_SLAVE;
            io = ATA_SECONDARY_IO;
            break;
        
        default:
            printf("Invalid device!!!\n");
    }

     ide_400ns_delay(io);
    uint8_t cmd = (drive == ATA_MASTER ? ATA_CMD_MASTER : ATA_CMD_SLAVE);
    // uint8_t slavebit = (drive == ATA_MASTER ? ATA_MASTER : ATA_SLAVE);

    x86_outb(io + ATA_REG_HDDEVSEL, (cmd | (uint8_t)((lba >> 24 & 0x0F))));
    x86_outb(io + 1, 0x00);
    x86_outb(io + ATA_REG_SECCOUNT0, 1);
    x86_outb(io + ATA_REG_LBA0, (uint8_t)((lba)));
    x86_outb(io + ATA_REG_LBA1, (uint8_t)((lba) >> 8));
    x86_outb(io + ATA_REG_LBA2, (uint8_t)((lba) >> 16));
    x86_outb(io + ATA_REG_COMMAND, ATA_CMD_READ_PIO);

    ide_poll(io);

    for(int i = 0; i < 256; i++) {
		uint16_t data = x86_inw(io + ATA_REG_DATA);
		*(uint16_t *)(buffer + i * 2) = data;
	}
    ide_400ns_delay(io);
}

void ata_read(uint32_t lba, uint16_t sectorNum, void* buffer, device* dev) {
    for (int i = 0; i < sectorNum;i++) {
        ata_read_one(lba + i, buffer, dev);
        buffer += 512;
    }
}
