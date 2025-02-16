#!/usr/bin/bash

cd qemu/build-arm
../configure --target-list=aarch64-softmmu --disable-werror
make -j$(nproc)
