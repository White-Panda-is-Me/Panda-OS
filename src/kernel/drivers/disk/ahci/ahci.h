#pragma once
#include <stdint.h>

// PCI Configuration Space Registers
#define PCI_CONFIG_VENDOR_ID    0x00
#define PCI_CONFIG_DEVICE_ID    0x02
#define PCI_CONFIG_CLASS        0x0B
#define PCI_CONFIG_SUBCLASS     0x0A
#define PCI_CONFIG_PROG_IF      0x09
#define PCI_CONFIG_HEADER_TYPE  0x0E
#define PCI_CONFIG_BAR5         0x24

// AHCI Controller Class and Subclass
#define PCI_CLASS_STORAGE       0x01
#define PCI_SUBCLASS_SATA       0x06
#define PCI_PROG_IF_AHCI        0x01

typedef struct {
    uint32_t clb;           // Command List Base Address
    uint32_t clbu;          // Command List Base Address Upper 32-Bits
    uint32_t fb;            // FIS Base Address
    uint32_t fbu;           // FIS Base Address Upper 32-Bits
    uint32_t is;            // Interrupt Status
    uint32_t ie;            // Interrupt Enable
    uint32_t cmd;           // Command and Status
    uint32_t res0;           // Reserved
    uint32_t tfd;           // Task File Data
    uint32_t sig;           // Signature
    uint32_t ssts;          // Serial ATA Status (SCR0: SStatus)
    uint32_t sctl;          // Serial ATA Control (SCR2: SControl)
    uint32_t serr;          // Serial ATA Error (SCR1: SError)
    uint32_t sact;          // Serial ATA Active (SCR3: SActive)
    uint32_t ci;            // Command Issue
    uint32_t sntf;          // Serial ATA Notification (SCR4: SNotification)
    uint32_t fbs;           // FIS-based Switching Control
    uint8_t devslp;         // Device Sleep
    uint8_t res1[10];       // Reserved
    uint32_t vs[16];        // Vendor Specific
} __attribute__((packed)) HBA_PORT;

typedef struct {
    // Generic Host Control
    uint32_t cap;           // Host Capabilities
    uint32_t ghc;           // Global Host Control
    uint32_t is;            // Interrupt Status
    uint32_t pi;            // Ports Implemented
    uint32_t vs;            // Version
    uint32_t ccc_ctl;       // Command Completion Coalescing Control
    uint32_t ccc_ports;     // Command Completion Coalsecing Ports
    uint32_t em_loc;        // Enclosure Management Location
    uint32_t em_ctl;        // Enclosure Management Control
    uint32_t cap2;          // Host Capabilities Extended
    uint32_t bohc;          // BIOS/OS Handoff Control and Status

    // Shit Resereved
    uint8_t reserved[0x74];         // 0xA0 - 0x2C = 0x74

    // Vendor
    uint8_t  vendor[0x100-0xA0];

    // 32 Ports
    HBA_PORT ports[2];              // 2 ports are absolutely enough for us
} __attribute__((packed)) HBA_MEM_REGS;

HBA_MEM_REGS* find_ahci_controllers();
uint32_t pci_read(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset);
