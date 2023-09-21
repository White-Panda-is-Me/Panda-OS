SRC_DIR?=src
ASM?=nasm
CC=i686-elf-gcc
CFILES=$(wildcard *.c)
ASMFILES=$(wildcard *.asm)
COBJFILES=$(patsubst %.c, $(BUILD_DIR)/kernel/c/%.obj, $(CFILES))
ASMOBJFILES=$(patsubst %.asm, $(BUILD_DIR)/kernel/asm/%.obj, $(ASMFILES))
COMMONCOBJFILES=$(shell find $(BUILD_DIR)/common/c/*.obj)
COMMONASMOBJFILES=$(shell find $(BUILD_DIR)/common/asm/*.obj)
CFLAGS=-std=c99 -g -ffreestanding -nostdlib -Wall -Werror -Wno-unused-variable -I. -I$(SRC_DIR)/common
LDFLAGS=-T linker.ld -nostdlib

.PHONY: all kernel clean always

all: kernel

kernel: $(BUILD_DIR)/kernel.bin

$(BUILD_DIR)/kernel.bin: $(ASMOBJFILES) $(COMMONASMOBJFILES) $(COBJFILES) $(COMMONCOBJFILES)
	$(CC) $(LDFLAGS) -Wl,--oformat=binary -Wl,-Map=$(BUILD_DIR)/kernel.map -o $@ $^ -lgcc

$(BUILD_DIR)/kernel/c/%.obj: %.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/kernel/asm/%.obj: %.asm
	mkdir -p $(@D)
	$(ASM) -f elf32 -o $@ $<
