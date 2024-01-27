export BUILD_DIR = $(abspath build)

$(BUILD_DIR)/disk.img: $(BUILD_DIR)/part.img
	dd if=/dev/zero of=$@ bs=512 count=93750
	parted $@ -s -a minimal mklabel gpt
	parted $@ -s -a minimal mkpart EFI FAT32 2048s 93716s
	parted $@ -s -a minimal toggle 1 boot
	dd if=$< of=$@ bs=512 count=91669 seek=2048 conv=notrunc
	rm src/main.o 

$(BUILD_DIR)/part.img: BOOTX64.efi kernel.elf
	dd if=/dev/zero of=$@ bs=512 count=91669    
	mformat -i $@ -h 32 -t 32 -n 64 -c 1
	mmd -i $@ "::EFI"
	mmd -i $@ "::EFI/Boot"
	mmd -i $@ "::kernel"
	mcopy -i $@ $(BUILD_DIR)/BOOTX64.efi "::EFI/Boot/BOOTX64.efi"
	mcopy -i $@ font.sfn "::EFI/Boot/font.sfn"
	mcopy -i $@ $(BUILD_DIR)/kernel.elf "::kernel/kernel.elf"

BOOTX64.efi: src/main.c
	$(MAKE) -C src/

kernel.elf:
	$(MAKE) -C src/kernel/

c:
	rm -r $(BUILD_DIR)/*
