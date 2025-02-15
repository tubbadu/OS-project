#!/usr/bin/bash

cd /mnt/qemu/build-riscv
../configure --target-list=riscv64-softmmu --disable-werror
make -j4
