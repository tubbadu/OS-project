#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include "../fft_module/fft_module.h"



int main() {
    int fd;
    struct fft_data data;
    int i;

    fd = open("/dev/fft", O_RDWR);
    if (fd < 0) {
        perror("Failed to open /dev/fft");
        return EXIT_FAILURE;
    }

    for (i = 0; i < SIZE; i++) {
        data.input[i] = i;
        data.inputi[i] = i+0x1000;
    }
    data.len = SIZE;

    // Perform FFT 
    if (ioctl(fd, FFT_COMPUTE, &data) < 0) {
        perror("Failed to perform FFT computation");
        close(fd);
        return EXIT_FAILURE;
    }

    for (i = 0; i < SIZE; i++) {
        // printf("Output[%d]  = %lu\n", i, data.output[i]);
        // printf("Outputi[%d] = %lu\n", i, data.outputi[i]);
        // printf(" input[%d] = 0x%X + j*0x%X\n", i, data.input[i], data.inputi[i]);
        printf("output[%d] = %ld + j*%ld \n", i, data.output[i], data.outputi[i]);
    }

    close(fd);
    return EXIT_SUCCESS;
}