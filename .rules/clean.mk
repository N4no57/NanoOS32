clean:
	rm -rfv build/ dist/ disc/ && \
	rm -rfv $(sysroot)/bin && \
	rm -rfv $(sysroot)/include && \
	rm -fv $(sysroot)/boot/kernel.bin && \
	rm -fv $(test_sysroot)/boot/kernel.bin && \
	rm -fv $(sysroot)/boot/kernel.elf