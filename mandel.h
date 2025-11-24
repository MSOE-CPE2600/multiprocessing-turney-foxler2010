/*
 * Filename: mandel.h
 * Description: Header file for mandel.c
 * Author: Drew Malone <malonea@msoe.edu
 * Created: 11/11/25
 * Compile with: make
 */

#define _POSIX_C_SOURCE 200809L

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>
#include "jpegrw.h"

struct thread_arg_t {
    imgRawImage *img;
    double xmin;
    double xmax;
    double ymin;
    double ymax;
    int max;
    int start;
    int end;
};

static int iterations_at_point(double x, double y, int max);

static void compute_image(imgRawImage *img, double xmin, double xmax,
                          double ymin, double ymax, int max, int num_threads);

static void* compute_region(void *arg);

static int iteration_to_color(int i, int max);

static void show_help();
