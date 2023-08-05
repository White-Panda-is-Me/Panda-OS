BUILD_DIR=build
SRC_DIR=src
ASM=nasm

.PHONY: run

disk: $(BUILD_DIR)/disk.img

$(BUILD_DIR)/disk.img: bootloader kernel
	cat $(BUILD_DIR)/stage1.bin $(BUILD_DIR)/stage2.bin $(BUILD_DIR)/kernel.bin > $(BUILD_DIR)/disk.img
	dd if=/dev/zero bs=512 count=2880 >> $(BUILD_DIR)/disk.img


# # # # # # # # # #
#                 #
#    bootloader   #
#                 #
# # # # # # # # # #

bootloader: stage1 stage2

stage1: $(BUILD_DIR)/stage1.bin

$(BUILD_DIR)/stage1.bin: $(SRC_DIR)/bootloader/stage1.asm
	$(ASM) -g $(SRC_DIR)/bootloader/stage1.asm -f bin -o $(BUILD_DIR)/stage1.bin

stage2: $(BUILD_DIR)/stage2.bin

$(BUILD_DIR)/stage1.bin: $(SRC_DIR)/bootloader/stage1.asm
	$(ASM) -g $(SRC_DIR)/bootloader/stage2.asm -f bin -o $(BUILD_DIR)/stage2.bin


# # # # # # # # # #
#                 #
#      kernel     #
#                 #
# # # # # # # # # #

kernel: $(BUILD_DIR)/kernel.bin

$(BUILD_DIR)/kernel.bin: $(SRC_DIR)/kernel/kernel.asm
	$(ASM) -g $(SRC_DIR)/kernel/kernel.asm -f bin -o $(BUILD_DIR)/kernel.bin


# # # # # # # # # #
#                 #
#       run       #
#                 #
# # # # # # # # # #

run:
	qemu-system-i386 -fda $(BUILD_DIR)/disk.img


# # # # # # # # # #
#                 #
#      clean      #
#                 #
# # # # # # # # # #

clean:
	rm $(BUILD_DIR)/disk.img
	rm $(BUILD_DIR)/stage1.bin
	rm $(BUILD_DIR)/kernel.bin
