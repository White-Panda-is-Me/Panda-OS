ARCH = x86_64
TARGET = $(BUILD_DIR)/kernel.elf
SRCS = kernel.c
CFILES=$(wildcard *.c) $(wildcard */*.c) $(wildcard */*/*.c) $(wildcard */*/*/*.c)
ASMFILES=$(wildcard *.asm) $(wildcard */*.asm) $(wildcard */*/*.asm) $(wildcard */*/*/*.asm)
COBJFILES=$(patsubst %.c, $(BUILD_DIR)/kernel/c/%.o, $(CFILES))
ASMOBJFILES=$(patsubst %.asm, $(BUILD_DIR)/kernel/asm/%.o, $(ASMFILES))
all: $(TARGET)

CC = gcc
LD = ld
ASM = nasm

$(TARGET): $(COBJFILES) $(ASMOBJFILES)
	mkdir -p $(BUILD_DIR)/kernel
	$(LD) -nostdlib -z max-page-size=0x1000 -Ttext=0x01000000 $(COBJFILES)$(ASMOBJFILES) -o $@

$(BUILD_DIR)/kernel/c/%.o: %.c
	@mkdir -p $(@D)
	$(CC) -I. -ffreestanding -std=c99 -fno-stack-protector -fno-stack-check -D__$(ARCH)__ -c $< -o $@

$(BUILD_DIR)/kernel/asm/%.o: %.asm
	@mkdir -p $(@D)
	$(ASM) -f elf64 -o $@ $<
