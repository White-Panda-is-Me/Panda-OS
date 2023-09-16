SRC_DIR?=src
ASM?=nasm
CC=i686-elf-gcc
CFILES=$(shell find *.c) $(shell find driver/cpu/*.c) $(shell find driver/disk/*.c)
ASMFILES=$(shell find *.asm)
COBJFILES=$(patsubst %.c, $(BUILD_DIR)/util/c/%.obj, $(CFILES))
ASMOBJFILES=$(patsubst %.asm, $(BUILD_DIR)/util/asm/%.obj, $(ASMFILES))
CFLAGS=-std=c99 -g -ffreestanding -nostdlib -Wall -Werror -I.
LDFLAGS=-T linker.ld -nostdlib

.PHONY: all util clean always

all: util

util: $(COBJFILES) $(ASMOBJFILES)

$(COBJFILES): $(CFILES)
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -o $@ $<

$(ASMOBJFILES): $(ASMFILES)
	mkdir -p $(@D)
	$(ASM) -f elf -o $@ $<
