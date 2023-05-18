#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "matrix.h"
#include <sys/resource.h>
#include <sys/time.h>
#include <assert.h>
#include <pthread.h>
#include "refcount.h"

#define NUM_THREADS 4
#define tv_ms_diff(tv1, tv2) \
        (tv2.tv_sec * 1000) + ((double)tv2.tv_usec / 1000) - \
        ((tv1.tv_sec * 1000) + ((double)tv1.tv_usec / 1000))


typedef struct global_thread_args_t {
    uint dim_rows;
    uint dim_cols;
    matrix *dest;
    matrix *a;
    matrix *b;
} global_thread_args;

typedef struct thread_args_t {
    uint start;
    uint end;
    global_thread_args *globals;
} thread_args;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

// construct a new zero-valued mxn matrix
matrix *new_matrix(uint num_rows, uint num_cols) {
    assert(num_rows > 0 && num_cols > 0);

    matrix *m = rc_malloc(sizeof(matrix), free_matrix);
    m->num_rows = num_rows;
    m->num_cols = num_cols;
    m->is_square = (num_rows == num_cols) ? 1 : 0;
    m->data = calloc(num_rows * num_cols, sizeof(double));

    return m;
}

// generates a matrix with random values within min, max interval
matrix *new_rand_matrix(uint num_rows, uint num_cols, double min, double max) {
    matrix *m = new_matrix(num_rows, num_cols);
    for (uint i = 0; i < num_rows; ++i) {
        for (uint j = 0; j < num_cols; ++j) {
            set_matrix_val(m, rand_interval(min, max), i, j);
        }
    }

    return m;
}

// generates zero-valued square matrix of size dim
matrix *new_sqr_matrix(uint dim) {
    return new_matrix(dim, dim);
}

// generates square matrix with random values within min, max interval
matrix *new_rand_sqr_matrix(uint dim, double min, double max) {
    return new_rand_matrix(dim, dim, min, max);
}

// generates identity matrix of dimension dim
matrix *new_eye_matrix(uint dim) {
    matrix *m = new_sqr_matrix(dim);
    for (uint i = 0; i < dim; ++i) {
        set_matrix_val(m, 1, i, i);
    }   
    return m;
}

// get value of matrix at position i, j
double get_matrix_val(matrix *m, uint i, uint j) {
    assert(i <= m->num_rows-1 && j <= m->num_cols-1);
    return m->data[i*m->num_cols + j];
}

void set_matrix_val(matrix *m, double val, uint i, uint j) {
    assert(i <= m->num_rows-1 && j <= m->num_cols-1);
    m->data[i*m->num_cols + j] = val;
}

matrix *get_matrix_col(matrix *m, uint c) {
    assert(c < m->num_cols);
    matrix *r = new_matrix(m->num_rows, 1);
    for (uint i = 0; i < m->num_rows; i++) {
        double val = get_matrix_val(m, i, c);
        set_matrix_val(r, val, i, 0);
    }
    return r;
}

matrix *get_matrix_row(matrix *m, uint c) {
    assert(c < m->num_rows);
    matrix *r = new_matrix(1, m->num_cols);
    for (uint i = 0; i < m->num_cols; i++) {
        double val = get_matrix_val(m, c, i);
        set_matrix_val(r, val, 0, i);
    }
    return r;
}

/**
 * PRIVATE
 * destroy fmatrices struct
*/
void free_fmatrices(void *_fm) {
    fmatrices *fm = (fmatrices *) _fm;
    for (uint i = 0; i < fm->count; i++) {
        free_matrix(fm->matrices[i]);
    }
}

