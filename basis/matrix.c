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
    int dim_rows;
    int dim_cols;
    matrix *dest;
    matrix *a;
    matrix *b;
} thread_args;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

// constructor - generates zero-valued matrix
matrix *new_matrix(unsigned int num_rows, unsigned int num_cols) {
    assert(num_rows > 0 && num_cols > 0);

    matrix *m = rc_malloc(sizeof(matrix), free_matrix);
    m->num_rows = num_rows;
    m->num_cols = num_cols;
    m->is_square = (num_rows == num_cols) ? 1 : 0;
    m->data = calloc(num_rows * num_cols, sizeof(double));

    return m;
}

// generates a matrix with random values within min, max interval
matrix *new_rand_matrix(unsigned int num_rows, unsigned int num_cols, double min, double max) {
    matrix *m = new_matrix(num_rows, num_cols);
    for (int i = 0; i < num_rows; ++i) {
        for (int j = 0; j < num_cols; ++j) {
            set_matrix_val(m, rand_interval(min, max), i, j);
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
        set_matrix_val(m, 1, i, i);
    }   
    return m;
}

double get_matrix_val(matrix *m, int i, int j) {
    return m->data[i*m->num_cols + j];
}

void set_matrix_val(matrix *m, double val, int i, int j) {
    m->data[i*m->num_cols + j] = val;
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
                fscanf(f, "%lf\t", &m[fm->count]->data[i*num_cols + j]);
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
    int num_cols = m->num_cols;
    for (int i = 0; i < m->num_rows; i++) {
        for (int j = 0; j < m->num_cols; j++) {
            double val = get_matrix_val(m, i, j);
            set_matrix_val(nm, val, i, j);
        }
    }

    return nm;
}

// destructor
void free_matrix(matrix *m) {
    free(m->data);
}

// print matrix to stdout
void print_matrix(matrix *m) {
    for (int i = 0; i < m->num_rows; i++) {
        for (int j = 0; j < m->num_cols; j++) {
            printf("%lf\t", get_matrix_val(m, i, j));
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

    int num_cols = a->num_cols;

    for (int i = 0; i < a->num_rows; i++) {
        for (int j = 0; j < num_cols; j++) {
            if (fabs(get_matrix_val(a, i, j) - get_matrix_val(b, i, j)) > 0.0001) {
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
            double val = get_matrix_val(m, i, j);
            set_matrix_val(m, num * val, i, j);
        }
    }
}

// per thread
void *matrix_mult_t(void *arg) {
    thread_args * args = (thread_args *) arg;
    for(int i = 0; i < args->dim_rows; i++) {
        for(int j = 0; j < args->dim_cols; j++) {
            double thread_private_tmp = 0;
            for(int k = args->start; k < args->end; k++) {
                thread_private_tmp += get_matrix_val(args->a, i, k) * get_matrix_val(args->b, k, j);
            }
            pthread_mutex_lock(&lock);
                double curr_val = get_matrix_val(args->dest, i, j);
                set_matrix_val(args->dest, curr_val + thread_private_tmp, i, j);
            pthread_mutex_unlock(&lock);
        }
    }
    return NULL;
}

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
    
    // make global thread worker args
    for(int i = 0; i < NUM_THREADS; i++) {
        work_args[i].start = current_start;
        work_args[i].end = current_start + range;
        work_args[i].dim_rows = arows;
        work_args[i].dim_cols = bcols;
        work_args[i].dest = res;
        work_args[i].a = a;
        work_args[i].b = b;
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