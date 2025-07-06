$(kernel_object_files): build/kernel/%.o : src/impl/kernel/%.c
	mkdir -vp $(dir $@)
	$(CROSS_GCC) -c $(CFLAGS) $< -o $@

$(driver_c_object_files): build/drivers/%.o : src/impl/drivers/%.c
	mkdir -vp $(dir $@)
	$(CROSS_GCC) -c $(CFLAGS) $< -o $@

$(x86_c_object_files): build/x86/%.o : src/impl/x86/%.c
	mkdir -vp $(dir $@)
	$(CROSS_GCC) -c $(CFLAGS) $< -o $@

$(x86_asm_object_files): build/x86/%.o : src/impl/x86/%.s
	mkdir -vp $(dir $@)
	$(CROSS_AS) $< -o $@

$(x86_nasm_object_files): build/x86/%.o : src/impl/x86/%.asm
	mkdir -vp $(dir $@)
	nasm -f elf32 $< -o $@