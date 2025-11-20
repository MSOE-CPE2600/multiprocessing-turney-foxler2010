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
    int num_children = 10;
    int max_iters = 1000;
	double x_zoomed = 0;
	double y_zoomed = 0;
	double scale = 4;
	int width = 1000;
	int height = 1000;
    const char *outfile_base = "mandel.jpg";

	// For each command line argument given,
	// override the appropriate configuration value.
	while ((optchar = getopt(argc,argv,"c:m:x:y:s:W:H:o:h")) != -1) {
		switch(optchar) {
            case 'c':
                num_children = atoi(optarg);
                break;
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
    
    double *x_values = malloc(sizeof(double)*NUM_FRAMES);
    constant(x_values, x_zoomed, NUM_FRAMES);
    double *y_values = malloc(sizeof(double)*NUM_FRAMES);
    constant(y_values, y_zoomed, NUM_FRAMES);
    double *scale_values = malloc(sizeof(double)*NUM_FRAMES);
    exponential_growth(scale_values, scale, TARGET_SCALE, NUM_FRAMES);

    char *base = NULL;
    char *ext = NULL;
    int outfile_base_l = split_filename(outfile_base, &base, &ext);
    // create children and give them each a different set of frames
    for (int i = 0; i < num_children; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            // this is the child
            // determine frame range to compute
            int start = i * (NUM_FRAMES / num_children);
            int end = start + (NUM_FRAMES / num_children);
            printf("\e[1mPID %d:\e[0m Generating frames %d-%d...\n", getpid(), start, end);
            // generate the frames and store them each to a new file
            for (int frame = start; frame < end; frame++) {
                char *outfile = parse_outfile(outfile_base_l, &base, &ext, frame);
                generate_frame(max_iters, x_values[frame], y_values[frame],
                               scale_values[frame], width, height, outfile);
                free(outfile);
            }
            // child is all done
            exit(EXIT_SUCCESS);
        } else {
            // this is the parent
            // print info about the child that was just created
            printf("\e[1mPID %d:\e[0m Created new child with PID %d\n", getpid(), pid);
        }
    }
    // wait for children to finish
    for (int i = 0; i < num_children; i++) {
        pid_t pid = wait(NULL);
        printf("\e[1mPID %d:\e[0m child with PID %d has exited\n", getpid(), pid);
    }

    // free buffers and linspaces
    free(x_values);
    free(y_values);
    free(scale_values);
    free(base);
    free(ext);

    exit(EXIT_SUCCESS);
}

int generate_frame(int max_iters, double x, double y,
                   double scale, int width, int height, char *outfile)
{
    pid_t pid = fork();
    if (pid == 0) {
        /*
         * convert number args to strings to be passed to execl()
         * 
         * snprintf returns the number of chars it *would* print
         * (excluding the null terminator), which is then used as
         * the length of the string.
         */
        char max_iters_s[snprintf(NULL, 0, "%d", max_iters)+1];
        snprintf(max_iters_s, sizeof max_iters_s, "%d", max_iters);
        char x_s[snprintf(NULL, 0, "%.6lf", x)+1];
        snprintf(x_s, sizeof x_s, "%.6lf", x);
        char y_s[snprintf(NULL, 0, "%.6lf", y)+1];
        snprintf(y_s, sizeof y_s, "%.6lf", y);
        char scale_s[snprintf(NULL, 0, "%.6lf", scale)+1];
        snprintf(scale_s, sizeof scale_s, "%.6lf", scale);
        char width_s[snprintf(NULL, 0, "%d", width)+1];
        snprintf(width_s, sizeof width_s, "%d", width);
        char height_s[snprintf(NULL, 0, "%d", height)+1];
        snprintf(height_s, sizeof height_s, "%d", height);

        // send this child to execute the mandel program with the stringified args
        execl("./mandel", "mandel", "-m", max_iters_s, "-x", x_s, "-y", y_s,
              "-s", scale_s, "-W", width_s, "-H", height_s, "-o", outfile, NULL);
        // execl() does not return, so we should never get here
        exit(EXIT_FAILURE);
    }
    // parent waits for the frame to be made
    wait(NULL);
    // parent doesn't do anything else for this frame
    return 0;
}

/**
 * Prints help message
 */
int show_help()
{
    printf("Use: mandelmovie [options]\n");
	printf("Where options are:\n");
    printf("-c <children> The number of child processes to use.\n");
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
