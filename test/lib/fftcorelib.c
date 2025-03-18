#include "fftcorelib.h"
#include <complex.h>
#include <math.h>
#include <stddef.h>
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

int fft_128(double complex *input, double complex *output){
	// open device
	int fd = open("/dev/fft", O_RDWR);
	if (fd < 0) {
		perror("Failed to open /dev/fft");
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
		perror("Failed to perform FFT computation");
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

/********************************************************************/

// Utility function to compute the next power of 2 greater than or equal to N
size_t next_power_of_2(size_t N) {
	return pow(2, ceil(log2(N)));
}

int FFTcore(double complex *input, double complex *output, size_t N) {
	// Find the next power of 2 greater than or equal to N
	size_t padded_size = next_power_of_2(N);
	
	// Allocate a temporary array for zero-padding the input
	double complex *padded_input = (double complex *)malloc(padded_size * sizeof(double complex));
	if (!padded_input) {
		return -1;  // Memory allocation failed
	}
	
	// Copy the input data to the padded array
	for (size_t i = 0; i < N; i++) {
		padded_input[i] = input[i];
	}
	
	// Zero pad the rest of the array
	for (size_t i = N; i < padded_size; i++) {
		padded_input[i] = 0.0 + 0.0*I;  // Initialize the extra values as zero (complex zero)
	}
	
	// Perform the FFT on the padded input
	double complex *padded_output = (double complex *)malloc(padded_size * sizeof(double complex));
	if (!padded_output) {
		free(padded_input);
		return -1;  // Memory allocation failed
	}
	
	int status = fft_128(padded_input, padded_output);
	if (status != 0) {
		free(padded_input);
		free(padded_output);
		return -1;  // FFT failed
	}
	
	// Copy the result of the FFT into the output array (of size N)
	for (size_t i = 0; i < N; i++) {
		output[i] = padded_output[i];
	}
	
	// Free allocated memory
	free(padded_input);
	free(padded_output);
	
	return 0;  // Success
}
