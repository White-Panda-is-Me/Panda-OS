# BUILD_DIR?=build
SRC_DIR?=src
ASM?=nasm
CC=i686-elf-gcc
CFILES=$(wildcard *.c) $(wildcard */*.c) $(wildcard */*/*.c) $(wildcard */*/*/*.c)
ASMFILES=$(wildcard *.asm) $(wildcard */*.asm) $(wildcard */*/*.asm) $(wildcard */*/*/*.asm)
COBJFILES=$(patsubst %.c, $(BUILD_DIR)/common/c/%.obj, $(CFILES))
ASMOBJFILES=$(patsubst %.asm, $(BUILD_DIR)/common/asm/%.obj, $(ASMFILES))
CFLAGS=-std=c99 -g -ffreestanding -nostdlib -Wall -Werror -I.
LDFLAGS=-T linker.ld -nostdlib

.PHONY: all common clean always

all: common

common: $(COBJFILES) $(ASMOBJFILES)

$(BUILD_DIR)/common/c/%.obj: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/common/asm/%.obj: %.asm
	@mkdir -p $(@D)
	$(ASM) -f elf -o $@ $<