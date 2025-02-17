#!/usr/bin/bash
mkdir -p qemu/build-riscv
cd qemu/build-riscv
../configure --target-list=riscv64-softmmu --disable-werror
make -j$(nproc)
