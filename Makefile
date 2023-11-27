export BUILD_DIR=$(abspath build)
export SRC_DIR=$(abspath src)
SRC_DIR=src
FORMAT=$(HOME)/programming/C/mlfs2/build/format
COPY=$(HOME)/programming/C/mlfs2/build/copy
ASM=nasm

.PHONY: run

disk: $(BUILD_DIR)/disk.img

$(BUILD_DIR)/disk.img: bootloader kernel
	@dd if=/dev/zero of=$(BUILD_DIR)/disk.img bs=1 count=1
	@$(FORMAT) $(BUILD_DIR)/disk.img $(BUILD_DIR)/stage1.bin 20480
	@$(COPY) $(BUILD_DIR)/disk.img $(BUILD_DIR)/stage2.bin
	@$(COPY) $(BUILD_DIR)/disk.img $(BUILD_DIR)/kernel.bin
	@$(COPY) $(BUILD_DIR)/disk.img ./test.txt

# # # # # # # # # #
#                 #
#    bootloader   #
#                 #
# # # # # # # # # #

bootloader: stage1 stage2

stage1: $(BUILD_DIR)/stage1.bin

$(BUILD_DIR)/stage1.bin: $(SRC_DIR)/bootloader/stage1/stage1.asm
	@mkdir -p build
	@$(ASM) -g $(SRC_DIR)/bootloader/stage1/stage1.asm -f bin -o $(BUILD_DIR)/stage1.bin

stage2: $(BUILD_DIR)/stage2.bin

$(BUILD_DIR)/stage2.bin: $(SRC_DIR)/bootloader/stage2
	@$(MAKE) -C $(SRC_DIR)/common
	@$(MAKE) -C $(SRC_DIR)/bootloader/stage2


# # # # # # # # # #
#                 #
#      kernel     #
#                 #
# # # # # # # # # #

kernel: $(BUILD_DIR)/kernel.bin

$(BUILD_DIR)/kernel.bin: $(SRC_DIR)/kernel/main.c
	@$(MAKE) -C $(SRC_DIR)/kernel


# # # # # # # # # #
#                 #
#       run       #
#                 #
# # # # # # # # # #

run:
	qemu-system-i386 -hda $(BUILD_DIR)/disk.img


# # # # # # # # # #
#                 #
#      clean      #
#                 #
# # # # # # # # # #

clean:
	rm -r $(BUILD_DIR)
