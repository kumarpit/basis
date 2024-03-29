#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "matrix.h"
#include <sys/resource.h>
#include <sys/time.h>
#include <assert.h>
#include <pthread.h>
#include "refcount.h"
#include "utils.h"
#include <stdbool.h>

#define NUM_THREADS 4
#define ZERO 0.001

#define CANNOT_MAKE_MATRIX \
    "Matrix requires non-zero dimensions \n" \

#define CANNOT_ACCESS_MATRIX_VALUE \
    "Out of bounds access to matrix. Accessed row %d and column %d for matrix with %d rows and %d columns. \n" \

#define CANNOT_GET_MATRIX_COL \
    "Out of bounds access to matrix. Accessed column %d for matrix with %d columns. \n" \

#define CANNOT_GET_MATRIX_ROW \
    "Out of bounds access to matrix. Accessed row %d for matrix with %d rows. \n" \

#define CANNOT_OPEN_FILE \
    "Cannot open %s \n" \

#define CANNOT_READ_FILE \
    "Cannot read file %s. Invalid formatting."


typedef struct matrix_t {
    uint num_rows;
    uint num_cols;
    double *data;
    int is_square;
} matrix;

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
    if (num_rows == 0 || num_cols == 0) {
        BASIS_ERROR(CANNOT_MAKE_MATRIX);
        return NULL;
    }

    matrix *m = rc_malloc(sizeof(matrix), free_matrix);
    m->num_rows = num_rows;
    m->num_cols = num_cols;
    m->is_square = (num_rows == num_cols) ? 1 : 0;
    m->data = rc_malloc(num_rows * num_cols * sizeof(double), NULL);

    return m;
}

// generates a matrix with random values within min, max interval
matrix *new_rand_matrix(uint num_rows, uint num_cols, double min, double max) {
    if (num_rows == 0 || num_cols == 0) {
        BASIS_ERROR(CANNOT_MAKE_MATRIX);
        return NULL;
    }
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
    if (dim == 0) {
        BASIS_ERROR(CANNOT_MAKE_MATRIX);
        return NULL;
    }
    return new_matrix(dim, dim);
}

// generates square matrix with random values within min, max interval
matrix *new_rand_sqr_matrix(uint dim, double min, double max) {
    if (dim == 0) {
        BASIS_ERROR(CANNOT_MAKE_MATRIX);
        return NULL;
    }
    return new_rand_matrix(dim, dim, min, max);
}

// generates identity matrix of dimension dim
matrix *new_eye_matrix(uint dim) {
    if (dim == 0) {
        BASIS_ERROR(CANNOT_MAKE_MATRIX);
        return NULL;
    }
    matrix *m = new_sqr_matrix(dim);
    for (uint i = 0; i < dim; ++i) {
        set_matrix_val(m, 1, i, i);
    }   
    return m;
}

// get value of matrix at position i, j
double get_matrix_val(matrix *m, uint i, uint j) {
    if (i > m->num_rows-1 || j > m->num_cols-1) {
        BASIS_FERROR(CANNOT_ACCESS_MATRIX_VALUE, i, j, m->num_rows, m->num_cols);
        return INFINITY;
    }
    return m->data[i*m->num_cols + j];
}

int set_matrix_val(matrix *m, double val, uint i, uint j) {
    if (i > m->num_rows-1 || j > m->num_cols-1) {
        BASIS_FERROR(CANNOT_ACCESS_MATRIX_VALUE, i, j, m->num_rows, m->num_cols);
        return 0;
    }
    m->data[i*m->num_cols + j] = val;
    return 1;
}

matrix *get_matrix_col(matrix *m, uint c) {
    if (c > m->num_cols) {
        BASIS_FERROR(CANNOT_GET_MATRIX_COL, c, m->num_cols);
        return NULL;
    }
    matrix *r = new_matrix(m->num_rows, 1);
    for (uint i = 0; i < m->num_rows; i++) {
        double val = get_matrix_val(m, i, c);
        set_matrix_val(r, val, i, 0);
    }
    return r;
}

matrix *get_matrix_row(matrix *m, uint c) {
    if (c > m->num_rows) {
        BASIS_FERROR(CANNOT_GET_MATRIX_ROW, c, m->num_rows);
        return NULL;
    }
    matrix *r = new_matrix(1, m->num_cols);
    for (uint i = 0; i < m->num_cols; i++) {
        double val = get_matrix_val(m, c, i);
        set_matrix_val(r, val, 0, i);
    }
    return r;
}

uint num_rows(matrix *m) {
    return m->num_rows;
}

