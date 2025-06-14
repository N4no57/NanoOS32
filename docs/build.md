## Build Requirements
- i686-elf cross-compiler
- GRUB tools
- xorriso
- QEMU (optional, for testing)

## Build Commands
### build-x86
Builds the entire OS kernel for the x86 architecture:
* Compiles all kernel and x86-specific C and assembly source files into object files.
* Links these into the final kernel.bin.
* Prepares the ISO image using GRUB and xorriso. 

Use this when you want a fully built and bootable image.
```
make build-x86
```
### run
Builds the kernel image (via build-x86) and runs it inside QEMU for testing.
Requires QEMU to be installed.
```
make run
```
### clean
Deletes all build artifacts and intermediate files including:
* build/ directory with object files
* dist/ directory with binaries
* disc/ directory with the ISO image
* The GRUB kernel binary in targets/x86/iso/boot/grub

Use this to start fresh or free up disk space.
```
make clean
```
### iso
builds only the ISO image for the kernel.
useful if `kernel.bin` is already built.
```
make iso
```
### kernel
builds only the `kernel.bin` file
### build-objs
Compiles only the object files for kernel and x86 source code, but does not link or create the ISO.
Useful for quick compilation if you want to check for errors or build incrementally before linking.
```
make build-objs
```