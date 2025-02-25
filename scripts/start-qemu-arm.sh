#!/bin/sh
source .env
cd buildroot-arm/output/images/

exec $QEMU_ARM -M virt \
	-cpu cortex-a53 -nographic -smp 1 -kernel Image \
	-append "rootwait root=/dev/vda console=ttyAMA0" \
	-netdev user,id=eth0 -device virtio-net-device,netdev=eth0 \
	-drive file=rootfs.ext4,if=none,format=raw,id=hd0 \
	-device virtio-blk-device,drive=hd0 
# 	-virtfs local,path="$BASEDIR/test",mount_tag=host0,security_model=passthrough,id=host0

# exec $QEMU_ARM -M virt \
#     -cpu cortex-a53 -nographic -smp 1 -kernel Image \
#     -append "rootwait root=/dev/vda console=ttyAMA0" \
#     -netdev user,id=eth0 -device virtio-net-device,netdev=eth0 \
#     -drive file=rootfs.ext4,if=none,format=raw,id=hd0 \
#     -device virtio-blk-device,drive=hd0 \
#     -fsdev local,path="$BASEDIR/test",security_model=passthrough,id=host0 \
#     -device virtio-9p-pci,fsdev=host0,mount_tag=host0


# exec $QEMU_ARM -M virt \
#     -cpu cortex-a53 -nographic -smp 1 -kernel Image \
#     -append "rootwait root=/dev/vda console=ttyAMA0" \
#     -netdev user,id=eth0 -device virtio-net-device,netdev=eth0 \
#     -drive file=rootfs.ext4,if=none,format=raw,id=hd0 \
#     -device virtio-blk-device,drive=hd0 \
#     -drive file=$BASEDIR/test/shared_disk.img,if=virtio,format=qcow2,id=shared_disk
