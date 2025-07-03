build-x86: headers
	mkdir -vp dist/x86 && \
	mkdir -vp disc/x86 && \
	$(CROSS_GCC) -o dist/x86/kernel.bin -T targets/x86/linker.ld $(LDFLAGS) -ffreestanding -nostdlib $(kernel_object_files) $(x86_object_files) && \
	cp dist/x86/kernel.bin $(sysroot)/boot && \
	grub-mkrescue /usr/lib/grub/i386-pc -o disc/x86/kernel.iso $(sysroot)