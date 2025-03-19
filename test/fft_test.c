#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include <math.h>
#include <complex.h>

#include "../fft_module/fft_module.h"
#include "../fftlib/fft_algorithm.h"

#define NUM_COMPLEX_VALUES 256



int main() {
	int fd;
	struct fft_data data;
	double complex expected_output[NUM_COMPLEX_VALUES], input[NUM_COMPLEX_VALUES], output[NUM_COMPLEX_VALUES];
	int i;
	
	for (i = 0; i < NUM_COMPLEX_VALUES; i++) {
		double preal = i*0.75;
		double pimag = i*0.25;
		
		input[i] = preal + I * pimag;
		expected_output[i] = input[i];
	}
	
	data.len = NUM_COMPLEX_VALUES;
	
	FFT(expected_output, NUM_COMPLEX_VALUES, 1);

	if(FFTcore(input, output, NUM_COMPLEX_VALUES) == EXIT_FAILURE){
		return EXIT_FAILURE;
	}


	
	int test_pass = 1;
	for (i = 0; i < NUM_COMPLEX_VALUES; i++) {
		printf("%d)\t expected: %lf + j%lf,\t actual: %lf + j%lf\n", i, creal(expected_output[i]), cimag(expected_output[i]), creal(output[i]), cimag(output[i]));
		
		if (expected_output[i] != output[i]) {
			test_pass = 0;
			break;
		}
	}

	
	if (test_pass) {
		printf("TEST PASS\n");
	} else {
		printf("TEST FAIL\n");
		return EXIT_FAILURE;
	}
	
	
	return EXIT_SUCCESS;
}
