CROSS_GCC := $(HOME)/opt/cross/bin/i686-elf-gcc
CROSS_AS  := $(HOME)/opt/cross/bin/i686-elf-as

# Common includes & flags
INCLUDES := -I src/intf -I src/drivers/floppy_driver
CFLAGS := $(INCLUDES) -ffreestanding -Wall -nostdlib

kernel_source_files := $(shell find src/impl/kernel -name '*.c')
kernel_object_files := $(patsubst src/impl/kernel/%.c, build/kernel/%.o, $(kernel_source_files))

driver_c_source_files := $(shell find src/impl/drivers -name '*.c')
driver_c_object_files := $(patsubst src/impl/drivers/%.c, build/drivers/%.o, $(driver_c_source_files))

x86_c_source_files := $(shell find src/impl/x86 -name '*.c')
x86_c_object_files := $(patsubst src/impl/x86/%.c, build/x86/%.o, $(x86_c_source_files))

x86_asm_source_files := $(shell find src/impl/x86 -name '*.s')
x86_asm_object_files := $(patsubst src/impl/x86/%.s, build/x86/%.o, $(x86_asm_source_files))

x86_nasm_source_files := $(shell find src/impl/x86 -name '*.asm')
x86_nasm_object_files := $(patsubst src/impl/x86/%.asm, build/x86/%.o, $(x86_nasm_source_files))

sysroot := targets/x86/sysroot