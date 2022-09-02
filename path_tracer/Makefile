make: main.c
	# -fopenmp
	clear
	gcc -o main main.c -lm
	./main

dump: test.ppm
	hexdump test.ppm

clean: main
	rm main

view: test.ppm
	pix test.ppm
