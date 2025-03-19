#ifndef FFT_ALGORITHM_H
#define FFT_ALGORITHM_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>

#ifndef M_PI
#define M_PI 3.1415926535897932384
#endif

 //#define NSAMPLES 2048 //1536

// /* Funzione per calcolare il log2 di un intero (floor del logaritmo in base 2) */
 int ilog2(int);
// 
// /* Verifica se n è una potenza di 2 */
 int check(int);
// 
// /* Calcola il numero con i bit invertiti, considerando il numero di bit pari a ilog2(N) */
 int reverse(int, int);
// 
// /* Riordina l’array in ordine "bit-reversed" */
 void ordina(double complex *, int);
// 
// /* Calcola la trasformata FFT in-place */
 void transform(double complex *, int);

/* Calcola la FFT e moltiplica ogni risultato per il passo di campionamento */
void FFT(double complex *, int, double);


double modulo(complex double);

int getNSAMPLES(int);

void recog_note(double frequency);

#endif // FFT_ALGORITHM_H