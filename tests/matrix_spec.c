#include <criterion/criterion.h>
#include "../basis/matrix.h"
#include "../basis/refcount.h"

Test(Matrix, read_matrix) {
    fmatrices *read = read_matrix("tests/test_matrices/matrix1.txt");
    matrix *m = new_matrix(3,3);
    double **data = m->data;
    
    data[0][0] = 1;
    data[0][1] = 2;
    data[0][2] = 3;

    data[1][0] = 2;
    data[1][1] = 3;
    data[1][2] = 4;

    data[2][0] = 4;
    data[2][1] = 5;
    data[2][2] = 6;

    cr_assert(are_equal_matrices(read->matrices[0], m)); 

    rc_free_ref(read);
    rc_free_ref(m);
}

Test(Matrix, scalar_mult) {
    fmatrices *read = read_matrix("tests/test_matrices/matrix1.txt");
    scalar_mult(read->matrices[0], 2);
    matrix *m = new_matrix(3,3);
    double **data = m->data;
    
    data[0][0] = 2;
    data[0][1] = 4;
    data[0][2] = 6;

    data[1][0] = 4;
    data[1][1] = 6;
    data[1][2] = 8;

    data[2][0] = 8;
    data[2][1] = 10;
    data[2][2] = 12;

    cr_assert(are_equal_matrices(read->matrices[0], m)); 

    rc_free_ref(read);
    rc_free_ref(m);
}

Test(Matrix, matrix_mult) {
    fmatrices *read = read_matrix("tests/test_matrices/matrix2.txt");
    matrix *res = matrix_mult(read->matrices[0], read->matrices[1]);
    cr_assert(are_equal_matrices(res, read->matrices[2]));
    rc_free_ref(read);
    rc_free_ref(res);
}