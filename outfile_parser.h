/*
 * Filename: outfile_parser.h
 * Description: Header file for outfile_parser.c
 * Author: Drew Malone <malonea@msoe.edu>
 * Created: 11/12/25
 * Compile with: make
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char* parse_outfile(int outfile_base_l, char **base, char **ext, int frame);

int split_filename(const char *filename, char **base, char **ext);
