BUILD_DIR?=../../../build
SRC_DIR?=src
ASM?=nasm
CC=i686-elf-gcc
LD=i686-elf-ld
OBJ=i686-elf-objcopy
CFILES=$(shell find -f /Users/mohammad/programming/assembly/bootloader/src/bootloader/stage2/*.c)
ASMFILES=$(shell find -f /Users/mohammad/programming/assembly/bootloader/src/bootloader/stage2/*.asm)
COBJFILES=$(CFILES:.c=.o)
ASMOBJFILES=$(ASMFILES:.asm=.o)
CFLAGS=-m32 -c -g -Os -march=i686 -ffreestanding -Wall -Werror -I. $(CFILES)
LDFLAGS=-static -T linker.ld -nostdlib --nmagic -o $(BUILD_DIR)/stage2.elf $(COBJFILES) $(ASMOBJFILES)
OBJFLAGS=-O binary $(BUILD_DIR)/stage2.elf stage2.bin
ASMFLAGS=-f bin 

$(BUILD_DIR)/stage2.bin: $(BUILD_DIR)/stage2.elf
	$(OBJ) -O binary $(BUILD_DIR)/stage2.elf stage2.bin

$(BUILD_DIR)/stage2.elf: $(CFILES) $(ASMFILES)
	$(CC) $(CFLAGS)
	nasm -f elf entry.asm -o entry.o
	nasm -f elf x86_func.asm -o x86_func.o
	$(LD) $(LDFLAGS)
	$(OBJ) $(OBJFLAGS)


# i686-elf-gcc -m32 -c -g -Os -march=i686 -ffreestanding -Wall -Werror -I. main.c my_func.c another_func.c
# i686-elf-ld -static -T linker.ld -nostdlib --nmagic -o my_program.elf main.o my_func.o another_func.o