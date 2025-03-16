#include "fft_algorithm.h"

#define SIZE 128

int main() {
	// int n = SIZE;
	
	double d = 1;

	double complex vec[SIZE];
	

	for(int i=0;i<SIZE;i++){
		double re, im;
		re = i;
		im = 15.8*i;
		vec[i] = re + im * I;
	}

	
	
	
	FFT(vec, SIZE, d);
	printf("Risultato FFT:\n");
	
	
	// int maxi = 0, i = 0;
	
	for(int i = 0; i < SIZE; i++) {
		
		// if(modulo(vec[i]) >= modulo(vec[maxi])){
		// 	maxi = i;
		// }
		
		printf("(%lf, %lf)\n", creal(vec[i]), cimag(vec[i]));
		// fprintf(output_fd, "%lf,%lf,%lf\n", creal(vec[i]), cimag(vec[i]), modulo(vec[i])); // Scrive la parte reale e immaginaria
	}
	
	return 0;
}