make: main.c
	gcc -o main main.c -fopenmp -lm
	./main

dump: test.ppm
	hexdump test.ppm

clean: main
	rm main

view: test.ppm
	pix test.ppm
