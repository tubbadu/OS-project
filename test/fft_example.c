#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include <math.h>
#include <complex.h>

#include "../fftlib/fft_algorithm.h"

int main() {
	
	// FILE* fd=fopen("/opt/data/average_prova.csv","r");
	FILE* fd=fopen("/opt/data/sinusoide_440hz.csv","r");
	// FILE* fd=fopen("/opt/data/sinusoide_500hz.csv","r");
	// FILE* fd=fopen("/opt/data/sinusoide_2093hz.csv","r");
	FILE* output_fd=fopen("/opt/data/output.csv","w+");
	
	if (fd <= 0) {
		perror("Failed to open file.");
		return 0;
	}
	
	double temp;
	int nlines = 0;
	while(fscanf(fd,"%lf",&temp) > 0){
		
		nlines++;
	}
	
	
	
	printf("file length:  %d\n", nlines);
	
	const int NSAMPLES = getNSAMPLES(nlines);
	printf("input length: %d\n", NSAMPLES);
	double complex *vec = malloc(sizeof(double complex) * NSAMPLES);
	
	rewind(fd); // put cursor back at the beginning of the file
	
	if(fd==NULL){
		printf("Failed to open file\n");
		return 1;
	} 
	
	for(int i=0;i<nlines;i++){
		double temp, re, im;
		fscanf(fd,"%lf\n",&re);
		im = 0;
		vec[i] = re + im * I;
	}
	for(int i=nlines; i<NSAMPLES; i++){
		vec[i] = 0;
	}
	
	
	FFT(vec, NSAMPLES, 1);
	printf("Risultato FFT:\n");
	
	
	int maxi = 0, i = 0;
	
	for(int i = 0; i < NSAMPLES; i++) {
		
		if(modulo(vec[i]) >= modulo(vec[maxi])){
			maxi = i;
		}
		
		// printf("(%lf, %lf)\n", creal(vec[i]), cimag(vec[i]));
		fprintf(output_fd, "%lf,%lf,%lf\n", creal(vec[i]), cimag(vec[i]), modulo(vec[i])); // Scrive la parte reale e immaginaria
	}
	
	
	
	printf("MAX MODULO È %d di valore %f!\n",maxi,modulo(vec[maxi]));
	int f;
	
	if(maxi > NSAMPLES/2){
		f = NSAMPLES - maxi;
	} else {
		f = maxi;
	}
	
	printf("La frequenza è %d Hz\n", f);
	
	return 0;
}