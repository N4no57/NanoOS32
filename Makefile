kernel_source_files := $(shell find src/impl/kernel -name '*.c')
kernel_object_files := $(patsubst src/impl/kernel/%.c, build/kernel/%.o, $(kernel_source_files))

x86_c_source_files := $(shell find src/impl/x86 -name '*.c')
x86_c_object_files := $(patsubst src/impl/x86/%.c, build/x86/%.o, $(x86_c_source_files))

x86_asm_source_files := $(shell find src/impl/x86 -name '*.s')
x86_asm_object_files := $(patsubst src/impl/x86/%.s, build/x86/%.o, $(x86_asm_source_files))

x86_object_files := $(x86_c_object_files) $(x86_asm_object_files)

CROSS_GCC := $(HOME)/opt/cross/bin/i686-elf-gcc
CROSS_AS  := $(HOME)/opt/cross/bin/i686-elf-as
CFLAGS    := -I src/intf -ffreestanding -O2 -Wall

$(kernel_object_files): build/kernel/%.o : src/impl/kernel/%.c
	mkdir -p $(dir $@)
	$(CROSS_GCC) -c $(CFLAGS) $< -o $@

$(x86_c_object_files): build/x86/%.o : src/impl/x86/%.c
	mkdir -p $(dir $@)
	$(CROSS_GCC) -c $(CFLAGS) $< -o $@

$(x86_asm_object_files): build/x86/%.o : src/impl/x86/%.s
	mkdir -p $(dir $@)
	$(CROSS_AS) $< -o $@

.PHONY: clean build-x86 run build-objs build-directories iso kernel
build-x86: build-objs
	mkdir -p dist/x86 && \
	mkdir -p disc/x86 && \
	$(HOME)/opt/cross/bin/i686-elf-gcc -o dist/x86/kernel.bin -T targets/x86/linker.ld -ffreestanding -O2 -nostdlib $(kernel_object_files) $(x86_object_files) && \
	cp dist/x86/kernel.bin targets/x86/iso/boot/grub && \
	grub-mkrescue /usr/lib/grub/i386-pc -o disc/x86/kernel.iso targets/x86/iso

clean:
	rm -rf build/ dist/ disc/ && \
	rm targets/x86/iso/boot/grub/kernel.bin

build-objs: $(kernel_object_files) $(x86_c_object_files) $(x86_asm_object_files)

kernel:
	mkdir -p dist/x86 && \
	$(HOME)/opt/cross/bin/i686-elf-gcc -o dist/x86/kernel.bin -T targets/x86/linker.ld -ffreestanding -O2 -nostdlib $(kernel_object_files) $(x86_object_files)

iso:
	mkdir -p disc/x86 && \
	cp dist/x86/kernel.bin targets/x86/iso/boot/grub && \
    grub-mkrescue /usr/lib/grub/i386-pc -o disc/x86/kernel.iso targets/x86/iso

run: build-x86
	qemu-system-i386 -cdrom ./disc/x86/kernel.iso