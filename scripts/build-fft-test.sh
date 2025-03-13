#!/bin/bash

source .env
CROSS_COMPILE="$BASEDIR/buildroot-arm/output/host/bin/aarch64-linux-"

cd $BASEDIR/test

CC=${CROSS_COMPILE}gcc
CFLAGS="-I../kernel"

TARGET="fft_test"

${CC} ${CFLAGS} -o ${TARGET} ${TARGET}.c ../fftlib/fft_algorithm.c  -lm

if [ $? -eq 0 ]; then
    echo "Compilation successful. The binary is named ${TARGET}."
else
    echo "Compilation failed."
fi