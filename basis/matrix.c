#include <stdlib.h>
#include <stdio.h>
#include "matrix.h"
#include <sys/resource.h>
#include <sys/time.h>
#include <assert.h>
#include <pthread.h>
#include "refcount.h"

#define NUM_THREADS 8
#define DIM 2048
#define tv_ms_diff(tv1, tv2) \
        (tv2.tv_sec * 1000) + ((double)tv2.tv_usec / 1000) - \
        ((tv1.tv_sec * 1000) + ((double)tv1.tv_usec / 1000))

// instead of ranging through k, range through i,j
// better use of cache
// If the only possible errors are programmer errors, don't return an error code, use asserts inside the function.
typedef struct thread_args_t {
    int start;
    int end;
} thread_args;

matrix *matrix_a;
matrix *matrix_b;
matrix *matrix_c;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void init() {
    matrix_a = new_rand_matrix(DIM, DIM, 0, 5);
    matrix_b = new_rand_matrix(DIM, DIM, 0, 5);
    matrix_c = new_matrix(DIM, DIM);
}

// constructor - generates zero-valued matrix
matrix *new_matrix(unsigned int num_rows, unsigned int num_cols) {
    assert(num_rows > 0 && num_cols > 0);

    matrix *m = rc_malloc(sizeof(matrix), free_matrix);
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

// generates identity matrix of dimension dim
matrix *new_eye_matrix(unsigned int dim) {
    matrix *m = new_sqr_matrix(dim);
    for (int i = 0; i < dim; ++i) {
        m->data[i][i] = 1.0;
    }   

    return m;
}


// PRIVATE
void free_fmatrices(fmatrices *fm) {
    for (int i = 0; i < fm->count; i++) {
        free_matrix(fm->matrices[i]);
    }
}

// reads matrices from file
fmatrices *read_matrix(char *filename) {
    FILE *f = fopen(filename, "r");
    fmatrices *fm = rc_malloc(sizeof(fmatrices), free_fmatrices);
    fm->matrices = malloc(SIZE * sizeof(matrix*));
    fm->count = 0;
    matrix **m = fm->matrices;

    int curr_size = SIZE;

    if (f == NULL) {
        fprintf(stderr, "Error: Cannot open %s\n", filename);
        exit(1);
    }

    while (1) {    
        int num_rows, num_cols;
        if (fscanf(f, "%d", &num_rows) == EOF) {
            break;
        };
        fscanf(f, "%d", &num_cols);
        m[fm->count] = new_matrix(num_rows, num_cols);

        for (int i = 0; i < num_rows; ++i) {
            for (int j = 0; j < num_cols; ++j) {
                fscanf(f, "%lf\t", &m[fm->count]->data[i][j]);
            }
        }

        fm->count++;
        
        if (fm->count == SIZE) {
            // DOUBLE SIZE AND COPY OVER ARRAY
            matrix **new = malloc(2 * curr_size * sizeof(matrix*));
            for (unsigned int i = 0; i < fm->count; i++) {
                new[i] = fm->matrices[i];
            }
            free(fm->matrices);
            fm->matrices = new;
            curr_size *= 2;
        }
    }
    return fm;
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
int are_equal_matrices(matrix *a, matrix *b) {
    if (!(a->num_rows == b->num_rows) && !(a->num_cols && b->num_cols)) {
        return 0;
    }

    for (int i = 0; i < a->num_rows; i++) {
        for (int j = 0; j < a->num_cols; j++) {
            if (fabs(a->data[i][j] - b->data[i][j]) > 0.0001) {
                return 0;
            }
        }
    }

    return 1;
}

// scalar multiplication
void scalar_mult(matrix *m, double num) {
    for (int i = 0; i < m->num_rows; i++) {
        for (int j = 0; j < m->num_cols; j++) {
            m->data[i][j] *= num; 
        }
    }
}

matrix *matrix_mult(matrix *a, matrix *b) {
    return a;
}