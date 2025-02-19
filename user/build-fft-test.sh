#!/bin/bash

CROSS_COMPILE=/home/selenkarakas/OS-project/buildroot-arm/output/host/bin/aarch64-linux-

CC=${CROSS_COMPILE}gcc
CFLAGS="-I../kernel"

TARGET=fft_test

${CC} ${CFLAGS} -o ${TARGET} fft_test.c

if [ $? -eq 0 ]; then
    echo "Compilation successful. The binary is named ${TARGET}."
else
    echo "Compilation failed."
fi