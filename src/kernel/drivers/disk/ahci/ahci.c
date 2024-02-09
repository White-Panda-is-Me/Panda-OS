#include "vendors.h"
#include "ahci.h"
#include <stdio.h>
#include <stddef.h>
#include <io.h>

uint32_t pci_read(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset) {
    uint32_t address = 0x80000000 | bus << 16 | device << 11 | function <<  8 | offset;
    x64_outl(0xCF8, address);
    for (volatile int i = 0; i < 100; ++i);
    return x64_inl(0xCFC);
}

int is_ahci_controller(uint8_t class_code, uint8_t subclass, uint8_t prog_if) {
    return (class_code == PCI_CLASS_STORAGE &&
            subclass == PCI_SUBCLASS_SATA &&
            prog_if == PCI_PROG_IF_AHCI);
}

HBA_MEM_REGS* find_ahci_controllers() { // 0xC0141F05 = 1100 0000 0001 0100 0001 1111 0101
    printf("AHCI Controller:\n");
    for (uint8_t bus = 0; bus < 0xFF; bus++) {
        for (uint8_t device = 0; device < 32; device++) {
            for (uint8_t function = 0; function < 8; function++) {
                uint16_t device_id = pci_read(bus, device, function, PCI_CONFIG_DEVICE_ID);
                uint16_t vendor_id = pci_read(bus, device, function, PCI_CONFIG_VENDOR_ID);
                if (vendor_id == 0xFFFF)
                    continue;
                // uint8_t class_code = pci_read(bus, device, function, PCI_CONFIG_CLASS);
                // uint8_t subclass = pci_read(bus, device, function, PCI_CONFIG_SUBCLASS);
                // uint8_t prog_if = pci_read(bus, device, function, PCI_CONFIG_PROG_IF);
                // if(bus == 0 && device == 31 && function == 2) {
                //     printf("HERE1\n");
                // // uint16_t device_id = pci_read(bus, device, function, PCI_CONFIG_DEVICE_ID);
                //     printf("  cc: %hhu, sc: %hhu, pig: %hhu\n", class_code, subclass, prog_if);
                // }
                for (int i = 0;i < 184;i++) {
                    if (vendor_id == (vendors_ids[i] & 0xFFFF) && device_id == (vendors_ids[i] >> 16)) {
                        if(bus == 0 && device == 31 && function == 2)
                            printf("HERE2\n");

                        uint32_t bar5 = pci_read(bus, device, function, PCI_CONFIG_BAR5);
                        printf("  AHCI Base Address: %p\n", bar5);
                        printf("  AHCI Controller Vendor: %s\n", vendors_ids[i]);
                        return (HBA_MEM_REGS*) bar5;
                    }
                }
            }
        }
    }
    printf("-> No AHCI controller found!\n");
    return NULL;
}

