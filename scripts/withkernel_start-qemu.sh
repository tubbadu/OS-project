#!/bin/bash

source .env

KERNEL_IMAGE="$BASEDIR/buildroot/output/images/Image"
ROOTFS_IMAGE="$BASEDIR/buildroot/output/images/rootfs.ext4"
MACHINE_TYPE="virt"
CPU_TYPE="cortex-a53"
MEMORY_SIZE="512M"
NET_OPTIONS="-netdev user,id=net0 -device virtio-net-device,netdev=net0"
DRIVE_OPTIONS="-drive file=$ROOTFS_IMAGE,if=none,id=hd0 -device virtio-blk-device,drive=hd0"

$QEMU_ARM \
    -machine $MACHINE_TYPE \
    -cpu $CPU_TYPE \
    -m $MEMORY_SIZE \
    -kernel $KERNEL_IMAGE \
    -append "root=/dev/vda rw console=ttyAMA0" \
    -nographic \
    $NET_OPTIONS \
    $DRIVE_OPTIONS
