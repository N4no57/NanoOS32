# ShitOS

**ShitOS** is a simple hobby operating system for x86 PCs, written in x86 assembly and C.  
It's designed for learning OS fundamentals such as bootloading, kernel development, and hardware interfacing.

---

## 🚀 Features

- Multiboot-compatible bootloader (via GRUB)
- VGA text-mode output
- Kernel written in C
- Freestanding cross-compilation toolchain
- ISO image creation with `grub-mkrescue`

---

## 🔧 Building

See [docs/build.md](docs/build.md) for full instructions.  
Quick setup:

```
make build-x86
qemu-system-i386 -cdrom ./disc/x86/kernel.iso