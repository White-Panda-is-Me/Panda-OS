ASM?=nasm
CC=i686-elf-gcc
CFILES=$(wildcard *.c) $(wildcard arch/x86/*.c)
ASMFILES=$(wildcard *.asm) $(wildcard arch/x86/*.asm)
COBJFILES=$(patsubst %.c, $(BUILD_DIR)/kernel/c/%.obj, $(CFILES))
ASMOBJFILES=$(patsubst %.asm, $(BUILD_DIR)/kernel/asm/%.obj, $(ASMFILES))
COMMONCOBJFILES=$(shell find $(BUILD_DIR)/common/c/*.obj)
COMMONASMOBJFILES=$(shell find $(BUILD_DIR)/common/asm/*.obj)
CFLAGS=-std=c99 -g -ffreestanding -nostdlib -Wall -Werror -Wextra -Wno-unused-variable -I. -I../common
LDFLAGS=-T linker.ld -nostdlib

.PHONY: all kernel clean always

all: kernel

kernel: $(BUILD_DIR)/kernel.bin

$(BUILD_DIR)/kernel.bin: $(ASMOBJFILES) $(COBJFILES) $(COMMONCOBJFILES) $(COMMONASMOBJFILES)
	$(CC) $(LDFLAGS) -Wl,--oformat=binary -Wl,-Map=$(BUILD_DIR)/kernel.map -o $@ $^ -lgcc

$(BUILD_DIR)/kernel/c/%.obj: %.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/kernel/asm/%.obj: %.asm
	mkdir -p $(@D)
	$(ASM) -f elf -o $@ $<
