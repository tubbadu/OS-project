#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include <math.h>
#include "../fft_module/fft_module.h"

#define NUM_COMPLEX_VALUES 128
#define NUM_VALUES (NUM_COMPLEX_VALUES * 2)

typedef struct {
	int64_t real;
	int64_t imag;
} Complex;

// void fft(Complex *v, int n, Complex *tmp) {
// 	if (n > 1) {
// 		int k, m;    
// 		Complex z, w, *vo, *ve;
// 		ve = tmp; 
// 		vo = tmp + n/2;
// 		for (k = 0; k < n/2; k++) {
// 			ve[k] = v[2*k];
// 			vo[k] = v[2*k + 1];
// 		}
// 		fft(ve, n/2, v);
// 		fft(vo, n/2, v);
// 		for (m = 0; m < n/2; m++) {
// 			w.real = cos(2 * M_PI * m / (int64_t)n);
// 			w.imag = -sin(2 * M_PI * m / (int64_t)n);
// 			z.real = w.real * vo[m].real - w.imag * vo[m].imag;
// 			z.imag = w.real * vo[m].imag + w.imag * vo[m].real; 
// 			v[m    ].real = ve[m].real + z.real;
// 			v[m    ].imag = ve[m].imag + z.imag;
// 			v[m+n/2].real = ve[m].real - z.real;
// 			v[m+n/2].imag = ve[m].imag - z.imag;
// 		}
// 	}
// 	else {
// 		return;
// 	}
// }

static void fft(Complex *input, Complex *output, int n) {
	if (n <= 1) {
		output[0] = input[0];
		return;
	}
	
	Complex *evenInput  = (Complex *)malloc(n / 2 * sizeof(Complex));
	Complex *oddInput   = (Complex *)malloc(n / 2 * sizeof(Complex));
	Complex *evenOutput = (Complex *)malloc(n / 2 * sizeof(Complex));
	Complex *oddOutput  = (Complex *)malloc(n / 2 * sizeof(Complex));
	
	for (int i = 0; i < n / 2; i++) {
		evenInput[i] = input[i * 2];
		oddInput[i] = input[i * 2 + 1];
	}
	
	fft(evenInput, evenOutput, n / 2);
	fft(oddInput, oddOutput, n / 2);
	
	for (int k = 0; k < n / 2; k++) {
		double angle = -2.0 * M_PI * k / n;
		Complex t = { (int64_t)(cos(angle) * oddOutput[k].real - sin(angle) * oddOutput[k].imag),
			(int64_t)(cos(angle) * oddOutput[k].imag + sin(angle) * oddOutput[k].real) };
			output[k].real = evenOutput[k].real + t.real;
			output[k].imag = evenOutput[k].imag + t.imag;
			output[k + n / 2].real = evenOutput[k].real - t.real;
			output[k + n / 2].imag = evenOutput[k].imag - t.imag;
	}
	
	free(evenInput);
	free(oddInput);
	free(evenOutput);
	free(oddOutput);
}

void generate_expected_output(struct fft_data *data, Complex *output, size_t len) {
	Complex v[NUM_COMPLEX_VALUES]; //, tmp[NUM_COMPLEX_VALUES];
	for (size_t i = 0; i < NUM_COMPLEX_VALUES; i++) {
		v[i].real = data->input[i];
		v[i].imag = data->inputi[i];
	}
	fft(v, output, NUM_COMPLEX_VALUES);
}

char getsign(int64_t x){
	if(x<0){
		return '-';
	} else {
		return '+';
	}
}

int printComplex(int64_t real, int64_t imag){
	char sign = '+';

	if(imag < 0){
		sign = '-';
	}
	
	return printf("%ld %c j * %ld", real, sign, abs(imag));
}

int main() {
	int fd;
	struct fft_data data;
	Complex expected_output[NUM_COMPLEX_VALUES];
	int i;
	
	fd = open("/dev/fft", O_RDWR);
	if (fd < 0) {
		perror("Failed to open /dev/fft");
		return EXIT_FAILURE;
	}
	
	for (i = 0; i < NUM_COMPLEX_VALUES; i++) {
		data.input[i]  = 2*i; // Real
		data.inputi[i] = i+0x1000; // Complex
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
		// printf("Index %d: Expected = %ld + j*%ld, Actual = %ld %c j*%ld\n", i, expected_output[i].real, expected_output[i].imag, data.output[i], getsign(data.outputi[i]), abs(data.outputi[i]));
		printf("Index %d: \t Expected = ", i);
		printComplex(expected_output[i].real, expected_output[i].imag);
		printf(",\t Actual = ");
		printComplex(data.output[i], data.outputi[i]);
		printf("\n");
		if (data.output[i] != expected_output[i].real || data.outputi[i] != expected_output[i].imag) {
			test_pass = 0;
			break;
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
