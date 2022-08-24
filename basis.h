#ifndef BASIS
#define BASIS

#include "utils.h"

/**
type definitions
*/
typedef struct _matrix {
    unsigned int num_rows;
    unsigned int num_cols;
    double **data;
    int is_square;
} matrix;

/**
basic matrix generation functions
*/
matrix *new_matrix(unsigned int num_rows, unsigned int num_cols);
matrix *new_sqr_matrix(unsigned int dim);
matrix *new_rand_matrix(unsigned int num_rows, unsigned int num_cols, double min, double max);
matrix *new_rand_sqr_matrix(unsigned int dim, double min, double max);
matrix *new_eye_matrix(unsigned int dim);
matrix *read_matrix(char *filename);
void free_matrix(matrix *m);

#endif