#include "pic.h"
#include "idt.h"
#include "isr.h"
#include <x86.h>
#include <stdio.h>

// PIC ports
#define PIC1_CMD_PORT  0x20
#define PIC1_DATA_PORT 0x21
#define PIC2_CMD_PORT  0xA0
#define PIC2_DATA_PORT 0xA1

#define PIC_CMD_EOI    0x20

// Initialization function for PIC
void pic_init() {
    // ICW1 - Initialization Command Word 1
    x86_outb(PIC1_CMD_PORT, 0x11);  // Initialize PIC1 // 0b00010001
    x86_outb(PIC2_CMD_PORT, 0x11);  // Initialize PIC2

    // ICW2 - Initialization Command Word 2
    x86_outb(PIC1_DATA_PORT, 0x20); // Set PIC1 base interrupt vector to 0x20
    x86_outb(PIC2_DATA_PORT, 0x28); // Set PIC2 base interrupt vector to 0x28
    
    // ICW3 - Initialization Command Word 3
    x86_outb(PIC1_DATA_PORT, 0x04); // Set PIC1 to cascade to IRQ2 (connected to PIC2)
    x86_outb(PIC2_DATA_PORT, 0x02); // Set PIC2 as slave
    
    // ICW4 - Initialization Command Word 4
    x86_outb(PIC1_DATA_PORT, 0x01); // Set PIC1 to 8086/88 mode
    x86_outb(PIC2_DATA_PORT, 0x01); // Set PIC2 to 8086/88 mode

    // Reseting masks
    x86_outb(PIC1_DATA_PORT, 0);
	x86_outb(PIC2_DATA_PORT, 0);
}

// Function to enable or disable IRQ line
void pic_set_irq_line(uint8_t irq, int enable) {
    uint16_t port;
    uint8_t value;

    if (irq < 8) {
        port = PIC1_DATA_PORT;
    } else {
        port = PIC2_DATA_PORT;
        irq -= 8;
    }

    value = x86_inb(port);

    if (enable) {
        value &= ~(1 << irq);
    } else {
        value |= (1 << irq);
    }

    x86_outb(port, value);
}

void send_eoi(uint8_t irq) {
    // If the interrupt came from the master PIC
    x86_outb(PIC1_CMD_PORT, PIC_CMD_EOI);  // Send EOI to the master PIC
    if (irq >= 8)
        x86_outb(PIC2_CMD_PORT, PIC_CMD_EOI);  // Send EOI to the slave PIC
}
