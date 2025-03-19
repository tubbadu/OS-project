#include "fft_algorithm.h"

int ilog2(int N)    // function to calculate the base-2 logarithm of an integer
{
	int k = N, i = 0;
	while(k) {
		k >>= 1;
		i++;
	}
	return i - 1;
}

double complex polar(double r, double theta){
	return r * cexp(I * theta);
}

int check(int n)    // used to check if the number of components in the input vector is a power of 2
{
	return n > 0 && (n & (n - 1)) == 0;
}

int reverse(int N, int n)    // computes the reverse number of each integer n with respect to the maximum number N
{
	int j, p = 0;
	for(j = 1; j <= ilog2(N); j++) {
		if(n & (1 << (ilog2(N) - j)))
			p |= 1 << (j - 1);
	}
	return p;
}

void sort(double complex* f1, int N)     // rearranges the elements of the vector by sorting them in reverse order
{
	double complex * f2 = malloc(N * sizeof(double complex));
	for(int i = 0; i < N; i++)
		f2[i] = f1[reverse(N, i)];
	for(int j = 0; j < N; j++)
		f1[j] = f2[j];
}

void transform(double complex* f, int N)     // computes the transformed vector
{
	sort(f, N);    // first, sort it in reverse order
	
	double complex W[N / 2]; // vector of the N-th roots of unity.
	// Previously N/2-1, but this caused an error in the following loop
	// because it tries to copy into an unallocated zone "W[N/2-1]"
	W[1] = polar(1., -2. * M_PI / N);
	W[0] = 1;
	for(int i = 2; i < N / 2; i++)
		W[i] = cexp(-I * 2. * M_PI * i / N); // pow(W[1], i);
	int n = 1;
	int a = N / 2;
	for(int j = 0; j < ilog2(N); j++) {
		for(int i = 0; i < N; i++) {
			if(!(i & n)) {
				/* At each step of doubling n, the indices */
				/* 'i' are used alternately in groups of n, one time yes, one time no. */
				double complex temp = f[i];
				double complex Temp = W[(i * a) % (n * a)] * f[i + n];
				f[i] = temp + Temp;
				f[i + n] = temp - Temp;
			}
		}
		n *= 2;
		a = a / 2;
	}
}

void FFT(double complex* f, int N, double d)
{
	transform(f, N);
	for(int i = 0; i < N; i++)
		f[i] *= d; // multiply the vector by the step size to get the actual transformed vector
}

int getNSAMPLES(int nlines){
	double log = log2(nlines);
	int intlog = log;
	
	if(intlog < log){
		intlog++;
	}
	return pow(2, intlog);
}
