BUILD ?= release
include .config/common.mk
include .config/$(BUILD).mk

include .rules/compile.mk
include .rules/clean.mk
include .rules/link.mk

x86_object_files := $(x86_c_object_files) $(driver_c_object_files) $(x86_asm_object_files) $(x86_nasm_object_files)

build-objs: $(kernel_object_files) $(x86_object_files)

.PHONY: kernel_image kernel clean

kernel_image: kernel
	mkdir -p os-image/boot/grub
	cp build/kernel.elf os-image/boot/
	cp grub.cfg os-image/boot/grub/
	dd if=/dev/zero of=kernel.img bs=512 count=62500
	mkfs.fat -F 16 kernel.img  62500
	mmd -i kernel.img ::/boot
	mmd -i kernel.img ::/boot/grub
	mcopy -i kernel.img build/kernel.elf ::/boot/
	mcopy -i kernel.img grub.cfg ::/boot/grub/
	@LOOPDEV=$$(sudo losetup --find --show kernel.img); \
	sudo mkdir -p mnt; \
	sudo mount $$LOOPDEV mnt; \
	sudo grub-install --target=i386-pc \
	                  --directory=/usr/lib/grub/i386-pc \
	                  --modules="normal multiboot" \
	                  --boot-directory=mnt/boot \
	                  --no-floppy \
	                  --force \
	                  --removable \
	                  $$LOOPDEV; \
	sudo umount mnt; \
	sudo losetup -d $$LOOPDEV; \
	sudo rm -r mnt

kernel: build-x86