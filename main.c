#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define RESO  1000 // detail


#define MAX_ITERATION 1000

int main () {
	float y_min = -1.12, y_max = 1.12;
	float x_min = -2, x_max = 0.5;

	int b = fabs(x_min-x_max) * RESO; // MAX
	int a = fabs(y_min-y_max) * RESO;	// IMUMA

	float normalize_x(int i, int width, float x_min, float x_max) {return x_min + (i / (float)width) * (x_max - x_min);}
	float normalize_y(int j, int height, float y_min, float y_max) {return y_min + (j / (float)height) * (y_max - y_min);}	

	printf("a: %d\tb: %d\n",a,b);

	FILE *fp = fopen("slika.ppm", "wb");
	(void) fprintf(fp, "P6\n%d %d\n255\n", b,a);

	char ***image = malloc(a * sizeof(char **));
	for (int x = 0; x < a; x++) {
		image[x] = malloc(b * sizeof(char *));
		for (int y = 0; y < b; y++) {
			image[x][y] = malloc(3 * sizeof(char));
		}
	}
	for (int j = 0; j < a; ++j) {
		for (int i = 0; i < b; ++i) {
			int iteration = 0;
			float x = 0.0;
			float y = 0.0;

			while (powf(x,2) + powf(y,2) <= 4 && iteration < MAX_ITERATION) {
				float xtemp;
				xtemp = powf(x,2) - powf(y,2) + normalize_x(i,b,x_min,x_max);
				y = 2*x*y + normalize_y(j,a,y_min,y_max);
				x = xtemp;
			    iteration = iteration + 1;
			}
			float sir = 100;
			image[j][i][2] = (sin((float)iteration/sir)+1)*255; 

		}
		printf("\r%0.1f%% done", (float)j/(float)a*100);
	}
	printf("\n");
/* print image */
	for (int j = 0; j < a; ++j) {
		for (int i = 0; i < b; ++i) {
				(void) fwrite(image[j][i], 1, 3, fp);
		}
	}
	(void) fclose(fp);
	return 0;
}
