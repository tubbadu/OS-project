#!/usr/bin/bash
mkdir -p qemu/build-arm
cd qemu/build-arm
../configure --target-list=aarch64-softmmu --disable-werror --enable-virtfs
make -j$(nproc)
