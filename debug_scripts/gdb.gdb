set disassembly-flavor intel
target remote | qemu-system-i386 -S -gdb stdio -m 32 -hda build/disk.img