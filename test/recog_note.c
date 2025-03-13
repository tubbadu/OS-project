const char* recog_note(int frequency){
    int semitone;
    const char *noteNames[] = {"A", "A#", "B", "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#"};
    semitone=round(12*log2(frequency/440));
    if(abs(semitone)>11){
        semitone=semitone%12;
    }
    if(semitone>=0){
        return noteNames[semitone];
    }else{
        return noteNames[12+semitone];
    }
}