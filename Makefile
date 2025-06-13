x86_asm_source_files := $(shell find src/x86 -name *.asm)
x86_asm_objs := $(patsubst src/x86/%.asm, build/x86/%.o $(x86_asm_source_files))


