#ifndef FFCORELIB_H
#define FFCORELIB_H

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include <math.h>
#include <complex.h>

#include "../../fft_module/fft_module.h"
// #include "../fftlib/fft_algorithm.h"

// #define double_to_int *(uint64_t*)&
// #define int_to_double *(double*)&

int FFTcore(double complex *, double complex*, size_t);

#endif // FFCORELIB_H