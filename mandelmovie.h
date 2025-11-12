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

int generate_frame();

char* parse_outfile(int outfile_base_l, char **base, char **ext, int frame);

int split_filename(const char *filename, char **base, char **ext);

int show_help();