/**
 * Read an arbitrary number of matrices from a file
 * The file specifying the matrices must specify their dimension
 * (rows, columns) and each row of the matrix on a new line (see ./tests/test_matrices/matrix{N}.txt)
 * Return a fmatrices struct specifying the count - number of matrices read
 * and a matrix ** (array of matrix pointers)
*/
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
        uint num_rows, num_cols;
        if (fscanf(f, "%d", &num_rows) == EOF) {
            break;
        };
        fscanf(f, "%d", &num_cols);
        m[fm->count] = new_matrix(num_rows, num_cols);

        for (uint i = 0; i < num_rows; ++i) {
            for (uint j = 0; j < num_cols; ++j) {
                fscanf(f, "%lf\t", &m[fm->count]->data[i*num_cols + j]);
            }
        }

        fm->count++;
        
        if (fm->count == SIZE) {
            // DOUBLE SIZE AND COPY OVER ARRAY
            matrix **new = malloc(2 * curr_size * sizeof(matrix*));
            for (uint i = 0; i < fm->count; i++) {
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
    uint num_cols = m->num_cols;
    for (uint i = 0; i < m->num_rows; i++) {
        for (uint j = 0; j < num_cols; j++) {
            double val = get_matrix_val(m, i, j);
            set_matrix_val(nm, val, i, j);
        }
    }
    return nm;
}

// destructor for matrix
void free_matrix(void *_m) {
    matrix *m = (matrix *)_m;
    free(m->data);
}

// print matrix to stdout
void print_matrix(matrix *m) {
    for (uint i = 0; i < m->num_rows; i++) {
        for (uint j = 0; j < m->num_cols; j++) {
            printf("%lf\t", get_matrix_val(m, i, j));
        }
        printf("\n");
    }
}

// checks if two matrices are equal (by value, not by reference)
int are_equal_matrices(matrix *a, matrix *b) {
    if (!(a->num_rows == b->num_rows) && !(a->num_cols && b->num_cols)) {
        return 0;
    }
    uint num_cols = a->num_cols;
    for (uint i = 0; i < a->num_rows; i++) {
        for (uint j = 0; j < num_cols; j++) {
            if (fabs(get_matrix_val(a, i, j) - get_matrix_val(b, i, j)) > 0.0001) {
                return 0;
            }
        }
    }
    return 1;
}

// scalar multiplication
void scalar_mult(matrix *m, double num) {
    for (uint i = 0; i < m->num_rows; i++) {
        for (uint j = 0; j < m->num_cols; j++) {
            double val = get_matrix_val(m, i, j);
            set_matrix_val(m, num * val, i, j);
        }
    }
}

/**
 * thread routine for matrix multiplication 
 * TODO DOCS!
*/
void *matrix_mult_t(void *arg) {
    thread_args *args = (thread_args *) arg;
    global_thread_args *g = args->globals;
    for(uint i = 0; i < g->dim_rows; i++) {
        for(uint j = 0; j < g->dim_cols; j++) {
            double thread_private_tmp = 0;
            for(uint k = args->start; k < args->end; k++) {
                thread_private_tmp += get_matrix_val(g->a, i, k) * get_matrix_val(g->b, k, j);
            }
            pthread_mutex_lock(&lock);
                double curr_val = get_matrix_val(g->dest, i, j);
                set_matrix_val(g->dest, curr_val + thread_private_tmp, i, j);
            pthread_mutex_unlock(&lock);
        }
    }
    return NULL;
}

/**
 * Multi-threaded matrix multiplication
*/
matrix *matrix_mult(matrix *a, matrix *b) {
    int arows = a->num_rows;
    int bcols = b->num_cols;
    matrix *res = new_matrix(arows, bcols);

    int dim = a->num_cols;
    pthread_t child_threads[NUM_THREADS];
    thread_args work_args[NUM_THREADS];
    int current_start, range;
    current_start = 0;

    range = dim / NUM_THREADS;
    
    global_thread_args g;
    g.dim_rows = arows;
    g.dim_cols = bcols;
    g.dest = res;
    g.a = a;
    g.b = b;

    for(int i = 0; i < NUM_THREADS; i++) {
        work_args[i].start = current_start;
        work_args[i].end = current_start + range;
        work_args[i].globals = &g;
        current_start += range;
    }
    work_args[NUM_THREADS-1].end = dim;
    
    for(int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&child_threads[i], NULL, matrix_mult_t, &work_args[i]);
    }
    for(int i = 0; i < NUM_THREADS; i++) {
        pthread_join(child_threads[i], NULL);
    }
    return res;
}