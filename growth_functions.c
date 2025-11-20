/*
 * Filename: growth_functions.c
 * Description: Functions that fill arrays with values for each index.
 *              Different formulas for different changes in growth over time.
 * Author: Drew Malone <malonea@msoe.edu>
 * Created: 11/19/25
 * Compile with: make
 */

#include "growth_functions.h"

int exponential_growth(double *values, double start, double end, int num_values)
{
    for (int i = 0; i < num_values; i++) {
        values[i] = start * pow(end / start, (double) i / (num_values - 1));
    }
    return 0;
}

int constant(double *values, double start, int num_values)
{
    for (int i = 0; i < num_values; i++) {
        values[i] = start;
    }
    return 0;
}
