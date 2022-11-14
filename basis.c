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

// copy a matrix
matrix *copy_matrix(matrix *m) {
    matrix *nm = new_matrix(m->num_rows, m->num_cols);

    for (int i = 0; i < m->num_rows; i++) {
        for (int j = 0; j < m->num_cols; j++) {
            nm->data[i][j] = m->data[i][j];
        }
    }

    return nm;
}

// destructor
void free_matrix(matrix *m) {
    for (int i = 0; i < m->num_rows; ++i) {
        free(m->data[i]);
    }
    free(m->data);
    free(m);
}

// print matrix to stdout
void print_matrix(matrix *m) {
    for (int i = 0; i < m->num_rows; i++) {
        for (int j = 0; j < m->num_cols; j++) {
            printf("%lf\t", m->data[i][j]);
        }
        printf("\n");
    }
}

// check matrix equality
// matrix a = matrix b is of the same order
// and same corresponding elements
int is_equal_matrix(matrix *a, matrix *b) {
    if (!(a->num_rows == b->num_rows) && !(a->num_cols && b->num_cols)) {
        return 0;
    }

    for (int i = 0; i < a->num_rows; i++) {
        for (int j = 0; j < a->num_cols; j++) {
            if (a->data[i][j] != b->data[i][j]) {
                return 0;
            }
        }
    }

    return 1;
}

// scalar multiplication
matrix *smult(matrix *m, double num) {
    matrix *nm = copy_matrix(m);

    for (int i = 0; i < nm->num_rows; i++) {
        for (int j = 0; j < nm->num_cols; j++) {
            nm->data[i][j] *= num; 
        }
    }

    return nm;
}

// matrix multiplication
matrix *mmult(matrix *a, matrix *b) {
    if (a->num_cols != b->num_rows) return NULL;
    matrix *ret = new_matrix(a->num_rows, b->num_cols);
    int i, j, k;
    
    for (i=0; i < a->num_rows; i++)
    for (k=0; k < a->num_cols; k++) 
    for (j=0; j < b->num_cols; j++)
        ret->data[i][j] += a->data[i][k] * b->data[k][j];
    
    return ret;
}

int main(int argc, char **argv) {
    matrix *m = read_matrix(argv[1]);
    m = mmult(m, m);
    print_matrix(m);
}