#include <stdlib.h>
#include <stdio.h>
#include "basis.h"


// constructor - generates zero-valued matrix
matrix *new_matrix(unsigned int num_rows, unsigned int num_cols) {
    if (num_rows == 0) {
        printf("Invalid num_rows");
        return NULL;
    }

    if (num_cols == 0) {
        printf("Invalid num_cols");
        return NULL;
    }

    matrix *m = calloc(1, sizeof(*m));
    m->num_rows = num_rows;
    m->num_cols = num_cols;
    m->is_square = (num_rows == num_cols) ? 1 : 0;
    m->data = calloc(num_rows, sizeof(*m->data));

    for (int i = 0; i < m->num_rows; ++i) {
        m->data[i] = calloc(num_cols, sizeof(**m->data));
    }

    return m;
}

// generates a matrix with random values within min, max interval
matrix *new_rand_matrix(unsigned int num_rows, unsigned int num_cols, double min, double max) {
    matrix *m = new_matrix(num_rows, num_cols);
    for (int i = 0; i < num_rows; ++i) {
        for (int j = 0; j < num_cols; ++j) {
            m->data[i][j] = rand_interval(min, max);
        }
    }

    return m;
}

// generates zero-valued square matrix
matrix *new_sqr_matrix(unsigned int dim) {
    return new_matrix(dim, dim);
}

// generates square matrix with random values within min, max interval
matrix *new_rand_sqr_matrix(unsigned int dim, double min, double max) {
    return new_rand_matrix(dim, dim, min, max);
}

// generates identity matrix of dim
matrix *new_eye_matrix(unsigned int dim) {
    matrix *m = new_sqr_matrix(dim);
    for (int i = 0; i < dim; ++i) {
        m->data[i][i] = 1.0;
    }   

    return m;
}

// reads matrix from file
matrix *read_matrix(char *filename) {
    FILE *f = fopen(filename, "r");

    if (f == NULL) {
        fprintf(stderr, "Error: Cannot open %s\n", filename);
        exit(1);
    }

    matrix* m;
    int num_rows, num_cols, is_square;

    fscanf(f, "%d", &num_rows);
    fscanf(f, "%d", &num_cols);

    m = new_matrix(num_rows, num_cols);

    for (int i = 0; i < num_rows; ++i) {
        for (int j = 0; j < num_cols; ++j) {
            fscanf(f, "%lf\t", &m->data[i][j]);
        }
    }

    return m;
}

// destructor
void free_matrix(matrix *m) {
    for (int i = 0; i < m->num_rows; ++i) {
        free(m->data[i]);
    }
    free(m->data);
    free(m);
}

int main(int argc, char **argv) {
    matrix *m = read_matrix(argv[1]);
    printf("%d", m->is_square);
}