uint num_cols(matrix *m) {
    return m->num_cols;
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
 *
 * TODO: ability to add comments to input file - skip lines starting with #
*/
fmatrices *read_matrix(char *filename) {
    FILE *f = fopen(filename, "r");
    fmatrices *fm = rc_malloc(sizeof(fmatrices), free_fmatrices);
    fm->matrices = rc_malloc(SIZE * sizeof(matrix*), NULL);
    fm->count = 0;
    matrix **m = fm->matrices;

    int curr_size = SIZE;

    if (f == NULL) {
        BASIS_FERROR(CANNOT_OPEN_FILE, filename);
        return NULL;
    }

    while (1) {    
        uint num_rows, num_cols;
        if (fscanf(f, "%d", &num_rows) == EOF) {
            break;
        };
        if (fscanf(f, "%d", &num_cols) == EOF) {
            BASIS_FERROR(CANNOT_READ_FILE, filename);
            return NULL;
        };
        m[fm->count] = new_matrix(num_rows, num_cols);

        for (uint i = 0; i < num_rows; ++i) {
            for (uint j = 0; j < num_cols; ++j) {
                if (fscanf(f, "%lf\t", &m[fm->count]->data[i*num_cols + j]) == EOF) {
                    BASIS_FERROR(CANNOT_READ_FILE, filename);
                    return NULL;
                }
            }
        }

        fm->count++;
        
        if (fm->count == SIZE) {
            // DOUBLE SIZE AND COPY OVER ARRAY
            matrix **new = rc_malloc(2 * curr_size * sizeof(matrix*), NULL);
            for (uint i = 0; i < fm->count; i++) {
                new[i] = fm->matrices[i];
            }
            rc_free_ref(fm->matrices);
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
    rc_free_ref(m->data);
}

// print matrix to stdout
void print_matrix(matrix *m) {
    for (uint i = 0; i < m->num_rows; i++) {
        for (uint j = 0; j < m->num_cols; j++) {
            printf("%.2lf\t", get_matrix_val(m, i, j));
        }
        printf("\n");
    }
}

// checks if two matrices are equal (by value, not by reference)
bool are_equal_matrices(matrix *a, matrix *b) {
    if (!(a->num_rows == b->num_rows) && !(a->num_cols && b->num_cols)) {
        return false;
    }
    uint num_cols = a->num_cols;
    for (uint i = 0; i < a->num_rows; i++) {
        for (uint j = 0; j < num_cols; j++) {
            if (fabs(get_matrix_val(a, i, j) - get_matrix_val(b, i, j)) > 0.0001) {
                return false;
            }
        }
    }
    return true;
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
    for (uint i = 0; i < g->dim_rows; i++) {
        for (uint j = 0; j < g->dim_cols; j++) {
            double thread_private_tmp = 0;
            for (uint k = args->start; k < args->end; k++) {
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

/**
 * ROW OPERATIONS
*/

// matrix[j][k] = scale * matrix[i][k] + matrix[j][k]
void add_row(matrix *m, uint i, uint j, double scale) {
    if (i > m->num_rows - 1) {
        BASIS_FERROR(CANNOT_GET_MATRIX_ROW, i, m->num_rows);
        return NULL;
    } else if (j > m->num_rows - 1) {
        BASIS_FERROR(CANNOT_GET_MATRIX_ROW, j, m->num_rows);
        return NULL;
    }
    for (uint k = 0; k < m->num_cols; k++) {
        double sum = scale * get_matrix_val(m, i, k) + get_matrix_val(m, j, k);
        set_matrix_val(m, sum, j, k);
    }
}

/**
 * Swap row i with row j
*/
void swap_row(matrix *m, uint i, uint j) {
    if (i > m->num_rows - 1) {
        BASIS_FERROR(CANNOT_GET_MATRIX_ROW, i, m->num_rows);
        return NULL;
    } else if (j > m->num_rows - 1) {
        BASIS_FERROR(CANNOT_GET_MATRIX_ROW, j, m->num_rows);
        return NULL;
    }
    if (i == j) return;
    for (uint k = 0; k < m->num_cols; k++) {
        double temp = get_matrix_val(m, j, k);
        set_matrix_val(m, get_matrix_val(m, i, k), j, k);
        set_matrix_val(m, temp, i, k);
    }
}

void mult_row(matrix *m, double scale, uint i) {
    if (i > num_rows - 1) {
        BASIS_FERROR(CANNOT_GET_MATRIX_ROW, i, m->num_rows);
        return NULL;
    }
    for (uint k = 0; k < m->num_cols; k++) {
        double val = get_matrix_val(m, i, k);
        set_matrix_val(m, scale * val, i, k);
    }
}

void normalize_row(matrix *m, uint i) {
    if (i >= m->num_rows) {
        BASIS_FERROR(CANNOT_GET_MATRIX_ROW, i, m->num_rows);
        return NULL;
    }
    double lead;
    bool found = false;
    for (uint j = 0; j < m->num_cols; j++) {
        double curr = get_matrix_val(m, i, j);
        if (curr > ZERO) {
            if (!found) {
                lead = curr;
                found = true;
            }
            set_matrix_val(m, curr / lead, i, j);
        } 
    }
}

/**
 * PRIVATE
*/
int pivot_idx(matrix *m, uint row, uint col) {
    for (uint i = row; i < m->num_rows; i++) {
        if (fabs(get_matrix_val(m, i, col)) >  ZERO) {
            return i;
        }
    }
    return -1;
}

void ref(matrix *m) {
    int i, j, pivot;
    i = j = 0;
    while (i < m->num_rows && j < m->num_cols) {
        pivot = pivot_idx(m, i, j);
        if (pivot < 0) {
            j++;
            continue;
        }
        swap_row(m, i, pivot);
        normalize_row(m, i);
        for (uint k = i+1; k < m->num_rows; k++) {
            if (fabs(get_matrix_val(m, k, j)) > ZERO) {
                add_row(m, i, k, -(get_matrix_val(m, k, j)));
            }
        }
        i++;j++;
    }
}

