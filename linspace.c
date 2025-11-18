/*
 * Filename: linspace.c
 * Description: Generates list of equally-spaced values between two numbers.
 * Author: Drew Malone <malonea@msoe.edu>
 * Created: 11/12/25
 * Compile with: make
 */

#include "linspace.h"

/**
 * @brief Generates a list of <num_values> equally-spaced values between the start and end number.
 * @param linspace array to write the values to. Must have num_spaces elements.
 * @param start the number at one end of the list (the "start")
 * @param end the number at the other end of the list (the "end")
 * @param num_values the number of values to generate, including the start and end number
 * @return 0 on success
 * 
 */

int expspace(double *linspace, double start, double end, int num_values)
{
    for (int i = 0; i < num_values; i++) {
        linspace[i] = start * pow(end / start, (double) i / (num_values - 1));
    }
    return 0;
}

int linspace(double *linspace, double start, double end, int num_values)
{
    double step_size = (end - start)/num_values;
    for (int i = 0; i < num_values; i++) {
        linspace[i] = start+(step_size*i);
    }
    return 0;
}