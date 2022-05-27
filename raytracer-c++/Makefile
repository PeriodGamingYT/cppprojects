make: main.cpp
	g++ main.cpp -o main
	rm out.ppm
	./main > out.ppm

clean: main
	rm main

view: out.ppm
	gimp ./out.ppm
