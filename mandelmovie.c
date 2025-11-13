/*
 * Filename: mandelmovie.c
 * Description: Mandelbrot Set zoom animation generator
 * Author: Drew Malone <malonea@msoe.edu>
 * Created: 11/11/25
 * Compile with: make
 */

#include "mandelmovie.h"

int main(int argc, char *argv[])
{
    char optchar;
	// These are the default configuration values used
	// if no command line arguments are given.
    int max_iters = 1000;
	double x_zoomed = 0;
	double y_zoomed = 0;
	double scale = 4;
	int width = 1000;
	int height = 1000;
    const char *outfile_base = "mandel.jpg";

	// For each command line argument given,
	// override the appropriate configuration value.
	while ((optchar = getopt(argc,argv,"m:x:y:s:W:H:o:h")) != -1) {
		switch(optchar) {
			case 'm':
				max_iters = atoi(optarg);
				break;
			case 'x':
				x_zoomed = atof(optarg);
				break;
			case 'y':
				y_zoomed = atof(optarg);
				break;
			case 's':
				scale = atof(optarg);
				break;
			case 'W':
				width = atoi(optarg);
				break;
			case 'H':
				height = atoi(optarg);
				break;
			case 'o':
				outfile_base = optarg;
				break;
			case 'h':
				show_help();
				exit(1);
				break;
		}
	}

    // TODO modify x, y, and scale vars for each frame
    char *base = NULL;
    char *ext = NULL;
    int outfile_base_l = split_filename(outfile_base, &base, &ext);
    for (int frame = 0; frame < NUM_FRAMES; frame++) {
        char *outfile = parse_outfile(outfile_base_l, &base, &ext, frame);
        generate_frame();
        free(outfile);
    }
    // free base and ext buffers
    free(base);
    free(ext);

    return EXIT_SUCCESS;
}

int generate_frame()
{
    // TODO
    return 0;
}

/**
 * Prints help message
 */
int show_help()
{
    printf("Use: mandelmovie [options]\n");
	printf("Where options are:\n");
	printf("-m <max>    The maximum number of iterations per point. (default=1000)\n");
	printf("-x <coord>  X coordinate of zoomed-in image's center point. (default=0)\n");
	printf("-y <coord>  Y coordinate of zoomed-in image's center point. (default=0)\n");
	printf("-s <scale>  Scale of the zoomed-in image in Mandlebrot coordinates (X-axis). (default=4)\n");
	printf("-W <pixels> Width of the images in pixels. (default=1000)\n");
	printf("-H <pixels> Height of the images in pixels. (default=1000)\n");
	printf("-o <file>   Set output file base name. (default=mandel.jpg)\n");
    printf("             This will be modified to add the frame number\n");
    printf("             between the name and the file extension.\n");
    printf("             ex. \"mandel_35.jpg\"\n");
	printf("-h          Show this help text.\n");
	printf("\nSome examples are:\n");
	printf("mandelmovie -x -0.5 -y -0.5 -s 0.2\n");
	printf("mandelmovie -x -.38 -y -.665 -s .05 -m 100\n");
	printf("mandelmovie -x 0.286932 -y 0.014287 -s .0005 -m 1000\n\n");
    return 0;
}