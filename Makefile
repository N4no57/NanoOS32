x86_asm_source_files := $(shell find src/x86 -name '*.s')
x86_asm_object_files := $(patsubst src/x86/%.s, build/x86/%.o, $(x86_asm_source_files))

$(x86_asm_object_files): build/x86/%.o : src/x86/%.s
	mkdir -p $(dir $@)
	$(HOME)/opt/cross/bin/i686-elf-as $< -o $@

.PHONY: build-x86
build-x86: $(x86_asm_object_files)
	mkdir -p dist/x86 && \
	$(HOME)/opt/cross/bin/i686-elf-gcc -o dist/x86/kernel.bin -T targets/x86/linker.ld -ffreestanding -O2 -nostdlib $(x86_asm_object_files) && \
	cp dist/x86/kernel.bin targets/x86/iso/boot/grub && \
	grub-mkrescue /usr/lib/grub/i386-pc -o disc/x86/kernel.iso targets/x86/iso