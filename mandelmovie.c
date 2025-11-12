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
    /*
     * Required Parsing:
     * constant:
     * - max iters
     * - outfile
     * - width
     * - height
     * different for each frame:
     * - scale
     * - x
     * - y
     */

    char optchar;
	// These are the default configuration values used
	// if no command line arguments are given.
	const char *outfile_base = "mandel.jpg";
	double x_zoomed = 0;
	double y_zoomed = 0;
	double scale = 4;
	int    width = 1000;
	int    height = 1000;
	int    max_iters = 1000;

	// For each command line argument given,
	// override the appropriate configuration value.
	while ((optchar = getopt(argc,argv,"x:y:s:W:H:m:o:h")) != -1) {
		switch(optchar) {
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
			case 'm':
				max_iters = atoi(optarg);
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

    // TODO debug
    printf("\e[1mDEBUG:\e[0m outfile_base: %s\n", outfile_base);
    printf("\e[1mDEBUG:\e[0m x_zoomed: %lf\n", x_zoomed);
    printf("\e[1mDEBUG:\e[0m y_zoomed: %lf\n", y_zoomed);
    printf("\e[1mDEBUG:\e[0m scale: %lf\n", scale);
    printf("\e[1mDEBUG:\e[0m width: %d\n", width);
    printf("\e[1mDEBUG:\e[0m height: %d\n", height);
    printf("\e[1mDEBUG:\e[0m max_iters: %d\n", max_iters);
    
    // TODO magic number
    for (int frame = 0; frame < 50; frame++) {
        char *outfile = parse_outfile(outfile_base, frame);
        // TODO debug
        printf("\e[1mDEBUG:\e[0m outfile: %s\n", outfile);
        generate_frame();
    }

    return EXIT_SUCCESS;
}

int generate_frame()
{
    // TODO
    return 0;
}

/**
 * Combines the given base filename (from user input)
 * with the frame number, to create a new outfile name
 * that is actually to be written to disk.
 */
char* parse_outfile(const char *outfile_base, int frame)
{
    // split filename into base and extension
    char *base = NULL;
    char *ext = NULL;
    int outfile_base_l = split_filename(outfile_base, base, ext);
    // figure out how many chars will be added based on frame number
    int chars_added;
    if (frame < 10) {
        // '_' + 1 digit = 2 chars added
        // TODO magic number
        chars_added = 2;
    } else {
        // '_' + 2 digits = 3 chars added
        // TODO magic number
        chars_added = 3;
    }
    // allocate space for the parsed outfile name
    char *outfile = malloc(outfile_base_l+chars_added);
    // put the base and extension back together with the frame number in the middle
    // ex. "mandel.jpg" becomes "mandel_0.jpg"
    sprintf(outfile, "%s_%d%s", base, frame, ext);
    // free base and ext buffers
    free(base);
    free(ext);
    // return pointer to parsed outfile name
    return outfile;
}

/**
 * Splits the given filename into its base and extension.
 * 
 * The resulting strings are allocated in heap,
 * and pointers written to the latter parameters.
 * 
 * The function returns the original filename's length,
 * so that it doesn't need to be calculated again.
 */
int split_filename(const char *filename, char *base, char *ext)
{
    // find the location of the '.' that separates
    // the "base" filename from its file extension
    int filename_l = strlen(filename);
    int ext_index;
    for (int i = filename_l; i >= 0; i--) {
        if (filename[i] == '.') {
            ext_index = i;
            break;
        }
    }
    // find file extension length
    int ext_l = filename_l - ext_index;
    ext = malloc(ext_l);
    // copy file extension into its own buffer
    for (int i = 0; i < ext_l; i++) {
        ext[i] = filename[ext_index+i];
    }
    // find base filename length
    // add 1 to length for the new null terminator
    int base_l = filename_l - ext_l + 1;
    base = malloc(base_l);
    // copy base filename into its own buffer
    for (int i = 0; i < base_l; i++) {
        base[i] = filename[i];
    }
    // the new null terminator; overwrites the '.'
    base[base_l-1] = '\0';
    return filename_l;
}

/**
 * Prints help message
 */
int show_help()
{
    // TODO
    printf("\e[1m\e[31mTODO write help message.\e[0m\n");
    return 0;
}