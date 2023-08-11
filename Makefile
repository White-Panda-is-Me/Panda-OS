export BUILD_DIR=$(abspath build)
SRC_DIR=src
FORMAT=format
COPY=copy
ASM=nasm

.PHONY: run

disk: $(BUILD_DIR)/disk.img

$(BUILD_DIR)/disk.img: bootloader kernel
	$(FORMAT) ./build/disk.img
	$(COPY) ./build/disk.img ./build/stage1.bin
	$(COPY) ./build/disk.img ./build/stage2.bin
	$(COPY) ./build/disk.img ./build/kernel.bin


util: $(BUILD_DIR)/util

$(BUILD_DIR)/util/c/%.obj $(BUILD_DIR)/util/asm/%.obj: $(SRC_DIR)/util
	$(MAKE) -C src/util

# # # # # # # # # #
#                 #
#    bootloader   #
#                 #
# # # # # # # # # #

bootloader: stage1 stage2

stage1: $(BUILD_DIR)/stage1.bin

$(BUILD_DIR)/stage1.bin: $(SRC_DIR)/bootloader/stage1/stage1.asm
	mkdir build
	$(ASM) -g $(SRC_DIR)/bootloader/stage1/stage1.asm -f bin -o $(BUILD_DIR)/stage1.bin

stage2: $(BUILD_DIR)/stage2.bin

$(BUILD_DIR)/stage2.bin: $(SRC_DIR)/bootloader/stage2
	$(MAKE) -C $(SRC_DIR)/util
	$(MAKE) -C $(SRC_DIR)/bootloader/stage2


# # # # # # # # # #
#                 #
#      kernel     #
#                 #
# # # # # # # # # #

kernel: $(BUILD_DIR)/kernel.bin

$(BUILD_DIR)/kernel.bin: $(SRC_DIR)/kernel/kernel.asm
	$(MAKE) -C $(SRC_DIR)/kernel


# # # # # # # # # #
#                 #
#       run       #
#                 #
# # # # # # # # # #

run:
	qemu-system-x86_64 -fda $(BUILD_DIR)/disk.img


# # # # # # # # # #
#                 #
#      clean      #
#                 #
# # # # # # # # # #

clean:
	rm -r $(BUILD_DIR)
