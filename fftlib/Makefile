all: build run

build:
	gcc -o main.out main.c fft_algorithm.c -lm
	
build2:
	gcc -o main.out fft-on-csv.c fft_algorithm.c -lm
	
run:
	./main.out
	
c:
	gcc fft-ita.c -lm && ./a.out

cpp:
	g++ fft-ita.cpp && ./a.out