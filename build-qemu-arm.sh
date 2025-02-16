#!/usr/bin/bash

cd /mnt/qemu/build-arm
../configure --target-list=aarch64-softmmu --disable-werror
make -j$(nproc)
