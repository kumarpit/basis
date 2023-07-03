#ifndef BASIS
#define BASIS

#include "utils.h"
#include <stdbool.h>

#define SIZE 10

/**
type definitions
*/

typedef struct matrix_t matrix;

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
// void write_rand_matrix(uint num_rows, uint num_cols, double min, double max);
matrix *new_rand_sqr_matrix(uint dim, double min, double max);
matrix *new_eye_matrix(unsigned int dim);
fmatrices *read_matrix(char *filename);
matrix *copy_matrix(matrix *m);
void free_matrix(void *m);
void print_matrix(matrix *m);
double get_matrix_val(matrix *m, uint i, uint j);
int set_matrix_val(matrix *m, double val, uint i, uint j);
matrix *get_matrix_col(matrix *m, unsigned int c);
matrix *get_matrix_row(matrix *m, unsigned int c);
uint num_rows(matrix *m);
uint num_cols(matrix *m);

// row operations
void swap_row(matrix *m, uint i, uint j);
void mult_row(matrix *m, double k, uint i);
void add_row(matrix *m, uint i, uint j, double scale);
void normalize_row(matrix *m, uint i);

/**
matrix operations
*/
bool are_equal_matrices(matrix *a, matrix *b);
void scalar_mult(matrix *m, double num);
matrix *matrix_mult(matrix *a, matrix *b);
void ref(matrix *m);

#endif