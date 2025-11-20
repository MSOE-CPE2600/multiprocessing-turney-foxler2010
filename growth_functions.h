/*
 * Filename: growth_functions.h
 * Description: Header file for growth_functions.c
 * Author: Drew Malone <malonea@msoe.edu>
 * Created: 11/12/25
 * Compile with: make
 */

#include <math.h>

int exponential_growth(double *values, double start, double end, int num_values);

int constant(double *values, double start, int num_values);