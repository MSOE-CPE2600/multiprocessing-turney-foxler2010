/*
 * Filename: mandelmovie.h
 * Description: Header file for mandelmovie.c
 * Author: Drew Malone <malonea@msoe.edu>
 * Created: 11/11/25
 * Compile with: make
 */

#define _POSIX_C_SOURCE 200809L

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "outfile_parser.h"

#define NUM_FRAMES 50

int generate_frame(int max_iters, double x_zoomed, double y_zoomed,
                   double scale, int width, int height, char *outfile);

int show_help();
