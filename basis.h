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
matrix *rand_matrix(unsigned int num_rows, unsigned int num_cols, double min, double max);
matrix *new_sqr_matrix(unsigned int dim);
void free_matrix(matrix *m);

#endif