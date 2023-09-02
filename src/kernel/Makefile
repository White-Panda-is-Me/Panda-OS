# BUILD_DIR?=build
SRC_DIR?=src
ASM?=nasm
CC=i686-elf-gcc
CFILES=$(shell find *.c) $(shell find cpu/*.c)
ASMFILES=$(shell find *.asm)
COBJFILES=$(patsubst %.c, $(BUILD_DIR)/kernel/c/%.obj, $(CFILES)) $(shell find -f ../../build/stage2/c/*.obj) $(shell find -f ../../build/stage2/asm/*.obj)
ASMOBJFILES=$(patsubst %.asm, $(BUILD_DIR)/kernel/asm/%.obj, $(ASMFILES))
UTIL_COBJFILES=$(shell find -f ../../build/util/c/*.obj)
UTIL_ASMOBJFILES=$(shell find -f ../../build/util/asm/*.obj)
CFLAGS=-std=c99 -g -ffreestanding -nostdlib -Wall -Werror -I../util
LDFLAGS=-T linker.ld -nostdlib

.PHONY: all kernel clean always

all: kernel

kernel: $(BUILD_DIR)/kernel.bin

$(BUILD_DIR)/kernel.bin: $(ASMOBJFILES) $(COBJFILES) $(UTIL_COBJFILES) $(UTIL_ASMOBJFILES)
	$(CC) $(LDFLAGS) -Wl,--oformat=binary -Wl,-Map=$(BUILD_DIR)/kernel.map -o $@ $^ -lgcc

$(BUILD_DIR)/kernel/c/%.obj: %.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/kernel/asm/%.obj: %.asm
	mkdir -p $(@D)
	$(ASM) -f elf -o $@ $<
