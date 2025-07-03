run: build-x86
	qemu-system-i386 -kernel ./dist/x86/kernel.elf -s -S