#pragma once
#include <stdint.h>

uint32_t __attribute__((cdecl)) ata_lba_read(uint32_t lba, uint8_t sectorCount, void* Buffer);