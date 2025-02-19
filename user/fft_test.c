#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include "../kernel/fft_module.h"

int main() {
    int fd;
    struct fft_data data;
    int i;

    fd = open("/dev/fft", O_RDWR);
    if (fd < 0) {
        perror("Failed to open /dev/fft");
        return EXIT_FAILURE;
    }

    for (i = 0; i < 16; i++) {
        data.input[i] = i;
    }
    data.len = 16;

    // Perform FFT 
    if (ioctl(fd, FFT_COMPUTE, &data) < 0) {
        perror("Failed to perform FFT computation");
        close(fd);
        return EXIT_FAILURE;
    }

    for (i = 0; i < 16; i++) {
        printf("Output[%d] = %lu\n", i, data.output[i]);
    }

    close(fd);
    return EXIT_SUCCESS;
}