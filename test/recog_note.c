#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>




const char* recog_note(int frequency){
    int semitone;
    const char *noteNames[] = {"A", "A#", "B", "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#"};
    semitone=round(12*log2(frequency/440));
    if(abs(semitone)>11){
        semitone=semitone%12;
    }
    printf("The note is:\n");
    if(semitone>=0){
        return noteNames[semitone];
    }else{
        return noteNames[12+semitone];
    }
}

/*
int main(){
    const char*note;
    double frequency;
    printf("Insert a frequency (Hz): \n");

    scanf("%lf", &frequency);
    
    note=recog_note(frequency);
    
    printf("%s", note);
}   
*/