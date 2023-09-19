# BUILD_DIR?=build
SRC_DIR?=src
ASM?=nasm
CC=i686-elf-gcc
CFILES=$(shell find *.c) $(shell find cpu/*.c)# $(shell find drivers/disk/*.c)
ASMFILES=$(shell find *.asm)
COBJFILES=$(patsubst %.c, $(BUILD_DIR)/kernel/c/%.obj, $(CFILES))
ASMOBJFILES=$(patsubst %.asm, $(BUILD_DIR)/kernel/asm/%.obj, $(ASMFILES))
CFLAGS=-std=c99 -g -ffreestanding -nostdlib -Wall -Werror
LDFLAGS=-T linker.ld -nostdlib

.PHONY: all kernel clean always

all: kernel

kernel: $(BUILD_DIR)/kernel.bin

$(BUILD_DIR)/kernel.bin: $(ASMOBJFILES) $(COBJFILES)
	$(CC) $(LDFLAGS) -Wl,--oformat=binary -Wl,-Map=$(BUILD_DIR)/kernel.map -o $@ $^ -lgcc

$(BUILD_DIR)/kernel/c/%.obj: %.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/kernel/asm/%.obj: %.asm
	mkdir -p $(@D)
	$(ASM) -f elf -o $@ $<
