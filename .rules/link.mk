build-x86: build-objs
	$(CROSS_GCC) -o build/kernel.elf -T linker.ld $(LDFLAGS) -ffreestanding -nostdlib $(kernel_object_files) $(x86_object_files)