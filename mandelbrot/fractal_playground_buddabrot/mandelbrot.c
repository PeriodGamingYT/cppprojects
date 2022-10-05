#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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

// Modified version of: https://stackoverflow.com/questions/11641629/generating-a-uniform-distribution-of-integers-in-c
double uniform_distribution(int rangeLow, int rangeHigh) {
  double myRand = rand()/(1.0 + RAND_MAX); 
  double range = rangeHigh - rangeLow + 1;
  double myRand_scaled = (myRand * range) + rangeLow;
  return myRand_scaled;
}

typedef struct {
  deci r;
  deci i;
} complex;

complex complexMul(complex a, complex b) {
  complex result = {
    .r = a.r * b.r - a.i * b.i,
    .i = a.r * b.i + a.i * b.r
  };
  
  return result;
}

complex complexAdd(complex a, complex b) {
  complex result = {
    .r = a.r + b.r,
    .i = a.i + b.i
  };
  
  return result;
}

double complexSqrMagnitude(complex a) {
  return a.r * a.r + a.i * a.i;
}

deci *points;
int n;
void buddhaPoints(complex c, int iterations) {
  n = 0;
  complex z = {
    .r = 0,
    .i = 0
  };

  while(
    n < iterations &&
    complexSqrMagnitude(z) <= 2.0
  ) {
    z = complexAdd(complexMul(z, z), c);
    points[n * 2] = z.r;
    points[n * 2 + 1] = z.i;
    n++;
  }
}

int rowFromReal(double real) {
  return floor((real - -2) * (imageHeight / (2 - -2)));
}

int colFromImag(double imag) {
  return floor((imag - -2) * (imageWidth / (2 - -2)));
}

int main() {
	int iterations = 512;
	ppmInit(
		1920,
		1080,
		// iterations
		255
	);

  points = (deci*)malloc(sizeof(deci*) * iterations * 2);
  int samples = imageWidth * imageHeight * 255;
  for(int i = 0; i < samples; i++) {
    complex sample = {
      .r = uniform_distribution(-2, 2),
      .i = uniform_distribution(-2, 2)
    };

    buddhaPoints(sample, iterations);
    if(n >= iterations) {
      continue;
    }

    for(int j = 0; j < n; j++) {
      complex c = {
        .r = points[j * 2],
        .i = points[j * 2 + 1]
      };
      
      if(
        c.r <= 2 &&
        c.r >= -2 &&
        c.i <= 2 &&
        c.i >= -2
      ) {
        int row = rowFromReal(c.r);
        int col = colFromImag(c.i);
        ppmAdd(col, row, 1);
      }
    }
  }

	// ppmAddSignature();
	for(int y = 0; y < imageHeight; y++) {
		for(int x = 0; x < imageWidth; x++) {
			ppmAddPixel(ppmImage[(y * imageWidth) + x]);
		}

		ppmNewline();
	}

	free(ppmImage);
	return 0;
}
