make: mandelbrot.c
	gcc -o fractal mandelbrot.c -lm
	./fractal > out.ppm

clean: fractal out.ppm
	rm -f fractal out.ppm
