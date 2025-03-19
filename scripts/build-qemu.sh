#!/usr/bin/bash
mkdir -p qemu/build
cd qemu/build
../configure --target-list=aarch64-softmmu --disable-werror --enable-virtfs
make -j$(nproc)
