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
#include "lib/fftcorelib.h"

const char *noteNames[] = {"A", "A#", "B", "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#"};

const char* recog_note(double frequency){
	int semitone;
	
	semitone=round(12*log2(frequency/440.0));
	if(abs(semitone)>11){
		semitone=semitone%12;
	}
	if(semitone>=0){
		return noteNames[semitone];
	}else{
		return noteNames[12+semitone];
	}
}


int main() {
	
	double Fs = 1470.0;
	// FILE* fd=fopen("/opt/data/average_prova.csv","r");
	FILE* fd=fopen("/opt/data/2048.csv","r");
	// FILE* fd=fopen("/opt/data/sinusoide_440hz.csv","r");
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
	
	
	const int NSAMPLES = getNSAMPLES(nlines);
	
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
		
	FFTcore(vec, vec, NSAMPLES);
	
	int maxi = 0;
	
	for(int i=0; i<NSAMPLES; i++){
		if(cabs(vec[maxi]) < cabs(vec[i])){
			maxi = i;
		}
	}
			
	double freq;
	if(maxi > NSAMPLES/2){
		freq = (NSAMPLES - maxi) * (Fs / NSAMPLES);
	} else {
		freq = maxi * (Fs / NSAMPLES);
	}
	
	printf("The frequency is %lf Hz\n", freq);
	
	printf("The recognized note is: %s\n", recog_note(freq));
	return 0;
}