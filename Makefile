#normal object files
kernel_source_files := $(shell find src/impl/kernel -name '*.c')
kernel_object_files := $(patsubst src/impl/kernel/%.c, build/ShitOS/kernel/%.o, $(kernel_source_files))

driver_c_source_files := $(shell find src/impl/drivers -name '*.c')
driver_c_object_files := $(patsubst src/impl/drivers/%.c, build/ShitOS/drivers/%.o, $(driver_c_source_files))

x86_c_source_files := $(shell find src/impl/x86 -name '*.c')
x86_c_object_files := $(patsubst src/impl/x86/%.c, build/ShitOS/x86/%.o, $(x86_c_source_files))

x86_asm_source_files := $(shell find src/impl/x86 -name '*.s')
x86_asm_object_files := $(patsubst src/impl/x86/%.s, build/ShitOS/x86/%.o, $(x86_asm_source_files))

x86_object_files := $(x86_c_object_files) $(driver_c_object_files) $(x86_asm_object_files)

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

CROSS_GCC := $(HOME)/opt/cross/bin/i686-elf-gcc
CROSS_AS  := $(HOME)/opt/cross/bin/i686-elf-as
CFLAGS    := -I src/intf -ffreestanding -O2 -Wall -nostdlib
TEST_FLAGS := -I src/intf -I src/test/tests -I src/test/testlib -I src/imple/drivers -ffreestanding -O2 -Wall -nostdlib

# normal object file
$(kernel_object_files): build/ShitOS/kernel/%.o : src/impl/kernel/%.c
	mkdir -p $(dir $@)
	$(CROSS_GCC) -c $(CFLAGS) $< -o $@

$(driver_c_object_files): build/ShitOS/drivers/%.o : src/impl/drivers/%.c
	mkdir -p $(dir $@)
	$(CROSS_GCC) -c $(CFLAGS) $< -o $@

$(x86_c_object_files): build/ShitOS/x86/%.o : src/impl/x86/%.c
	mkdir -p $(dir $@)
	$(CROSS_GCC) -c $(CFLAGS) $< -o $@

$(x86_asm_object_files): build/ShitOS/x86/%.o : src/impl/x86/%.s
	mkdir -p $(dir $@)
	$(CROSS_AS) $< -o $@

#object files for test kernel
$(test_kernel_object_files): build/test/kernel/%.o : src/test/testkernel/%.c
	mkdir -p $(dir $@)
	$(CROSS_GCC) -c $(TEST_FLAGS) $< -o $@

$(test_driver_c_object_files): build/test/drivers/%.o : src/impl/drivers/%.c
	mkdir -p $(dir $@)
	$(CROSS_GCC) -c $(TEST_FLAGS) $< -o $@

$(test_x86_c_object_files): build/test/x86/%.o : src/impl/x86/%.c
	mkdir -p $(dir $@)
	$(CROSS_GCC) -c $(TEST_FLAGS) $< -o $@

$(test_x86_asm_object_files): build/test/x86/%.o : src/impl/x86/%.s
	mkdir -p $(dir $@)
	$(CROSS_AS) $< -o $@

$(test_object_files): build/test/x86/%.o : src/test/tests/%.c
	mkdir -p $(dir $@)
	$(CROSS_GCC) -c $(TEST_FLAGS) $< -o $@

$(testlib_object_files): build/test/x86/%.o : src/test/testlib/%.c
	mkdir -p $(dir $@)
	$(CROSS_GCC) -c $(TEST_FLAGS) $< -o $@

.PHONY: clean build-x86 run build-objs build-directories iso kernel test-x86
build-x86: build-objs
	mkdir -p dist/x86 && \
	mkdir -p disc/x86 && \
	$(CROSS_GCC) -o dist/x86/kernel.bin -T targets/x86/linker.ld -ffreestanding -O2 -nostdlib $(kernel_object_files) $(x86_object_files) && \
	cp dist/x86/kernel.bin targets/x86/iso/boot/grub && \
	grub-mkrescue /usr/lib/grub/i386-pc -o disc/x86/kernel.iso targets/x86/iso

clean:
	rm -rf build/ dist/ disc/ && \
	rm targets/x86/iso/boot/grub/kernel.bin

build-objs: $(kernel_object_files) $(x86_object_files)

test-x86: $(test_kernel_object_files) $(test_x86_object_files)
	$(info test_kernel_object_files = $(test_kernel_object_files))
	$(info test_x86_object_files = $(test_x86_object_files))
	mkdir -p dist/test_x86 && \
	mkdir -p disc/test_x86 && \
	$(CROSS_GCC) -o dist/test_x86/kernel.bin -T targets/test_x86/linker.ld -ffreestanding -O2 -nostdlib $(test_kernel_object_files) $(test_x86_object_files) && \
	cp dist/test_x86/kernel.bin targets/test_x86/iso/boot/grub && \
	grub-mkrescue /usr/lib/grub/i386-pc -o disc/test_x86/kernel.iso targets/test_x86/iso

kernel: build-objs
	mkdir -p dist/x86 && \
	$(CROSS_GCC) -o dist/x86/kernel.bin -T targets/x86/linker.ld -ffreestanding -O2 -nostdlib $(kernel_object_files) $(x86_object_files)

iso:
	mkdir -p disc/x86 && \
	cp dist/x86/kernel.bin targets/x86/iso/boot/grub && \
    grub-mkrescue /usr/lib/grub/i386-pc -o disc/x86/kernel.iso targets/x86/iso

run-test: test-x86
	qemu-system-i386 -cdrom ./disc/test_x86/kernel.iso

run: build-x86
	qemu-system-i386 -cdrom ./disc/x86/kernel.iso