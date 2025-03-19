#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Funzione per calcolare la prossima potenza di 2
int nextPowerOfTwo(int n) {
    if (n == 0) return 1;
    int power = 1;
    while (power < n) {
        power *= 2;
    }
    return power;
}

int main() {
    FILE *inputFile, *outputFile;
    char line[1024];
    int count = 0;

    // Apri il file CSV in lettura
    inputFile = fopen("average_prova.csv", "r");
    if (inputFile == NULL) {
        perror("Errore nell'apertura del file di input");
        return EXIT_FAILURE;
    }

    // Conta il numero di elementi nel file CSV
    while (fgets(line, sizeof(line), inputFile)) {
        count++;
    }

    // Determina la prossima potenza di 2
    int nextPower = nextPowerOfTwo(count);

    // Chiudi e riapri il file per rileggere i dati
    fclose(inputFile);
    inputFile = fopen("average_prova.csv", "r");
    if (inputFile == NULL) {
        perror("Errore nell'apertura del file di input");
        return EXIT_FAILURE;
    }

    // Apri il file CSV in scrittura
    outputFile = fopen("output_prova.csv", "w");
    if (outputFile == NULL) {
        perror("Errore nell'apertura del file di output");
        fclose(inputFile);
        return EXIT_FAILURE;
    }

    // Copia i dati originali nel nuovo file
    while (fgets(line, sizeof(line), inputFile)) {
        fputs(line, outputFile);
    }

    // Aggiungi zeri fino a raggiungere la prossima potenza di 2
    for (int i = count; i < nextPower; i++) {
        fprintf(outputFile, "0\n");
    }

    // Chiudi i file
    fclose(inputFile);
    fclose(outputFile);

    printf("File modificato con successo. Il nuovo file è 'output.csv'.\n");

    return EXIT_SUCCESS;
}