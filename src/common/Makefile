# BUILD_DIR?=build
SRC_DIR?=src
ASM?=nasm
CC=i686-elf-gcc
CFILES=$(shell find *.c)# $(shell find cpu/*.c) $(shell find drivers/disk/*.c)
ASMFILES=$(shell find *.asm)
COBJFILES=$(patsubst %.c, $(BUILD_DIR)/common/c/%.obj, $(CFILES))
ASMOBJFILES=$(patsubst %.asm, $(BUILD_DIR)/common/asm/%.obj, $(ASMFILES))
CFLAGS=-std=c99 -g -ffreestanding -nostdlib -Wall -Werror -I. -I$(SRC_DIR)/common
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
