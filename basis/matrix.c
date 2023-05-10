#include <stdlib.h>
#include <stdio.h>
#include "matrix.h"
#include <sys/resource.h>
#include <sys/time.h>
#include "omp.h"
#include <assert.h>
#include <pthread.h>
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
// TODO, account for floating point variation
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
    struct timeval tv_begin, tv_end;
    struct rusage usage_begin, usage_end;
    
    matrix *ret = new_matrix(a->num_rows, b->num_cols);
    
    int r = getrusage(RUSAGE_SELF, &usage_begin);
    assert (r >= 0);
    r = gettimeofday(&tv_begin, 0);
    assert(r >= 0);

    int i, j, k;
    #pragma omp parallel for private(i,j,k) shared(ret, a, b)
    for (i=0; i < a->num_rows; i++)
    for (k=0; k < a->num_cols; k++) 
    for (j=0; j < b->num_cols; j++)
        ret->data[i][j] += a->data[i][k] * b->data[k][j];

    r = gettimeofday(&tv_end, 0);
    assert (r >= 0);
    r = getrusage(RUSAGE_SELF, &usage_end);
    assert (r >= 0);

    // Compute elapsed time in ms
    printf("----OPENMP----\n");
    printf("Elapsed time (ms): %7.2f\n", tv_ms_diff(tv_begin, tv_end));
    printf("User time (ms)   : %7.2f\n",
        tv_ms_diff(usage_begin.ru_utime, usage_end.ru_utime));
    printf("System time (ms) : %7.2f\n",
        tv_ms_diff(usage_begin.ru_stime, usage_end.ru_stime));

    return ret;
}

// redo matrix mult using pthreads just for fun
void *pt_mmult(void *arg) {
    struct timeval tv_begin, tv_end;
    struct rusage usage_begin, usage_end;
    thread_args *range = (thread_args *) arg;
    
    int r = getrusage(RUSAGE_SELF, &usage_begin);
    assert (r >= 0);
    r = gettimeofday(&tv_begin, 0);
    assert(r >= 0);
    
    for (int i=0; i < matrix_a->num_rows; i++) {
        for (int j=0; j < matrix_b->num_cols; j++) { 
            double temp = 0;
            for (int k=range->start; k < range->end; k++) {
                temp += matrix_a->data[i][k] * matrix_b->data[k][j];
            }
            pthread_mutex_lock(&lock);
            matrix_c->data[i][j] += temp;
            pthread_mutex_unlock(&lock);
        }
    }

    r = gettimeofday(&tv_end, 0);
    assert (r >= 0);
    r = getrusage(RUSAGE_SELF, &usage_end);
    assert (r >= 0);
}

int main(int argc, char **argv) {
    pthread_t ts[NUM_THREADS];
    thread_args work_ranges[NUM_THREADS];
    int curr_start, range;
    curr_start = 0;
    range = DIM / NUM_THREADS;
    for (int i=0; i < NUM_THREADS; i++) {
        work_ranges[i].start = curr_start;
        work_ranges[i].end = curr_start + range;
        curr_start += range;
    }
    work_ranges[NUM_THREADS-1].end = DIM;
    init();
    for(int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&ts[i], NULL, pt_mmult, &work_ranges[i]);
    }

    for(int i = 0; i < NUM_THREADS; i++) {
        pthread_join(ts[i], NULL);
    }
    
    matrix *ret = mmult(matrix_a, matrix_b);

    assert(is_equal_matrix(ret, matrix_c) == 1);
}