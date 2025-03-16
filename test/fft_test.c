#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include <math.h>

#include "../fft_module/fft_module.h"
#include "../fftlib/fft_algorithm.h"


#define NUM_COMPLEX_VALUES 64
#define NUM_VALUES (NUM_COMPLEX_VALUES * 2)




void generate_expected_output(struct fft_data *data, double complex *output, size_t len) {
	for (int k=0; k<NUM_COMPLEX_VALUES; k++){
		output[k] = data->input[k] + I * data->inputi[k];
	}
	
	FFT(output, NUM_COMPLEX_VALUES, 1);
}

char getsign(int64_t x){
	if(x<0){
		return '-';
	} else {
		return '+';
	}
}

int printComplex(double real, double imag){
	char sign = '+';

	if(imag < 0){
		sign = '-';
	}
	
	return printf("%lf %c j %lf", real, sign, fabs(imag));
}

int main() {
	int fd;
	struct fft_data data;
	double complex expected_output[NUM_COMPLEX_VALUES];
	int i;
	
	fd = open("/dev/fft", O_RDWR);
	if (fd < 0) {
		perror("Failed to open /dev/fft");
		return EXIT_FAILURE;
	}
	
	for (i = 0; i < NUM_COMPLEX_VALUES; i++) {
		double complex input;
		input = i*0.75 + I*i*0.25;
		
		data.input[i]  = input; 
	}
	data.len = NUM_COMPLEX_VALUES;
	
	generate_expected_output(&data, expected_output, data.len);
	
	if (ioctl(fd, FFT_COMPUTE, &data) < 0) {
		perror("Failed to perform FFT computation");
		close(fd);
		return EXIT_FAILURE;
	}
	
	
	

	
	int test_pass = 1;
	for (i = 0; i < NUM_COMPLEX_VALUES; i++) {
		double  expecteda, expectedb;
		expecteda = creal(expected_output[i]);
		expectedb = cimag(expected_output[i]);
		
		printf("Index %d: \t Expected = %lf, Actual = %lf\n", i, expected_output[i], data.output[i]);
		// printComplex(expecteda, expectedb);
		// printf(",\t Actual = ");
		// printComplex(data.output[i], data.outputi[i]);
		// printf("\ndifference: %lf, %lf\n", fabs(data.output[i] - expecteda), fabs(data.outputi[i] - expectedb));
		if (data.output[i] != expected_output[i]) {
			test_pass = 0;
			// break;
		}
	}

	close(fd);
	
	if (test_pass) {
		printf("TEST PASS\n");
	} else {
		printf("TEST FAIL\n");
		return EXIT_FAILURE;
	}
	
	
	return EXIT_SUCCESS;
}
