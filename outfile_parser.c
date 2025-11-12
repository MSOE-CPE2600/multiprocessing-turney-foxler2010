/*
 * Filename: outfile_parser.c
 * Description: Helper methods to add numbers onto a filename.
 * Author: Drew Malone <malonea@msoe.edu>
 * Created: 11/12/25
 * Compile with: make
 */

#include "outfile_parser.h"

/**
 * Combines the given base filename (from user input)
 * with the frame number, to create a new outfile name
 * that is actually to be written to disk.
 */
char* parse_outfile(int outfile_base_l, char **base, char **ext, int frame)
{
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
    sprintf(outfile, "%s_%d%s", *base, frame, *ext);
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
int split_filename(const char *filename, char **base, char **ext)
{
    // find the location of the '.' that separates
    // the "base" filename from its file extension
    int filename_l = strlen(filename);
    // ext_index defaults to the length so that if there's no '.' in the string then
    // the frame number will be appended to the end of the filename and ext will be empty
    int ext_index = filename_l;
    for (int i = filename_l; i >= 0; i--) {
        if (filename[i] == '.') {
            ext_index = i;
            break;
        }
    }
    // find file extension length
    int ext_l = filename_l - ext_index;
    *ext = malloc(ext_l);
    // copy file extension into its own buffer
    for (int i = 0; i < ext_l; i++) {
        (*ext)[i] = filename[ext_index+i];
    }
    // find base filename length
    // add 1 to length for the new null terminator
    int base_l = filename_l - ext_l + 1;
    *base = malloc(base_l);
    // copy base filename into its own buffer
    for (int i = 0; i < base_l; i++) {
        (*base)[i] = filename[i];
    }
    // the new null terminator; overwrites the '.'
    (*base)[base_l-1] = '\0';
    return filename_l;
}
