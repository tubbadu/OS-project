#!/usr/bin/bash

FILES="qemu/hw/misc/fft_core.c \
qemu/include/hw/misc/fft_core.h \
qemu/hw/riscv/Kconfig \
qemu/hw/misc/meson.build \
qemu/hw/riscv/virt.c \
qemu/hw/misc/Kconfig \
qemu/include/hw/riscv/virt.h"

DEST="files"

# cd $DEST && mkdir -p qemu/hw/misc/ qemu/include/hw/misc qemu/hw/riscv qemu/include/hw/riscv
cd /mnt

for FILE in $FILES; do
    echo "Synching file: $FILE"
    cp $FILE $DEST/$FILE
done