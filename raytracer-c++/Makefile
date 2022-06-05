make: main.cpp
	g++ main.cpp -o main
	./main > out.ppm

clean: main out.ppm
	rm -f main out.ppm

view: out.ppm
	gimp ./out.ppm
