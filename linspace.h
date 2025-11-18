/*
 * Filename: linspace.h
 * Description: Header file for linspace.c
 * Author: Drew Malone <malonea@msoe.edu>
 * Created: 11/12/25
 * Compile with: make
 */

#include <math.h>

int expspace(double *linspace, double start, double end, int num_values);

int linspace(double *linspace, double start, double end, int num_values);

int constant(double *constant, double value, int num_values);
