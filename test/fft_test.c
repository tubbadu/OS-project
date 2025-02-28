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

    fd = open("/dev/fft_core", O_RDWR);
    
    if (fd < 0) {
        perror("Failed to open /dev/fft_core");
        return EXIT_FAILURE;
    }

    for (int k = 0; k < 128; k++) {
        data.input[k].real = 0;
        data.input[k].imag = 0;
    }
    data.len = 128;

    // Perform FFT 
    if (ioctl(fd, FFT_COMPUTE, &data) < 0) {
        perror("Failed to perform FFT computation");
        close(fd);
        return EXIT_FAILURE;
    }

    for (int k = 0; k < 128; k++) {
        printf("Output[%d] = %lu\n", k, data.output[k]);
    }

    close(fd);
    return EXIT_SUCCESS;
}