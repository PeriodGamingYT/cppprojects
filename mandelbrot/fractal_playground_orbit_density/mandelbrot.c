#include <stdio.h>
#include <stdlib.h>

typedef long double deci;

int imageWidth;
int imageHeight;
int *ppmImage;

void ppmInit(
	int width, 
	int height,
	int colors
) {
	imageWidth = width;
	imageHeight = height;
	ppmImage = (int*)malloc(
		sizeof(int*) * 
		imageWidth * 
		imageHeight
	);

	colors = colors > 256 ? 256 : colors;
	printf("P3 %d %d %d\n", width, height, colors);
}

void ppmNewline() {
	printf("\n");
}

void ppmAddPixel(int color) {
	printf("%d %d %d ", color, color, color);
}

void ppmAdd(int x, int y, int color) {
	if(
		x < 0 ||
		y < 0 ||
		x > imageWidth ||
		y > imageHeight
	) {
		return;
	}

	if(ppmImage[(y * imageWidth) + x] + color > 256) {
		ppmImage[(y * imageWidth) + x] = 256;
		return;
	}
	
	ppmImage[(y * imageWidth) + x] += color;
}

void ppmSet(int x, int y, int color) {
	if(
		x < 0 ||
		y < 0 ||
		x > imageWidth ||
		y > imageHeight
	) {
		return;
	}

	color = color > 256 ? 256 : color;
	ppmImage[(y * imageWidth) + x] = color;
}

void ppmAddSignature() {
	FILE *signature = fopen("signature.ppm", "r");
	int signatureWidth, signatureHeight;
	fscanf(
		signature, 
		"P6\n%d %d\n255\n", 
		&signatureWidth, 
		&signatureHeight
	);

	for(int y = 0; y < signatureHeight; y++) {
		for(int x = 0; x < signatureWidth; x++) {
			int nextSignaturePixel = fgetc(signature);
			ppmSet(
				imageWidth - signatureWidth - 20 + x,
				imageHeight - signatureHeight - 20 + y,
				nextSignaturePixel
			);

			fgetc(signature);
			fgetc(signature);
		}
	}
	
	fclose(signature);
}

int main() {
	int iterations = 512;
	ppmInit(
		4500,
		3000,
		iterations
	);

	for(int y = 0; y < imageHeight; y++) {
		for(int x = 0; x < imageWidth; x++) {
			deci iterX = 0;
			deci iterY = 0;
			int i;
			for(
				i = 0;
				i <= iterations &&
				iterX * iterX - iterY * iterY <= 4.0;
				i++) {
				deci real = (x - imageWidth / 2.0) * 4.0 / imageWidth;
				deci imag = (y - imageHeight / 2.0) * 4.0 / imageWidth;
				deci newX = iterX * iterX - iterY * iterY + real;
				
				iterY = 2.0 * iterY * iterX + imag;
				iterX = newX;
				ppmAdd(
					(iterX + 2) / 4 * imageWidth, 
					(iterY + 2) / 4 * imageHeight,
					1
				);
			}
		}
	}

	ppmAddSignature();
	for(int y = 0; y < imageHeight; y++) {
		for(int x = 0; x < imageWidth; x++) {
			ppmAddPixel(ppmImage[(y * imageWidth) + x]);
		}

		ppmNewline();
	}

	free(ppmImage);
	return 0;
}
