#!/bin/sh
source .env
cd buildroot/output/images/

exec $QEMU_ARM -M virt \
	-cpu cortex-a53 -nographic -smp 1 -kernel Image \
	-append "rootwait root=/dev/vda console=ttyAMA0" \
	-netdev user,id=eth0 -device virtio-net-device,netdev=eth0 \
	-drive file=rootfs.ext4,if=none,format=raw,id=hd0 \
	-device virtio-blk-device,drive=hd0 
