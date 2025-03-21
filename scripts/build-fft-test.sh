#!/bin/bash

source .env
CROSS_COMPILE="$BASEDIR/buildroot/output/host/bin/aarch64-linux-"

cd $BASEDIR/test

CC=${CROSS_COMPILE}gcc
CFLAGS="-I../kernel"

TARGET="$1" #"fft_test"

${CC} ${CFLAGS} -o ${TARGET} ${TARGET}.c ../fftlib/fft_algorithm.c lib/fftcorelib.c  -lm

if [ $? -eq 0 ]; then
    echo "Compilation successful. The binary is named ${TARGET}."
else
    echo "Compilation failed."
fi