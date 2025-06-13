x86_asm_source_files := $(shell find src/x86 -name '*.s')
x86_asm_object_files := $(patsubst src/x86/%.s, build/x86/%.o, $(x86_asm_source_files))

$(x86_asm_object_files): build/x86/%.o : src/x86/%.s
	mkdir -p $(dir $@)
	$(HOME)/opt/cross/bin/i686-elf-as $< -o $@