#pragma once
#include <stdint.h>

void chs_ata_read(uint16_t cylinders, uint8_t head, uint8_t sector, uint8_t sectorCount, uint8_t* buffer);