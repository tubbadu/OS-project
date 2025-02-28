#ifndef FFT_MODULE_H
#define FFT_MODULE_H

#define DEVICE_NAME "fft_core"    ///< The device will appear at /dev/fft using this value
#define CLASS_NAME "fft_class" ///< The device class -- this is a character device driver

#define FFT_k 0xFF
#define FFT_COMPUTE _IOWR(FFT_k, 1, struct fft_data *)

#define FFT_BASE_ADDR 0x090c0000
#define STATUS_ID 0x0
#define IN_START_ID 0x8
#define IN_END_ID 0x400
#define OUT_START_ID 0x408
#define OUT_END_ID 0x800

#define FFT_INPUT_START FFT_BASE_ADDR + IN_START_ID
#define FFT_OUTPUT_START FFT_BASE_ADDR + OUT_START_ID
#define FFT_BUF_LENGTH 128
#define FFT_STATUS_ID FFT_BASE_ADDR + STATUS_ID

struct Complex {
    uint64_t real;
    uint64_t imag;
};


// #define FFT_INPUT_START 0x090c0000
// #define FFT_OUTPUT_START 0x090c0320
// #define FFT_BUF_LENGTH 16 // in uint64_t cells
// #define FFT_STATUS_ID 0x090c0640

struct fft_data {
    struct Complex  input[FFT_BUF_LENGTH];
    struct Complex output[FFT_BUF_LENGTH];
    size_t len;
};

#endif // FFT_MODULE_H