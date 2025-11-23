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
	int width = img->width;
	int height = img->height;

	int num_pixels = width * height;
	int pixels_per_thread = num_pixels / num_threads;
	int extra_pixels = num_pixels % num_threads;
	printf("\t%d pixels total, %d pixels per thread, %d extra pixels, %d sum (should match total)\n",
		   num_pixels, pixels_per_thread, extra_pixels, (pixels_per_thread * num_threads) + extra_pixels);

	struct thread_arg_t *image_info = malloc(sizeof(struct thread_arg_t));
	image_info->width = img->width;
	image_info->height = img->height;
	image_info->num_pixels_region = pixels_per_thread;

	pthread_t thread[num_threads];
	int i;
	for (i = 0; i < num_threads-1; i++) {
		pthread_create(thread[i], NULL, &compute_region, image_info);
	}
	image_info->num_pixels_region = pixels_per_thread + extra_pixels;
	pthread_create(thread[i+1], NULL, &compute_region, image_info);
	free(arg);

	for (i = 0; i < num_threads; i++) {
		pthread_join(thread[i], NULL);
	}
}

static void* compute_region(void *arg)
{
	struct thread_arg_t *image_info = (struct thread_arg_t*) arg;
	int width = image_info->width;
	int height = image_info->height;
	// For every pixel in the image...
	for (j = 0; j < height; j++) {
		for (i = 0; i < width; i++) {
			// Determine the point in x,y space for that pixel.
			double x = xmin + i*(xmax-xmin)/width;
			double y = ymin + j*(ymax-ymin)/height;
			// Compute the iterations at that point.
			int iters = iterations_at_point(x, y, max);
			// Set the pixel in the bitmap.
			setPixelCOLOR(img, i, j, iteration_to_color(iters, max));
		}
	}
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
