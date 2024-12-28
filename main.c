#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

#define RESO  400 // detail
#define MAX_ITERATION 1000

#define CORENUM 256 

int main () {
	double y_min = 0, y_max = 1.12; 
	double x_min = -5, x_max = 0.5;

	double x_diff = fabs(x_min - x_max);
	double y_diff = fabs(y_min - y_max);

	printf("x_diff = %e, y_diff = %e\n", x_diff, y_diff);

	int b = (x_diff * RESO); // MAX
	int a = (y_diff * RESO); // IMUMA

	double normalize_x(int i, int width, double x_min, double x_max) {return x_min + (i / (double)width) * (x_max - x_min);}
	double normalize_y(int j, int height, double y_min, double y_max) {return y_min + (j / (double)height) * (y_max - y_min);}	

	printf("a: %d\tb: %d\n",a,b);

	FILE *fp = fopen("output.ppm", "wb");
	(void) fprintf(fp, "P6\n%d %d\n255\n", b,a);

	char ***image = malloc(a * sizeof(char **));
	for (int x = 0; x < a; x++) {
		image[x] = malloc(b * sizeof(char *));
		for (int y = 0; y < b; y++) {
			image[x][y] = malloc(3 * sizeof(char));
		}
	}

	typedef struct{
		int tid;
		int a;
	}t_arg;

	void *draw_t (void* arg) { 
		t_arg* argumenti = (t_arg*) arg;
		int start_j = argumenti->tid * (argumenti->a / CORENUM);
		int end_j = (argumenti->tid + 1) * (argumenti->a / CORENUM);

		if (argumenti->tid == CORENUM - 1) 
    	    end_j = argumenti->a;

		for (int j = start_j; j < end_j; ++j) {
			for (int i = 0; i < b; ++i) {
				int iteration = 0;
				double x = 0.0;
				double y = 0.0;
	
				while (powf(x,2) + powf(y,2) <= 4 && iteration < MAX_ITERATION) {
					double xtemp;
					xtemp = powf(x,2) - powf(y,2) + normalize_x(i,b,x_min,x_max);
					y = 2*x*y + normalize_y(j,a,y_min,y_max);
					x = xtemp;
				    iteration = iteration + 1;
				}
				double sir = 100;
				image[j][i][2] = (((double)iteration/sir))*255; 
			}
		}
		return NULL;
	}

	pthread_t threads[CORENUM];
	t_arg argumenti[CORENUM];

	for (int t=0; t < CORENUM; t++) {
		argumenti[t].tid = t;
		argumenti[t].a = a;
		if (pthread_create(&threads[t], NULL, draw_t, &argumenti[t]) != 0) {
			printf ("error creating threads\n");
			return 1;
		}
	}	

	for (int t=0; t < CORENUM; ++t) {
		    pthread_join(threads[t], NULL);
	}	
	
	/* print image */
	for (int j = 0; j < a; ++j) {
		for (int i = 0; i < b; ++i) {
			(void) fwrite(image[j][i], 1, 3, fp);
		}
	}
	(void) fclose(fp);
	return 0;
}
