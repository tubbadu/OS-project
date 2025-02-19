#!/bin/bash

export PATH=$(echo "$PATH" | tr -d ' \t\n')
source .env

KERNEL_DIR="$BASEDIR/buildroot-arm/output/build/linux-6.12.9"
MODULE_DIR="$KERNEL_DIR/drivers/platform"
SCRIPT_DIR="$BASEDIR/scripts"
OUTPUT_DIR="$BASEDIR/buildroot-arm/output"
TARGET_DIR="$OUTPUT_DIR/target"
CROSS_COMPILE="$OUTPUT_DIR/host/bin/arm-linux-"

cp "$BASEDIR/src/kernel/fft_module.c" "$MODULE_DIR/"
cp "$BASEDIR/src/kernel/fft_module.h" "$MODULE_DIR/"

echo 'obj-$(CONFIG_FFT_MODULE) += fft_module.o' >> $MODULE_DIR/Makefile

cat <<EOL > $MODULE_DIR/Kconfig
config FFT_MODULE
    tristate "FFT Hardware Accelerator Module"
    depends on ARM64
    default n
    help
      This is a driver for the FFT hardware accelerator.
EOL

if ! grep -q 'source "drivers/platform/Kconfig"' $KERNEL_DIR/drivers/Kconfig; then
    echo 'source "drivers/platform/Kconfig"' >> $KERNEL_DIR/drivers/Kconfig
fi

make -C $KERNEL_DIR ARCH=arm CROSS_COMPILE=$CROSS_COMPILE menuconfig

make -C $KERNEL_DIR ARCH=arm CROSS_COMPILE=$CROSS_COMPILE

cp $MODULE_DIR/fft_module.ko $TARGET_DIR/lib/modules/$(uname -r)/kernel/drivers/platform/

$QEMU_ARM -M virt -kernel $OUTPUT_DIR/images/zImage -append "root=/dev/vda rw" -drive file=$OUTPUT_DIR/images/rootfs.ext2,format=raw -net nic -net user -nographic

# Load the module in QEMU ??????????????????????????

#ssh root@localhost -p 2222 'insmod /lib/modules/$(uname -r)/kernel/drivers/platform/fft_module.ko'
#ssh root@localhost -p 2222 'depmod -a && modprobe fft_module'

#echo "FFT module build and installation script completed."
