/*
 * Filename: mandel.c
 * Description: Based on example code found here:
 *              https://users.cs.fiu.edu/~cpoellab/teaching/cop4610_fall22/project3.html
 *              Converted to use jpg instead of BMP and other minor changes.
 * Modified by: Drew Malone <malonea@msoe.edu>
 * Created: 11/11/25
 * Comile with: make
 */

#include "mandel.h"

int main(int argc, char *argv[])
{
	char c;
	// These are the default configuration values used
	// if no command line arguments are given.
	const char *outfile = "mandel.jpg";
	double xcenter = 0;
	double ycenter = 0;
	double xscale = 4;
	double yscale = 0; // calc later
	int    image_width = 1000;
	int    image_height = 1000;
	int    max = 1000;
	int    num_threads = 1;

	// For each command line argument given,
	// override the appropriate configuration value.
	while ((c = getopt(argc,argv,"x:y:s:W:H:m:t:o:h")) != -1) {
		switch(c) {
			case 'x':
				xcenter = atof(optarg);
				break;
			case 'y':
				ycenter = atof(optarg);
				break;
			case 's':
				xscale = atof(optarg);
				break;
			case 'W':
				image_width = atoi(optarg);
				break;
			case 'H':
				image_height = atoi(optarg);
				break;
			case 'm':
				max = atoi(optarg);
				break;
			case 't':
				num_threads = atoi(optarg);
				break;
			case 'o':
				outfile = optarg;
				break;
			case 'h':
				show_help();
				exit(1);
				break;
		}
	}

	// Calculate y scale based on x scale (settable) and image sizes in X and Y (settable)
	yscale = xscale / image_width * image_height;

	// Display the configuration of the image.
	printf("mandel: x=%lf y=%lf xscale=%lf yscale=%1f max=%d threads=%d outfile=%s\n", xcenter, ycenter,
		   xscale, yscale, max, num_threads, outfile);
	
	// Create a raw image of the appropriate size.
	imgRawImage* img = initRawImage(image_width, image_height);
	// Fill it with a black
	setImageCOLOR(img, 0);
	// Compute the Mandelbrot image
	compute_image(img, xcenter-xscale/2, xcenter+xscale/2, ycenter-yscale/2, ycenter+yscale/2, max, num_threads);
	// Save the image in the stated file.
	storeJpegImageFile(img, outfile);
	// free the mallocs
	freeRawImage(img);

	return EXIT_SUCCESS;
}

/**
 * Return the number of iterations at point x, y
 * in the Mandelbrot space, up to a maximum of max.
 * 
 * This is the actual "mandelbrot math" part!!
 */
int iterations_at_point(double x, double y, int max)
{
	double x0 = x;
	double y0 = y;
	int iter = 0;
	while((x*x + y*y <= 4) && iter < max) {
		double xt = x*x - y*y + x0;
		double yt = 2*x*y + y0;
		x = xt;
		y = yt;
		iter++;
	}
	return iter;
}

/**
 * Compute an entire Mandelbrot image, writing each point to the given bitmap.
 * Scale the image to the range (xmin-xmax,ymin-ymax), limiting iterations to "max"
 */
void compute_image(imgRawImage *img, double xmin, double xmax, double ymin, double ymax, int max, int num_threads)
{
	int total_pixels = img->width * img->height;
	// how many pixels each thread should generate; rounded up
	int region_size = (int) ceil((double) total_pixels / num_threads);
	printf("\ttotal pixels: %d, region size: %d\n", total_pixels, region_size);
	pthread_t thread[num_threads];
	struct thread_arg_t *args[num_threads];
	for(int i = 0; i < num_threads; i++) {
		args[i] = malloc(sizeof(struct thread_arg_t));
		args[i]->img = img;
		args[i]->xmin = xmin;
		args[i]->xmax = xmax;
		args[i]->ymin = ymin;
		args[i]->ymax = ymax;
		args[i]->max = max;
		args[i]->start = i * region_size;
		args[i]->end = (int) fmin(args[i]->start + region_size, total_pixels);
		printf("\tcreating thread with range %d-%d\n", args[i]->start, args[i]->end);
		pthread_create(&thread[i], NULL, &compute_region, args[i]);
	}
	for (int i = 0; i < num_threads; i++) {
		pthread_join(thread[i], NULL);
		free(args[i]);
	}
}

static void* compute_region(void *arg_void)
{
	struct thread_arg_t *arg = (struct thread_arg_t*) arg_void;
	int img_x = 0;
	int img_y = 0;
	double mandel_x = 0;
	double mandel_y = 0;
	for (int pixel = arg->start; pixel < arg->end; pixel++) {
		// determine pixel coords based off of the pixel's number and width
		img_x = pixel % arg->img->width;
		img_y = pixel / arg->img->width;
		// Determine the point in x,y space for that pixel.
		mandel_x = arg->xmin + img_x * (arg->xmax - arg->xmin) / arg->img->width;
		mandel_y = arg->ymin + img_y * (arg->ymax - arg->ymin) / arg->img->height;
		// Compute the iterations at that point.
		int iters = iterations_at_point(mandel_x, mandel_y, arg->max);
		// Set the pixel in the bitmap.
		setPixelCOLOR(arg->img, img_x, img_y, iteration_to_color(iters, arg->max));
	}
	return NULL;
}

/**
 * Convert a iteration number to a color.
 * Here, we just scale to gray with a maximum of imax.
 * Modify this function to make more interesting colors.
 */
int iteration_to_color(int iters, int max)
{
	int color = 0xFFFFFF*iters / (double) max;
	return color;
}

/**
 * Show help message
 */
void show_help()
{
	printf("Use: mandel [options]\n");
	printf("Where options are:\n");
	printf("-t <threads> The number of threads to use.\n");
	printf("-m <max>    The maximum number of iterations per point. (default=1000)\n");
	printf("-x <coord>  X coordinate of image center point. (default=0)\n");
	printf("-y <coord>  Y coordinate of image center point. (default=0)\n");
	printf("-s <scale>  Scale of the image in Mandlebrot coordinates (X-axis). (default=4)\n");
	printf("-W <pixels> Width of the image in pixels. (default=1000)\n");
	printf("-H <pixels> Height of the image in pixels. (default=1000)\n");
	printf("-o <file>   Set output file. (default=mandel.bmp)\n");
	printf("-h          Show this help text.\n");
	printf("\nSome examples are:\n");
	printf("mandel -x -0.5 -y -0.5 -s 0.2\n");
	printf("mandel -x -.38 -y -.665 -s .05 -m 100\n");
	printf("mandel -x 0.286932 -y 0.014287 -s .0005 -m 1000\n\n");
}
