#ifndef FFT_MODULE_H
#define FFT_MODULE_H

#define DEVICE_NAME "fft"    ///< The device will appear at /dev/fft using this value
#define CLASS_NAME "fft_class" ///< The device class -- this is a character device driver

#define FFT_k 0xFF
#define FFT_COMPUTE _IOWR(FFT_k, 1, struct fft_data *)

#define SIZE 128


#define BASEADDRESS 0x090c0000
#define STATUS_ID 0x0
#define IN_START_ID 0x8
#define IN_END_ID 0x808
#define OUT_START_ID 0x810
#define OUT_END_ID 0x1010

#define FFT_INPUT_START BASEADDRESS + IN_START_ID
#define FFT_OUTPUT_START BASEADDRESS + OUT_START_ID
#define FFT_BUF_LENGTH SIZE // in uint64_t cells
#define FFT_STATUS_ID BASEADDRESS + STATUS_ID


struct fft_data {
    uint64_t input[FFT_BUF_LENGTH];
    uint64_t inputi[FFT_BUF_LENGTH];
    uint64_t output[FFT_BUF_LENGTH];
    uint64_t outputi[FFT_BUF_LENGTH];
    size_t len;
};

#endif // FFT_MODULE_H