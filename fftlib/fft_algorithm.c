#include "fft_algorithm.h"

// #define NSAMPLES 2048 //1536

/* Funzione per calcolare il log2 di un intero (floor del logaritmo in base 2) */
int ilog2(int N) {
	int k = N, i = 0;
	while (k) {
		k >>= 1;
		i++;
	}
	return i - 1;
}

/* Verifica se n è una potenza di 2 */
int check(int n) {
	return n > 0 && (n & (n - 1)) == 0;
}

/* Calcola il numero con i bit invertiti, considerando il numero di bit pari a ilog2(N) */
int reverse(int N, int n) {
	int j, p = 0;
	int L = ilog2(N);
	for (j = 1; j <= L; j++) {
		if(n & (1 << (L - j)))
			p |= 1 << (j - 1);
	}
	return p;
}

/* Riordina l’array in ordine "bit-reversed" */
void ordina(float complex *f1, int N) {
	float complex *f2 = malloc(sizeof(float complex) * N);
	int i, j;
	for(i = 0; i < N; i++) {
		f2[i] = f1[reverse(N, i)];
	}
	for(j = 0; j < N; j++)
		f1[j] = f2[j];
}

/* Calcola la trasformata FFT in-place */
void transform(float complex *f, int N) {
	ordina(f, N);
	float complex *W;
	W = (float complex *)malloc((N / 2) * sizeof(float complex));
	if(W == NULL) {
		fprintf(stderr, "Errore nell'allocazione della memoria\n");
		exit(EXIT_FAILURE);
	}
	/* Calcola la radice N-esima dell'unità */
	W[0] = 1 + 0 * I;
	W[1] = cos(-2. * M_PI / N) + I * sin(-2. * M_PI / N);
	int i;
	for(i = 2; i < N / 2; i++) {
		W[i] = cpow(W[1], i);
	}
	
	int n = 1;
	int a = N / 2;
	int j, k;
	int L = ilog2(N);
	for(j = 0; j < L; j++) {
		for(i = 0; i < N; i++) {
			if ((i & n) == 0) {
				float complex temp = f[i];
				/* Calcola l'indice per il fattore di rotazione */
				int idx = (i * a) % (n * a);
				float complex Temp = W[idx] * f[i + n];
				f[i] = temp + Temp;
				f[i + n] = temp - Temp;
			}
		}
		n *= 2;
		a /= 2;
	}
	free(W);
}

/* Calcola la FFT e moltiplica ogni risultato per il passo di campionamento */
void FFT(float complex *f, int N, float d) {
	transform(f, N);
	int i;
	for(i = 0; i < N; i++)
		f[i] *= d;
}


float modulo(complex float num){
	return sqrt(pow(creal(num),2) + pow(cimag(num),2));
}

int getNSAMPLES(int nlines){
	float log = log2(nlines);
	int intlog = log;
	
	if(intlog < log){
		// printf("%d < %f\n", intlog, log);
		intlog++;
	} else {
		// printf("%d >= %f\n", intlog, log);
	}
	return pow(2, intlog);
}
// int main() {
// 	// int n = NSAMPLES;
// 
// 	float d = 1;
// 
// 	
// 	
// 	
// 	// FILE* fd=fopen("prova.csv","r");
// 	FILE* fd=fopen("average_prova.csv","r");
// 	// FILE* fd=fopen("sinusoide_440hz.csv","r");
// 	// FILE* fd=fopen("sinusoide_500hz.csv","r");
// 	// FILE* fd=fopen("sinusoide_2093hz.csv","r");
// 	FILE* output_fd=fopen("output.csv","w+");
// 
// 	float temp;
// 	int nlines = 0;
// 	while(fscanf(fd,"%lf",&temp) > 0){
// 		nlines++;
// 	}
// 	
// 	
// 	const int NSAMPLES = getNSAMPLES(nlines);
// 	printf("file length:  %d\n", nlines);
// 	printf("input length: %d\n", NSAMPLES);
// 	float complex *vec = malloc(sizeof(float complex) * NSAMPLES);
// 	
// 	rewind(fd); // put cursor back at the beginning of the file
// 	
// 	if(fd==NULL){
// 		printf("Failed to open file\n");
// 		return 1;
// 	}
// 	for(int i=0;i<nlines;i++){
// 		float temp, re, im;
// 		fscanf(fd,"%lf\n",&re);
// 		im = 0;
// 		vec[i] = re + im * I;
// 	}
// 	for(int i=nlines; i<NSAMPLES; i++){
// 		vec[i] = 0;
// 	}
// 	
// 	
// 
// 	FFT(vec, NSAMPLES, d);
// 	printf("Risultato FFT:\n");
// 	
// 	
// 	int maxi = 0, i = 0;
// 	
// 	for(int i = 0; i < NSAMPLES; i++) {
// 		
// 		if(modulo(vec[i]) >= modulo(vec[maxi])){
// 			maxi = i;
// 		}
// 		
// 		printf("(%lf, %lf)\n", creal(vec[i]), cimag(vec[i]));
// 		fprintf(output_fd, "%lf,%lf,%lf\n", creal(vec[i]), cimag(vec[i]), modulo(vec[i])); // Scrive la parte reale e immaginaria
// 	}
// 	
// 	
// 	
// 	printf("MAX MODULO È %d di valore %f!\n",maxi,modulo(vec[maxi]));
// 	int f;
// 	
// 	if(maxi > NSAMPLES/2){
// 		f = NSAMPLES - maxi;
// 	} else {
// 		f = maxi;
// 	}
// 	
// 	printf("La frequenza è %d Hz\n", f);
// 	
// 	return 0;
// }
