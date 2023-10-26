# SRC_DIR?=src
ASM?=nasm
CC=i686-elf-gcc
CFILES=$(wildcard *.c)
ASMFILES=$(wildcard *.asm)
COBJFILES=$(patsubst %.c, $(BUILD_DIR)/stage2/c/%.obj, $(CFILES))
ASMOBJFILES=$(patsubst %.asm, $(BUILD_DIR)/stage2/asm/%.obj, $(ASMFILES))
COMMONCOBJFILES=$(shell find $(BUILD_DIR)/common/c/*.obj)
COMMONASMOBJFILES=$(shell find $(BUILD_DIR)/common/asm/*.obj)
CFLAGS=-std=c99 -g -ffreestanding -nostdlib -Wall -Werror -Wno-unused-variable -I. -I../../common
LDFLAGS=-T linker.ld -nostdlib

.PHONY: all stage2 clean always

all: stage2

stage2: $(BUILD_DIR)/stage2.bin

$(BUILD_DIR)/stage2.bin: $(ASMOBJFILES) $(COBJFILES)
	$(CC) $(LDFLAGS) -Wl,--oformat=binary -Wl,-Map=$(BUILD_DIR)/stage2.map -o $@ $^ -lgcc

$(BUILD_DIR)/stage2/c/%.obj: %.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/stage2/asm/%.obj: %.asm
	mkdir -p $(@D)
	$(ASM) -f elf32 -o $@ $<
