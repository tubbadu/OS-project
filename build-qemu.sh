#!/usr/bin/bash

cd /mnt/qemu
./configure --target-list=riscv64-softmmu --disable-werror
make -j7
