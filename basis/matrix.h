#ifndef BASIS
#define BASIS

#include "utils.h"

#define SIZE 10

/**
type definitions
*/

typedef struct _matrix {
    uint num_rows;
    uint num_cols;
    double *data;
    int is_square;
} matrix;

typedef struct _fmatrices {
    matrix **matrices;
    uint count;
} fmatrices;


/**
initialization functions
*/
matrix *new_matrix(unsigned int num_rows, unsigned int num_cols);
matrix *new_sqr_matrix(uint dim);
matrix *new_rand_matrix(uint num_rows, uint num_cols, double min, double max);
matrix *new_rand_sqr_matrix(uint dim, double min, double max);
matrix *new_eye_matrix(unsigned int dim);
fmatrices *read_matrix(char *filename);
matrix *copy_matrix(matrix *m);
void free_matrix(void *m);
void print_matrix(matrix *m);
double get_matrix_val(matrix *m, uint i, uint j);
void set_matrix_val(matrix *m, double val, uint i, uint j);
matrix *get_matrix_col(matrix *m, unsigned int c);
matrix *get_matrix_row(matrix *m, unsigned int c);

/**
matrix operations
*/
int are_equal_matrices(matrix *a, matrix *b);
void scalar_mult(matrix *m, double num);
matrix *matrix_mult(matrix *a, matrix *b);

#endif