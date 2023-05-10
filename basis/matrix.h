#ifndef BASIS
#define BASIS

#include "utils.h"

/**
type definitions
TODO: use 1d array instead of 2d
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
matrix *copy_matrix(matrix *m);
void free_matrix(matrix *m);
void print_matrix(matrix *m);

/**
Basic matrix operationsc
*/
int is_equal_matrix(matrix *a, matrix *b);
matrix *smult(matrix *m, double num);
matrix *mmult(matrix *a, matrix *b);

#endif