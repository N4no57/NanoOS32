BUILD ?= release
include .config/common.mk
include .config/$(BUILD).mk

include .rules/compile.mk
include .rules/clean.mk
include .rules/$(BUILD)_link.mk
include .rules/$(BUILD)_run.mk

x86_object_files := $(x86_c_object_files) $(driver_c_object_files) $(x86_asm_object_files) $(x86_nasm_object_files)

#test kernel object files
test_kernel_source_files := $(shell find src/test/testkernel -name '*.c')
test_kernel_object_files := $(patsubst src/test/testkernel/%.c, build/test/kernel/%.o, $(test_kernel_source_files))

test_driver_c_object_files := $(patsubst src/impl/drivers/%.c, build/test/drivers/%.o, $(driver_c_source_files))

test_x86_c_object_files := $(patsubst src/impl/x86/%.c, build/test/x86/%.o, $(x86_c_source_files))

test_x86_asm_object_files := $(patsubst src/impl/x86/%.s, build/test/x86/%.o, $(x86_asm_source_files))

test_source_files := $(shell find src/test/tests -name '*.c')
test_object_files := $(patsubst src/test/tests/%.c, build/test/x86/%.o, $(test_source_files))

testlib_source_files := $(shell find src/test/testlib -name '*.c')
testlib_object_files := $(patsubst src/test/testlib/%.c, build/test/x86/%.o, $(testlib_source_files))

test_x86_object_files := $(test_object_files) $(testlib_object_files) $(test_x86_c_object_files) $(test_x86_asm_object_files) $(test_driver_c_object_files)

TEST_FLAGS := -I src/intf -I src/test/tests -I src/test/testlib -I src/imple/drivers -ffreestanding -O2 -Wall -nostdlib

# normal object file

#object files for test kernel
$(test_kernel_object_files): build/test/kernel/%.o : src/test/testkernel/%.c
	mkdir -vp $(dir $@)
	$(CROSS_GCC) -c $(TEST_FLAGS) $< -o $@

$(test_driver_c_object_files): build/test/drivers/%.o : src/impl/drivers/%.c
	mkdir -vp $(dir $@)
	$(CROSS_GCC) -c $(TEST_FLAGS) $< -o $@

$(test_x86_c_object_files): build/test/x86/%.o : src/impl/x86/%.c
	mkdir -vp $(dir $@)
	$(CROSS_GCC) -c $(TEST_FLAGS) $< -o $@

$(test_x86_asm_object_files): build/test/x86/%.o : src/impl/x86/%.s
	mkdir -vp $(dir $@)
	$(CROSS_AS) $< -o $@

$(test_object_files): build/test/x86/%.o : src/test/tests/%.c
	mkdir -vp $(dir $@)
	$(CROSS_GCC) -c $(TEST_FLAGS) $< -o $@

$(testlib_object_files): build/test/x86/%.o : src/test/testlib/%.c
	mkdir -vp $(dir $@)
	$(CROSS_GCC) -c $(TEST_FLAGS) $< -o $@

.PHONY: clean build-x86 run build-objs headers iso kernel test-x86 
build-objs: $(kernel_object_files) $(x86_object_files)

headers: build-objs
	mkdir -vp $(sysroot)/include
	cp -v src/intf/stdio.h $(sysroot)/include && \
	cp -v src/intf/stdlib.h $(sysroot)/include && \
	cp -v src/intf/string.h $(sysroot)/include && \
	cp -v src/intf/terminal.h $(sysroot)/include


test-x86: $(test_kernel_object_files) $(test_x86_object_files)
	$(info test_kernel_object_files = $(test_kernel_object_files))
	$(info test_x86_object_files = $(test_x86_object_files))
	mkdir -vp dist/test_x86 && \
	mkdir -vp disc/test_x86 && \
	$(CROSS_GCC) -o dist/test_x86/kernel.bin -T targets/test_x86/linker.ld -ffreestanding -O2 -nostdlib $(test_kernel_object_files) $(test_x86_object_files) && \
	cp dist/test_x86/kernel.bin $(test_sysroot)/boot && \
	grub-mkrescue /usr/lib/grub/i386-pc -o disc/test_x86/kernel.iso $(test_sysroot)

kernel: build-objs
	mkdir -vp dist/x86 && \
	$(CROSS_GCC) -o dist/x86/kernel.bin -T targets/x86/linker.ld -ffreestanding -O2 -nostdlib $(kernel_object_files) $(x86_object_files)

iso:
	mkdir -vp disc/x86 && \
	cp dist/x86/kernel.bin targets/x86/iso/boot/grub && \
    grub-mkrescue /usr/lib/grub/i386-vpc -o disc/x86/kernel.iso targets/x86/iso

run-test: test-x86
	qemu-system-i386 -cdrom ./disc/test_x86/kernel.iso

