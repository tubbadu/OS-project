#!/usr/bin/bash

kate \
	qemu/hw/misc/fft_core.c \
	qemu/include/hw/misc/fft_core.h \
	qemu/hw/riscv/Kconfig \
	qemu/hw/misc/meson.build \
	qemu/hw/riscv/virt.c \
	qemu/hw/misc/Kconfig \
	qemu/include/hw/riscv/virt.h 
