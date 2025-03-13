#ifndef FFT_MODULE_H
#define FFT_MODULE_H

#define DEVICE_NAME "fft"    ///< The device will appear at /dev/fft using this value
#define CLASS_NAME "fft_class" ///< The device class -- this is a character device driver

#define FFT_k 0xFF
#define FFT_COMPUTE _IOWR(FFT_k, 1, struct fft_data *)

#define FFT_INPUT_START 0x090c0000
#define FFT_OUTPUT_START 0x090c0320
#define FFT_BUF_LENGTH 16 // in uint64_t cells
#define FFT_STATUS_ID 0x090c0640

struct fft_data {
    uint64_t input[FFT_BUF_LENGTH];
    uint64_t output[FFT_BUF_LENGTH];
    size_t len;
};

#endif // FFT_MODULE_H