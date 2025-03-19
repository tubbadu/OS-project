#include "fftcorelib.h"

#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>
#include <string.h>

static inline uint64_t double_to_int(double value) {
	uint64_t result;
	memcpy(&result, &value, sizeof(result));
	return result;
}

static inline double int_to_double(uint64_t value) {
	double result;
	memcpy(&result, &value, sizeof(result));
	return result;
}

// It computes a 128-point FFT on input and writes the result into output.
int fft_128(double complex *input, double complex *output){
	// open device
	int fd = open("/dev/fft", O_RDWR);
	if (fd < 0) {
		perror("Failed to open /dev/fft\n");
		return EXIT_FAILURE;
	}
	
	struct fft_data data;
	data.len = 128;
	for(int i=0; i<128; i++){
		data.input[i]  = double_to_int(creal(input[i]));
		data.inputi[i] = double_to_int(cimag(input[i]));
	}
	
	// start the conversion
	if (ioctl(fd, FFT_COMPUTE, &data) < 0) {
		perror("Failed to perform FFT computation\n");
		close(fd);
		return EXIT_FAILURE;
	}
	
	for(int i=0; i<128; i++){
		double preal = int_to_double(data.output[i]);
		double pimag = int_to_double(data.outputi[i]);
		output[i] = preal + (I * pimag);
	}
	close(fd);
	return 0;
}

// Helper: Compute next power of two (minimum 128)
static size_t next_power_of_two(size_t n) {
	size_t power = 1;
	while (power < n) {
		power <<= 1;
	}
	return (power < 128) ? 128 : power;
}

// Recursive FFT function that uses the hardware FFT when N == 128.
// Assumes N is a power of two and N >= 128.
static int fft_recursive(double complex *x, double complex *X, size_t N) {
	if (N == 128) {
		// Base case: Use the hardware FFT.
		return fft_128(x, X);
	}
	
	size_t half = N / 2;
	double complex *even = malloc(half * sizeof(double complex));
	double complex *odd  = malloc(half * sizeof(double complex));
	double complex *E    = malloc(half * sizeof(double complex));
	double complex *O    = malloc(half * sizeof(double complex));
	if (!even || !odd || !E || !O) {
		free(even); free(odd); free(E); free(O);
		return EXIT_FAILURE;
	}
	
	// Split input into even and odd indices.
	for (size_t i = 0; i < half; i++) {
		even[i] = x[2 * i];
		odd[i]  = x[2 * i + 1];
	}
	
	int ret;
	ret = fft_recursive(even, E, half);
	if (ret != EXIT_SUCCESS) {
		free(even); free(odd); free(E); free(O);
		return ret;
	}
	ret = fft_recursive(odd, O, half);
	if (ret != EXIT_SUCCESS) {
		free(even); free(odd); free(E); free(O);
		return ret;
	}
	
	// Combine the FFTs of the even and odd parts.
	for (size_t k = 0; k < half; k++) {
		double complex twiddle = cexp(-2.0 * I * M_PI * k / N) * O[k];
		X[k]         = E[k] + twiddle;
		X[k + half]  = E[k] - twiddle;
	}
	
	free(even);
	free(odd);
	free(E);
	free(O);
	return EXIT_SUCCESS;
}

// Exposed function: Compute FFT for an input of any length.
// If the input length is not a power-of-two (or is less than 128), the input
// is zero-padded up to the next power-of-two (with a minimum of 128).
// The first N output values (corresponding to the original length) are returned.
int FFTcore(double complex *input, double complex *output, size_t N) {
	
	if(log2(N) != floor(log2(N))){
		perror("ERROR: input length must be a power of 2.\n");
		return EXIT_FAILURE;
	}
		
	// Determine padded length (next power-of-two, minimum 128).
	size_t padded_N = next_power_of_two(N);
	
	// Allocate temporary buffers.
	double complex *inbuf = malloc(padded_N * sizeof(double complex));
	double complex *outbuf = malloc(padded_N * sizeof(double complex));
	if (!inbuf || !outbuf) {
		free(inbuf);
		free(outbuf);
		return EXIT_FAILURE;
	}
	
	// Copy input and pad with zeros.
	for (size_t i = 0; i < N; i++) {
		inbuf[i] = input[i];
	}
	for (size_t i = N; i < padded_N; i++) {
		inbuf[i] = 0.0 + 0.0 * I;
	}
	
	// Compute the FFT recursively.
	int ret = fft_recursive(inbuf, outbuf, padded_N);
	if (ret != EXIT_SUCCESS) {
		free(inbuf);
		free(outbuf);
		return ret;
	}
	
	// Copy back only the first N results.
	for (size_t i = 0; i < N; i++) {
		output[i] = outbuf[i];
	}
	
	free(inbuf);
	free(outbuf);
	return EXIT_SUCCESS;
}
