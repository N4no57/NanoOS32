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
	dd if=/dev/zero of=kernel.img bs=512 count=2880
	mkfs.fat -F 12 kernel.img

kernel: build-x86