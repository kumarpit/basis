#ifndef BASIS
#define BASIS

#include "utils.h"

#define SIZE 10

/**
type definitions
*/
typedef struct _matrix {
    unsigned int num_rows;
    unsigned int num_cols;
    double *data;
    int is_square;
} matrix;

typedef struct _fmatrices {
    matrix **matrices;
    int count;
} fmatrices;


/**
basic matrix generation functions
*/
matrix *new_matrix(unsigned int num_rows, unsigned int num_cols);
matrix *new_sqr_matrix(unsigned int dim);
matrix *new_rand_matrix(unsigned int num_rows, unsigned int num_cols, double min, double max);
matrix *new_rand_sqr_matrix(unsigned int dim, double min, double max);
matrix *new_eye_matrix(unsigned int dim);
fmatrices *read_matrix(char *filename);
matrix *copy_matrix(matrix *m);
void free_matrix(matrix *m);
void print_matrix(matrix *m);
double get_matrix_val(matrix *m, int i, int j);
void set_matrix_val(matrix *m, double val, int i, int j);

/**
Basic matrix operationsc
*/
int are_equal_matrices(matrix *a, matrix *b);
void scalar_mult(matrix *m, double num);
matrix *matrix_mult(matrix *a, matrix *b);

#endif