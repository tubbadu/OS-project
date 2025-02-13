#!/usr/bin/bash
cd ./qemu
kate hw/misc/crypto_core.c include/hw/misc/crypto_core.h hw/riscv/Kconfig hw/misc/meson.build hw/riscv/virt.c hw/misc/Kconfig include/hw/riscv/virt.h
