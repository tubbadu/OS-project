#include "fft_algorithm.h"

int ilog2(int N)    //funzione per calcolare il logaritmo in base 2 di un intero
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

int check(int n)    //usato per controllare se il numero di componenti del vettore di input è una potenza di 2
{
	return n > 0 && (n & (n - 1)) == 0;
}

int reverse(int N, int n)    //calcola il reverse number di ogni intero n rispetto al numero massimo N
{
	int j, p = 0;
	for(j = 1; j <= ilog2(N); j++) {
		if(n & (1 << (ilog2(N) - j)))
			p |= 1 << (j - 1);
	}
	return p;
}

void ordina(double complex* f1, int N)     //dispone gli elementi del vettore ordinandoli per reverse order
{
	double complex * f2 = malloc(N * sizeof(double complex));
	for(int i = 0; i < N; i++)
		f2[i] = f1[reverse(N, i)];
	for(int j = 0; j < N; j++)
		f1[j] = f2[j];
}

void transform(double complex* f, int N)     //calcola il vettore trasformato
{
	ordina(f, N);    //dapprima lo ordina col reverse order
	
	double complex W[N / 2]; //vettore degli zeri dell'unità.
	//Prima N/2-1 ma genera errore con ciclo for successivo
	//in quanto prova a copiare in una zona non allocata "W[N/2-1]"
	W[1] = polar(1., -2. * M_PI / N);
	W[0] = 1;
	for(int i = 2; i < N / 2; i++)
		W[i] = cexp(-I * 2. * M_PI * i / N); //pow(W[1], i);
	int n = 1;
	int a = N / 2;
	for(int j = 0; j < ilog2(N); j++) {
		for(int i = 0; i < N; i++) {
			if(!(i & n)) {
				/*ad ogni step di raddoppiamento di n, vengono utilizzati gli indici */
				/*'i' presi alternativamente a gruppetti di n, una volta si e una no.*/
				double complex temp = f[i];
				double complex Temp = W[(i * a) % (n * a)] * f[i + n];
				// printf("temp = (%lf, %lf), Temp = (%lf, %lf)\n", creal(temp), cimag(temp), creal(Temp), cimag(Temp));
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
		f[i] *= d; //moltiplica il vettore per il passo in modo da avere il vettore trasformato effettivo
}

double modulo(complex double num){
	double ret = sqrt(pow(creal(num),2) + pow(cimag(num),2));
	return ret;
}

int getNSAMPLES(int nlines){
	double log = log2(nlines);
	int intlog = log;
	
	if(intlog < log){
		// printf("%d < %f\n", intlog, log);
		intlog++;
	} else {
		// printf("%d >= %f\n", intlog, log);
	}
	return pow(2, intlog);
}

void recog_note(double frequency){
    int semitone;
    const char *noteNames[] = {"A", "A#", "B", "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#"};
    semitone=round(12*log2(frequency/440.0));
    if(abs(semitone)>11){
        semitone=semitone%12;
    }
    printf("The note is: ");
    if(semitone>=0){
        printf("%s\n",noteNames[semitone]); 
    }else{
        printf("%s\n",noteNames[12+semitone]);
    }
}