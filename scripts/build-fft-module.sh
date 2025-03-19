#!/bin/bash

export PATH=$(echo "$PATH" | tr -d ' \t\n')
source .env

KERNEL_DIR="$BASEDIR/buildroot/output/build/linux-6.12.9"
BUILDROOT_DIR="$BASEDIR/buildroot"
MODULE_DIR="$KERNEL_DIR/drivers/platform"
SCRIPT_DIR="$BASEDIR/scripts"
OUTPUT_DIR="$BASEDIR/buildroot/output"
TARGET_DIR="$OUTPUT_DIR/target"
CROSS_COMPILE="$OUTPUT_DIR/host/bin/arm-linux-"

cd $BUILDROOT_DIR
make ARCH=aarch64 CROSS_COMPILE=$CROSS_COMPILE linux-menuconfig
make ARCH=aarch64 CROSS_COMPILE=$CROSS_COMPILE linux-rebuild

mkdir -p $TARGET_DIR/lib/modules/6.12.9/kernel/drivers/platform/

cp $KERNEL_DIR/drivers/platform/fft_module.ko $TARGET_DIR/lib/modules/6.12.9/kernel/drivers/platform/